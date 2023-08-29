#include "heron_mission/planner.h"


Planner::Planner(ros::NodeHandle &n) : node_(n)
{
    ros::NodeHandle prv_node_("~");

    // Obstacle cloud manager
    obstacle_cloud_ = new ObstacleCloud(node_, 5.);  // update cloud buffer at 5Hz

    // Publish debug information about the current mission
    mission_debug_pub_ = node_.advertise<geometry_msgs::Vector3>("planner/mission_debug", 1000);
    trajectory_pub_ = node_.advertise<sensor_msgs::PointCloud>("planner/trajectory", 1000);

    // Publish speed & heading
    cmd_course_pub_ = node_.advertise<heron_msgs::Course>("cmd_course", 1000);
    course_output_.speed = 0.0;
    course_output_.yaw = 0.0;

    // Publish force & torque
    cmd_wrench_pub_ = node_.advertise<geometry_msgs::Wrench>("cmd_wrench", 1000);
    wrench_output_.torque.z = 0.0;
    wrench_output_.force.x = 0.0;

    // Publish forward & yaw velocity
    cmd_twist_pub_ = node_.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
    twist_output_.linear.x = 0.0;
    twist_output_.angular.z = 0.0;

    mission_request_ = WAITING;
    mission_type_ = NO_MISSION;
    output_type_ = NONE;

    prv_node_.param<double>("dt_mission", DT_MISSION_, 5.);

    // GPS mission constants
    prv_node_.param<double>("dist_to_success", DIST_TO_SUCCESS_, 1.0);
    prv_node_.param<double>("turn_cone", TURN_CONE_, M_PI / 4.);

    // Trajectory mission constants
    prv_node_.param<double>("torque_scale_yaw", TORQUE_SCALE_YAW_, 10.0);
    prv_node_.param<double>("force_scale_vel", FORCE_SCALE_YAW_, 10.0);

    prv_node_.param<double>("cmp_scale_e0", CMP_SCALE_E0_, 1.0);
    prv_node_.param<double>("cmp_scale_e1", CMP_SCALE_E1_, 1.0);

    prv_node_.param<double>("drag_coef", DRAG_COEF_, 0.5);

    // Dynamic Window Approach constants
    prv_node_.param<double>("sim_tim", SIM_TIME_, 2.);

    prv_node_.param<int>("linear_velocity/count", LINEAR_VELOCITY_.count, 5);
    prv_node_.param<double>("linear_velocity/acc", LINEAR_VELOCITY_.vel, 1.0);
    prv_node_.param<double>("linear_velocity/min", LINEAR_VELOCITY_.min, -0.5);
    prv_node_.param<double>("linear_velocity/max", LINEAR_VELOCITY_.max, +1.5);

    prv_node_.param<int>("angular_velocity/count", ANGULAR_VELOCITY_.count, 15);
    prv_node_.param<double>("angular_velocity/acc", ANGULAR_VELOCITY_.vel, 1.0);
    prv_node_.param<double>("angular_velocity/min", ANGULAR_VELOCITY_.min, -1.0);
    prv_node_.param<double>("angular_velocity/max", ANGULAR_VELOCITY_.max, +1.0);

    prv_node_.param<int>("security_factor/count", SECURITY_FACTOR_.count, 3);
    prv_node_.param<double>("security_factor/acc", SECURITY_FACTOR_.vel, 1.0);
    prv_node_.param<double>("security_factor/min", SECURITY_FACTOR_.min, 1.0);
    prv_node_.param<double>("security_factor/max", SECURITY_FACTOR_.max, 2.0);

    sf_chos_ = SECURITY_FACTOR_.max;

    prv_node_.param<int>("tree_search_depth", TREE_SEARCH_DEPTH_, 1);
    prv_node_.param<double>("futur_impact", FUTUR_IMPACT_, 0.);
    prv_node_.param<double>("dynamic_ceil", KVW_, 1.0);

    prv_node_.param<double>("no_collision_reward", NO_COLLISION_REWARD_, 20.);
    prv_node_.param<double>("robot_radius", ROBOT_RADIUS_, 1.);
    
    prv_node_.param<double>("barycenter/goal", B_GOAL_, 1.0);
    prv_node_.param<double>("barycenter/obstacle", B_OBSTACLE_, 1.0);
    prv_node_.param<double>("barycenter/speed", B_SPEED_, 1.0);
    prv_node_.param<double>("barycenter/security", B_SECURITY_, 1.0);

    // GPS data
    prv_node_.param<double>("timeout/gps", GPS_TIMEOUT_, 0.5);
    prv_node_.param<double>("cov_limit/gps_pos", POS_COV_LIMIT_, 0.2);
    gps_timed_out_ = false;
    gps_time_ = 0.0;

    // IMU data
    prv_node_.param<double>("timeout/imu", IMU_TIMEOUT_, 0.5);
    prv_node_.param<double>("cov_limit/imu_yaw", YAW_COV_LIMIT_, 0.2);
    prv_node_.param<double>("cov_limit/imu_lac", LAC_COV_LIMIT_, 0.2);
    imu_timed_out_ = false;
    imu_time_ = 0.0;

    // Velocity estimator data
    prv_node_.param<double>("timeout/velest", VELEST_TIMEOUT_, 0.5);
    prv_node_.param<double>("cov_limit/velest", VEL_COV_LIMIT_, 0.2);
    velest_timed_out_ = false;
    velest_time_ = 0.0;

    current_buoy_ = "";
    buoy_id_ = 0;

    // Activate & deactivate control
    activate_control_client_ = node_.serviceClient<std_srvs::SetBool>("activate_control");

    spawn_buoy_client_ = node_.serviceClient<heron_srvs::AskModel>("/model_manager/spawn_buoy");
    delete_model_client_ = node_.serviceClient<gazebo_msgs::DeleteModel>("/gazebo/delete_model");
    set_model_state_client_ = node_.serviceClient<gazebo_msgs::SetModelState>("/gazebo/set_model_state");
}

