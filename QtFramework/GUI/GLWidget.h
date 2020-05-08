#pragma once


#include <GL/glew.h>
#include <QGLWidget>
#include <QListWidget>
#include <QGLFormat>
#include <Core/Matrices.h>
#include <Core/DCoordinates3.h>
#include <Core/RealSquareMatrices.h>
#include "../Parametric/ParametricCurves3.h"
#include "../Parametric/ParametricSurfaces3.h"
#include <QTimer>
#include <QRadioButton>
#include "../Core/Lights.h"
#include "../Core/TriangulatedMeshes3.h"
#include "../Cyclic/CyclicCurves3.h"
#include "../Core/ShaderPrograms.h"

namespace cagd
{
    class GLWidget: public QGLWidget
    {
        Q_OBJECT

    private:

        // variables defining the projection matrix
        double       _aspect;            // aspect ratio of the rendering window
        double       _fovy;              // field of view in direction y
        double       _z_near, _z_far;    // distance of near and far clipping planes

        // variables defining the model-view matrix
        double       _eye[3], _center[3], _up[3];

        // variables needed by transformations
        int         _angle_x, _angle_y, _angle_z;
        double      _zoom;
        double      _trans_x, _trans_y, _trans_z;

        // your other declarations

        int choice;
        int tab;
        int material;
        int light;
        int nrshader;

        CyclicCurve3* _cc[2];
        GenericCurve3* _image_of_cc[2];

        ShaderProgram _shader_two_sided, _shader_directional, _shader_reflection, _shader_toon;

        //off
        QTimer *_timer;
        GLfloat _angle ;
        TriangulatedMesh3 _angel, _sphere, _cube, _cone, _icosahedron;
        TriangulatedMesh3 _mouse;

    public:
        // special and default constructor
        // the format specifies the properties of the rendering window
        GLWidget(QWidget* parent = 0);

        // redeclared virtual functions
        void initializeGL();
        void paintGL();
        void resizeGL(int w, int h);
        virtual ~GLWidget();

    public slots:
        // public event handling methods/slots
        void set_angle_x(int value);
        void set_angle_y(int value);
        void set_angle_z(int value);

        void set_zoom_factor(double value);

        void set_trans_x(double value);
        void set_trans_y(double value);
        void set_trans_z(double value);
        void set_selected_tab(int index);
        void set_selected_model(int index);
        void set_selected_material(int index);
        void set_selected_light(int index);
        void set_selected_shader(int index);

        void loadCyclicCurves();
        void loadModels();
        void loadShaders();
        void animate();

        void paintCyclicCurves();
        void paintModels();

    private:


    };
}