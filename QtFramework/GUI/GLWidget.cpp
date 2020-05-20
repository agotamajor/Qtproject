#include "GLWidget.h"

#if !defined(__APPLE__)
#include <GL/glu.h>
#include <QListWidget>
#include <QTabWidget>

#endif

#include <fstream>
#include <iostream>
#include "../Core/Exceptions.h"
#include "../Core/Matrices.h"
#include "../Test/TestFunctions.h"
#include "../Core/Materials.h"
#include "../Core/Constants.h"
#include "../Core/HCoordinates3.h"
#include "../Core/Lights.h"
#include <QMouseEvent>

using namespace std;

namespace cagd
{
//--------------------------------
// special and default constructor
//--------------------------------
GLWidget::GLWidget(QWidget *parent): QGLWidget(parent)
{

}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    xpress = event->x();
    ypress = event->y();
    QWidget::mousePressEvent(event);
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    xrelease = event->x();
    yrelease = event->y();
    QWidget::mouseReleaseEvent(event);
}

//--------------------------------------------------------------------------------------
// this virtual function is called once before the first call to paintGL() or resizeGL()
//--------------------------------------------------------------------------------------
void GLWidget::initializeGL()
{
    // creating a perspective projection matrix
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    _aspect = (double)width() / (double)height();
    _z_near = 1.0;
    _z_far  = 1000.0;
    _fovy   = 45.0;


    choice = 0;
    light = 0;
    tab = 1;
    nrshader = 0;
    material = 0;
    alpha = 1.0;

    gluPerspective(_fovy, _aspect, _z_near, _z_far);

    // setting the model view matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    _eye[0] = _eye[1] = 0.0; _eye[2] = 6.0;
    _center[0] = _center[1] = _center[2] = 0.0;
    _up[0] = _up[2] = 0.0; _up[1] = 1.0;

    gluLookAt(_eye[0], _eye[1], _eye[2], _center[0], _center[1], _center[2], _up[0], _up[1], _up[2]);

    // enabling the depth test
    glEnable(GL_DEPTH_TEST);

    // setting the background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // initial values of transformation parameters
    _angle_x = _angle_y = _angle_z = 0.0;
    _trans_x = _trans_y = _trans_z = 0.0;
    _zoom = 1.0;

    selected_patch = 0;
    selected_point_row = 0;
    selected_point_column = 0;
    normal_render = 0;
    // initial values of choice

    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_DEPTH_TEST);

    glewInit();
    loadCyclicCurves();
    loadModels();
    loadShaders();
    loadFOTPatch();

    try
    {
        // initializing the OpenGL Extension Wrangler library
        GLenum error = glewInit();

        if (error != GLEW_OK)
        {
            throw Exception("Could not initialize the OpenGL Extension Wrangler Library!");
        }

        if (!glewIsSupported("GL_VERSION_2_0"))
        {
            throw Exception("Your graphics card is not compatible with OpenGL 2.0+! "
                            "Try to update your driver or buy a new graphics adapter!");
        }

        // create and store your geometry in display lists or vertex buffer objects
        // ...

    }
    catch (Exception &e)
    {
        cout << e << endl;
    }
}

//---------------------------------------------------------------------------
//   Initialization
//----------------------------------------------------------------------------