void Planner::update_gps_mission()
{
    double dx = x_cmd_ - x_meas_;
    double dy = y_cmd_ - y_meas_;

    double dist_to_goal = norm2(dx, dy);
    bool mission_succeed = (dist_to_goal < DIST_TO_SUCCESS_);

    if (mission_succeed)
        mission_request_ = NO_MISSION;
    else
    {
        double desired_yaw = std::atan2(dy, dx);

        // if we are going backward, face the other way around
        if (vel_cmd_ < 0) desired_yaw += M_PI;

        double error_yaw = sawtooth(desired_yaw - yaw_meas_);
        double vel_comp = 1. - std::min(1., fabs(error_yaw) / TURN_CONE_);

        course_output_.speed = vel_cmd_ * vel_comp;
        course_output_.yaw = desired_yaw;
        output_type_ = COURSE;

        geometry_msgs::Vector3 dbg_info;
        dbg_info.x = x_meas_;
        dbg_info.y = y_meas_;
        dbg_info.z = desired_yaw;
        mission_debug_pub_.publish(dbg_info);
    }
}

void Planner::update_traj_mission()
{
    // Aliases make calculations easier
    double x = x_meas_;
    double y = y_meas_;
    double yaw = yaw_meas_;
    double v = vel_meas_;

    // Use a simple boat model to estimates positions speed
    double d_x = v * cos(yaw);
    double d_y = v * sin(yaw);

    // Load desired trajectory
    double xt, yt;
    double d_xt, d_yt;
    double dd_xt, dd_yt;
    double t = ros::Time::now().toSec();
    trajectory(t, xt, yt);
    d_trajectory(t, d_xt, d_yt);
    dd_trajectory(t, dd_xt, dd_yt);

    // Compute compensator components
    double cmp_x = dd_xt + 2*CMP_SCALE_E1_ * (d_xt - d_x) + CMP_SCALE_E0_ * (xt - x);
    double cmp_y = dd_yt + 2*CMP_SCALE_E1_ * (d_yt - d_y) + CMP_SCALE_E0_ * (yt - y);

    // Compute compensator norm (gives an idea of the error)
    double error = sqrt(cmp_x*cmp_x + cmp_y*cmp_y);

    // Define the transition matrix A between compensator & control
    // A = {a b} such as A.U + b = compensator & U = {u_yaw}
    //     {c d}                                     {u_vel}
    double a = -v * sin(yaw);
    double b = cos(yaw);
    double c = v * cos(yaw);
    double d = sin(yaw);

    // Compute the determinant of A (simple here)
    double delta = -v;

    // Compensator - b
    double aux = cmp_x + DRAG_COEF_ * d_x;
    double auy = cmp_y + DRAG_COEF_ * d_y;

    // Deduce control values
    double u_yaw, u_vel;
    if (-EPS < delta)
    {
        u_yaw = 3.0;  // turn until we face the target
        u_vel = 2.0;  // ensure delta will be non zero next time!
    } else
    {
        u_yaw = (auy * b - aux * d) / v;
        u_vel = (aux * c - auy * a) / v;
    }

    wrench_output_.torque.z = TORQUE_SCALE_YAW_ * u_yaw;
    wrench_output_.force.x = FORCE_SCALE_YAW_ * u_vel;
    output_type_ = WRENCH;

    // Move the targeted buoy
    set_buoy_position(xt, yt);

    geometry_msgs::Vector3 dbg_info;
    dbg_info.x = u_yaw;
    dbg_info.y = u_vel;
    dbg_info.z = error;
    mission_debug_pub_.publish(dbg_info);
}

