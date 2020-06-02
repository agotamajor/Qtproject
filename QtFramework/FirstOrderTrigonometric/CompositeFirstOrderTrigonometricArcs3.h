#pragma once

#include "FirstOrderTrigonometric/FirstOrderTrigonometricArc3.h"
#include "Core/Constants.h"
#include "Core/Colors4.h"
#include <fstream>
#include <iostream>

using namespace std;

namespace cagd
{
class CompositeFirstOrderTrigonometricArcs3
{
public:
    class ArcAttributes
    {
    public:
        FirstOrderTrigonometricArc3 *_arc;
        GenericCurve3 *_img;
        Color4 *_color;
    };

    protected:
        GLuint _control_point_count;
    public:
        std::vector<ArcAttributes> _arcs;

        CompositeFirstOrderTrigonometricArcs3();
        CompositeFirstOrderTrigonometricArcs3(const CompositeFirstOrderTrigonometricArcs3& c);
        CompositeFirstOrderTrigonometricArcs3& operator =(const CompositeFirstOrderTrigonometricArcs3& c);

        GLuint getControlPointCount();
        GLvoid setControlPointCount(GLuint control_point_count);
        GLvoid Render(bool d1Arc, bool d2Arc, bool controlpoints);
        GLvoid insertArc(GLdouble alpha, GLuint div_point_count, Color4* color);
        GLvoid DeleteArc(GLuint index);

        GLvoid setArc(GLuint index, FirstOrderTrigonometricArc3* p);
        FirstOrderTrigonometricArc3* getArc(GLuint index);

        GLvoid setImg(GLuint index, GenericCurve3 *img);
        GenericCurve3* getImg(GLuint index);

        GLvoid setColor(GLuint index, Color4 *color);
        Color4* getColor(GLuint index);

        GLboolean UpdateVertexBufferObjectOfData();
        GLboolean DeleteVertexBufferObjectOfData();

        ~CompositeFirstOrderTrigonometricArcs3();
    private:

    };
}