void GLWidget::loadCyclicCurves()
{
    GLenum usage_flag = GL_STATIC_DRAW;
    GLuint n = div_point_count;
    GLdouble u;
    GLdouble step = TWO_PI / (2*n + 1);

    _cc[0] = nullptr;
    _image_of_cc[0] = nullptr;
    _cc[0] = new (nothrow) CyclicCurve3(n, usage_flag);
    if(!_cc[0])
    {
        cout << "The generation of the cyclic curve was unsuccessful" << std::endl;
        return;
    }


    for(GLuint i = 0; i <= 2*n; i++)
    {
        u = i*step;
        DCoordinate3 &cp = (*_cc[0])[i];

        cp[0] = cos(u);
        cp[1] = sin(u);
        cp[2] = 0;
    }


    if(!_cc[0]->UpdateVertexBufferObjectsOfData(usage_flag))
    {
        cout << "The update was unsuccessful" << std::endl;
        return;
    }

    _cc[1] = nullptr;
    _image_of_cc[1] = nullptr;
    _cc[1] = new (nothrow) CyclicCurve3(2, usage_flag);
    if(!_cc[1])
    {
        cout << "The generation of the cyclic curve was unsuccessful" << std::endl;
        return;
    }

    GLint nknot = 5;
    ColumnMatrix<GLdouble> knot_vector(nknot);
    ColumnMatrix<DCoordinate3> points(nknot);
    points[0] = DCoordinate3(-1, 0, 1);
    points[1] = DCoordinate3(0, 1, 1);
    points[2] = DCoordinate3(1, 0, 0);
    points[3] = DCoordinate3(0, -1, 0);
    points[4] = DCoordinate3(-1, 0, 0);


    for(int i = 0; i < nknot; i++)
    {
        knot_vector[i] = i*step;
    }

    if(!_cc[1]->UpdateDataForInterpolation(knot_vector, points))
    {
        cout << "The update was unsuccessful" << std::endl;
        return;
    }

    if(!_cc[1]->UpdateVertexBufferObjectsOfData(usage_flag))
    {
        cout << "The update was unsuccessful" << std::endl;
        return;
    }

}

void GLWidget::loadModels(){
    _angel.LoadFromOFF("Models/angel.off", true);
    _angel.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW);

    _cone.LoadFromOFF("Models/cone.off",true);
    _cone.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW);

    _cube.LoadFromOFF("Models/cube.off",true);
    _cube.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW);

    _icosahedron.LoadFromOFF("Models/icosahedron.off",true);
    _icosahedron.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW);
}


void GLWidget::loadShaders(){
    if (!_shader_two_sided.InstallShaders("Shaders/two_sided_lighting.vert ", "Shaders/two_sided_lighting.frag", GL_TRUE)) {
        cout<<"Shader error"<<std::endl;
        return;
    }

    if (!_shader_toon.InstallShaders("Shaders/toon.vert ", "Shaders/toon.frag", GL_TRUE)) {
        cout<<"Shader error"<<std::endl;
        return;
    }

    if (!_shader_directional.InstallShaders("Shaders/directional_light.vert ", "Shaders/directional_light.frag", GL_TRUE)) {
        cout<<"Shader error"<<std::endl;
        return;
    }

    if (!_shader_reflection.InstallShaders("Shaders/reflection_lines.vert ", "Shaders/reflection_lines.frag", GL_TRUE)) {
        cout<<"Shader error"<<std::endl;
        return;
    }
}

void GLWidget::loadFOTPatch()
{
    _compsurface = new CompositeFirstOrderTrigonometricPatches3(4,4);

    _compsurface->InsertNewIsolatedPatch(alpha, -0.5, 0.5, -0.5, 0.5, 10, 50, 10, 50);
    _compsurface->UpdateVertexBufferObject();

    _compsurface->InsertNewNeighbourPatch(alpha, 0, 0);
    _compsurface->InsertNewNeighbourPatch(alpha, 0, 1);
    _compsurface->InsertNewNeighbourPatch(alpha, 0, 2);
    _compsurface->InsertNewNeighbourPatch(alpha, 0, 3);

    _compsurface->setMaterial(0, &MatFBGold);
    _compsurface->setShader(0, &_shader_two_sided);
    _compsurface->setMaterial(1, &MatFBBrass);
    _compsurface->setShader(1, &_shader_two_sided);
}

void GLWidget::loadFOTArc()
{

}

