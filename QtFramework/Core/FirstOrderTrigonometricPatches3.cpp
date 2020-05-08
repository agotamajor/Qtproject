#include "FirstOrderTrigonometricPatches3.h"

using namespace cagd ;

// alpha - user-defined
FirstOrderTrigonometricPatches3:: FirstOrderTrigonometricPatches3(GLdouble alpha):
    TensorProductSurface3 (0.0, alpha, 0.0, alpha, 4, 4),
    _alpha(alpha)
{

}

GLboolean FirstOrderTrigonometricPatches3:: UBlendingFunctionValues(
        GLdouble u_knot , RowMatrix<GLdouble>& blending_values) const
{
    if (u_knot < 0.0 || u_knot > _alpha)
        return GL_FALSE;

    blending_values.ResizeColumns(4);
    GLdouble u = u_knot;
    blending_values(3) = (u-sinh(u))/(_alpha-sinh(_alpha));
    blending_values(2) = ((_alpha-u+sinh(_alpha-u)+sinh(u)-sinh(_alpha)+u*cosh(_alpha)+(-1)*_alpha*cosh(u))*sinh(_alpha))/
            ((_alpha-sinh(_alpha))*(2*sinh(_alpha)+(-1)*_alpha+(-1)*_alpha*cosh(_alpha)));
    blending_values(1) = ((u+sinh(u)+sinh(_alpha-u)-sinh(_alpha)+(_alpha-u)*cosh(_alpha)+(-1)*_alpha*cosh(_alpha-u))*sinh(_alpha))/
            ((_alpha-sinh(_alpha))*(2*sinh(_alpha)+(-1)*_alpha+(-1)*_alpha*cosh(_alpha)));
    blending_values(0) = (_alpha-u-sinh(_alpha-u))/(_alpha-sinh(_alpha));
    return GL_TRUE;
}


GLboolean FirstOrderTrigonometricPatches3:: VBlendingFunctionValues(
        GLdouble v_knot , RowMatrix<GLdouble>& blending_values) const
{
    if (v_knot < 0.0 || v_knot > _alpha)
        return GL_FALSE;

    blending_values.ResizeColumns(4);
    GLdouble v = v_knot;
    blending_values(3) = (v-sinh(v))/(_alpha-sinh(_alpha));
    blending_values(2) = ((_alpha-v+sinh(_alpha-v)+sinh(v)-sinh(_alpha)+v*cosh(_alpha)+(-1)*_alpha*cosh(v))*sinh(_alpha))/
            ((_alpha-sinh(_alpha))*(2*sinh(_alpha)+(-1)*_alpha+(-1)*_alpha*cosh(_alpha)));
    blending_values(1) = ((v+sinh(v)+sinh(_alpha-v)-sinh(_alpha)+(_alpha-v)*cosh(_alpha)+(-1)*_alpha*cosh(_alpha-v))*sinh(_alpha))/
            ((_alpha-sinh(_alpha))*(2*sinh(_alpha)+(-1)*_alpha+(-1)*_alpha*cosh(_alpha)));
    blending_values(0) = (_alpha-v-sinh(_alpha-v))/(_alpha-sinh(_alpha));
    return GL_TRUE;
}

