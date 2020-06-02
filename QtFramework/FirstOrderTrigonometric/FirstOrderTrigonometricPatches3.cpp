#include "FirstOrderTrigonometricPatches3.h"

using namespace cagd ;

// alpha - user-defined
FirstOrderTrigonometricPatches3:: FirstOrderTrigonometricPatches3(GLdouble alpha, GLuint row_count, GLuint column_count):
    TensorProductSurface3 (0.0, alpha, 0.0, alpha, 4, 4),
    _alpha(alpha)
{
}

void FirstOrderTrigonometricPatches3::setAlpha(GLdouble alpha)
{
    _alpha = alpha;
    SetUInterval(0, alpha);
    SetVInterval(0, alpha);
}

void FirstOrderTrigonometricPatches3::getAlpha(GLdouble alpha) const
{
    alpha = _alpha;
}

GLdouble FirstOrderTrigonometricPatches3::calculateF2(GLdouble t) const
{
    return ((_alpha+(-1)*t+sin(_alpha-t)+sin(t)-sin(_alpha)+t*cos(_alpha)+(-1)*_alpha*cos(t))*sin(_alpha))/((_alpha-sin(_alpha))*(2*sin(_alpha)-_alpha+(-1)*_alpha*cos(_alpha)));
}

GLdouble FirstOrderTrigonometricPatches3::calculateF2derivative(GLdouble t) const
{
    return (sin(_alpha)*(-1-cos(_alpha-t)+cos(t)+cos(_alpha)+_alpha*sin(t)))/((_alpha-sin(_alpha))*(2*sin(_alpha)-_alpha+(-1)*_alpha*cos(_alpha)));
}

GLdouble FirstOrderTrigonometricPatches3::calculateF3(GLdouble t) const
{
    return (t-sin(t))/(_alpha-sin(_alpha));
}

GLdouble FirstOrderTrigonometricPatches3::calculateF3derivative(GLdouble t) const
{
    return (1.0-cos(t))/(_alpha-sin(_alpha));
}

GLboolean FirstOrderTrigonometricPatches3::UBlendingFunctionValues(
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

GLboolean FirstOrderTrigonometricPatches3::VBlendingFunctionValues(GLdouble v_knot,RowMatrix<GLdouble>& blending_values) const
{
    if (v_knot < 0.0 || v_knot > _alpha)
        return GL_FALSE;

    blending_values.ResizeColumns(4);

    blending_values(0) = calculateF3(_alpha-v_knot);
    blending_values(1) = calculateF2(_alpha-v_knot);
    blending_values(2) = calculateF2(v_knot);
    blending_values(3) = calculateF3(v_knot);

    return GL_TRUE;
}


GLboolean FirstOrderTrigonometricPatches3 :: CalculatePartialDerivatives (GLuint maximum_order_of_derivatives, GLdouble u,GLdouble v, PartialDerivatives& pd) const
{
    if (u < 0.0 || u>_alpha || v< 0.0 || v>_alpha || maximum_order_of_derivatives<1)
        return GL_FALSE;

    RowMatrix<GLdouble> u_blending_values(4),d1_u_blending_values(4);

    u_blending_values(0) = calculateF3(_alpha-u);
    u_blending_values(1) = calculateF2(_alpha-u);
    u_blending_values(2) = calculateF2(u);
    u_blending_values(3) = calculateF3(u);

    d1_u_blending_values(0) = -calculateF3derivative(_alpha-u);
    d1_u_blending_values(1) = -calculateF2derivative(_alpha-u);
    d1_u_blending_values(2) = calculateF2derivative(u);
    d1_u_blending_values(3) = calculateF3derivative(u);

    RowMatrix<GLdouble> v_blending_values(4),d1_v_blending_values(4);

    v_blending_values(0) = calculateF3(_alpha-v);
    v_blending_values(1) = calculateF2(_alpha-v);
    v_blending_values(2) = calculateF2(v);
    v_blending_values(3) = calculateF3(v);

    d1_v_blending_values(0) = -calculateF3derivative(_alpha-v);
    d1_v_blending_values(1) = -calculateF2derivative(_alpha-v);
    d1_v_blending_values(2) = calculateF2derivative(v);
    d1_v_blending_values(3) = calculateF3derivative(v);

    pd.LoadNullVectors();
    for(GLuint row = 0; row < 4; ++row)
    {
        DCoordinate3 aux_d0_v, aux_d1_v;
        for(GLuint column = 0; column < 4; ++column)
        {
            aux_d0_v += _data(row, column) * v_blending_values(column);
            aux_d1_v += _data(row, column) * d1_v_blending_values(column);
        }
        pd(0, 0) += aux_d0_v * u_blending_values(row);
        pd(1, 0) += aux_d0_v * d1_u_blending_values(row);
        pd(1, 1) += aux_d1_v * u_blending_values(row);
    }
    return GL_TRUE;
}
