#ifndef OBSTACLE_CLOUD_H
#define OBSTACLE_CLOUD_H

#include <cmath>
#include <vector>
#include <ros/ros.h>

#include "sensor_msgs/PointCloud.h"
#include "geometry_msgs/Point32.h"
#include "geometry_msgs/PointStamped.h"

#include "tf/transform_datatypes.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"

#include "cloudy/trajectory_circle.h"
#include "cloudy/tools.h"


class ObstacleCloud
{
private:
    ros::NodeHandle node_;
    ros::Subscriber cloud_sub_;

    ros::Duration dt_;
    ros::Time last_update_;

    std::vector<geometry_msgs::Point32> cloud_;

    tf2_ros::Buffer buffer_;
    tf2_ros::TransformListener tf2_;
    std::string frame_in_, frame_out_;

public:
    ObstacleCloud(ros::NodeHandle &n, double f);
    ~ObstacleCloud() {}

    // Get last lidar observation -> point cloud of the nearest obstacles
    void cloud_callback(const sensor_msgs::PointCloud msg);

    double obstacle_tack(double sector);

    bool segment_clear(double from_goal_to_robot_heading, double length, double security_distance);

    double distance_to_obstacle(double xv, double yv, double hv,
                                double v, double w, double security_distance);
};

#endif  // OBSTACLE_CLOUD_H
