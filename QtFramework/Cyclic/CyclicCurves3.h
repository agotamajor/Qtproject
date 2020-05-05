#pragma once

#include <GL/glew.h>
#include "../Core/LinearCombination3.h"
#include "../Core/Matrices.h"

namespace cagd
{
    class CyclicCurve3 : public LinearCombination3
    {
        protected:
            GLuint _n; // order
            GLdouble _c_n; // normalizing constant
            GLdouble _lambda_n; // phase change
            TriangularMatrix<GLdouble> _bc; // binomial c o e f f i c i e n t s
            GLdouble _CalculateNormalizingCoefficient(GLuint n);
            GLvoid _CalculateBinomialCoefficients(GLuint m, TriangularMatrix<GLdouble> &bc );

        public:
            // special constructor
            CyclicCurve3 (GLuint n, GLenum data_usage_flag = GL_STATIC_DRAW);
            // redeclare and define inherited pure v i r t u a l methods
            GLboolean BlendingFunctionValues (GLdouble u, RowMatrix<GLdouble> &values) const ;
            GLboolean CalculateDerivatives(GLuint max_order_of_derivatives, GLdouble u, Derivatives &d) const ;
    };
}
