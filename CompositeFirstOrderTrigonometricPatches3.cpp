#include "CompositeFirstOrderTrigonometricPatches3.h"
#include "FirstOrderTrigonometric/FirstOrderTrigonometricPatches3.h"
#include <typeinfo>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <QFile>
#include <QTextStream>

using namespace cagd;
using namespace std;


CompositeFirstOrderTrigonometricPatches3::CompositeFirstOrderTrigonometricPatches3(GLuint u_div_point_count, GLuint v_div_point_count, GLuint u_iso_line_count, GLuint v_iso_line_count):
    _v_iso_line_count(v_iso_line_count),
    _u_iso_line_count(u_iso_line_count),
    _u_div_point_count(u_div_point_count),
    _v_div_point_count(v_div_point_count)
{
}

CompositeFirstOrderTrigonometricPatches3::CompositeFirstOrderTrigonometricPatches3(const CompositeFirstOrderTrigonometricPatches3& c):
    _v_iso_line_count(c._v_iso_line_count),
    _u_iso_line_count(c._u_iso_line_count),
    _u_div_point_count(c._u_div_point_count),
    _v_div_point_count(c._v_div_point_count),
    _patches(c._patches)
{
}

CompositeFirstOrderTrigonometricPatches3& CompositeFirstOrderTrigonometricPatches3::operator =(const CompositeFirstOrderTrigonometricPatches3& c)
{
    if(this != &c){
        _v_iso_line_count = c._v_iso_line_count;
        _u_iso_line_count = c._u_iso_line_count;
        _u_div_point_count = c._u_div_point_count;
        _v_div_point_count = c._v_div_point_count;
        this->_patches = c._patches;
    }
    return *this;
}

GLvoid CompositeFirstOrderTrigonometricPatches3::DeletePatch(GLuint index)
{
    _patches.erase(_patches.begin()+index);
}

GLvoid CompositeFirstOrderTrigonometricPatches3::setPatch(GLuint index, FirstOrderTrigonometricPatches3* p)
{
    _patches[index]._patch = p;
}

FirstOrderTrigonometricPatches3* CompositeFirstOrderTrigonometricPatches3::getPatch(GLuint index)
{
    return _patches[index]._patch;
}

GLvoid CompositeFirstOrderTrigonometricPatches3:: setMaterial(GLuint index, Material* material)
{
    _patches[index]._material = material;
}

Material CompositeFirstOrderTrigonometricPatches3::getMaterial(GLuint index)
{
    return *(_patches[index]._material);
}

GLvoid CompositeFirstOrderTrigonometricPatches3::setMesh(GLuint index, TriangulatedMesh3* mesh)
{
    _patches[index]._mesh = mesh;
}

TriangulatedMesh3 CompositeFirstOrderTrigonometricPatches3::getMesh(GLuint index)
{
    return *(_patches[index]._mesh);
}


GLvoid CompositeFirstOrderTrigonometricPatches3::setVIsoLineCount(GLuint v_iso_line_count)
{
    _v_iso_line_count = v_iso_line_count;
    for (GLuint j = 0; j < _patches.size(); j++)
    {
        delete  _patches[j]._v_isoparametric_lines;
        _patches[j]._v_isoparametric_lines = _patches[j]._patch->GenerateVIsoparametricLines(1, _v_iso_line_count, _v_div_point_count, GL_STATIC_DRAW);
        for(GLuint i = 0; i<_v_iso_line_count; i++)
        {
            (*_patches[j]._v_isoparametric_lines)[i]->UpdateVertexBufferObjects(GL_STATIC_DRAW);
        }

    }
}

GLvoid CompositeFirstOrderTrigonometricPatches3::setUIsoLineCount(GLuint u_iso_line_count)
{
    _u_iso_line_count = u_iso_line_count;
    for (GLuint j = 0; j < _patches.size(); j++)
    {
        delete  _patches[j]._u_isoparametric_lines;
        _patches[j]._u_isoparametric_lines = _patches[j]._patch->GenerateUIsoparametricLines(1, _u_iso_line_count, _u_div_point_count, GL_STATIC_DRAW);
        for(GLuint i = 0; i<_u_iso_line_count; i++)
        {
            (*_patches[j]._u_isoparametric_lines)[i]->UpdateVertexBufferObjects(GL_STATIC_DRAW);
        }

    }
}

