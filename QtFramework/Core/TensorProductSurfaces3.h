#pragma once

#include "DCoordinates3.h"
#include <GL/glew.h>
#include <iostream>
#include "Matrices.h"
#include "TriangulatedMeshes3.h"
#include <vector>
//-------------------------------------------------------------------------------------
#include "GenericCurves3.h"
//-------------------------------------------------------------------------------------

namespace cagd
{
    class TensorProductSurface3
    {
    public:
        class PartialDerivatives: public TriangularMatrix<DCoordinate3>
        {
            public:
               PartialDerivatives(GLuint maximum_order_of_derivatives=1);

               // homework: initializes all partial derivatives to the origin
               GLvoid LoadNullVectors();
        };

    protected:
        GLboolean            _u_closed, _v_closed; // is the surface closed in direction u or v
        GLuint               _vbo_data;            // vertex buffer object of the control net
        GLdouble             _u_min, _u_max;       // definition domain in direction u
        GLdouble             _v_min, _v_max;       // definition domain in direction v
        Matrix<DCoordinate3> _data;                // the control net (usually stores position vectors)

    public:
        // homework: special constructor
        TensorProductSurface3(
             GLdouble u_min, GLdouble u_max,
             GLdouble v_min, GLdouble v_max,
             GLuint row_count = 4, GLuint column_count = 4,
             GLboolean u_closed = GL_FALSE, GLboolean v_closed = GL_FALSE);

        // homework: copy constructor
        TensorProductSurface3(const TensorProductSurface3& surface);

        // homework: assignment operator
        TensorProductSurface3& operator =(const TensorProductSurface3& surface);

        // homework: set/get the definition domain of the surface
        GLvoid SetUInterval(GLdouble u_min, GLdouble u_max);
        GLvoid SetVInterval(GLdouble v_min, GLdouble v_max);

        GLvoid GetUInterval(GLdouble& u_min, GLdouble& u_max) const;
        GLvoid GetVInterval(GLdouble& v_min, GLdouble& v_max) const;

        // homework: set coordinates of a selected data point
        GLboolean SetData(GLuint row, GLuint column, GLdouble x, GLdouble y, GLdouble z);
        GLboolean SetData(GLuint row, GLuint column, const DCoordinate3& point);

        // homework: get coordinates of a selected data point
        GLboolean GetData(
                GLuint row, GLuint column,
                GLdouble& x, GLdouble& y, GLdouble& z) const;
        GLboolean GetData(GLuint row, GLuint column, DCoordinate3& point) const;

        // homework: get data by value
        DCoordinate3 operator ()(GLuint row, GLuint column) const;

        // homework: get data by reference
        DCoordinate3& operator ()(GLuint row, GLuint column);

        // blending function values in directions u and v
        virtual GLboolean UBlendingFunctionValues(GLdouble u_knot, RowMatrix<GLdouble>& blending_values) const = 0;
        virtual GLboolean VBlendingFunctionValues(GLdouble v_knot, RowMatrix<GLdouble>& blending_values) const = 0;

        // calculates the point and higher (mixed) ordered partial derivatives of the
        // tensor product surface
        //
        // s(u, v) = \sum_{i=0}^{row_count} \sum{j = 0}^{column_count} p_{ij} F_i(u) G_j(v),
        //
        // where(u, v) \in [u_{\min}, u_{\max}] \times [v_{\min}, v_{\max}]
        virtual GLboolean CalculatePartialDerivatives(GLuint maximum_order_of_derivatives, GLdouble u, GLdouble v, PartialDerivatives& pd) const = 0;

        // generates a triangulated mesh that approximates the shape of the surface above
        virtual TriangulatedMesh3* GenerateImage(GLuint maximum_order_of_derivatives, GLuint u_div_point_count, GLuint v_div_point_count, GLenum usage_flag = GL_STATIC_DRAW) const;

        // ensures interpolation, i.e.
        //
        // s(u_i, v_j) = d_{ij}, i = 0,1,...,row_count, j = 0,1,...,column_count
        GLboolean UpdateDataForInterpolation(const RowMatrix<GLdouble>& u_knot_vector, const ColumnMatrix<GLdouble>& v_knot_vector, Matrix<DCoordinate3>& data_points_to_interpolate);

        // homework: VBO handling methods (assume that Matrix<DCoordinate3> _data corresponds to a control net!)
        virtual GLvoid    DeleteVertexBufferObjectsOfData();
        virtual GLboolean RenderData(GLenum render_mode = GL_LINE_STRIP) const;
        virtual GLboolean UpdateVertexBufferObjectsOfData(GLenum usage_flag = GL_STATIC_DRAW);

        RowMatrix<GenericCurve3*>* GenerateUIsoparametricLines(GLuint maximum_order_of_derivatives, GLuint iso_line_count, GLuint u_div_point_count, GLenum usage_flag = GL_STATIC_DRAW) const;
        RowMatrix<GenericCurve3*>* GenerateVIsoparametricLines(GLuint maximum_order_of_derivatives, GLuint iso_line_count, GLuint v_div_point_count, GLenum usage_flag = GL_STATIC_DRAW) const;

        // homework: destructor
        virtual ~TensorProductSurface3();
    };
}
