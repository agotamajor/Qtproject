#include "FirstOrderTrigonometricArc3.h"
#include "../Core/Constants.h"
#include <iostream>
#include <cmath>

using namespace std;
namespace cagd
{

    GLdouble FirstOrderTrigonometricArc3::_CalculateNormalizingCoefficient(GLuint n)
    {
        if(!n)
        {
            return 1.0;
        }

        GLdouble c = 1.0 / 3.0;

        for (GLuint i = 2; i <= n; ++i)
        {
            c *= (GLdouble)i / (GLdouble)(2 * i + 1);
        }

        return c ;
    }

    GLvoid FirstOrderTrigonometricArc3::_CalculateBinomialCoefficients (GLuint m, TriangularMatrix<GLdouble> &bc)
    {
        bc.ResizeRows(m + 1);
        bc(0, 0) = 1.0;

        for (GLuint r = 1; r <= m; ++r)
        {
            bc(r, 0) = 1.0;
            bc(r, r) = 1.0;
            for(GLuint i = 1; i <= r / 2; ++i)
            {
                bc(r, i) = bc(r - 1, i - 1) + bc(r - 1, i);
                bc(r, r - i) = bc(r, i);
            }
        }
    }

    FirstOrderTrigonometricArc3::FirstOrderTrigonometricArc3 (GLuint n, GLdouble alpha, GLenum data_usage_flag):
        LinearCombination3 (0.0, TWO_PI, 2 * n + 1, data_usage_flag),
        _alpha(alpha),
        _n(n),
        _c_n(_CalculateNormalizingCoefficient(n)),
        _lambda_n (TWO_PI / (2 * n + 1))
    {
        _CalculateBinomialCoefficients (2 * _n , _bc);
    }

    GLdouble FirstOrderTrigonometricArc3::calculateF2(GLdouble t) const
    {
        return ((_alpha+(-1)*t+sin(_alpha-t)+sin(t)-sin(_alpha)+t*cos(_alpha)+(-1)*_alpha*cos(t))*sin(_alpha))/((_alpha-sin(_alpha))*(2*sin(_alpha)-_alpha+(-1)*_alpha*cos(_alpha)));
    }

    GLdouble FirstOrderTrigonometricArc3::calculateF2derivative(GLdouble t) const
    {
        return (sin(_alpha)*(-cos(t-_alpha)+_alpha*sin(t)+cos(t)+cos(_alpha)-1))/((_alpha-sin(_alpha))*(2*sin(_alpha)+(-1)*_alpha*cos(_alpha)-_alpha));
    }

    GLdouble FirstOrderTrigonometricArc3::calculateF3(GLdouble t) const
    {
        return (t-sin(t))/(_alpha-sin(_alpha));
    }

    GLdouble FirstOrderTrigonometricArc3::calculateF3derivative(GLdouble t) const
    {
        return (1.0-cos(t))/(_alpha-sin(_alpha));
    }

    GLboolean FirstOrderTrigonometricArc3::BlendingFunctionValues(
            GLdouble u_knot, RowMatrix<GLdouble>& blending_values) const
    {
        if (u_knot < 0.0 || u_knot > _alpha)
            return GL_FALSE;

        blending_values.ResizeColumns(4);

        blending_values(0) = calculateF3(_alpha-u_knot);
        blending_values(1) = calculateF2(_alpha-u_knot);
        blending_values(2) = calculateF2(u_knot);
        blending_values(3) = calculateF3(u_knot);

        return GL_TRUE;
    }


    GLboolean FirstOrderTrigonometricArc3::CalculateDerivatives (
            GLuint max_order_of_derivatives, GLdouble u, Derivatives& d) const
    {
        d.ResizeRows( max_order_of_derivatives + 1);
        d.LoadNullVectors();

        RowMatrix<GLdouble> u_blending_values(4),d1_u_blending_values(4);

        u_blending_values(0) = calculateF3(_alpha-u);
        u_blending_values(1) = calculateF2(_alpha-u);
        u_blending_values(2) = calculateF2(u);
        u_blending_values(3) = calculateF3(u);

        d1_u_blending_values(0) = -calculateF3derivative(_alpha-u);
        d1_u_blending_values(1) = -calculateF2derivative(_alpha-u);
        d1_u_blending_values(2) = calculateF2derivative(u);
        d1_u_blending_values(3) = calculateF3derivative(u);

        for ( GLuint r = 0; r <= max_order_of_derivatives; ++r ) {
            DCoordinate3 aux_d0_v, aux_d1_v;
            for(GLuint column = 0; column < 4; ++column)
            {
                aux_d0_v += _data(r) * u_blending_values(r);
                aux_d1_v += _data(r) * d1_u_blending_values(r);
            }
            d[0] += aux_d0_v * u_blending_values(r);
            d[1] += aux_d0_v * d1_u_blending_values(r);
            d[2] += aux_d1_v * u_blending_values(r);
        }

        cout<<d<<endl;
        return GL_TRUE;
    }
}
