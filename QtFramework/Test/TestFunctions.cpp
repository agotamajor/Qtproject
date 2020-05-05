#include <cmath>
#include "TestFunctions.h"
#include "../Core/Constants.h"

using namespace cagd;
using namespace std;


GLdouble spiral_on_cone::u_min = -TWO_PI;
GLdouble spiral_on_cone::u_max = +TWO_PI;

DCoordinate3 spiral_on_cone::d0(GLdouble u)
{
    return DCoordinate3(u * cos(u), u * sin(u), u);
}

DCoordinate3 spiral_on_cone::d1(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    return DCoordinate3(c - u * s, s + u * c, 1.0);
}

DCoordinate3 spiral_on_cone::d2(GLdouble u)
{
    GLdouble c = cos(u), s = sin(u);
    return DCoordinate3(-2.0 * s - u * c, 2.0 * c - u * s, 0);
}


GLdouble rose_curve1::u_min = -TWO_PI;
GLdouble rose_curve1::u_max = +TWO_PI;
GLuint rose_curve1::k = 8;

DCoordinate3 rose_curve1::d0(GLdouble u)
{
    return DCoordinate3(cos(k*u)*cos(u), cos(k*u)*sin(u), u);
}

DCoordinate3 rose_curve1::d1(GLdouble u)
{
    return DCoordinate3((-1)*k*sin(k*u)*cos(u)-cos(k*u)*sin(u), (-1)*k*sin(k*u)*sin(u)+cos(k*u)*cos(u), 1);
}

DCoordinate3 rose_curve1::d2(GLdouble u)
{
    return DCoordinate3((-1)*k*k*cos(k*u)*cos(u)+2*k*sin(k*u)*sin(u)-cos(k*u)*cos(u), (-1)*k*k*cos(k*u)*sin(u)-2*k*sin(k*u)*cos(u)-cos(k*u)*sin(u), 0);
}


GLdouble rose_curve2::u_min = -TWO_PI;
GLdouble rose_curve2::u_max = +TWO_PI;
GLuint rose_curve2::k = 2;

DCoordinate3 rose_curve2::d0(GLdouble u)
{
    return DCoordinate3(cos(k*u)*cos(u), cos(k*u)*sin(u), u);
}

DCoordinate3 rose_curve2::d1(GLdouble u)
{
    return DCoordinate3((-1)*k*sin(k*u)*cos(u)-cos(k*u)*sin(u), (-1)*k*sin(k*u)*sin(u)+cos(k*u)*cos(u), 1);
}

DCoordinate3 rose_curve2::d2(GLdouble u)
{
    return DCoordinate3((-1)*k*k*cos(k*u)*cos(u)+2*k*sin(k*u)*sin(u)-cos(k*u)*cos(u), (-1)*k*k*cos(k*u)*sin(u)-2*k*sin(k*u)*cos(u)-cos(k*u)*sin(u), 0);
}


GLdouble cycloid::u_min = -4*TWO_PI;
GLdouble cycloid::u_max = +4*TWO_PI;
GLdouble cycloid::a = 0.5;

DCoordinate3 cycloid::d0(GLdouble u)
{
     return DCoordinate3(a*(u-sin(u)), a*(1-cos(u)), u);
}

DCoordinate3 cycloid::d1(GLdouble u)
{
    return DCoordinate3(a*(1-cos(u)), a*sin(u), 1);
}

DCoordinate3 cycloid::d2(GLdouble u)
{
    return DCoordinate3(a*sin(u), a*cos(u), 0);
}



GLdouble nephroid::u_min = -TWO_PI;
GLdouble nephroid::u_max = +TWO_PI;

DCoordinate3 nephroid::d0(GLdouble u)
{
     return DCoordinate3(6*0.5*cos(u)-4*0.5*cos(u)*cos(u)*cos(u), 4*0.5*sin(u)*sin(u)*sin(u), 0);
}

DCoordinate3 nephroid::d1(GLdouble u)
{
    return DCoordinate3((-1)*6*0.5*sin(u)+12*0.5*cos(u)*cos(u)*sin(u), 12*0.5*sin(u)*sin(u)*cos(u),0);
}

DCoordinate3 nephroid::d2(GLdouble u)
{
    return DCoordinate3((-1)*6*0.5*cos(u)+12*0.5*((-1)*sin(2*u)*sin(u)+cos(u)*cos(u)*cos(u)), 12*0.5*(sin(2*u)*cos(u)-sin(u)*sin(u)*sin(u)),0);
}



GLdouble witch_of_agnesi::u_min = -TWO_PI;
GLdouble witch_of_agnesi::u_max = +TWO_PI;
GLdouble witch_of_agnesi::a = 2;

