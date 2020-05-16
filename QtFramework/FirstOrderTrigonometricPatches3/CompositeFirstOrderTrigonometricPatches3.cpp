#include "CompositeFirstOrderTrigonometricPatches3.h"
#include "FirstOrderTrigonometricPatches3/FirstOrderTrigonometricPatches3.h"
#include <typeinfo>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <QFile>
#include <QTextStream>
#include "FirstOrderTrigonometricPatches3/PatchConstants.h"

using namespace cagd;
using namespace std;

const int dx[] = {-1,0,1,0};
const int dy[] = {0,1,0,-1};

CompositeFirstOrderTrigonometricPatches3::CompositeFirstOrderTrigonometricPatches3(GLuint maxi, GLuint maxj)
{
    this->maxi = maxi;
    this->maxj = maxj;
}

CompositeFirstOrderTrigonometricPatches3::CompositeFirstOrderTrigonometricPatches3(const CompositeFirstOrderTrigonometricPatches3& c)
{
    this->maxi = c.maxi;
    this->maxj = c.maxj;
    this->_patches = c._patches;
}

CompositeFirstOrderTrigonometricPatches3& CompositeFirstOrderTrigonometricPatches3::operator =(const CompositeFirstOrderTrigonometricPatches3& c)
{
    this->maxi = c.maxi;
    this->maxj = c.maxj;
    this->_patches = c._patches;
    return *this;
}

CompositeFirstOrderTrigonometricPatches3::~CompositeFirstOrderTrigonometricPatches3()
{
    this->_patches.clear();
}

GLvoid CompositeFirstOrderTrigonometricPatches3::InsertPatch(FirstOrderTrigonometricPatches3* p)
{
    PatchAttributes attr;
    attr._patch = p;
    _patches.push_back(attr);
}

GLvoid CompositeFirstOrderTrigonometricPatches3::DeletePatch(GLuint index)
{
    _patches.erase(_patches.begin()+index-1);
}

GLvoid CompositeFirstOrderTrigonometricPatches3::Render()
{
        for (GLuint i = 0; i < _patches.size(); ++i)
        {
            if (_patches[i]._mesh && _patches[i]._material && _patches[i]._shader)
            {
                _patches[i]._shader->Enable();
                _patches[i]._material->Apply();
                _patches[i]._mesh->Render();
                _patches[i]._shader->Disable();
            }
        }
}

GLvoid CompositeFirstOrderTrigonometricPatches3::Render(int render_normal)
{
        for (GLuint i = 0; i < _patches.size(); ++i)
        {
            if (_patches[i]._mesh && _patches[i]._material && _patches[i]._shader)
            {
                if(render_normal == 2)
                {
                    _patches[i]._mesh->RenderNormals();
                }
                for(GLuint j = 0; j<_patches[i]._u_isoparametric_lines->GetColumnCount(); ++j)
                {
                   // (*_patches[i]._u_isoparametric_lines)[j]->RenderDerivatives(0,GL_LINE_STRIP);
                    //(*_patches[i]._u_isoparametric_lines)[j]->RenderDerivatives(1,GL_LINES);
                }
                _patches[i]._material->Apply();
                _patches[i]._shader->Enable();
                _patches[i]._mesh->Render();
                _patches[i]._shader->Disable();
            }
        }
}

GLvoid CompositeFirstOrderTrigonometricPatches3::setPatch(GLuint index, FirstOrderTrigonometricPatches3* p)
{
    _patches[index]._patch=p;
}

GLvoid CompositeFirstOrderTrigonometricPatches3:: setMaterial(GLuint index, Material* m)
{
    _patches[index]._material = m;
}

GLvoid CompositeFirstOrderTrigonometricPatches3::setShader(GLuint index, ShaderProgram* s)
{
    _patches[index]._shader = s;
}

FirstOrderTrigonometricPatches3* CompositeFirstOrderTrigonometricPatches3::getPatch(GLuint index)
{
    return _patches[index]._patch;
}

Material CompositeFirstOrderTrigonometricPatches3::getMaterial(GLuint index)
{
    return *(_patches[index]._material);
}

ShaderProgram CompositeFirstOrderTrigonometricPatches3::getShader(GLuint index)
{
    return *(_patches[index]._shader);
}

