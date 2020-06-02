#pragma once

#include <GL/glew.h>
#include "../Core/LinearCombination3.h"
#include "../Core/Matrices.h"

namespace cagd
{
    class FirstOrderTrigonometricArc3 : public LinearCombination3
    {
    protected:
        GLdouble _alpha;
        GLdouble calculateF2(GLdouble t) const;
        GLdouble calculateF2derivative(GLdouble t) const;
        GLdouble calculateF2derivative2(GLdouble t) const;
        GLdouble calculateF3(GLdouble t) const;
        GLdouble calculateF3derivative(GLdouble t) const;
        GLdouble calculateF3derivative2(GLdouble t) const;
    public:
        FirstOrderTrigonometricArc3 (GLdouble alpha=1.0);
        void setAlpha(GLdouble alpha);
        void getAlpha(GLdouble alpha) const;
        GLboolean BlendingFunctionValues (GLdouble u, RowMatrix<GLdouble> &values) const ;
        GLboolean CalculateDerivatives(GLuint max_order_of_derivatives, GLdouble u, Derivatives &d) const ;
    };
}