void Planner::update_dwa_mission()
{
    double dx = x_cmd_ - x_meas_;
    double dy = y_cmd_ - y_meas_;
    double dist_to_goal = norm2(dx, dy);
    double goal_yaw = std::atan2(dy, dx);
    double error_yaw = sawtooth(yaw_meas_ - goal_yaw);

    if (dist_to_goal < DIST_TO_SUCCESS_)
        mission_request_ = NO_MISSION;
    else
    {
        geometry_msgs::Vector3 dbg_info;

        // If we can reach the goal directly (no obstacle on the straight line trajectory)
        if (obstacle_cloud_->segment_clear(error_yaw, dist_to_goal, SECURITY_FACTOR_.min))
        {
            double spd_cmp = 1. - std::min(pow(fabs(error_yaw) / TURN_CONE_, 2.), 1.);
            double speed = LINEAR_VELOCITY_.max * spd_cmp;

            course_output_.speed = speed;
            course_output_.yaw = goal_yaw;
            output_type_ = COURSE;

            plan_.clear();

            dbg_info.x = 1.0;
            dbg_info.y = speed;
            dbg_info.z = error_yaw;
        }
        else
        {
            plan_ = std::vector<Dynamic>(TREE_SEARCH_DEPTH_);

            RobotState robot_state;
            robot_state.pos = Pose{x_meas_, y_meas_, yaw_meas_};
            robot_state.dyn = Dynamic{vel_meas_, lac_meas_, sf_chos_};

            // Fill the plan_ class variable
            double tree_score = eval_robot_state(robot_state);

            double best_score = plan_[0].score;
            double best_linear_vel = plan_[0].v;
            double best_angular_vel = plan_[0].w;
            sf_chos_ = plan_[0].sf;

            // if we can't find any admissible dynamic
            // or if we are stuck on a local minimum
            // let's turn around!
            if (best_score < EPS)
            {
                double w_maneuver = ANGULAR_VELOCITY_.max / 4.;
                twist_output_.linear.x = 0.0;
                twist_output_.angular.z = obstacle_cloud_->obstacle_tack(w_maneuver * DT_MISSION_) * w_maneuver;
            }
            else
            {
                twist_output_.linear.x = best_linear_vel;
                twist_output_.angular.z = best_angular_vel;
            }
            output_type_ = TWIST;

            dbg_info.x = best_score;
            dbg_info.y = best_angular_vel;
            dbg_info.z = sf_chos_;
        }

        publish_plan();
        mission_debug_pub_.publish(dbg_info);
    }
}

double Planner::eval_robot_state(const RobotState &parent, int depth)
{  // ASSUMING THAT k_depth IS NON NEGATIVE -> OTHERWISE NON SENSE
   // increase until TREE_SEARCH_DEPTH_ is reached
    std::vector<double> dynamic_scores{};
    plan_[depth] = Dynamic{0., 0., SECURITY_FACTOR_.min, 0.};

    // Range of admissible linear velocities
    double min_linear_velocity, linear_velocity_step;
    get_min_and_step(LINEAR_VELOCITY_, parent.dyn.v, min_linear_velocity, linear_velocity_step);

    // Range of admissible angular velocities
    double min_angular_velocity, angular_velocity_step;
    get_min_and_step(ANGULAR_VELOCITY_, parent.dyn.w, min_angular_velocity, angular_velocity_step);

    // Range of admissible security factors
    double min_security_factor, security_factor_step;
    get_min_and_step(SECURITY_FACTOR_, parent.dyn.sf, min_security_factor, security_factor_step);

    Dynamic dynamic;  // for each dynamic...
    for (int k_linear=0; k_linear<LINEAR_VELOCITY_.count; k_linear++)
    {
        dynamic.v = min_linear_velocity + k_linear * linear_velocity_step;
        for (int k_angular=0; k_angular<ANGULAR_VELOCITY_.count; k_angular++)
        {
            dynamic.w = min_angular_velocity + k_angular * angular_velocity_step;

            if (fabs(dynamic.v * dynamic.w) > KVW_)
                continue;

            for (int k_security=0; k_security<SECURITY_FACTOR_.count; k_security++)
            {
                dynamic.sf = min_security_factor + k_security * security_factor_step;

                dynamic.score = eval_dynamic(parent, dynamic, depth+1);  // compute dynamic score
                dynamic_scores.push_back(dynamic.score);

                if (dynamic.score > plan_[depth].score)
                    plan_[depth] = dynamic;
            }
        }
    }

    return filter(dynamic_scores);
}