DCoordinate3 witch_of_agnesi::d0(GLdouble u)
{
     return DCoordinate3(a*0.5*tan(u), a*0.5*cos(u)*cos(u), 0);
}

DCoordinate3 witch_of_agnesi::d1(GLdouble u)
{
    return DCoordinate3(a*0.5/(cos(u)*cos(u)), (-1)*a*0.5*sin(2*u), 0);
}

DCoordinate3 witch_of_agnesi::d2(GLdouble u)
{
    return DCoordinate3(a*a*0.5*tan(u)/(cos(u)*cos(u)), (-1)*a*a*cos(2*u), 0);
}



GLdouble fermat_spiral::u_min = -TWO_PI;
GLdouble fermat_spiral::u_max = +TWO_PI;

DCoordinate3 fermat_spiral::d0(GLdouble u)
{
     return DCoordinate3(5*sqrt(u)*cos(u), 5*sqrt(u)*sin(u), u);
}

DCoordinate3 fermat_spiral::d1(GLdouble u)
{
    return DCoordinate3(5*(cos(u)/(2*sqrt(u))-sqrt(u)*sin(u)), 5*(sin(u)/(2*sqrt(u))+sqrt(u)*cos(u)), 1);
}

DCoordinate3 fermat_spiral::d2(GLdouble u)
{
    return DCoordinate3(5*((-1.0)*4*u*u*cos(u)+(-1.0)*4*u*sin(u)+(-1.0)*cos(u))/(4*u*sqrt(u)), 5*((-1)*4*u*u*sin(u)+4*u*cos(u)+(-1.0)*sin(u))/(4*u*sqrt(u)), 0);
}


GLdouble  lissajous_knockoff::u_min = -TWO_PI;
GLdouble  lissajous_knockoff::u_max = +TWO_PI;

DCoordinate3  lissajous_knockoff::d0(GLdouble u)
{
     return DCoordinate3(cos(3*u), sin(5*u), u);
}

DCoordinate3  lissajous_knockoff::d1(GLdouble u)
{
    return DCoordinate3((-1)*3*sin(3*u), 5*cos(5*u), 1);
}

DCoordinate3  lissajous_knockoff::d2(GLdouble u)
{
    return DCoordinate3((-1)*9*cos(3*u), (-1)*25*sin(5*u), 0);
}

GLdouble  sphere::u_min = 0;
GLdouble  sphere::u_max = +TWO_PI;
GLdouble  sphere::v_min = 0;
GLdouble  sphere::v_max = +PI;

GLdouble  sphere::a = 0.5;

DCoordinate3  sphere::d00(GLdouble u, GLdouble v)
{
    return DCoordinate3(a*cos(u)*sin(v), a*sin(u)*sin(v), a*cos(v));
}

DCoordinate3  sphere::d10(GLdouble u, GLdouble v)
{

    return DCoordinate3((-1)*a*sin(u)*sin(v), a*cos(u)*sin(v), 0);
}

DCoordinate3  sphere::d01(GLdouble u, GLdouble v)
{
    return DCoordinate3(a*cos(u)*cos(v), a*sin(u)*cos(v), (-1)*a*sin(v));
}



GLdouble  conoid::u_min = 0;
GLdouble  conoid::u_max = TWO_PI;
GLdouble  conoid::v_min = 0;
GLdouble  conoid::v_max = PI/2;


DCoordinate3  conoid::d00(GLdouble u, GLdouble v)
{
    return DCoordinate3(0.5*cos(u)*sin(2*v), 0.5*sin(u)*sin(2*v), 0.5*(cos(v)*cos(v)-cos(u)*cos(u)*sin(v)*sin(v)));
}

DCoordinate3  conoid::d10(GLdouble u, GLdouble v)
{

    return DCoordinate3((-1)*0.5*sin(u)*sin(2*v), 0.5*cos(u)*sin(2*v), 0.5*sin(v)*sin(v)*sin(2*u));
}

DCoordinate3  conoid::d01(GLdouble u, GLdouble v)
{
    return DCoordinate3(cos(u)*cos(2*v), sin(u)*cos(2*v), 0.5*((-1)*sin(2*v)-cos(u)*cos(u)*sin(2*v)));
}


GLdouble  boy::u_min = 0;
GLdouble  boy::u_max = +PI;
GLdouble  boy::v_min = 0;
GLdouble  boy::v_max = +PI;

GLdouble  boy::k = 1;
DCoordinate3  boy::d00(GLdouble u, GLdouble v)
{
    GLdouble K=cos(u)/(sqrt(2)-k*sin(2*u)*sin(3*v));
    return DCoordinate3(K*(cos(u)*cos(2*v)+sqrt(2)*sin(u)*cos(v)), K*(cos(u)*sin(2*v)-sqrt(2)*sin(u)*sin(v)), 3*K*cos(u));
}

