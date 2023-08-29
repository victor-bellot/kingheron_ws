#include "cloudy/scan_to_cloud.h"


void scanCallback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
    sensor_msgs::PointCloud cloud;
    projector_.projectLaser(*msg, cloud);
    cloud_pub.publish(cloud);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "scan_to_cloud");

    ros::NodeHandle parameter_node("~");
    std::string scan_topic_name;
    parameter_node.param<std::string>("scan_topic_name", scan_topic_name, "laser/scan");

    ros::NodeHandle node_handle;

    ros::Subscriber scan_sub = node_handle.subscribe(scan_topic_name, 1000, scanCallback);
    cloud_pub = node_handle.advertise<sensor_msgs::PointCloud>("laser/cloud", 1000);

    ros::spin();

    return 0;
}
