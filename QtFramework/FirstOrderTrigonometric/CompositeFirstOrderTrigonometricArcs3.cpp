#include "CompositeFirstOrderTrigonometricArcs3.h"
#include "Core/Exceptions.h"
#include <QTextStream>
#include <iostream>

using namespace cagd;

CompositeFirstOrderTrigonometricArcs3::CompositeFirstOrderTrigonometricArcs3()
{

}

CompositeFirstOrderTrigonometricArcs3::CompositeFirstOrderTrigonometricArcs3(const CompositeFirstOrderTrigonometricArcs3& c):
    _arcs(c._arcs)
{
}

CompositeFirstOrderTrigonometricArcs3& CompositeFirstOrderTrigonometricArcs3::operator =(const CompositeFirstOrderTrigonometricArcs3& c)
{
    if(this != &c){
        this->_arcs = c._arcs;
    }
    return *this;
}

GLuint CompositeFirstOrderTrigonometricArcs3::getControlPointCount()
{
    return _control_point_count;
}

GLvoid CompositeFirstOrderTrigonometricArcs3::setControlPointCount(GLuint control_point_count)
{
    _control_point_count = control_point_count;
}

GLvoid CompositeFirstOrderTrigonometricArcs3::Render(bool d1Arc, bool d2Arc, bool control_points)
{
    for(GLuint i=0; i<_arcs.size();i++){
        if(_arcs[i]._img && _arcs[i]._color)
        {
            glColor3f(1.0f,1.0f,0.0f);
            _arcs[i]._img -> RenderDerivatives(0 ,GL_LINE_STRIP);

            if(d1Arc){
                glColor3f(0.0f, 0.5f, 0.0f );
                _arcs[i]._img -> RenderDerivatives(1, GL_LINES );
            }

            if(d2Arc){
                glColor3f(0.2f, 0.2f, 0.0f );
                _arcs[i]._img -> RenderDerivatives(2, GL_LINES );
            }
        }

        if(_arcs[i]._arc)
        {
            glColor3f(1.0f, 0.0f, 0.0f );
            _arcs[i]._arc -> RenderData(GL_LINE_STRIP);
        }

        if(control_points)
        {

            glPointSize(10);
                glColor3f(1, 1, 1);
                glBegin(GL_POINTS);
                for(GLuint j = 0; j < _control_point_count; j++)
                {
                     DCoordinate3 cp = (*_arcs[i]._arc)[j];
                     glVertex3dv(&cp[0]);
                }
                glEnd();
        }

    }
}

GLvoid CompositeFirstOrderTrigonometricArcs3::insertArc(GLdouble alpha, GLuint div_point_count, Color4* color)
{
    GLuint size = _arcs.size();
    _arcs.resize(size+1);
    FirstOrderTrigonometricArc3* arc = new FirstOrderTrigonometricArc3(alpha);
    GenericCurve3* image_of_arc = nullptr;

    GLuint n = div_point_count;
    GLdouble u;
    GLdouble step = TWO_PI / n;

    for(GLuint i = 0; i < n; i++)
    {
        u = i*step;
        DCoordinate3 &cp = (*arc)[i];

        cp[0] = cos(u);
        cp[1] = sin(u);
        cp[2] = 0;
    }

    if(!arc->UpdateVertexBufferObjectsOfData(GL_STATIC_DRAW))
    {
        cout << "The update was unsuccessful" << std::endl;
        return;
    }

    arc -> UpdateVertexBufferObjectsOfData();
    image_of_arc = arc -> GenerateImage(2, 100);

    if(!image_of_arc)
    {
        cout << "The generation of the arc was unsuccessful" << std::endl;
        return;
    }

    if(!image_of_arc->UpdateVertexBufferObjects(1, GL_STATIC_DRAW))
    {
        cout << "The update was unsuccessful" << std::endl;
        return;
    }

    _arcs[size]._arc = arc;
    _arcs[size]._img = image_of_arc;
    _arcs[size]._color = color;
}

GLvoid CompositeFirstOrderTrigonometricArcs3::DeleteArc(GLuint index)
{
    _arcs.erase(_arcs.begin()+index);
}

GLvoid CompositeFirstOrderTrigonometricArcs3::setArc(GLuint index, FirstOrderTrigonometricArc3* a)
{
    _arcs[index]._arc = a;
}

FirstOrderTrigonometricArc3* CompositeFirstOrderTrigonometricArcs3::getArc(GLuint index)
{
    return _arcs[index]._arc;
}

GLvoid CompositeFirstOrderTrigonometricArcs3::setImg(GLuint index, GenericCurve3 *img)
{
    _arcs[index]._img = img;
}

GenericCurve3* CompositeFirstOrderTrigonometricArcs3::getImg(GLuint index)
{
    return _arcs[index]._img;
}

GLvoid CompositeFirstOrderTrigonometricArcs3::setColor(GLuint index, Color4 *color)
{
    _arcs[index]._color = color;
}

Color4* CompositeFirstOrderTrigonometricArcs3::getColor(GLuint index)
{
    return _arcs[index]._color;
}

GLboolean CompositeFirstOrderTrigonometricArcs3::UpdateVertexBufferObjectOfData()
{
    for(GLuint i=0; i<_arcs.size(); i++)
    {
        _arcs[i]._arc->UpdateVertexBufferObjectsOfData();
        _arcs[i]._img->DeleteVertexBufferObjects();
                _arcs[i]._img=_arcs[i]._arc->GenerateImage(1, 100);
                _arcs[i]._img->UpdateVertexBufferObjects(1);
    }
    return GL_TRUE;
}

GLboolean CompositeFirstOrderTrigonometricArcs3::DeleteVertexBufferObjectOfData()
{
    for(GLuint i=0; i<_arcs.size(); i++)
    {
        _arcs[i]._arc->DeleteVertexBufferObjectsOfData();
    }
    return GL_TRUE;
}

CompositeFirstOrderTrigonometricArcs3::~CompositeFirstOrderTrigonometricArcs3()
{
    this->_arcs.clear();
}
