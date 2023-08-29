#include <cloudy/obstacle_cloud.h>


ObstacleCloud::ObstacleCloud(ros::NodeHandle &n, double f) :
node_(n), dt_(1./f), tf2_(buffer_), frame_in_("front_laser"), frame_out_("virtual")
{
    cloud_sub_ = node_.subscribe("laser/cloud", 1, &ObstacleCloud::cloud_callback, this);
}

void ObstacleCloud::cloud_callback(const sensor_msgs::PointCloud msg)
{
    ros::Time time = ros::Time::now();

    if (dt_ < time - last_update_)
    {
        cloud_.clear();
        for (auto point : msg.points)
            cloud_.push_back(point);
        last_update_ = time;
    }
}

double ObstacleCloud::obstacle_tack(double sector)
// portside = -1 ; starboard = +1
{
    double r, h;
    double proximity;

    double starboard = 0.;
    double portside = 0.;

    for (auto point : cloud_)
    {
        r = norm2(point.x, point.y);
        h = atan2(point.y, point.x);

        proximity = 1. / (1. + pow(r, 2.));

        if (-2.*sector < h && h < 0.)
            starboard += proximity;
        
        if (0. < h && h < +2.*sector)
            portside += proximity;
    }
    
    return (starboard < portside ? -1. : +1.);
}

bool ObstacleCloud::segment_clear(double from_goal_to_robot_heading, double length, double security_distance)
// Tell if the segment rooted at the robot position and described by heading & length is free to go with security_distance
{
    double px, py;

    for (auto point : cloud_)
    {
        px = point.x;
        py = point.y;

        // Transpose obstacle point into the toward goal robot frame
        transpose(px, py, 0., 0., from_goal_to_robot_heading);

        if ((fabs(py) < security_distance) && (fabs(px) - pythagore(security_distance, fabs(py)) < length))
            return false;
    }

    return true;
}

double ObstacleCloud::distance_to_obstacle(double xv, double yv, double hv,
                                           double v, double w, double security_distance)
{
    // Distance to the nearest obstacle on the trajectory (v, w)
    // starting from the virtual robot pose (xv, yv, hv) in the reference frame (odom)

    double distance = HUGE_VAL;
    double d;  // current distance between the virtual robot & the obstacle on its trajectory

    double px, py;  // point position in the virtual frame

    // Define in the TF buffer the transform from odom (reference frame) to virtual (imagined robot frame)
    geometry_msgs::TransformStamped tfs;
    tfs.header.frame_id = "odom";
    tfs.child_frame_id = "virtual";
    tfs.transform.translation.x = xv;
    tfs.transform.translation.y = yv;
    tfs.transform.translation.z = 0.;
    tfs.transform.rotation = tf::createQuaternionMsgFromRollPitchYaw(0., 0., hv);
    buffer_.setTransform(tfs, "default_authority", true);

    TrajectoryCircle circle = TrajectoryCircle(v, w, security_distance);

    for (auto point : cloud_)
    {
        geometry_msgs::PointStamped point_in;
        point_in.header.frame_id = frame_in_;
        point_in.point.x = point.x;
        point_in.point.y = point.y;
        point_in.point.z = 0.0;

        geometry_msgs::PointStamped point_out;

        try
            // Equivalent as transposing from laser to reference then from reference to virtual
            {buffer_.transform(point_in, point_out, frame_out_);}
        
        catch (tf2::TransformException &ex)
            {ROS_WARN("Failure %s\n", ex.what());}

        px = point_out.point.x;
        py = point_out.point.y;

        if (v * px < 0)  // discard points we are going away from
            continue;

        d = HUGE_VAL;

        if (fabs(v) < EPS)  // ponctual trajectory
            {if (norm2(px, py) < security_distance) d = 0.;}

        else 
        {
            if (fabs(w) < EPS)  // linear trajectory
                {if (fabs(py) < security_distance) d = fabs(px) - pythagore(security_distance, fabs(py));}

            else  // circular trajectory
                {d = circle.distance(px, py);}
        }

        if (d < distance)
            distance = d;
    }

    return distance;
}