GLvoid CompositeFirstOrderTrigonometricPatches3::InsertNewIsolatedPatch(
        GLdouble alpha,
        GLdouble x_min, GLdouble x_max,
        GLdouble y_min, GLdouble y_max,
        GLdouble z_min, GLdouble z_max,
        GLuint u_iso_lines, GLuint u_div_point_count,
        GLuint v_iso_lines, GLuint v_div_point_count)
{
    GLdouble x_step = (x_max - x_min) / 3.0;
    GLdouble y_step = (y_max - y_min) / 3.0;
    GLdouble z_step = (z_max - z_min) / 3.0;

    FirstOrderTrigonometricPatches3 *patch = new FirstOrderTrigonometricPatches3(alpha);

    for(GLuint i = 0; i < 4; ++ i)
    {
        GLdouble x = x_min + i * x_step;
        for(GLuint j = 0; j < 4; ++ j)
        {
            GLdouble y  = y_min + j * y_step;
            GLdouble z = qrand() % ((2) + 2) - 2;

            (*patch)(i, j)[0] = x;
            (*patch)(i, j)[1] = y;
            (*patch)(i, j)[2] = z; // / / / //
        }
    }

    GLuint size = _patches.size();
    _patches.resize(size + 1);

    _patches[size]._patch = patch;
    _patches[size]._mesh = patch->GenerateImage(1, 50, 50);
    _patches[size]._u_isoparametric_lines = _patches[size]._patch->GenerateUIsoparametricLines(1, u_iso_lines, u_div_point_count, GL_STATIC_DRAW);
    for(GLuint i = 0; i<u_iso_lines; i++)
    {
        (*_patches[size]._u_isoparametric_lines)[i]->UpdateVertexBufferObjects(GL_STATIC_DRAW);
    }
    _patches[size]._v_isoparametric_lines = _patches[size]._patch->GenerateVIsoparametricLines(1, v_iso_lines, v_div_point_count, GL_STATIC_DRAW);
    _patches[size]._shader = new ShaderProgram();
}

GLvoid CompositeFirstOrderTrigonometricPatches3::UpdateIsoparametricLines(GLuint patch_index)
{
    _patches[patch_index]._mesh = _patches[patch_index]._patch->GenerateImage(1, 50, 50);
    _patches[patch_index]._u_isoparametric_lines = _patches[patch_index]._patch->GenerateUIsoparametricLines(1, 10, 50, GL_STATIC_DRAW);
    _patches[patch_index]._v_isoparametric_lines = _patches[patch_index]._patch->GenerateVIsoparametricLines(1, 10, 50, GL_STATIC_DRAW);

}

GLvoid CompositeFirstOrderTrigonometricPatches3::Update()
{
    for(GLuint i=0; i<_patches.size(); i++)
    {
        _patches[i]._mesh = _patches[i]._patch->GenerateImage(1, 50, 50);
        _patches[i]._u_isoparametric_lines = _patches[i]._patch->GenerateUIsoparametricLines(1, 10, 50, GL_STATIC_DRAW);
        _patches[i]._v_isoparametric_lines = _patches[i]._patch->GenerateVIsoparametricLines(1, 10, 50, GL_STATIC_DRAW);
        for(GLuint j=0; j<_patches[i]._neighbors.size(); j++)
        {
            JoinNeighbours(_patches[i]._neighbors[j]);
        }
    }
}


GLboolean CompositeFirstOrderTrigonometricPatches3::UpdateVertexBufferObject()
{
    for(GLuint i=0; i<_patches.size(); i++)
    {
        _patches[i]._mesh->UpdateVertexBufferObjects();
    }
    return GL_TRUE;
}

