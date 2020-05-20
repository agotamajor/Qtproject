#pragma once
#include "../Core/TensorProductSurfaces3.h"

namespace cagd
{
class FirstOrderTrigonometricPatches3: public TensorProductSurface3
{

private:
    GLdouble _alpha;
    GLdouble calculateF2(GLdouble t) const;
    GLdouble calculateF2derivative(GLdouble t) const;
    GLdouble calculateF3(GLdouble t) const;
    GLdouble calculateF3derivative(GLdouble t) const;

public:
    FirstOrderTrigonometricPatches3(GLdouble alpha = 1.0);
    void setAlpha(GLdouble alpha);
    void getAlpha(GLdouble alpha) const;
    GLboolean UBlendingFunctionValues (GLdouble u_knot, RowMatrix<GLdouble>& blending_values) const;
    GLboolean VBlendingFunctionValues (GLdouble v_knot, RowMatrix<GLdouble>& blending_values) const;
    GLboolean CalculatePartialDerivatives(GLuint maximum_order_of_derivatives, GLdouble u, GLdouble v, PartialDerivatives& pd) const;

};
}
