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
        RowMatrix<GenericCurve3*>*              _u_isoparametric_lines;
        RowMatrix<GenericCurve3*>*              _v_isoparametric_lines;
        std::vector<NeighbourRelation>          _neighbours;
    };

protected:

    //controlnet attributes
    GLuint _v_iso_line_count, _u_iso_line_count;
    GLuint _u_div_point_count, _v_div_point_count;

public:
    std::vector<PatchAttributes> _patches;

    CompositeFirstOrderTrigonometricPatches3(GLuint u_div_point_count, GLuint v_div_point_count, GLuint u_iso_line_count, GLuint v_iso_line_count);
    CompositeFirstOrderTrigonometricPatches3(const CompositeFirstOrderTrigonometricPatches3& c);
    CompositeFirstOrderTrigonometricPatches3& operator =(const CompositeFirstOrderTrigonometricPatches3& c);

    GLvoid Render(bool render_normal, bool control_points, bool controlnet, bool ulines, bool vlines, ShaderProgram shader);

    GLvoid setVIsoLineCount(GLuint v_iso_line_count);
    GLvoid setUIsoLineCount(GLuint u_iso_line_count);
    GLuint getVIsoLineCount();
    GLuint getUIsoLineCount();

    GLvoid DeletePatch(GLuint index);
    GLvoid setPatch(GLuint index, FirstOrderTrigonometricPatches3* p);
    FirstOrderTrigonometricPatches3* getPatch(GLuint index);

    GLvoid setMaterial(GLuint index, Material* material);
    Material getMaterial(GLuint index);

    GLvoid setMesh(GLuint index, TriangulatedMesh3* mesh);
    TriangulatedMesh3 getMesh(GLuint index);

    GLvoid InsertNewIsolatedPatch(GLdouble alpha, GLdouble x_min,GLdouble x_max,GLdouble y_min, GLdouble y_max, Material* material=&MatFBGold);
    GLboolean UpdateVertexBufferObject();
    GLboolean DeleteVertexBufferObject();
    GLvoid InsertNewNeighbourPatch(GLdouble alpha, GLuint patch_index, GLuint direction);
    GLvoid UpdateIsoparametricLines(GLuint);
    GLvoid Update();
    GLvoid JoinNeighbours(NeighbourRelation n);

    ~CompositeFirstOrderTrigonometricPatches3();
};
}
