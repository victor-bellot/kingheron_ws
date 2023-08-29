#include <ros/ros.h>
#include <tf/tf.h>
#include <vector>
#include <cmath>

#include "string.h"

#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Vector3.h"
#include "geometry_msgs/Wrench.h"
#include "heron_msgs/Drive.h"
#include "heron_msgs/Course.h"
#include "gazebo_msgs/ModelState.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/ChannelFloat32.h"

#include "std_srvs/SetBool.h"
#include "heron_srvs/GpsMission.h"
#include "heron_srvs/AskModel.h"
#include "heron_srvs/DwaMission.h"
#include "gazebo_msgs/DeleteModel.h"
#include "gazebo_msgs/SetModelState.h"

#include "cloudy/obstacle_cloud.h"
#include "cloudy/tools.h"  // for EPS, sawtooth & abs_ang_dif

#define DERIV_TIME_STEP 0.5  // in seconds : used to estimate time derivatives of trajectory

enum MissionType
{
    WAITING,
    NO_MISSION,
    GPS_MISSION,
    TRAJ_MISSION,
    DWA_MISSION,
};

enum OutputType
{
    NONE,
    COURSE,
    WRENCH,
    TWIST,
};

struct Pose
{
    double x, y, h;
};

struct Dynamic
{
    double v, w, sf;
    double score;
};

struct RobotState
{
    Pose pos;
    Dynamic dyn;
};

struct DynamicParameter
{
    int count;
    double vel, min, max;
};

class Planner {
private:
    ros::NodeHandle node_;
    
    // Obstacle manager
    ObstacleCloud *obstacle_cloud_;

    // Debug publishers
    ros::Publisher mission_debug_pub_;
    ros::Publisher trajectory_pub_;

    // GPS mission publishes Course (vel + yaw)
    ros::Publisher cmd_course_pub_;
    heron_msgs::Course course_output_;

    // Trajectory mission publishes Wrench (force + torque)
    ros::Publisher cmd_wrench_pub_;
    geometry_msgs::Wrench wrench_output_;

    // DWA mission publishes Twist (vel + lac)
    ros::Publisher cmd_twist_pub_;
    geometry_msgs::Twist twist_output_;

    MissionType mission_request_;
    MissionType mission_type_;
    OutputType output_type_;

    // GPS position Feedback timeout
    double gps_time_, GPS_TIMEOUT_;
    double POS_COV_LIMIT_;
    bool gps_timed_out_;

    // IMU yaw & yaw_rate (lacet) Feedback timeout
    double imu_time_, IMU_TIMEOUT_;
    double YAW_COV_LIMIT_, LAC_COV_LIMIT_;
    bool imu_timed_out_;

    // Velocity estimation Feedback timeout
    double velest_time_, VELEST_TIMEOUT_;
    double VEL_COV_LIMIT_;
    bool velest_timed_out_;

    ///////////////////////////////////////
    ////////// MISSION CONSTANTS //////////

    // GPS mission constants
    double DIST_TO_SUCCESS_;
    double TURN_CONE_;

    // Trajectory mission constants
    double TORQUE_SCALE_YAW_, FORCE_SCALE_YAW_;
    double CMP_SCALE_E0_, CMP_SCALE_E1_;
    double DRAG_COEF_;

    // Dynamic Window Approach constants
    double SIM_TIME_;

    DynamicParameter LINEAR_VELOCITY_;
    DynamicParameter ANGULAR_VELOCITY_;
    DynamicParameter SECURITY_FACTOR_;
    
    int TREE_SEARCH_DEPTH_;
    double FUTUR_IMPACT_, KVW_;
    double NO_COLLISION_REWARD_, ROBOT_RADIUS_;
    double B_GOAL_, B_OBSTACLE_, B_SPEED_, B_SECURITY_;

    ///////////////////////////////////////

    std::string current_buoy_;
    int buoy_id_;

    // Command & measure variables
    double x_cmd_, y_cmd_, vel_cmd_;
    double x_meas_, y_meas_, vel_meas_;
    double yaw_meas_, lac_meas_;
    double sf_chos_;

    std::vector<Dynamic> plan_;

    ros::ServiceClient activate_control_client_;

    ros::ServiceClient spawn_buoy_client_;
    ros::ServiceClient delete_model_client_;
    ros::ServiceClient set_model_state_client_;

public:
    double DT_MISSION_;

    Planner(ros::NodeHandle &n);
    ~Planner() {}

    void update_gps_mission();
    void update_traj_mission();
    void update_dwa_mission();

    double eval_robot_state(const RobotState &parent, int depth=0);
    double eval_dynamic(const RobotState &parent, const Dynamic &dynamic, int depth);
    double objective_function(double goal_heading, double goal_distance, double dist_to_obstacle, double speed);
    
    void publish_plan();
    double heading_error(Pose pose);

    bool activate_control(bool activated);

    bool delete_buoy();
    bool spawn_buoy(double x=0.0, double y=0.0);
    bool set_buoy_position(double x, double y);

    // Get new data & call update control method given the current control mode
    void odom_callback(const nav_msgs::Odometry msg);

    // Update control mode
    void control_update(const ros::TimerEvent &event);
    void mission_update(const ros::TimerEvent &event);
    void console_update(const ros::TimerEvent &event);

    bool gps_mission_service(heron_srvs::GpsMission::Request &req, heron_srvs::GpsMission::Response &resp);
    bool traj_mission_service(std_srvs::SetBool::Request &req, std_srvs::SetBool::Response &resp);
    bool dwa_mission_service(heron_srvs::DwaMission::Request &req, heron_srvs::DwaMission::Response &resp);

    void get_min_and_step(const DynamicParameter &dp, double x, double &min_x, double &step_x);
};

void normalize_parameter(const DynamicParameter &dp, double &x);

void dd_trajectory(double t, double &dd_xt, double &dd_yt);
void d_trajectory(double t, double &d_xt, double &d_yt);
void trajectory(double t, double &xt, double &yt);
