#pragma once

//����� ��
const double PI = 3.1415926;

// �������������� ����������, �^3/(��*�^2)
double G;
const double G0 = 6.6743e-11;

// ��������������� �������, �
const double au = 149597870700;

// ������ ������, ��
const double sol = 0.00465047;
// ����� ������, ��
const double SolMass = 2e30;
// ����������� ����������� ������, �
const double SolTemp = 5772;


double Gf(double scale)
{
    return G0 / pow(au, 3) * pow(scale, 3) * SolMass;
}