//-----------------------
// the rendering function
//-----------------------
void GLWidget::paintGL()
{
    // clears the color and depth buffers

    // stores/duplicates the original model view matrix
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    // applying transformations
    glRotatef(_angle_x, 1.0, 0.0, 0.0);
    glRotatef(_angle_y, 0.0, 1.0, 0.0);
    glRotatef(_angle_z, 0.0, 0.0, 1.0);
    glTranslated(_trans_x, _trans_y, _trans_z);
    glScaled(_zoom, _zoom, _zoom);

    switch(tab){
    case 0:
        paintFOTPatch();
        break;
    case 1:
        paintCyclicCurves(_curve);
        break;
    case 2:
        paintModels();
        break;
    }

    // pops the current matrix stack, replacing the current matrix with the one below it on the stack,
    // i.e., the original model view matrix is restored
    glPopMatrix();
}

//---------------------------------------------------------------------------
//   Drawing
//----------------------------------------------------------------------------

void GLWidget::paintCyclicCurves(int nr)
{
    GLenum usage_flag = GL_STATIC_DRAW;
    _image_of_cc[nr] = _cc[nr]->GenerateImage(2, 200, usage_flag);

    if(!_image_of_cc[nr])
    {
        cout << "The generation of the cyclic curve was unsuccessful" << std::endl;
        return;
    }

    if(!_image_of_cc[nr]->UpdateVertexBufferObjects(_scale_nr, usage_flag))
    {
        cout << "The update was unsuccessful" << std::endl;
        return;
    }

    if (_image_of_cc[nr]) {
        glColor3f (1.0f, 0.972f, 0.239f);
        _image_of_cc[nr]->RenderDerivatives(0, GL_LINE_LOOP);

        glColor3f (1.0f, 0.972f, 0.239f);
        _cc[nr]->RenderData(GL_LINE_LOOP);
        glPointSize (8.0f);

        if (_control_points_cc)
            _cc[nr]->RenderData(GL_POINTS);

        glPointSize (5.0f);
        if (_d1) {
            glColor3f (1.0f, 0.517f, 0.239f);
            _image_of_cc[nr]->RenderDerivatives(1, GL_LINES);
        }
        if (_d2) {
            glColor3f (0.780f, 0.239f, 1.0f);
            _image_of_cc[nr]->RenderDerivatives(2, GL_LINES);
        }
        glPointSize (1.0f);
    }
}

void GLWidget::paintModels()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHT0);

//    dl = 0;
//    HCoordinate3 direction(0.0, 0.0, 0.0, 1.0);
//    Color4         ambient(1.0, 0.8, 0.4, 1.0);
//    Color4         diffuse(0.8, 0.8, 0.8, 1.0);
//    Color4        specular(1.0, 1.0, 1.0, 1.0);

//    switch(light){
//    case 0:
//        glEnable(GL_LIGHT0);
//        dl = new DirectionalLight (GL_LIGHT0, direction, ambient, diffuse, specular);
//        break;
//    case 1:
//        glEnable(GL_LIGHT1);
//        dl = new DirectionalLight (GL_LIGHT1, direction, ambient, diffuse, specular);
//        break;
//    case 2:
//        glEnable(GL_LIGHT2);
//        dl = new DirectionalLight (GL_LIGHT2, direction, ambient, diffuse, specular);
//        break;
//    case 3:
//        glEnable(GL_LIGHT3);
//        dl = new DirectionalLight (GL_LIGHT3, direction, ambient, diffuse, specular);
//        break;
//    case 4:
//        glEnable(GL_LIGHT4);
//        dl = new DirectionalLight (GL_LIGHT4, direction, ambient, diffuse, specular);
//        break;
//    case 5:
//        glEnable(GL_LIGHT5);
//        dl = new DirectionalLight (GL_LIGHT5, direction, ambient, diffuse, specular);
//        break;
//    case 6:
//        glEnable(GL_LIGHT6);
//        dl = new DirectionalLight (GL_LIGHT6, direction, ambient, diffuse, specular);
//        break;
//    default:
//        glEnable(GL_LIGHT7);
//        dl = new DirectionalLight (GL_LIGHT7, direction, ambient, diffuse, specular);
//        break;
//    }

    //dl->Enable();
    try {

        switch(nrshader){
        case 0:
            //none
            break;
        case 1:
            _shader_two_sided.Enable();
            break;
        case 2:
            _shader_toon.Enable();
            break;
        case 3:
            _shader_directional.Enable();
            break;
        default:
            _shader_reflection.Enable();
        }

    } catch ( Exception &e)
    {
        cerr << e << endl ;
    }

    switch(material){
    case 0:
        MatFBBrass.Apply();
        break;
    case 1:
        MatFBGold.Apply();
        break;
    case 2:
        MatFBSilver.Apply();
        break;
    case 3:
        MatFBEmerald.Apply();
        break;
    case 4:
        MatFBPearl.Apply();
        break;
    case 5:
        MatFBRuby.Apply();
        break;
    default:
        MatFBTurquoise.Apply();
        break;
    }

    switch(choice){
    case 0:
        _angel.Render();
        break;
    case 1:
        _cone.Render();
        break;
    case 2:
        _cube.Render();
        break;
    default:
        _icosahedron.Render();
        break;
    }