GLuint CompositeFirstOrderTrigonometricPatches3::getVIsoLineCount()
{
    return _v_iso_line_count;
}

GLuint CompositeFirstOrderTrigonometricPatches3::getUIsoLineCount()
{
    return _u_iso_line_count;
}

GLvoid CompositeFirstOrderTrigonometricPatches3::Render(bool render_normal, bool control_points, bool controlnet, bool ulines, bool vlines, ShaderProgram shader)
{
    for (GLuint ind = 0; ind < _patches.size(); ind++)
    {
        if (_patches[ind]._mesh && _patches[ind]._material)
        {
            if(render_normal)
            {
                _patches[ind]._mesh->RenderNormals();
            }

            if(control_points)
            {
                glDisable (GL_LIGHTING);
                glDisable(GL_NORMALIZE);
                glDisable(GL_LIGHT0);

                glPointSize(10);
                glColor3f(1, 1, 1);
                glBegin(GL_POINTS);

                for(GLuint i = 0; i < 4; i++)
                {
                    for(GLuint j = 0; j < 4; j++)
                    {
                        glVertex3dv(&((*_patches[ind]._patch)(i, j)[0]));
                    }
                }
                glEnd();
            }

            if(controlnet)
            {
                glDisable (GL_LIGHTING);
                glDisable(GL_NORMALIZE);
                glDisable(GL_LIGHT0);
                for(GLuint i = 0; i < 4; i++)
                {
                    for(GLuint j = 0; j < 4; j++)
                    {
                        glBegin(GL_LINE_STRIP);
                        glVertex3dv(&((*_patches[ind]._patch)(i, j)[0]));
                        if(j<3)
                        {
                            glVertex3dv(&((*_patches[ind]._patch)(i, j+1)[0]));
                        }
                        glEnd();

                        glBegin(GL_LINE_STRIP);
                        glVertex3dv(&((*_patches[ind]._patch)(i, j)[0]));
                        if(i<3)
                        {
                            glVertex3dv(&((*_patches[ind]._patch)(i+1, j)[0]));
                        }
                        glEnd();
                    }
                }
            }

            glEnable(GL_LIGHTING);
            glEnable(GL_NORMALIZE);
            glEnable(GL_LIGHT0);
            if(ulines)
            {
                for(GLuint j = 0; j<_patches[ind]._u_isoparametric_lines->GetColumnCount(); ++j)
                {
                    (*_patches[ind]._u_isoparametric_lines)[j]->RenderDerivatives(0, GL_LINE_STRIP);
                }
            }

            if(vlines)
            {
                for(GLuint j = 0; j<_patches[ind]._v_isoparametric_lines->GetColumnCount(); ++j)
                {
                    (*_patches[ind]._v_isoparametric_lines)[j]->RenderDerivatives(0, GL_LINE_STRIP);
                }
            }

            shader.Enable();
            _patches[ind]._material->Apply();
            _patches[ind]._mesh->Render();
            shader.Disable();
        }
    }
}

GLvoid CompositeFirstOrderTrigonometricPatches3::InsertNewIsolatedPatch(
        GLdouble alpha,
        GLdouble x_min, GLdouble x_max,
        GLdouble y_min, GLdouble y_max,
        Material* material)
{
    GLdouble x_step = x_max - x_min;
    GLdouble y_step = y_max - y_min;

    FirstOrderTrigonometricPatches3 *patch = new FirstOrderTrigonometricPatches3(alpha, 4, 4);
    GLdouble x = x_min, y;

    for(GLuint i = 0; i < 4; i++)
    {
        x += x_step;
        y = y_min;
        for(GLuint j = 0; j < 4; j++)
        {
            y  += y_step;
            GLdouble z = (GLdouble)rand() / RAND_MAX*2 - 4;

            (*patch)(i, j)[0] = x;
            (*patch)(i, j)[1] = y;
            (*patch)(i, j)[2] = z;
        }
    }

    GLuint size = _patches.size();
    _patches.resize(size + 1);

    _patches[size]._patch = patch;
    _patches[size]._mesh = patch->GenerateImage(1, _u_div_point_count, _v_div_point_count);
    _patches[size]._mesh->UpdateVertexBufferObjects();

    _patches[size]._u_isoparametric_lines = _patches[size]._patch->GenerateUIsoparametricLines(1, _u_iso_line_count, _u_div_point_count);
    for(GLuint i = 0; i<_u_iso_line_count; i++)
    {
        (*_patches[size]._u_isoparametric_lines)[i]->UpdateVertexBufferObjects(1.0);
    }
    _patches[size]._v_isoparametric_lines = _patches[size]._patch->GenerateVIsoparametricLines(1, _v_iso_line_count, _v_div_point_count);

    for(GLuint i = 0; i<_v_iso_line_count; i++)
    {
        (*_patches[size]._v_isoparametric_lines)[i]->UpdateVertexBufferObjects(1.0);
    }
    _patches[size]._material = material;
}


