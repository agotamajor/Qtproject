#include "FirstOrderTrigonometric/FirstOrderTrigonometricPatches3.h"
#include "Core/ShaderPrograms.h"
#include "Core/Materials.h"
#include "Core/GenericCurves3.h"
#include <vector>

namespace cagd
{
class CompositeFirstOrderTrigonometricPatches3
{
public:
    class NeighbourRelation             //helps to keep track of joining
    {
    public:
        FirstOrderTrigonometricPatches3* _parent_patch;
        FirstOrderTrigonometricPatches3* _child_patch;
        GLuint _direction;              //top, left, bottom, right
    };

    struct PatchAttributes
    {
    public:
        FirstOrderTrigonometricPatches3*        _patch;
        TriangulatedMesh3*                      _mesh;
        Material*                               _material;
        ShaderProgram*                          _shader;
        RowMatrix<GenericCurve3*>*              _u_isoparametric_lines;
        RowMatrix<GenericCurve3*>*              _v_isoparametric_lines;
        std::vector<NeighbourRelation>          _neighbours;
        GLuint                                  _u_div_point_count, _v_div_point_count;
    };

protected:

    //controlnet attributes
    GLuint _controlnet_maxi;
    GLuint _controlnet_maxj;
public:
    std::vector<PatchAttributes> _patches;

    CompositeFirstOrderTrigonometricPatches3(GLuint controlnet_maxi = 4, GLuint controlnet_maxj = 4);
    CompositeFirstOrderTrigonometricPatches3(const CompositeFirstOrderTrigonometricPatches3& c);
    CompositeFirstOrderTrigonometricPatches3& operator =(const CompositeFirstOrderTrigonometricPatches3& c);

    GLvoid Render(int, bool);

    GLvoid DeletePatch(GLuint index);
    GLvoid setPatch(GLuint index, FirstOrderTrigonometricPatches3* p);
    FirstOrderTrigonometricPatches3* getPatch(GLuint index);

    GLvoid setMaterial(GLuint index, Material* material);
    Material getMaterial(GLuint index);

    GLvoid setShader(GLuint index, ShaderProgram* shader);
    ShaderProgram getShader(GLuint index);

    GLvoid setMesh(GLuint index, TriangulatedMesh3* mesh);
    TriangulatedMesh3 getMesh(GLuint index);

    GLvoid InsertNewIsolatedPatch(
            GLdouble alpha,
            GLdouble x_min,GLdouble x_max,GLdouble y_min, GLdouble y_max,
            GLuint u_iso_lines, GLuint u_div_point_count,
            GLuint v_iso_lines, GLuint v_div_point_count);
    GLboolean UpdateVertexBufferObject();
    GLboolean DeleteVertexBufferObject();
    GLvoid InsertNewNeighbourPatch(GLdouble alpha, GLuint patch_index, GLuint direction);
    GLvoid UpdateIsoparametricLines(GLuint);
    GLvoid Update();
    GLvoid JoinNeighbours(NeighbourRelation n);

    ~CompositeFirstOrderTrigonometricPatches3();
};
}