double Planner::eval_dynamic(const RobotState &parent, const Dynamic &dynamic, int depth)
{
    RobotState child;
    child.dyn = dynamic;

    double dh = dynamic.w * SIM_TIME_;

    if (fabs(dynamic.w) < EPS)
    {
        // Next virtual robot position in its parent frame
        child.pos.x = dynamic.v * SIM_TIME_;
        child.pos.y = 0.0;
    }
    else
    {
        double radius = dynamic.v / dynamic.w;

        // Next virtual robot position in its parent frame
        child.pos.x = radius * sin(dh);
        child.pos.y = radius * (1. - cos(dh));
    }

    // Transpose it into the reference frame
    transpose(child.pos.x, child.pos.y, parent.pos.x, parent.pos.y, parent.pos.h);
    child.pos.h = parent.pos.h + dh;

    // Compute the heading error toward the goal buoy
    double goal_heading = M_PI - heading_error(child.pos);

    // Compute the distance to obstacle in the robot frame (since lidar gives us points in this frame)
    // Maximum distance we can achieved without hitting an obstacle
    double dist_to_obstacle = obstacle_cloud_->distance_to_obstacle(parent.pos.x, parent.pos.y, parent.pos.h,
                                                            dynamic.v, dynamic.w, dynamic.sf * ROBOT_RADIUS_);
    dist_to_obstacle = std::max(0., dist_to_obstacle);  // dist_to_obstacle <= 0 when we collide

    // If the choosen velocity enable the robot to stop before the nearest obstacle
    // LINEAR_VELOCITY_.vel is the rate of change of linear velocity -> linear acceleration
    if (fabs(dynamic.v) < sqrt(2 * dist_to_obstacle * LINEAR_VELOCITY_.vel))
    {
        double dynamic_score = objective_function(goal_heading, dist_to_obstacle, dynamic.v, dynamic.sf);

        if (depth < TREE_SEARCH_DEPTH_)
        {
            double descendant_score = eval_robot_state(child, depth);  // compute the futur plan
            double best_futur = plan_[depth].score;  // get score from the plan for depth (futur)

            return (1. - FUTUR_IMPACT_) * dynamic_score + FUTUR_IMPACT_ * descendant_score;
        }
        else
            {return dynamic_score;}
    }
    else
        {return 0.;}
}

double Planner::objective_function(double goal_heading, double dist_to_obstacle, double speed, double security)
{
    // Our Dynamic Window Approach policy
    // goal_heading = PI - heading_error -> in [0, PI]
    // dist_to_obstacle = distance_on_trajectory -> in [0, NO_COLLISION_REWARD_]
    // speed = linear_vel -> in [MIN_LINEAR_VEL_, MAX_LINEAR_VEL_]
    // security = dynamic_security_factor -> in [MIN_SECURITY_, MAX_SECURITY_]

    // Normalize quantities
    goal_heading /= M_PI;
    dist_to_obstacle = std::min(dist_to_obstacle, NO_COLLISION_REWARD_) / NO_COLLISION_REWARD_;
    normalize_parameter(LINEAR_VELOCITY_, speed);
    normalize_parameter(SECURITY_FACTOR_, security);

    return (B_GOAL_*goal_heading + B_OBSTACLE_*dist_to_obstacle + B_SPEED_*speed + B_SECURITY_*security) 
        / (B_GOAL_ + B_OBSTACLE_ + B_SPEED_ + B_SECURITY_);
}