GLboolean CompositeFirstOrderTrigonometricPatches3::UpdateVertexBufferObject()
{
    for(GLuint i=0; i<_patches.size(); i++)
    {
        _patches[i]._mesh->UpdateVertexBufferObjects();
    }
    return GL_TRUE;
}

GLboolean CompositeFirstOrderTrigonometricPatches3::DeleteVertexBufferObject()
{
    for(GLuint i=0; i<_patches.size(); i++)
    {
        _patches[i]._mesh->DeleteVertexBufferObjects();
    }
    return GL_TRUE;
}

GLvoid CompositeFirstOrderTrigonometricPatches3::UpdateIsoparametricLines(GLuint patch_index)
{
    _patches[patch_index]._mesh ->DeleteVertexBufferObjects();
    _patches[patch_index]._mesh = _patches[patch_index]._patch->GenerateImage(1, _u_div_point_count, _v_div_point_count);
    _patches[patch_index]._u_isoparametric_lines = _patches[patch_index]._patch->GenerateUIsoparametricLines(1, _u_iso_line_count, _u_div_point_count, GL_STATIC_DRAW);
    _patches[patch_index]._v_isoparametric_lines = _patches[patch_index]._patch->GenerateVIsoparametricLines(1, _v_iso_line_count, _v_div_point_count, GL_STATIC_DRAW);
}

GLvoid CompositeFirstOrderTrigonometricPatches3::Update()
{
    for(GLuint i=0; i<_patches.size(); i++)
    {
        UpdateIsoparametricLines(i);
        for(GLuint j=0; j<_patches[i]._neighbours.size(); j++)
        {
            JoinNeighbours(_patches[i]._neighbours[j]);
        }
    }
}


GLvoid CompositeFirstOrderTrigonometricPatches3::JoinNeighbours(NeighbourRelation n)
{
    switch (n._direction)
    {
    case 0:
        for(GLuint i = 0; i < 4; i++)
        {
            (*(n._child_patch))(3,i) = (*n._parent_patch)(0, i);
            (*(n._child_patch))(2,i) = 2*((*(n._parent_patch))(0, i)) - ((*(n._parent_patch))(1, i));
        }
        break;

    case 1:
        for(GLuint i = 0; i < 4; i++)
        {
            (*(n._child_patch))(i,0) = (*(n._parent_patch))(i, 3);
            (*(n._child_patch))(i,1) = 2*((*(n._parent_patch))(i, 3)) - ((*(n._parent_patch))(i, 2));
        }
        break;

    case 2:
        for(GLuint i = 0; i < 4; i++)
        {
            (*(n._child_patch))(0,i) = (*(n._parent_patch))(3, i);
            (*(n._child_patch))(1,i) = 2*((*(n._parent_patch))(3, i)) - ((*(n._parent_patch))(2, i));
        }
        break;

    case 3:
        for(GLuint i = 0; i < 4; i++)
        {
            (*(n._child_patch))(i,3) = (*(n._parent_patch))(i, 0);
            (*(n._child_patch))(i,2) = 2*((*(n._parent_patch))(i, 0)) - ((*(n._parent_patch))(i, 1));
        }
        break;
    }
}


//direction:
//1 - left
//2 - top
//3 - right
//4 - bottom