DCoordinate3  boy::d10(GLdouble u, GLdouble v)
{
    return DCoordinate3((cos(u)*(sqrt(2)*cos(v)*cos(u)-cos(2*v)*sin(u)))/(sqrt(2)-k*sin(3*v)*sin(2*u)) + (-1)*(sin(u)*(sqrt(2)*cos(v)*sin(u)+cos(2*v)*cos(u)))/(sqrt(2)-k*sin(3*v)*sin(2*u))+(2*k*sin(3*v)*cos(u)*(sqrt(2)*cos(v)*sin(u)+cos(2*v)*cos(u))*cos(2*u))/((sqrt(2)+(-1)*k*sin(3*v)*sin(2*u))*(sqrt(2)+(-1)*k*sin(3*v)*sin(2*u))),
                        (cos(u)*((-1)*sqrt(2)*sin(v)*cos(u)-sin(2*v)*sin(u)))/(sqrt(2)-k*sin(3*v)*sin(2*u)) + (-1)*(sin(u)*((-1)*sqrt(2)*sin(v)*sin(u)+sin(2*v)*cos(u)))/(sqrt(2)-k*sin(3*v)*sin(2*u))+(2*k*sin(3*v)*cos(u)*((-1)*sqrt(2)*sin(v)*sin(u)+sin(2*v)*cos(u))*cos(2*u))/((sqrt(2)+(-1)*k*sin(3*v)*sin(2*u))*(sqrt(2)+(-1)*k*sin(3*v)*sin(2*u))),
                        (6*cos(u)*(k*sin(3*v)*sin(u)*sin(2*u)+k*sin(3*v)*cos(u)*cos(2*u)+(-1)*sqrt(2)*sin(u)))/((k*sin(3*v)*sin(2*u)-sqrt(2))*(k*sin(3*v)*sin(2*u)-sqrt(2))));
}

DCoordinate3  boy::d01(GLdouble u, GLdouble v)
{
    return DCoordinate3((cos(u)*((-1)*2*cos(u)*sin(2*v)+(-1)*sqrt(2)*sin(u)*sin(v)))/(sqrt(2)-k*sin(3*v)*sin(2*u))+(3*k*sin(2*u)*cos(u)*cos(3*v)*(sqrt(2)*cos(v)*sin(u)+cos(2*v)*cos(u))*cos(2*u))/((sqrt(2)+(-1)*k*sin(3*v)*sin(2*u))*(sqrt(2)+(-1)*k*sin(3*v)*sin(2*u))),
                        (cos(u)*((-1)*sqrt(2)*sin(u)*cos(v)+2*cos(2*v)*cos(u)))/(sqrt(2)-k*sin(3*v)*sin(2*u)) +(3*k*sin(2*u)*cos(u)*cos(3*v)*((-1)*sqrt(2)*sin(v)*sin(u)+sin(2*v)*cos(u))*cos(2*u))/((sqrt(2)+(-1)*k*sin(3*v)*sin(2*u))*(sqrt(2)+(-1)*k*sin(3*v)*sin(2*u))),
                        (9*k*cos(u)*cos(u)*sin(2*u)*cos(3*v))/((sqrt(2)-k*sin(2*u)*sin(3*v))*(sqrt(2)-k*sin(2*u)*sin(3*v))));
}

GLdouble  roman::u_min = 0;
GLdouble  roman::u_max = +PI;
GLdouble  roman::v_min = 0;
GLdouble  roman::v_max = +PI;

GLdouble  roman::k = 0;

DCoordinate3  roman::d00(GLdouble u, GLdouble v)
{
    GLdouble K=cos(u)/(sqrt(2)-k*sin(2*u)*sin(3*v));
    return DCoordinate3(K*(cos(u)*cos(2*v)+sqrt(2)*sin(u)*cos(v)), K*(cos(u)*sin(2*v)-sqrt(2)*sin(u)*sin(v)), 3*K*cos(u));
}

