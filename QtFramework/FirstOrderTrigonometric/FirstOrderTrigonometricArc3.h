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
            GLuint _n; // order
            GLdouble _c_n; // normalizing constant
            GLdouble _lambda_n; // phase change
            TriangularMatrix<GLdouble> _bc; // binomial c o e f f i c i e n t s
            GLdouble _CalculateNormalizingCoefficient(GLuint n);
            GLvoid _CalculateBinomialCoefficients(GLuint m, TriangularMatrix<GLdouble> &bc );
            GLdouble calculateF2(GLdouble t) const;
            GLdouble calculateF2derivative(GLdouble t) const;
            GLdouble calculateF3(GLdouble t) const;
            GLdouble calculateF3derivative(GLdouble t) const;

        public:
            // special constructor
            FirstOrderTrigonometricArc3 (GLuint n, GLdouble _alpha, GLenum data_usage_flag = GL_STATIC_DRAW);
            // redeclare and define inherited pure v i r t u a l methods
            GLboolean BlendingFunctionValues (GLdouble u, RowMatrix<GLdouble> &values) const ;
            GLboolean CalculateDerivatives(GLuint max_order_of_derivatives, GLdouble u, Derivatives &d) const ;
    };
}
