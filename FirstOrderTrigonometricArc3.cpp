#include "FirstOrderTrigonometricArc3.h"

using namespace cagd;

FirstOrderTrigonometricArc3::FirstOrderTrigonometricArc3(GLdouble alpha) :
    LinearCombination3 (0.0, alpha, 4),
    _alpha(alpha)
{
}

void FirstOrderTrigonometricArc3::setAlpha(GLdouble alpha)
{
    _alpha = alpha;
}

void FirstOrderTrigonometricArc3::getAlpha(GLdouble alpha) const
{
    alpha = _alpha;
}

GLdouble FirstOrderTrigonometricArc3::calculateF2(GLdouble t) const
{
    return ((_alpha+(-1)*t+sin(_alpha-t)+sin(t)-sin(_alpha)+t*cos(_alpha)+(-1)*_alpha*cos(t))*sin(_alpha))/((_alpha-sin(_alpha))*(2*sin(_alpha)-_alpha+(-1)*_alpha*cos(_alpha)));
}

GLdouble FirstOrderTrigonometricArc3::calculateF2derivative(GLdouble t) const
{
    return (sin(_alpha)*(-1-cos(_alpha-t)+cos(t)+cos(_alpha)+_alpha*sin(t)))/((_alpha-sin(_alpha))*(2*sin(_alpha)-_alpha+(-1)*_alpha*cos(_alpha)));
}

GLdouble FirstOrderTrigonometricArc3::calculateF2derivative2(GLdouble t) const
{
    return (sin(_alpha)*(-sin(_alpha-t)-sin(t)+_alpha*cos(t)))/((_alpha-sin(_alpha))*(2*sin(_alpha)-_alpha+(-1)*_alpha*cos(_alpha)));
}

GLdouble FirstOrderTrigonometricArc3::calculateF3(GLdouble t) const
{
    return (t-sin(t))/(_alpha-sin(_alpha));
}

GLdouble FirstOrderTrigonometricArc3::calculateF3derivative(GLdouble t) const
{
    return (1.0-cos(t))/(_alpha-sin(_alpha));
}

GLdouble FirstOrderTrigonometricArc3::calculateF3derivative2(GLdouble t) const
{
    return sin(t)/(_alpha-sin(_alpha));
}

GLboolean FirstOrderTrigonometricArc3::BlendingFunctionValues(GLdouble u, RowMatrix<GLdouble>& blending_values) const
{
    if (u < 0.0 || u > _alpha)
        return GL_FALSE;

    blending_values.ResizeColumns(4);

    blending_values(0) = calculateF3(_alpha-u);
    blending_values(1) = calculateF2(_alpha-u);
    blending_values(2) = calculateF2(u);
    blending_values(3) = calculateF3(u);

    return GL_TRUE;
}

GLboolean FirstOrderTrigonometricArc3::CalculateDerivatives(GLuint maximum_order_of_derivatives, GLdouble u, Derivatives &d)const
{
    if (u < 0.0 || u>_alpha || maximum_order_of_derivatives<2)
        return GL_FALSE;

    RowMatrix<GLdouble> u_blending_values(4),d1_u_blending_values(4),d2_u_blending_values(4);

    u_blending_values(0) = calculateF3(_alpha-u);
    u_blending_values(1) = calculateF2(_alpha-u);
    u_blending_values(2) = calculateF2(u);
    u_blending_values(3) = calculateF3(u);

    d1_u_blending_values(0) = -calculateF3derivative(_alpha-u);
    d1_u_blending_values(1) = -calculateF2derivative(_alpha-u);
    d1_u_blending_values(2) = calculateF2derivative(u);
    d1_u_blending_values(3) = calculateF3derivative(u);

    d2_u_blending_values(0) = calculateF3derivative2(_alpha-u);
    d2_u_blending_values(1) = calculateF2derivative2(_alpha-u);
    d2_u_blending_values(2) = calculateF2derivative2(u);
    d2_u_blending_values(3) = calculateF3derivative2(u);

    d.ResizeRows(maximum_order_of_derivatives + 1);
    d.LoadNullVectors();

    for(GLuint j = 0; j < _data.GetRowCount(); j++)
    {
        d[0] += _data[j] * u_blending_values(j);
    }

    for(GLuint j = 0; j < _data.GetRowCount(); j++)
    {
        d[1] += _data[j] * d1_u_blending_values(j);
    }

    for(GLuint j = 0; j < _data.GetRowCount(); j++)
    {
        d[2] += _data[j] * d2_u_blending_values(j);
    }

    return GL_TRUE;
}