void Planner::publish_plan()
{
    int POINT_PER_SIM = 16;
    double dt = SIM_TIME_ / ((double) POINT_PER_SIM);

    double t;
    double sr;  // security radius
    double v, w;
    double xr, yr;
    double radius, dr;

    double x0 = 0.;
    double y0 = 0.;
    double h0 = 0.;

    int red = 0xff0000;
    int green = 0x00ff00;
    int white = 0xffffff;

    std::vector<int> offsets{-1, +1, 0};
    std::vector<float> colors{*reinterpret_cast<float*>(&red),
                              *reinterpret_cast<float*>(&green),
                              *reinterpret_cast<float*>(&white)};

    sensor_msgs::PointCloud trajectory_cloud;
    sensor_msgs::ChannelFloat32 channel;
    geometry_msgs::Point32 point;

    trajectory_cloud.header.frame_id = "front_laser";
    trajectory_cloud.points.clear();
    trajectory_cloud.channels.clear();

    for (auto dynamic : plan_)
    {
        v = dynamic.v;
        w = dynamic.w;
        sr = dynamic.sf * ROBOT_RADIUS_;

        if (fabs(w) > EPS) radius = v / w;

        for (int k_offset=0; k_offset<3; k_offset++)
        {
            dr = offsets[k_offset] * sr;

            for (int k=0; k<=POINT_PER_SIM; k++)
            {
                t = k * dt;

                if (fabs(w) < EPS)
                {
                    xr = v * t;
                    yr = -dr;
                } else
                {
                    xr = (radius + dr) * sin(w * t);
                    yr = radius - (radius + dr) * cos(w * t);
                }
                
                transpose(xr, yr, x0, y0, h0);

                point.x = xr;
                point.y = yr;
                point.z = 0.;
                trajectory_cloud.points.push_back(point);

                // -1 -> red (portside) ; +1 -> green (starboard) ; 0 -> white
                channel.values.push_back(colors[k_offset]);
            }
        }

        x0 += xr;
        y0 += yr;
        h0 += w * SIM_TIME_;
    }

    channel.name = "rgb";
    trajectory_cloud.channels.push_back(channel);
    trajectory_pub_.publish(trajectory_cloud);
}

double Planner::heading_error(Pose pose)
{
    double desired_yaw = std::atan2(y_cmd_ - pose.y, x_cmd_ - pose.x);
    return angular_gap(pose.h, desired_yaw);
}

bool Planner::activate_control(bool activated)
{
    std_srvs::SetBool srv;
    srv.request.data = activated;

    return activate_control_client_.call(srv);
}

bool Planner::delete_buoy()
{
    if (current_buoy_ == "")
        return true;
    else {
        gazebo_msgs::DeleteModel srv;

        srv.request.model_name = current_buoy_;
        current_buoy_ = "";
        buoy_id_++;

        return delete_model_client_.call(srv);
    }
}

bool Planner::spawn_buoy(double x, double y)
{
    if (current_buoy_ == "")
    {
        heron_srvs::AskModel srv;
        geometry_msgs::Point position;
        position.x = x;
        position.y = y;

        current_buoy_ = "buoy" + std::to_string(buoy_id_);
        srv.request.model_name = current_buoy_;
        srv.request.position = position;

        return spawn_buoy_client_.call(srv);
    }
    else
        return true;
}

bool Planner::set_buoy_position(double x, double y)
{
    gazebo_msgs::SetModelState srv;
    gazebo_msgs::ModelState model_state;

    geometry_msgs::Pose pose;
    pose.position.x = x;
    pose.position.y = y;
    pose.position.z = 0.;
    pose.orientation.x = 0.;
    pose.orientation.y = 0.;
    pose.orientation.z = 0.;
    pose.orientation.w = 0.;

    geometry_msgs::Twist twist;
    twist.linear.x = 0.;
    twist.linear.y = 0.;
    twist.linear.z = 0.;
    twist.angular.x = 0.;
    twist.angular.y = 0.;
    twist.angular.z = 0.;

    model_state.model_name = current_buoy_;
    model_state.pose = pose;
    model_state.twist = twist;
    model_state.reference_frame = "";  // world frame

    srv.request.model_state = model_state;

    return set_model_state_client_.call(srv);
}