GLvoid CompositeFirstOrderTrigonometricPatches3::InsertNewNeighbourPatch(GLdouble alpha, GLuint patch_index, GLuint direction)
{
    FirstOrderTrigonometricPatches3 *new_patch = new FirstOrderTrigonometricPatches3(alpha, 4, 4);    //new patch

    int dx[] = {-1, 0, 1, 0};           //offset; dx[0]-dy[0] <-> left, etc.
    int dy[] = {0, 1, 0, -1};
    int scale = 3;                      //amount of offset

    for(GLuint i = 0; i < 4; i++)
    {
        for(GLuint j = 0; j < 4; j++)
        {
            (*new_patch)(i, j).x() = (*_patches[patch_index]._patch)(i, j).x() + scale*dx[direction];
            (*new_patch)(i, j).y() = (*_patches[patch_index]._patch)(i, j).y() + scale*dy[direction];
            (*new_patch)(i, j).z() = (*_patches[patch_index]._patch)(i, j).z();
        }
    }

    switch (direction)
    {
    case 0:
        for(GLuint i = 0; i < 4; i++)
        {
            (*new_patch)(3,i) = (*_patches[patch_index]._patch)(0, i);
            (*new_patch)(2,i) = 2*((*_patches[patch_index]._patch)(0, i)) - ((*_patches[patch_index]._patch)(1, i));
        }
        break;

    case 1:
        for(GLuint i = 0; i < 4; i++)
        {
            (*new_patch)(i,0) = (*_patches[patch_index]._patch)(i, 3);
            (*new_patch)(i,1) = 2*((*_patches[patch_index]._patch)(i, 3)) - ((*_patches[patch_index]._patch)(i, 2));
        }
        break;

    case 2:
        for(GLuint i = 0; i < 4; i++)
        {
            (*new_patch)(0,i) = (*_patches[patch_index]._patch)(3, i);
            (*new_patch)(1,i) = 2*((*_patches[patch_index]._patch)(3, i)) - ((*_patches[patch_index]._patch)(2, i));
        }
        break;

    case 3:
        for(GLuint i = 0; i < 4; i++)
        {
            (*new_patch)(i,3) = (*_patches[patch_index]._patch)(i, 0);
            (*new_patch)(i,2) = 2*((*_patches[patch_index]._patch)(i, 0)) - ((*_patches[patch_index]._patch)(i, 1));
        }
        break;
    }

    GLuint size = _patches.size();
    _patches.resize(size+1);

    _patches[size]._patch = new_patch;
    _patches[size]._mesh = new_patch->GenerateImage(1, _u_div_point_count, _v_div_point_count);
    _patches[size]._mesh->UpdateVertexBufferObjects();

    _patches[size]._u_isoparametric_lines = _patches[size]._patch->GenerateUIsoparametricLines(1, _u_iso_line_count, _u_div_point_count, GL_STATIC_DRAW);
    for(GLuint i = 0; i< _patches[patch_index]._u_isoparametric_lines->GetColumnCount(); i++)
    {
        (*_patches[size]._u_isoparametric_lines)[i]->UpdateVertexBufferObjects(GL_STATIC_DRAW);
    }

    _patches[size]._v_isoparametric_lines = _patches[size]._patch->GenerateVIsoparametricLines(1, _v_iso_line_count, _v_div_point_count, GL_STATIC_DRAW);

    for(GLuint i = 0; i< _patches[patch_index]._v_isoparametric_lines->GetColumnCount(); i++)
    {
        (*_patches[size]._v_isoparametric_lines)[i]->UpdateVertexBufferObjects(GL_STATIC_DRAW);
    }

    //default material and shader
    _patches[size]._material = &MatFBRuby;

    //add as neighbour to parent patch
    GLuint nsize = _patches[patch_index]._neighbours.size();
    _patches[patch_index]._neighbours.resize(nsize+1);
    _patches[patch_index]._neighbours[nsize]._direction = direction;
    _patches[patch_index]._neighbours[nsize]._parent_patch = _patches[patch_index]._patch;
    _patches[patch_index]._neighbours[nsize]._child_patch = _patches[size]._patch;
}

CompositeFirstOrderTrigonometricPatches3::~CompositeFirstOrderTrigonometricPatches3()
{
    this->_patches.clear();
}
