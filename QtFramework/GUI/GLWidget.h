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
#include "../FirstOrderTrigonometric/FirstOrderTrigonometricArc3.h"
#include "../FirstOrderTrigonometric/FirstOrderTrigonometricPatches3.h"
#include "../FirstOrderTrigonometric/CompositeFirstOrderTrigonometricPatches3.h"

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
        DirectionalLight *dl;
        ShaderProgram _shader_two_sided, _shader_directional, _shader_reflection, _shader_toon;

// cyclic curve ----------------------------
        double _scale_nr = 1.0;
        int _curve = 0;
        bool _d1 = false;
        bool _d2 = false;
        bool _control_points_cc = false;
        GLuint div_point_count = 1;

        CyclicCurve3* _cc[2];
        GenericCurve3* _image_of_cc[2];

// models -----------------------------------
        QTimer *_timer;
        GLfloat _angle ;
        TriangulatedMesh3 _angel, _sphere, _cube, _cone, _icosahedron, _mouse;

// projekt ----------------------------------
        double alpha;
        int selected_patch;
        int selected_point_row;
        int selected_point_column;
        double point_x;
        double point_y;
        double point_z;
        bool control_points_fot=false;
        int normal_render;

        //arc
        FirstOrderTrigonometricArc3* p;
        GenericCurve3* _image_of_fot;

        //patch
        CompositeFirstOrderTrigonometricPatches3* _compsurface;
        FirstOrderTrigonometricPatches3 _patch;

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

//definition, initialization --------------------------------
        void loadCyclicCurves();
        void loadModels();
        void loadShaders();
        void loadFOTPatch();
        void loadFOTArc();

//drawing, rendering ----------------------------------------
        void paintCyclicCurves(int nr);
        void paintModels();
        void paintFOTPatch();
        void paintFOTArc();

//cyclic curve ----------------------------------------------
        void set_d1(int value);
        void set_d2(int value);
        void set_scale(double value);
        void set_control_point_cc(int value);
        void set_control_point_nr(int value);
        void set_curve(int value);

        void deleteCyclicCurve();

//first order trigonometric surface ------------------------
        void set_selected_alpha(double value);
        void set_selected_patch(int index);
        void set_selected_point_row(int index);
        void set_selected_point_column(int index);
        void set_patch_point_x(double value);
        void set_patch_point_y(double value);
        void set_patch_point_z(double value);
        void set_normal_render(int state);
        void set_control_point_fot(int value);

    private:
        GLuint xpress, xrelease, ypress, yrelease;
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);

    };
}
