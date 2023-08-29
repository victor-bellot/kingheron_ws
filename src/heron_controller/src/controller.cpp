/**
Software License Agreement (BSD)

\file      controller.cpp
\copyright Copyright (c) 2014, Clearpath Robotics, Inc., All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that
the following conditions are met:
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the
   following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
   following disclaimer in the documentation and/or other materials provided with the distribution.
 * Neither the name of Clearpath Robotics nor the names of its contributors may be used to endorse or promote
   products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WAR-
RANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, IN-
DIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "heron_controller/controller.h"

Controller::Controller(ros::NodeHandle &n) : node_(n)
{
    force_compensator_ = new ForceCompensator(node_);

    //Assume no messages are being received. Don't send out anything new until commands are received
    control_mode = NO_CONTROL;

    ros::NodeHandle prv_node_("~");

    active_control_srv = node_.advertiseService("activate_control", &Controller::activate_control_service, this);
    is_active_control_ = false;

    //Timeouts for sensors
    //if no data has been received in a while, disable certain PID controls
    prv_node_.param<double>("timeout/imu_data", imu_data_timeout_, 1 / 5.0);
    imu_data_time_ = 0;
    imu_timeout_ = true;

    prv_node_.param<double>("timeout/vel_data", vel_data_timeout_, 1 / 5.0);
    vel_data_time_ = 0;
    vel_timeout_ = true;

    //Timeouts for control formats
    //if no command has been received in a while, stop sending drive commands
    prv_node_.param<double>("timeout/course_cmd", course_cmd_timeout_, 0.5);
    course_cmd_time_ = 0;

    prv_node_.param<double>("timeout/helm_cmd", helm_cmd_timeout_, 0.5);
    helm_cmd_time_ = 0;

    prv_node_.param<double>("timeout/wrench_cmd", wrench_cmd_timeout_, 0.5);
    wrench_cmd_time_ = 0;

    //If the commands don't show up in this much time don't send out drive commands
    prv_node_.param<double>("timeout/twist_cmd", twist_cmd_timeout_, 0.5);
    twist_cmd_time_ = 0;

    // If PID dt exceeds pid_timeout_, maybe it's this first call
    pid_timeout_ = 0.1;

    //Setup Fwd Vel Controller
    fvel_dbg_pub_ = node_.advertise<geometry_msgs::Vector3>("fwd_vel_debug", 1000);
    prv_node_.param<double>("fwd_vel/kf", fvel_kf_, 10); //Feedforward Gain
    prv_node_.param<double>("fwd_vel/kp", fvel_kp_, 90.0);  //Proportional Gain
    prv_node_.param<double>("fwd_vel/ki", fvel_ki_, 0.0); //Integral Gain
    prv_node_.param<double>("fwd_vel/kd", fvel_kd_, 1.0); //Derivative Gain
    prv_node_.param<double>("fwd_vel/imax", fvel_imax_, 0.0); //Clamp Integral Outputs
    prv_node_.param<double>("fwd_vel/imin", fvel_imin_, 0.0);
    fvel_meas_ = 0;
    fvel_time_ = ros::Time::now().toSec();

    //Setup Yaw Rate Controller
    yr_dbg_pub_ = node_.advertise<geometry_msgs::Vector3>("yaw_rate_debug", 1000);
    prv_node_.param<double>("yaw_rate/kf", yr_kf_, 10); //Feedforward Gain
    prv_node_.param<double>("yaw_rate/kp", yr_kp_, 2.0);  //Proportional Gain
    prv_node_.param<double>("yaw_rate/ki", yr_ki_, 0.0); //Integral Gain
    prv_node_.param<double>("yaw_rate/kd", yr_kd_, 1.0); //Derivative Gain
    prv_node_.param<double>("yaw_rate/imax", yr_imax_, 0.0); //Clamp Integral Outputs
    prv_node_.param<double>("yaw_rate/imin", yr_imin_, 0.0);
    yr_meas_ = 0;
    yr_time_ = ros::Time::now().toSec();

    //Setup Yaw Controller
    y_dbg_pub_ = node_.advertise<geometry_msgs::Vector3>("yaw_debug", 1000);
    prv_node_.param<double>("yaw/kf", y_kf_, 5.0); //Feedforward Gain
    prv_node_.param<double>("yaw/kp", y_kp_, 5.0);  //Proportional Gain
    prv_node_.param<double>("yaw/ki", y_ki_, 0.5); //Integral Gain
    prv_node_.param<double>("yaw/kd", y_kd_, 1.0); //Derivative Gain
    prv_node_.param<double>("yaw/imax", y_imax_, 0.0); //Clamp Integral output at max yaw torque
    prv_node_.param<double>("yaw/imin", y_imin_, 0.0);
    y_meas_ = 0;
    y_time_ = ros::Time::now().toSec();

    ROS_DEBUG("Fwd Vel Params (F,P,I,D,iMax,iMin):%f,%f,%f,%f,%f,%f", fvel_kf_, fvel_kp_, fvel_ki_, fvel_kd_,
              fvel_imax_, fvel_imin_);
    ROS_DEBUG("Yaw Rate Params (F,P,I,D,iMax,iMin):%f,%f,%f,%f,%f,%f", yr_kf_, yr_kp_, yr_ki_, yr_kd_, yr_imax_,
              yr_imin_);
    ROS_DEBUG("Yaw Params (F,P,I,D,iMax,iMin):%f,%f,%f,%f,%f,%f", y_kf_, y_kp_, y_ki_, y_kd_, y_imax_, y_imin_);

    //Setup velocity Controller
    fvel_pid_.reset();
    fvel_pid_.initPid(fvel_kp_, fvel_ki_, fvel_kd_, fvel_imax_, fvel_imin_);
    fvel_cmd_ = 0;

    //Setup Yaw rate Controller
    yr_pid_.reset();
    yr_pid_.initPid(yr_kp_, yr_ki_, yr_kd_, yr_imax_, yr_imin_);
    yr_cmd_ = 0;

    //Setup Yaw Controller
    y_pid_.reset();
    y_pid_.initPid(y_kp_, y_ki_, y_kd_, y_imax_, y_imin_);
    y_cmd_ = 0;

    prv_node_.param<double>("max/fwd_vel", max_fwd_vel_, MAX_FWD_VEL);
    prv_node_.param<double>("max/fwd_force", max_fwd_force_, 2 * MAX_FWD_THRUST); // 2 thrusters
    prv_node_.param<double>("max/bck_vel", max_bck_vel_, MAX_BCK_VEL);
    prv_node_.param<double>("max/bck_force", max_bck_force_, 2 * MAX_BCK_THRUST);

    prv_node_.param<double>("cov_limits/velocity", vel_cov_limit_, 0.28);
    prv_node_.param<double>("cov_limits/imu", imu_cov_limit_, 1.0);

    force_output_.force.x = 0;
    force_output_.force.y = 0;
    force_output_.force.z = 0;

    force_output_.torque.x = 0;
    force_output_.torque.y = 0;
    force_output_.torque.z = 0;

    min_abs_force_ = max_fwd_force_ * 0.06;  // in Newton?
    min_abs_torque_ = 0.2;  // in Newton.meter?
}

double Controller::fvel_compensator()
{
    //calculate pid force X
    double current_time = ros::Time::now().toSec();

    double fvel_error = fvel_cmd_ - fvel_meas_;
    double fvel_dt = std::min(current_time - fvel_time_, pid_timeout_);
    fvel_time_ = current_time;

    double fvel_comp_output = fvel_pid_.computeCommand(fvel_error, ros::Duration(fvel_dt));
    fvel_comp_output = fvel_comp_output + fvel_kf_ * fvel_cmd_;

    geometry_msgs::Vector3 dbg_info;
    dbg_info.x = fvel_cmd_;
    dbg_info.y = fvel_meas_;
    dbg_info.z = fvel_comp_output;
    fvel_dbg_pub_.publish(dbg_info);

    return fvel_comp_output;
}

double Controller::yr_compensator()
{
    //calculate pid torque z
    double current_time = ros::Time::now().toSec();

    double yr_error = yr_cmd_ - yr_meas_;
    double yr_dt = std::min(current_time - yr_time_, pid_timeout_);
    yr_time_ = current_time;

    double yr_comp_output = yr_pid_.computeCommand(yr_error, ros::Duration(yr_dt));
    yr_comp_output = yr_comp_output + yr_kf_ * yr_cmd_; //feedforward

    geometry_msgs::Vector3 dbg_info;
    dbg_info.x = yr_cmd_;
    dbg_info.y = yr_meas_;
    dbg_info.z = yr_comp_output;
    yr_dbg_pub_.publish(dbg_info);

    return yr_comp_output;
}

double Controller::y_compensator()
{
    //calculate pid torque z
    double current_time = ros::Time::now().toSec();

    double y_error = sawtooth(y_cmd_ - y_meas_);
    double y_dt = std::min(current_time - y_time_, pid_timeout_);
    y_time_ = current_time;

    double y_comp_output = y_pid_.computeCommand(y_error, ros::Duration(y_dt));

    geometry_msgs::Vector3 dbg_info;
    dbg_info.x = y_error;
    dbg_info.y = y_meas_;
    dbg_info.z = y_comp_output;
    y_dbg_pub_.publish(dbg_info);

    return y_comp_output;
}

void Controller::fwd_vel_mapping()
{
    if (fvel_cmd_ > 0)
        force_output_.force.x = dead_zone(max_fwd_force_ * fvel_cmd_ / max_fwd_vel_, min_abs_force_);
    else
        force_output_.force.x = dead_zone(max_bck_force_ * fvel_cmd_ / max_bck_vel_, min_abs_force_);
}

void Controller::update_fwd_vel_control()
{
    force_output_.force.x = dead_zone(fvel_compensator(), min_abs_force_);
}

void Controller::update_yaw_rate_control()
{
    force_output_.torque.z = dead_zone(yr_compensator(), min_abs_torque_);
}

void Controller::update_yaw_control()
{
    yr_cmd_ = y_compensator();
    force_output_.torque.z = dead_zone(yr_compensator(), min_abs_torque_);
}

//Callback to receive twist msgs (cmd_vel style)
void Controller::twist_callback(const geometry_msgs::Twist msg)
{
    yr_cmd_ = msg.angular.z;
    update_yaw_rate_control();

    fvel_cmd_ = msg.linear.x;
    if (control_mode == TWIST_LIN_CONTROL)
        fwd_vel_mapping();
    else
        update_fwd_vel_control();

    twist_cmd_time_ = ros::Time::now().toSec();
}

//Callback to receive raw wrench commands (force along x axis and torque about z axis).
void Controller::wrench_callback(const geometry_msgs::Wrench msg)
{
    force_output_.force.x = msg.force.x;
    force_output_.torque.z = msg.torque.z;
    wrench_cmd_time_ = ros::Time::now().toSec();
}

//Callback for yaw command which receives a yaw (rad) and speed (m/s) command
void Controller::course_callback(const heron_msgs::Course msg)
{
    // Save yaw command
    y_cmd_ = msg.yaw;

    // Save speed command
    fvel_cmd_ = msg.speed;

    course_cmd_time_ = ros::Time::now().toSec();
}

//Callback for helm commands which receives a thrust percentage (0..1) and a yaw rate (rad/s)
void Controller::helm_callback(const heron_msgs::Helm msg)
{
    //Calculate Thrust control
    double thrust = msg.thrust;
    if (thrust >= 0)
        force_output_.force.x = thrust * (max_fwd_force_ / 1);
    else
        force_output_.force.x = thrust * (max_bck_force_ / 1);

    //Save yaw rate command to be processed when feedback is available
    yr_cmd_ = msg.yaw_rate;

    helm_cmd_time_ = ros::Time::now().toSec();
}

//ENU
void Controller::odom_callback(const nav_msgs::Odometry msg)
{
    // indices : 0 <-> x ; 7 <-> y ; 35 <-> yaw
    // pose <-> x, y & yaw ; twist <-> x_dot, y_dot & yaw_dot

    //check if navsat/vel is being integrated into odometry
    if (msg.twist.covariance[0] < vel_cov_limit_ && msg.twist.covariance[7] < vel_cov_limit_)
        vel_data_time_ = ros::Time::now().toSec();

    //check if imu/data is being integrated into odometry
    if (msg.pose.covariance[35] < imu_cov_limit_ && msg.twist.covariance[35] < imu_cov_limit_)
        imu_data_time_ = ros::Time::now().toSec();

    y_meas_ = tf::getYaw(msg.pose.pose.orientation);
    yr_meas_ = msg.twist.twist.angular.z;
    fvel_meas_ = msg.twist.twist.linear.x * std::cos(y_meas_) + msg.twist.twist.linear.y * std::sin(y_meas_);

    switch (control_mode)
    {
        case COURSE_CONTROL:
            update_fwd_vel_control();
            update_yaw_control();
            break;
        case HELM_CONTROL:
            update_yaw_rate_control();
            break;
        case WRENCH_CONTROL:
            break;
        case TWIST_CONTROL:
            update_fwd_vel_control();
            update_yaw_rate_control();
            break;
        case TWIST_LIN_CONTROL:
            update_yaw_rate_control();
            break;
        case NO_CONTROL:
            break;
    }
}

void Controller::console_update(const ros::TimerEvent &event)
{
    std::string output = "";
    switch (control_mode)
    {
        case COURSE_CONTROL:
            output = "Boat controlling yaw position";
            break;
        case HELM_CONTROL:
            output = "Boat Controlling yaw rate";
            break;
        case WRENCH_CONTROL:
            output = "Boat in raw wrench/RC control";
            break;
        case TWIST_CONTROL:
            output = "Boat controlling forward and yaw velocity";
            break;
        case TWIST_LIN_CONTROL:
            output = "Boat controlling yaw velocity and mapping velocity linearly";
            break;
        case NO_CONTROL:
            output = "No commands being processed";
            break;
        default:
            break;
    }

    if (imu_timeout_)
        output += ": IMU data not received or being received too slowly";

    if (vel_timeout_)
        output += ": GPS Velocity data not received or being received too slowly";

    ROS_INFO("%s", output.c_str());
}

void Controller::control_update(const ros::TimerEvent &event)
{
    imu_timeout_ = (ros::Time::now().toSec() - imu_data_time_ > imu_data_timeout_);
    vel_timeout_ = (ros::Time::now().toSec() - vel_data_time_ > vel_data_timeout_);

    if (is_active_control_)
    {
        std::vector<double> find_latest;

        find_latest.push_back(wrench_cmd_time_);

        if (!imu_timeout_)
        {
            find_latest.push_back(twist_cmd_time_);
            find_latest.push_back(helm_cmd_time_);
        }

        if (!imu_timeout_ && !vel_timeout_)
            find_latest.push_back(course_cmd_time_);

        double max = *std::max_element(find_latest.begin(), find_latest.end());

        if (max == 0)
        {
            control_mode = NO_CONTROL;
            force_output_.torque.z = 0;
            force_output_.force.x = 0;
            return;
        } else if (max == twist_cmd_time_ && !imu_timeout_ && !vel_timeout_) {
            control_mode = TWIST_CONTROL;
        } else if (max == twist_cmd_time_ && !imu_timeout_) {
            control_mode = TWIST_LIN_CONTROL;
        } else if (max == course_cmd_time_ && !imu_timeout_ && !vel_timeout_) {
            control_mode = COURSE_CONTROL;
        } else if (max == helm_cmd_time_ && !imu_timeout_) {
            control_mode = HELM_CONTROL;
        } else if (max == wrench_cmd_time_) {
            control_mode = WRENCH_CONTROL;
        }
    } else
    {
        control_mode = NO_CONTROL;
        force_output_.torque.z = 0;
        force_output_.force.x = 0;
    }

    force_compensator_->pub_thrust_cmd(force_output_);
}

bool Controller::activate_control_service(std_srvs::SetBool::Request &req, std_srvs::SetBool::Response &resp)
{
    is_active_control_ = req.data;
    resp.success = is_active_control_;
    resp.message = "Activated Control state updated.";
    return true;
}

double sawtooth(double alpha)
{
    return 2.0 * atan(tan(alpha / 2.0));
}

double dead_zone(double action, double limit)
{
    if (action > 0)
    {
        if (action < +limit)
            return 0;
    } else
    {
        if (action > -limit)
            return 0;
    }

    return action;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "controller");
    ros::NodeHandle nh;
    Controller heron_control(nh);

    ros::Subscriber twist_sub = nh.subscribe("cmd_vel", 1, &Controller::twist_callback, &heron_control);
    ros::Subscriber wrench_sub = nh.subscribe("cmd_wrench", 1, &Controller::wrench_callback, &heron_control);
    ros::Subscriber helm_sub = nh.subscribe("cmd_helm", 1, &Controller::helm_callback, &heron_control);
    ros::Subscriber course_sub = nh.subscribe("cmd_course", 1, &Controller::course_callback, &heron_control);

    ros::Subscriber odom_sub = nh.subscribe("odometry/filtered", 1, &Controller::odom_callback, &heron_control);

    ros::Timer control_output = nh.createTimer(ros::Duration(1/20.0), &Controller::control_update, &heron_control);
    ros::Timer console_update = nh.createTimer(ros::Duration(1.0), &Controller::console_update, &heron_control);

    ros::spin();

    return 0;
}