//    dl->Disable();

//    if (dl) {
//        delete dl;
//        dl = 0;
//    }

    _shader_two_sided.Disable();
    _shader_directional.Disable();
    _shader_toon.Disable();
    _shader_reflection.Disable();
    glDisable (GL_LIGHTING);
    glDisable(GL_NORMALIZE);
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHT1);
    glDisable(GL_LIGHT2);
    glDisable(GL_LIGHT3);
    glDisable(GL_LIGHT4);
    glDisable(GL_LIGHT5);
    glDisable(GL_LIGHT6);
    glDisable(GL_LIGHT7);
}

void GLWidget::paintFOTPatch()
{
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHT0);

    _compsurface->Render(normal_render, control_points_fot);

    glDisable (GL_LIGHTING);
    glDisable(GL_NORMALIZE);
    glDisable(GL_LIGHT0);
}

void GLWidget::paintFOTArc()
{

}

//----------------------------------------------------------------------------
// when the main window is resized one needs to redefine the projection matrix
//----------------------------------------------------------------------------
void GLWidget::resizeGL(int w, int h)
{
    // setting the new size of the rendering context
    glViewport(0, 0, w, h);

    // redefining the projection matrix
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    _aspect = (double)w / (double)h;

    gluPerspective(_fovy, _aspect, _z_near, _z_far);

    // switching back to the model view matrix
    glMatrixMode(GL_MODELVIEW);

    updateGL();
}

//-----------------------------------
// implementation of the public slots
//-----------------------------------

void GLWidget::set_angle_x(int value)
{
    if (_angle_x != value)
    {
        _angle_x = value;
        updateGL();
    }
}

void GLWidget::set_angle_y(int value)
{
    if (_angle_y != value)
    {
        _angle_y = value;
        updateGL();
    }
}

void GLWidget::set_angle_z(int value)
{
    if (_angle_z != value)
    {
        _angle_z = value;
        updateGL();
    }
}

void GLWidget::set_zoom_factor(double value)
{
    if (_zoom != value)
    {
        _zoom = value;
        updateGL();
    }
}

void GLWidget::set_trans_x(double value)
{
    if (_trans_x != value)
    {
        _trans_x = value;
        updateGL();
    }
}

void GLWidget::set_trans_y(double value)
{
    if (_trans_y != value)
    {
        _trans_y = value;
        updateGL();
    }
}

void GLWidget::set_trans_z(double value)
{
    if (_trans_z != value)
    {
        _trans_z = value;
        updateGL();
    }
}

void GLWidget::set_selected_model(int index)
{
    choice = index;
    updateGL();
}

void GLWidget::set_selected_tab(int index)
{
    choice=0;
    tab = index;
    updateGL();
}