void Planner::odom_callback(const nav_msgs::Odometry msg)
{
    // Are x & y measurements from the odometry/gps good to use?
    if (msg.pose.covariance[0] < POS_COV_LIMIT_ && msg.pose.covariance[7] < POS_COV_LIMIT_)
        gps_time_ = ros::Time::now().toSec();

    // Are yaw & yaw_rate (lacet) measurements from the IMU good to use?
    if (msg.pose.covariance[35] < YAW_COV_LIMIT_ && msg.twist.covariance[35] < LAC_COV_LIMIT_)
        imu_time_ = ros::Time::now().toSec();

    // Is velocity estimation good to use?
    if (msg.twist.covariance[0] < VEL_COV_LIMIT_ && msg.twist.covariance[7] < VEL_COV_LIMIT_)
        velest_time_ = ros::Time::now().toSec();

    x_meas_ = msg.pose.pose.position.x;
    y_meas_ = msg.pose.pose.position.y;

    yaw_meas_ = tf::getYaw(msg.pose.pose.orientation);
    lac_meas_ = msg.twist.twist.angular.z;

    vel_meas_ = msg.twist.twist.linear.x * std::cos(yaw_meas_) + msg.twist.twist.linear.y * std::sin(yaw_meas_);
}

void Planner::control_update(const ros::TimerEvent &event)
{
    double current_time = ros::Time::now().toSec();

    // GPS position time out checking
    gps_timed_out_ = (current_time - gps_time_ > GPS_TIMEOUT_);

    // IMU yaw time out checking
    imu_timed_out_= (current_time - imu_time_ > IMU_TIMEOUT_);

    // velocity estimation time out checking
    velest_timed_out_ = (current_time - velest_time_ > VELEST_TIMEOUT_);

    bool control_updated = true;
    bool buoy_deleted = true;
    bool buoy_spawned = true;

    // If we change mission -> remove buoy & clear plan
    if (mission_request_ != WAITING)
    {
        buoy_deleted = delete_buoy();
        plan_.clear();
        publish_plan();
    }

    switch (mission_request_)
    {
        case GPS_MISSION:
            if (!gps_timed_out_ && !imu_timed_out_)
            {
                mission_type_ = GPS_MISSION;
                control_updated = activate_control(true);
                buoy_spawned = spawn_buoy(x_cmd_, y_cmd_);
            }
            break;
        case TRAJ_MISSION:
            if (!gps_timed_out_ && !imu_timed_out_ && !velest_timed_out_)
            {
                mission_type_ = TRAJ_MISSION;
                control_updated = activate_control(true);
                buoy_spawned = spawn_buoy();
            }
            break;
        case DWA_MISSION:
            if (!gps_timed_out_ && !imu_timed_out_ && !velest_timed_out_)
            {
                mission_type_ = DWA_MISSION;
                control_updated = activate_control(true);
                buoy_spawned = spawn_buoy(x_cmd_, y_cmd_);
            }
            break;
        case NO_MISSION:
            mission_type_ = NO_MISSION;
            control_updated = activate_control(false);
            break;
        case WAITING:
            break;
    }

    // If request is met -> turn to waiting mode
    if (mission_request_ != WAITING && control_updated && buoy_deleted && buoy_spawned)
        mission_request_ = WAITING;
}

void Planner::mission_update(const ros::TimerEvent &event)
{
    switch (mission_type_)
    {
        case GPS_MISSION:
            update_gps_mission();
            break;
        case TRAJ_MISSION:
            update_traj_mission();
            break;
        case DWA_MISSION:
            update_dwa_mission();
            break;
        case NO_MISSION:
            output_type_ = NONE;
            break;
    }

    switch (output_type_)
    {
        case COURSE:
            cmd_course_pub_.publish(course_output_);
            break;
        case WRENCH:
            cmd_wrench_pub_.publish(wrench_output_);
            break;
        case TWIST:
            cmd_twist_pub_.publish(twist_output_);
            break;
        case NONE:
            break;
    }
}

void Planner::console_update(const ros::TimerEvent &event)
{
    std::string output = "";
    switch (mission_type_)
    {
        case GPS_MISSION:
            output = "Goto GPS point mission";
            break;
        case TRAJ_MISSION:
            output = "Follow trajectory mission";
            break;
        case DWA_MISSION:
            output = "Goto GPS point using DWA mission";
            break;
        case NO_MISSION:
            output = "No missions being processed";
            break;
        default:
            break;
    }

    if (gps_timed_out_)
        output += ": position badly estimates or being received too slowly";

    if (imu_timed_out_)
        output += ": yaw badly estimates or being received too slowly";

    if (velest_timed_out_)
        output += ": velocity badly estimates or being received too slowly";

    ROS_INFO("%s", output.c_str());
}

