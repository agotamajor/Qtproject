#include "FirstOrderTrigonometricPatches3/FirstOrderTrigonometricPatches3.h"
#include "Core/ShaderPrograms.h"
#include "Core/Materials.h"
#include "Core/GenericCurves3.h"

namespace cagd
{
    class CompositeFirstOrderTrigonometricPatches3
    {
    public:
        class NeighbourRelation
        {
            public:
                FirstOrderTrigonometricPatches3* _parent_patch;
                FirstOrderTrigonometricPatches3* _child_patch;
                GLuint _direction;
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
                std::vector<NeighbourRelation>          _neighbors;
        };
    protected:

        GLuint maxi;
        GLuint maxj;
    public:
        std::vector<PatchAttributes> _patches;

        CompositeFirstOrderTrigonometricPatches3(GLuint maxi = 4, GLuint maxj = 4);
        CompositeFirstOrderTrigonometricPatches3(const CompositeFirstOrderTrigonometricPatches3& c);
        CompositeFirstOrderTrigonometricPatches3& operator =(const CompositeFirstOrderTrigonometricPatches3& c);
        ~CompositeFirstOrderTrigonometricPatches3();

        GLvoid InsertPatch(FirstOrderTrigonometricPatches3* p);
        GLvoid DeletePatch(GLuint index);
        GLvoid Render();
        GLvoid Render(int);

        GLvoid setPatch(GLuint index, FirstOrderTrigonometricPatches3* p);
        //GLvoid setMesh(GLuint index, TriangulatedMesh3* m);
        GLvoid setMaterial(GLuint index, Material* m);
        GLvoid setShader(GLuint index, ShaderProgram* s);

        FirstOrderTrigonometricPatches3* getPatch(GLuint index);
        //TriangulatedMesh3 getMesh(GLuint index);
        Material getMaterial(GLuint index);
        ShaderProgram getShader(GLuint index);


        // x,y,z min - max
        GLvoid InsertNewIsolatedPatch(
                GLdouble alpha,
                GLdouble,GLdouble,GLdouble,
                GLdouble,GLdouble,GLdouble,
                GLuint u_iso_lines, GLuint u_div_point_count,
                GLuint v_iso_lines, GLuint v_div_point_count);
        GLboolean UpdateVertexBufferObject();
        GLvoid AddNeighbouringPatch(GLdouble alpha, GLuint patch_index, GLuint direction);
        GLvoid UpdateIsoparametricLines(GLuint);
        GLvoid Update();
        GLvoid JoinNeighbours(NeighbourRelation n);
    };
}
