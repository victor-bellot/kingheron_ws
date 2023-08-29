#include <ros/ros.h>

#include "sensor_msgs/LaserScan.h"
#include "sensor_msgs/PointCloud.h"

#include "laser_geometry/laser_geometry.h"


void scanCallback(const sensor_msgs::LaserScan::ConstPtr& msg);

laser_geometry::LaserProjection projector_;
ros::Publisher cloud_pub;