bool Planner::gps_mission_service(heron_srvs::GpsMission::Request &req, heron_srvs::GpsMission::Response &resp)
{
    x_cmd_ = req.gps_x;
    y_cmd_ = req.gps_y;
    vel_cmd_ = req.vel;

    resp.success = true;
    resp.message = "GPS mission received!";
    mission_request_ = GPS_MISSION;

    return resp.success;
}

bool Planner::traj_mission_service(std_srvs::SetBool::Request &req, std_srvs::SetBool::Response &resp)
{
    if (req.data)
        mission_request_ = TRAJ_MISSION;
    else
        mission_request_ = NO_MISSION;

    resp.success = true;
    resp.message = "Follow trajectory command updated!";

    return resp.success;
}

bool Planner::dwa_mission_service(heron_srvs::DwaMission::Request &req, heron_srvs::DwaMission::Response &resp)
{
    x_cmd_ = req.goal_x;
    y_cmd_ = req.goal_y;

    resp.success = true;
    resp.message = "DWA mission received!";
    mission_request_ = DWA_MISSION;

    return resp.success;
}

void Planner::get_min_and_step(const DynamicParameter &dp, double x, double &min_x, double &step_x)
{
    double max_x = std::min(x + dp.vel*DT_MISSION_, dp.max);
    min_x = std::max(dp.min, x - dp.vel*DT_MISSION_);
    step_x = (max_x - min_x) / (dp.count - 1.);
}

void normalize_parameter(const DynamicParameter &dp, double &x)
{
    x = (x - dp.min) / (dp.max - dp.min);
}

void dd_trajectory(double t, double &dd_xt, double &dd_yt)
{
    double d_xt, d_yt;
    d_trajectory(t, d_xt, d_yt);

    double d_xt_pdt, d_yt_pdt;
    d_trajectory(t+DERIV_TIME_STEP, d_xt_pdt, d_yt_pdt);

    dd_xt = (d_xt_pdt - d_xt) / DERIV_TIME_STEP;
    dd_yt = (d_yt_pdt - d_yt) / DERIV_TIME_STEP;
}

void d_trajectory(double t, double &d_xt, double &d_yt)
{
    double xt, yt;
    trajectory(t, xt, yt);

    double xt_pdt, yt_pdt;
    trajectory(t+DERIV_TIME_STEP, xt_pdt, yt_pdt);

    d_xt = (xt_pdt - xt) / DERIV_TIME_STEP;
    d_yt = (yt_pdt - yt) / DERIV_TIME_STEP;
}

void trajectory(double t, double &xt, double &yt)
{
    double radius = 4.0;  // in meters
    double omega = 0.36;  // in radians per seconds

    double theta = omega * t;
    if (fmod(theta, 4 * M_PI) < 2 * M_PI)
    {
        xt = radius * (cos(-theta) - 1);
        yt = radius * sin(-theta);
    } else
    {
        xt = radius * (cos(theta + M_PI) + 1);
        yt = radius * sin(theta + M_PI);
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "mission");
    ros::NodeHandle nh;
    Planner heron_mission(nh);

    ros::Subscriber odom_sub = nh.subscribe("odometry/filtered", 1, &Planner::odom_callback, &heron_mission);

    ros::ServiceServer gps_mission_server = nh.advertiseService("planner/gps_mission", &Planner::gps_mission_service, &heron_mission);
    ros::ServiceServer traj_mission_server = nh.advertiseService("planner/traj_mission", &Planner::traj_mission_service, &heron_mission);
    ros::ServiceServer dwa_mission_server = nh.advertiseService("planner/dwa_mission", &Planner::dwa_mission_service, &heron_mission);

    ros::Timer control_update = nh.createTimer(ros::Duration(1./20.), &Planner::control_update, &heron_mission);
    ros::Timer mission_update = nh.createTimer(ros::Duration(heron_mission.DT_MISSION_), &Planner::mission_update, &heron_mission);
    ros::Timer console_update = nh.createTimer(ros::Duration(2.0), &Planner::console_update, &heron_mission);

    ros::spin();

    return 0;
}
