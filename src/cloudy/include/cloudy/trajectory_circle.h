#ifndef TRAJECTORY_CIRCLE_H
#define TRAJECTORY_CIRCLE_H

#include <cmath>
#include "cloudy/tools.h"


class TrajectoryCircle
{
private:
    double lateral_offset_;
    double radius_, dr_;
    double sign_;

public:
    ~TrajectoryCircle() {}
    TrajectoryCircle(double v, double w, double dr);

    double distance(double x, double y);
};

#endif  // TRAJECTORY_CIRCLE_H
