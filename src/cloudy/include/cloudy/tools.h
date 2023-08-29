#ifndef TOOLS_H
#define TOOLS_H

#include <cmath>
#include <string>
#include <vector>
#include <algorithm>

#define EPS 0.01  // define a very small constant under which we can consider values to be zero


double sawtooth(double alpha);
double angular_gap(double alpha, double beta);

double norm2(double x, double y);
double pythagore(double c, double a);

double xor_fun(double u, double v);
double filter(std::vector<double> &tab);

void change_component(double &x, double &y, double rho);
void change_frame(double &x, double &y, double tx, double ty);
void transpose(double &x, double &y, double tx, double ty, double rho);

#endif  // TOOLS_H
