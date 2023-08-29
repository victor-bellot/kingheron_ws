#include "cloudy/tools.h"

double sawtooth(double alpha)
{
    return 2.0 * atan(tan(alpha / 2.0));
}

double angular_gap(double alpha, double beta)
{
    return fabs(sawtooth(alpha - beta));
}

double norm2(double x, double y)
{
    return sqrt(pow(x, 2.) + pow(y, 2.));
}

double pythagore(double c, double a)
{
    return sqrt(pow(c, 2.) - pow(a, 2.));
}

double xor_fun(double u, double v)
{
    return (u + v) / (1. + u * v);
}

double filter(std::vector<double> &tab)
{
    std::sort(tab.begin(), tab.end());

    int k_down = std::ceil(tab.size()/2.);
    int k_up = tab.size();

    double acc = 0.;
    for (int k=k_down; k<k_up; k++)
        acc += tab[k];
    
    return (k_down < k_up ? acc / (k_up - k_down) : 0.);
}

void change_component(double &x, double &y, double rho)
// IN: (x,y) -> coodinates relative to frame X expressed in 1
//     rho -> rotation angle from 0 to 1 (along the invariant z axis)
// OUT: (x,y) -> coordinates relative to frame X expressed in 0
{
    double z = x;  // save x
    x = x * cos(rho) - y * sin(rho);
    y = z * sin(rho) + y * cos(rho);
}

void change_frame(double &x, double &y, double tx, double ty)
// IN: (x,y) -> coordinates relative to frame 1 expressed in X
//     (tx,ty) -> coordinates of the origin of 1 relative to 0 expressed in X
// OUT: (x,y) -> coordinates relative to frame 0 expressed in X
{
    x += tx;
    y += ty;
}

void transpose(double &x, double &y, double tx, double ty, double rho)
// IN: (x,y) -> coodinates relative to frame 1 expressed in 1
//     (tx,ty) -> coordinates of the origin of 1 relative to 0 expressed in 0
//     rho -> rotation angle from 0 to 1 (along the invariant z axis)
// OUT: (x,y) -> coordinates relative to frame 0 expressed in 0
{
    change_component(x, y, rho);
    change_frame(x, y, tx, ty);
}