GLvoid CompositeFirstOrderTrigonometricPatches3::JoinNeighbours(NeighbourRelation n)
{
    switch (n._direction)
    {
        case TOP:
        for(GLuint i = 0; i < 4; ++ i)
        {
            (*(n._child_patch))(3,i) = (*n._parent_patch)(0, i);
            (*(n._child_patch))(2,i) = 2*((*(n._parent_patch))(0, i)) - ((*(n._parent_patch))(1, i));
        }
        break;

        case RIGHT:
        for(GLuint i = 0; i < 4; ++ i)
        {
            (*(n._child_patch))(i,0) = (*(n._parent_patch))(i, 3);
            (*(n._child_patch))(i,1) = 2*((*(n._parent_patch))(i, 3)) - ((*(n._parent_patch))(i, 2));
        }
        break;

        case BOTTOM:
        for(GLuint i = 0; i < 4; ++ i)
        {
            (*(n._child_patch))(0,i) = (*(n._parent_patch))(3, i);
            (*(n._child_patch))(1,i) = 2*((*(n._parent_patch))(3, i)) - ((*(n._parent_patch))(2, i));
        }
        break;

        case LEFT:
        for(GLuint i = 0; i < 4; ++ i)
        {
            (*(n._child_patch))(i,3) = (*(n._parent_patch))(i, 0);
            (*(n._child_patch))(i,2) = 2*((*(n._parent_patch))(i, 0)) - ((*(n._parent_patch))(i, 1));
        }
        break;
    }
}

GLvoid CompositeFirstOrderTrigonometricPatches3::AddNeighbouringPatch(GLdouble alpha, GLuint patch_index, GLuint direction)
{
    FirstOrderTrigonometricPatches3 *new_patch = new FirstOrderTrigonometricPatches3(alpha);


    for(GLuint i = 0; i < 4; ++ i)
    {
        for(GLuint j = 0; j < 4; ++ j)
        {
            (*new_patch)(i, j) = (*_patches[patch_index]._patch)(i, j);
        }
    }

    for(GLuint i = 0; i < 4; ++ i)
    {
        for(GLuint j = 0; j < 4; ++ j)
        {
            (*new_patch)(i, j).x() += 2*dx[direction];
            (*new_patch)(i, j).y() += 2*dy[direction];
        }
    }

    switch (direction)
    {
        case TOP:
        for(GLuint i = 0; i < 4; ++ i)
        {
            (*new_patch)(3,i) = (*_patches[patch_index]._patch)(0, i);
            (*new_patch)(2,i) = 2*((*_patches[patch_index]._patch)(0, i)) - ((*_patches[patch_index]._patch)(1, i));
        }
        break;

        case RIGHT:
        for(GLuint i = 0; i < 4; ++ i)
        {
            (*new_patch)(i,0) = (*_patches[patch_index]._patch)(i, 3);
            (*new_patch)(i,1) = 2*((*_patches[patch_index]._patch)(i, 3)) - ((*_patches[patch_index]._patch)(i, 2));
        }
        break;

        case BOTTOM:
        for(GLuint i = 0; i < 4; ++ i)
        {
            (*new_patch)(0,i) = (*_patches[patch_index]._patch)(3, i);
            (*new_patch)(1,i) = 2*((*_patches[patch_index]._patch)(3, i)) - ((*_patches[patch_index]._patch)(2, i));
        }
        break;

        case LEFT:
        for(GLuint i = 0; i < 4; ++ i)
        {
            (*new_patch)(i,3) = (*_patches[patch_index]._patch)(i, 0);
            (*new_patch)(i,2) = 2*((*_patches[patch_index]._patch)(i, 0)) - ((*_patches[patch_index]._patch)(i, 1));
        }
        break;
    }

    GLuint size = _patches.size();
    _patches.resize(size+1);

    _patches[size]._patch = new_patch;
    _patches[size]._mesh = new_patch->GenerateImage(1, 50, 50);
    _patches[size]._mesh->UpdateVertexBufferObjects();

    _patches[size]._u_isoparametric_lines = _patches[size]._patch->GenerateUIsoparametricLines(1, 10, 50, GL_STATIC_DRAW);
    _patches[size]._v_isoparametric_lines = _patches[size]._patch->GenerateVIsoparametricLines(1, 10, 50, GL_STATIC_DRAW);
    _patches[size]._material = &MatFBEmerald;
    _patches[size]._shader = new ShaderProgram();

    GLuint nsize = _patches[patch_index]._neighbors.size();

    _patches[patch_index]._neighbors.resize(nsize+1);

    GLuint nsize2 = _patches[patch_index]._neighbors.size();

    _patches[patch_index]._neighbors[nsize]._parent_patch = _patches[patch_index]._patch;
    _patches[patch_index]._neighbors[nsize]._child_patch = _patches[size]._patch;
    _patches[patch_index]._neighbors[nsize]._direction = direction;
}
