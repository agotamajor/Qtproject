#pragma once

#include "../Core/DCoordinates3.h"

namespace cagd
{
    namespace spiral_on_cone
    {
        extern GLdouble u_min, u_max;
        DCoordinate3 d0(GLdouble);
        DCoordinate3 d1(GLdouble);
        DCoordinate3 d2(GLdouble);
    }

    namespace rose_curve1
    {
        extern GLdouble u_min, u_max;
        extern GLuint k;

        DCoordinate3 d0(GLdouble);
        DCoordinate3 d1(GLdouble);
        DCoordinate3 d2(GLdouble);
    }

    namespace rose_curve2
    {
        extern GLdouble u_min, u_max;
        extern GLuint k;

        DCoordinate3 d0(GLdouble);
        DCoordinate3 d1(GLdouble);
        DCoordinate3 d2(GLdouble);
    }

    namespace cycloid
    {
        extern GLdouble u_min, u_max;
        extern GLdouble a;

        DCoordinate3 d0(GLdouble);
        DCoordinate3 d1(GLdouble);
        DCoordinate3 d2(GLdouble);
    }

    namespace nephroid
    {
        extern GLdouble u_min, u_max;

        DCoordinate3 d0(GLdouble);
        DCoordinate3 d1(GLdouble);
        DCoordinate3 d2(GLdouble);
    }

    namespace witch_of_agnesi
    {
        extern GLdouble u_min, u_max;
        extern GLdouble a;

        DCoordinate3 d0(GLdouble);
        DCoordinate3 d1(GLdouble);
        DCoordinate3 d2(GLdouble);
    }

    namespace fermat_spiral
    {
        extern GLdouble u_min, u_max;

        DCoordinate3 d0(GLdouble);
        DCoordinate3 d1(GLdouble);
        DCoordinate3 d2(GLdouble);
    }

    namespace lissajous_knockoff
    {
        extern GLdouble u_min, u_max;

        DCoordinate3 d0(GLdouble);
        DCoordinate3 d1(GLdouble);
        DCoordinate3 d2(GLdouble);
    }


    namespace sphere
    {
        // possible shape parameters
        extern GLdouble a;

        // definition domain
        extern GLdouble u_min, u_max, v_min, v_max;

        DCoordinate3 d00(GLdouble u, GLdouble v); // zeroth order partial derivative, i.e. surface point
        DCoordinate3 d10(GLdouble u, GLdouble v); // first order partial derivative in direction u
        DCoordinate3 d01(GLdouble u, GLdouble v); // first order partial derivative in direction v

    }

    namespace klein
    {
        // possible shape parameters
        extern GLdouble a, b;

        // definition domain
        extern GLdouble u_min, u_max, v_min, v_max;

        DCoordinate3 d00(GLdouble u, GLdouble v); // zeroth order partial derivative, i.e. surface point
        DCoordinate3 d10(GLdouble u, GLdouble v); // first order partial derivative in direction u
        DCoordinate3 d01(GLdouble u, GLdouble v); // first order partial derivative in direction v

    }

    namespace conoid
    {
        // possible shape parameters
        extern GLdouble a, b;

        // definition domain
        extern GLdouble u_min, u_max, v_min, v_max;

        DCoordinate3 d00(GLdouble u, GLdouble v); // zeroth order partial derivative, i.e. surface point
        DCoordinate3 d10(GLdouble u, GLdouble v); // first order partial derivative in direction u
        DCoordinate3 d01(GLdouble u, GLdouble v); // first order partial derivative in direction v

    }


    namespace boy
    {
        // possible shape parameters
        extern GLdouble k;

        // definition domain
        extern GLdouble u_min, u_max, v_min, v_max;

        DCoordinate3 d00(GLdouble u, GLdouble v); // zeroth order partial derivative, i.e. surface point
        DCoordinate3 d10(GLdouble u, GLdouble v); // first order partial derivative in direction u
        DCoordinate3 d01(GLdouble u, GLdouble v); // first order partial derivative in direction v

    }

    namespace roman
    {
        // possible shape parameters
        extern GLdouble k;

        // definition domain
        extern GLdouble u_min, u_max, v_min, v_max;

        DCoordinate3 d00(GLdouble u, GLdouble v); // zeroth order partial derivative, i.e. surface point
        DCoordinate3 d10(GLdouble u, GLdouble v); // first order partial derivative in direction u
        DCoordinate3 d01(GLdouble u, GLdouble v); // first order partial derivative in direction v

    }

    namespace torus
    {
        // possible shape parameters
        extern GLdouble r, R;

        // definition domain
        extern GLdouble u_min, u_max, v_min, v_max;

        DCoordinate3 d00(GLdouble u, GLdouble v); // zeroth order partial derivative, i.e. surface point
        DCoordinate3 d10(GLdouble u, GLdouble v); // first order partial derivative in direction u
        DCoordinate3 d01(GLdouble u, GLdouble v); // first order partial derivative in direction v

    }

}