DCoordinate3  roman::d10(GLdouble u, GLdouble v)
{
    return DCoordinate3((cos(u)*(sqrt(2)*cos(v)*cos(u)-cos(2*v)*sin(u)))/(sqrt(2)-k*sin(3*v)*sin(2*u)) + (-1)*(sin(u)*(sqrt(2)*cos(v)*sin(u)+cos(2*v)*cos(u)))/(sqrt(2)-k*sin(3*v)*sin(2*u))+(2*k*sin(3*v)*cos(u)*(sqrt(2)*cos(v)*sin(u)+cos(2*v)*cos(u))*cos(2*u))/((sqrt(2)+(-1)*k*sin(3*v)*sin(2*u))*(sqrt(2)+(-1)*k*sin(3*v)*sin(2*u))),
                        (cos(u)*((-1)*sqrt(2)*sin(v)*cos(u)-sin(2*v)*sin(u)))/(sqrt(2)-k*sin(3*v)*sin(2*u)) + (-1)*(sin(u)*((-1)*sqrt(2)*sin(v)*sin(u)+sin(2*v)*cos(u)))/(sqrt(2)-k*sin(3*v)*sin(2*u))+(2*k*sin(3*v)*cos(u)*((-1)*sqrt(2)*sin(v)*sin(u)+sin(2*v)*cos(u))*cos(2*u))/((sqrt(2)+(-1)*k*sin(3*v)*sin(2*u))*(sqrt(2)+(-1)*k*sin(3*v)*sin(2*u))),
                        (6*cos(u)*(k*sin(3*v)*sin(u)*sin(2*u)+k*sin(3*v)*cos(u)*cos(2*u)+(-1)*sqrt(2)*sin(u)))/((k*sin(3*v)*sin(2*u)-sqrt(2))*(k*sin(3*v)*sin(2*u)-sqrt(2))));
}

DCoordinate3  roman::d01(GLdouble u, GLdouble v)
{
    return DCoordinate3((cos(u)*((-1)*2*cos(u)*sin(2*v)+(-1)*sqrt(2)*sin(u)*sin(v)))/(sqrt(2)-k*sin(3*v)*sin(2*u))+(3*k*sin(2*u)*cos(u)*cos(3*v)*(sqrt(2)*cos(v)*sin(u)+cos(2*v)*cos(u))*cos(2*u))/((sqrt(2)+(-1)*k*sin(3*v)*sin(2*u))*(sqrt(2)+(-1)*k*sin(3*v)*sin(2*u))),
                        (cos(u)*((-1)*sqrt(2)*sin(u)*cos(v)+2*cos(2*v)*cos(u)))/(sqrt(2)-k*sin(3*v)*sin(2*u)) +(3*k*sin(2*u)*cos(u)*cos(3*v)*((-1)*sqrt(2)*sin(v)*sin(u)+sin(2*v)*cos(u))*cos(2*u))/((sqrt(2)+(-1)*k*sin(3*v)*sin(2*u))*(sqrt(2)+(-1)*k*sin(3*v)*sin(2*u))),
                        (9*k*cos(u)*cos(u)*sin(2*u)*cos(3*v))/((sqrt(2)-k*sin(2*u)*sin(3*v))*(sqrt(2)-k*sin(2*u)*sin(3*v))));
}

GLdouble  torus::u_min = -TWO_PI;
GLdouble  torus::u_max = +TWO_PI;
GLdouble  torus::v_min = -TWO_PI;
GLdouble  torus::v_max = +TWO_PI;

GLdouble  torus::r = 2;
GLdouble  torus::R = 6;

DCoordinate3  torus::d00(GLdouble u, GLdouble v)
{
    return DCoordinate3((R+r*cos(v))*cos(u), (R+r*cos(v))*sin(u), r*sin(v));
}

DCoordinate3  torus::d10(GLdouble u, GLdouble v)
{
    return DCoordinate3((-1)*sin(u)*(r*cos(v)+R), cos(u)*(r*cos(v)+R), 1);
}

DCoordinate3  torus::d01(GLdouble u, GLdouble v)
{
    return DCoordinate3((-1)*r*cos(u)*sin(v), (-1)*r*sin(v)*sin(u), r*cos(v));
}


GLdouble  klein::u_min = -PI;
GLdouble  klein::u_max = +PI;
GLdouble  klein::v_min = -TWO_PI;
GLdouble  klein::v_max = +TWO_PI;

GLdouble  klein::a = 5.5;
GLdouble  klein::b = 9.3;

DCoordinate3  klein::d00(GLdouble u, GLdouble v)
{
    GLdouble r=4*(1-cos(u)/2);
    return DCoordinate3(a*cos(u)*(1+sin(u))+r*cos(u)*cos(v), b*sin(u)+r*sin(u)*cos(v), r*sin(v));
}

DCoordinate3  klein::d10(GLdouble u, GLdouble v)
{
    GLdouble r=4*(1-cos(u)/2);
    return DCoordinate3(a*((-1)*sin(u)*(1+sin(u))+cos(u)*cos(u))-r*cos(v)*sin(u), b*cos(u)+r*cos(u)*cos(v), 1);
}

DCoordinate3  klein::d01(GLdouble u, GLdouble v)
{
    GLdouble r=4*(1-cos(u)/2);
    return DCoordinate3(-r*cos(u)*sin(v), (-1)*r*sin(u)*sin(v), r*cos(v));
}


