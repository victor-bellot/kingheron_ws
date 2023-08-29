#include "cloudy/trajectory_circle.h"


TrajectoryCircle::TrajectoryCircle(double v, double w, double dr) :
// Create a trajectory circle from a dynamic (v != 0, w != 0)
// Expressed in the frame of the current virtual robot
lateral_offset_(v/w), radius_(fabs(lateral_offset_)), dr_(dr), sign_(lateral_offset_/radius_) {}

double TrajectoryCircle::distance(double x, double y)
{
    double r = norm2(x, y - lateral_offset_);

    if (dr_ >= radius_ + r)
        return 0.;  // collision : this circle can't achieve security distance dr_

    if (radius_ - dr_ < r && r < radius_ + dr_)
    {
        double r_mul = radius_ * r;
        double r_div = radius_ / r;

        double p_angle = std::atan2(fabs(x), sign_ * (lateral_offset_ - y));
        double s_angle = std::acos((r_div - pow(dr_,2.)/r_mul + 1./r_div) / 2.);  // Al-Kashi

        return radius_ * (p_angle - s_angle);
    }
    
    return HUGE_VAL;
}
