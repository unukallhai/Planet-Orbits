#pragma once

#include <cmath>
#include <fstream>

#include "consts.h"

using namespace std;


double* OrbitParams(double scale, double dt, double M1, double M2, double X1, double X2, double Y1, double Y2, double x0, double y0, double vx0, double vy0)
{ 
    double r1 = pow(pow(x0 - X1, 2) + pow(y0 - Y1, 2), 0.5);
    double r2 = pow(pow(x0 - X2, 2) + pow(y0 - Y2, 2), 0.5);

    double vx = vx0 - G * (M1 * (x0 - X1) * dt / pow(r1, 3) + M2 * (x0 - X2) * dt / pow(r2, 3));
    double vy = vy0 - G * (M1 * (y0 - Y1) * dt / pow(r2, 3) + M2 * (y0 - Y2) * dt / pow(r2, 3));
    double x = x0 + vx * dt;
    double y = y0 + vy * dt;

    double* coords = new double[4] {x, y, vx, vy};
    return coords;
}
void OrbitMain(double scale, double n, double dt, double X_center, double Y_center, double X1, double X2, double Y1, double Y2, double M1, double M2, double r_planet, double vy0)
{
    G = Gf(scale);

    double MM = M1 + M2;

    double x0 = X_center - r_planet * scale;
    double y0 = Y_center;

    double vx0 = 0;

    double T = 2 * PI * pow(r_planet * au, 1.5) / pow(G0 * MM * SolMass, 0.5);
    double TT = n * T;
    dt = dt * 3600 * 24;

    int flag = int(TT / dt / 5000)+1;
    int i = 1;
    double* coords;

    ofstream out;
    out.open("orbit.txt", ios::ate);
    if (TT / 3600 / 24 < 365.25)
    {
        out <<  TT / 3600 / 24  << " суток" << endl;
    }
    else
    {
        out << TT / 365.25 / 3600 / 24 << " лет" << endl;
    }

    out << x0 << " " << y0 << endl;

    if (flag == 1)
    {
        while (0 < TT)
        {
            coords = OrbitParams(scale, dt, M1, M2, X1, X2, Y1, Y2, x0, y0, vx0, vy0);
            x0 = coords[0];
            y0 = coords[1];
            vx0 = coords[2];
            vy0 = coords[3];
            out << x0 << " " << y0 << endl;
            TT = TT - dt;
        }
    }
    else
    {
        while (0 < TT)
        {
            coords = OrbitParams(scale, dt, M1, M2, X1, X2, Y1, Y2, x0, y0, vx0, vy0);
            x0 = coords[0];
            y0 = coords[1];
            vx0 = coords[2];
            vy0 = coords[3];

            if (i % flag == 0)
            {
                out << x0 << " " << y0 << endl;
            }
            TT = TT - dt;
            i = i + 1;
        }
    }
    out.close();
}