GLboolean  FirstOrderTrigonometricPatches3:: CalculatePartialDerivatives(
        GLuint maximum_order_of_partial_derivatives,
        GLdouble u,
        GLdouble v, PartialDerivatives& pd) const
{
    if (u < 0.0 || u > _alpha || v < 0.0 || v > _alpha || maximum_order_of_partial_derivatives > 1)
        return GL_FALSE ;

    // blending function values and their derivatives in u−direction
    RowMatrix<GLdouble> u_blending_values(4), d1_u_blending_values(4);

    u_blending_values(3) = (u-sinh(u))/(_alpha-sinh(_alpha));
    u_blending_values(2) = ((_alpha-u+sinh(_alpha-u)+sinh(u)-sinh(_alpha)+u*cosh(_alpha)+(-1)*_alpha*cos(u))*sinh(_alpha))/
            ((_alpha-sinh(_alpha))*(2*sinh(_alpha)+(-1)*_alpha+(-1)*_alpha*cosh(_alpha)));
    u_blending_values(1) = ((u+sinh(u)+sinh(_alpha-u)-sinh(_alpha)+(_alpha-u)*cosh(_alpha)+(-1)*_alpha*cosh(_alpha-u))*sinh(_alpha))/
            ((_alpha-sinh(_alpha))*(2*sinh(_alpha)+(-1)*_alpha+(-1)*_alpha*cosh(_alpha)));
    u_blending_values(0) = (_alpha-u-sinh(_alpha-u))/(_alpha-sinh(_alpha));

    d1_u_blending_values(3) = (cosh(u)-1)/(sinh(_alpha)-_alpha);
    d1_u_blending_values(2) = (sinh(_alpha)*((-1)*cosh(u-_alpha)+(-1)*_alpha*sinh(u)+cosh(u)+cosh(_alpha)-1))/
            ((_alpha-sinh(_alpha))*(2*sinh(_alpha)+(-1)*_alpha*cosh(_alpha)-_alpha));
    d1_u_blending_values(1) = (sinh(_alpha)*((-1)*sinh(u-_alpha)+2*cosh(u)-cosh(_alpha)+1))/
            ((_alpha-sinh(_alpha))*(2*sinh(_alpha)+(-1)*_alpha*cosh(_alpha)-_alpha));
    d1_u_blending_values(0) = (cosh(u-_alpha)-1)/(_alpha-sinh(_alpha));

    // blending function values and their derivatives in v−direction
    RowMatrix<GLdouble> v_blending_values (4) , d1_v_blending_values(4);

    v_blending_values(3) = (v-sinh(v))/(_alpha-sinh(_alpha));
    v_blending_values(2) = ((_alpha-v+sinh(_alpha-v)+sinh(v)-sinh(_alpha)+v*cosh(_alpha)+(-1)*_alpha*cos(v))*sinh(_alpha))/
            ((_alpha-sinh(_alpha))*(2*sinh(_alpha)+(-1)*_alpha+(-1)*_alpha*cosh(_alpha)));
    v_blending_values(1) = ((v+sinh(v)+sinh(_alpha-v)-sinh(_alpha)+(_alpha-v)*cosh(_alpha)+(-1)*_alpha*cosh(_alpha-v))*sinh(_alpha))/
            ((_alpha-sinh(_alpha))*(2*sinh(_alpha)+(-1)*_alpha+(-1)*_alpha*cosh(_alpha)));
    v_blending_values(0) = (_alpha-v-sinh(_alpha-v))/(_alpha-sinh(_alpha));

    d1_v_blending_values(3) = (cosh(v)-1)/(sinh(_alpha)-_alpha);
    d1_v_blending_values(2) = (sinh(_alpha)*((-1)*cosh(v-_alpha)+(-1)*_alpha*sinh(v)+cosh(v)+cosh(_alpha)-1))/
            ((_alpha-sinh(_alpha))*(2*sinh(_alpha)+(-1)*_alpha*cosh(_alpha)-_alpha));
    d1_v_blending_values(1) = (sinh(_alpha)*((-1)*sinh(v-_alpha)+2*cosh(v)-cosh(_alpha)+1))/
            ((_alpha-sinh(_alpha))*(2*sinh(_alpha)+(-1)*_alpha*cosh(_alpha)-_alpha));
    d1_v_blending_values(0) = (cosh(v-_alpha)-1)/(_alpha-sinh(_alpha));

    pd.ResizeRows(2);
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
        // surface point
        pd(1, 0) += aux_d0_v * d1_u_blending_values(row);
        // f i r s t order u−directional p a r t i a l derivative
        pd(1, 1) += aux_d1_v * u_blending_values(row);
        // f i r s t order v−directional p a r t i a l derivative
    }

    return GL_TRUE;
}
