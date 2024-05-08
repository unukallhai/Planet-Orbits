#pragma once

//число пи
const double PI = 3.1415926;

// гравитационная постоянная, м^3/(кг*с^2)
double G;
const double G0 = 6.6743e-11;

// астрономическая единица, м
const double au = 149597870700;

// радиус Солнца, ае
const double sol = 0.00465047;
// масса Солнца, кг
const double SolMass = 2e30;
// эффективная температура Солнца, К
const double SolTemp = 5772;


double Gf(double scale)
{
    return G0 / pow(au, 3) * pow(scale, 3) * SolMass;
}
