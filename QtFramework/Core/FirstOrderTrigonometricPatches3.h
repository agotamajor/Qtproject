#pragma once
#include "../Core/TensorProductSurfaces3.h"

namespace cagd
{
    class FirstOrderTrigonometricPatches3: public TensorProductSurface3
    {

        public :
        GLdouble _alpha;
        FirstOrderTrigonometricPatches3(GLdouble alpha);
        // we have to implement pure v i r t u a l methods introduced in class TensorProductSurface3
        GLboolean UBlendingFunctionValues (GLdouble u_knot, RowMatrix<GLdouble>& blending_values) const;
        GLboolean VBlendingFunctionValues (GLdouble v_knot, RowMatrix<GLdouble>& blending_values) const;
        GLboolean CalculatePartialDerivatives (GLuint maximum_order_of_partial_derivatives,
                                                GLdouble u, GLdouble v, PartialDerivatives& pd) const;
    };
}