void GLWidget::set_selected_material(int index)
{
    material = index;
    updateGL();
}

void GLWidget::set_selected_light(int index){
    light = index;
    updateGL();
}

void GLWidget::set_selected_shader(int index){
    nrshader = index;
    updateGL();
}


//cyclic curve --------------------------------
void GLWidget::set_d1(int value)
{
    if(value)
    {
        _d1 = true;
    }
    else
    {
        _d1 = false;
    }
    updateGL();
}

void GLWidget::set_d2(int value)
{
    if(value)
    {
        _d2 = true;
    }
    else
    {
        _d2 = false;
    }
    updateGL();
}

void GLWidget::set_control_point_cc(int value)
{
    if(value)
    {
        _control_points_cc = true;
    }
    else
    {
        _control_points_cc = false;
    }
    updateGL();
}

void GLWidget::set_control_point_nr(int value)
{
    if(div_point_count != (GLuint)value)
    {
        div_point_count = (GLuint)value;
        deleteCyclicCurve();
        loadCyclicCurves();
        updateGL();
    }
}

void GLWidget::set_scale(double value)
{
    if(_scale_nr != value)
    {
        _scale_nr = value;
        _image_of_cc[_curve]->UpdateVertexBufferObjects(_scale_nr);
        updateGL();
    }

}

void GLWidget::set_curve(int value)
{
    if(_curve != value)
    {
        _curve = value;
        updateGL();
    }

}

void GLWidget::deleteCyclicCurve()
{
    if(_cc[_curve])
    {
        delete _cc[_curve];
        _cc[_curve] = nullptr;
    }
    if(_image_of_cc[_curve])
    {
        delete _image_of_cc[_curve];
        _image_of_cc[_curve] = nullptr;
    }
}

//first order trigonometric patch -------------------------

    void GLWidget::set_selected_alpha(double value)
    {
        if (alpha != value)
        {
            alpha = value;
            delete _compsurface;
            _compsurface = 0;
            loadFOTPatch();
            updateGL();
        }
    }

    void GLWidget::set_selected_patch(int index)
    {
        selected_patch = index;
    }

    void GLWidget::set_selected_point_row(int index)
    {
        selected_point_row = index;
    }

    void GLWidget::set_selected_point_column(int index)
    {
        selected_point_column = index;
    }

    void GLWidget::set_patch_point_x(double value)
    {
        point_x = value;

        FirstOrderTrigonometricPatches3* p = _compsurface->_patches[selected_patch]._patch;
        (*p)(selected_point_row, selected_point_column)[0] = point_x;
        _compsurface->Update();
        _compsurface->UpdateVertexBufferObject();
        updateGL();

    }

    void GLWidget::set_patch_point_y(double value)
    {
        point_y = value;

        FirstOrderTrigonometricPatches3* p = _compsurface->_patches[selected_patch]._patch;
        (*p)(selected_point_row, selected_point_column)[1] = point_y;
        _compsurface->Update();
        _compsurface->UpdateVertexBufferObject();
        updateGL();
    }

    void GLWidget::set_patch_point_z(double value)
    {
        point_z = value;

        FirstOrderTrigonometricPatches3* p = _compsurface->_patches[selected_patch]._patch;
        (*p)(selected_point_row, selected_point_column)[2] = point_z;
        _compsurface->Update();
        _compsurface->UpdateVertexBufferObject();
        updateGL();
    }

    void GLWidget::set_normal_render(int state)
    {
        normal_render = state;
        updateGL();
    }

    void GLWidget::set_control_point_fot(int value)
    {
          if(value)
          {
             control_points_fot = true;
          }
          else
          {
             control_points_fot = false;
          }
          updateGL();
    }


    GLWidget ::~GLWidget()
    {

        deleteCyclicCurve();
        if(_compsurface)
        {
            delete _compsurface;
            _compsurface = 0;
        }
    }

}

