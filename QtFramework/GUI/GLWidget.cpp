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


using namespace std;



namespace cagd
{
    //--------------------------------
    // special and default constructor
    //--------------------------------
    GLWidget::GLWidget(QWidget *parent): QGLWidget(parent)
    {
        _timer = new QTimer(this);
        _timer->setInterval(0);
        connect(_timer, SIGNAL(timeout()), this, SLOT(animate()));
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
        tab = 0;
        nrshader = 0;
        material = 0;
        derivative1 = 2;
        derivative2 = 2;
        animation = 0;

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
        loadCurves();
        loadCyclicCurves();
        loadModels();
        loadSurfaces();
        loadShaders();

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

    void GLWidget::loadCurves(){
        RowMatrix<ParametricCurve3::Derivative> derivative(3);
        for(GLint i = 0; i < 8; i++){
            _pc[i] = nullptr;
        }

        derivative(0) = spiral_on_cone::d0;
        derivative(1) = spiral_on_cone::d1;
        derivative(2) = spiral_on_cone::d2;
        _pc[0] = new (nothrow)ParametricCurve3(derivative, spiral_on_cone::u_min, spiral_on_cone::u_max);

        derivative(0) = rose_curve1::d0;
        derivative(1) = rose_curve1::d1;
        derivative(2) = rose_curve1::d2;
        _pc[1] = new (nothrow)ParametricCurve3(derivative, rose_curve1::u_min, rose_curve1::u_max);

        derivative(0) = rose_curve2::d0;
        derivative(1) = rose_curve2::d1;
        derivative(2) = rose_curve2::d2;
        _pc[2] = new (nothrow)ParametricCurve3(derivative, rose_curve2::u_min, rose_curve2::u_max);

        derivative(0) = cycloid::d0;
        derivative(1) = cycloid::d1;
        derivative(2) = cycloid::d2;
        _pc[3] = new (nothrow)ParametricCurve3(derivative, cycloid::u_min, cycloid::u_max);

        derivative(0) = nephroid::d0;
        derivative(1) = nephroid::d1;
        derivative(2) = nephroid::d2;
        _pc[4] = new (nothrow)ParametricCurve3(derivative, nephroid::u_min, nephroid::u_max);

        derivative(0) = witch_of_agnesi::d0;
        derivative(1) = witch_of_agnesi::d1;
        derivative(2) = witch_of_agnesi::d2;
        _pc[5] = new (nothrow)ParametricCurve3(derivative, witch_of_agnesi::u_min, witch_of_agnesi::u_max);

        derivative(0) = fermat_spiral::d0;
        derivative(1) = fermat_spiral::d1;
        derivative(2) = fermat_spiral::d2;
        _pc[6] = new (nothrow)ParametricCurve3(derivative, fermat_spiral::u_min, fermat_spiral::u_max);

        derivative(0) = lissajous_knockoff::d0;
        derivative(1) = lissajous_knockoff::d1;
        derivative(2) = lissajous_knockoff::d2;
        _pc[7] = new (nothrow)ParametricCurve3(derivative, lissajous_knockoff::u_min, lissajous_knockoff::u_max);

        for(GLint i = 0; i < 8; i++){
            if (!_pc[i])
            {
                // error : either close the application , or handle this exception
                cout << "The generation of the parametric curve was unsuccessful" << std::endl;
                return;

            }
        }
    }

    void GLWidget::loadCyclicCurves()
    {
        GLenum usage_flag = GL_STATIC_DRAW;
        GLuint n = 2;
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
        if(_angel.LoadFromOFF("Models/angel.off",true)){
            if(_angel.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW)){
            _angle = 0.0;
            _timer->start();
            }
        }

        _cone.LoadFromOFF("Models/cone.off",true);
        _cone.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW);

        _cube.LoadFromOFF("Models/cube.off",true);
        _cube.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW);

        _icosahedron.LoadFromOFF("Models/icosahedron.off",true);
        _icosahedron.UpdateVertexBufferObjects(GL_DYNAMIC_DRAW);
    }

    void GLWidget::loadSurfaces(){

        TriangularMatrix<ParametricSurface3::PartialDerivative> derivative(3);
        for(GLint i = 0; i < 8; i++){
            _ps[i] = nullptr;
        }

        derivative(0,0) = klein::d00;
        derivative(1,1) = klein::d01;
        derivative(1,0) = klein::d10;
        _ps[0] = new (nothrow)ParametricSurface3(derivative, klein::u_min, klein::u_max, klein::v_min, klein::v_max);

        derivative(0,0) = torus::d00;
        derivative(1,1) = torus::d01;
        derivative(1,0) = torus::d10;
        _ps[1] = new (nothrow)ParametricSurface3(derivative, torus::u_min, torus::u_max, torus::v_min, torus::v_max);

        derivative(0,0) = boy::d00;
        derivative(1,1) = boy::d01;
        derivative(1,0) = boy::d10;
        _ps[2] = new (nothrow)ParametricSurface3(derivative, boy::u_min, boy::u_max, boy::v_min, boy::v_max);

        derivative(0,0) = roman::d00;
        derivative(1,1) = roman::d01;
        derivative(1,0) = roman::d10;
        _ps[3] = new (nothrow)ParametricSurface3(derivative, roman::u_min, roman::u_max, roman::v_min, roman::v_max);

        derivative(0,0) = sphere::d00;
        derivative(1,1) = sphere::d01;
        derivative(1,0) = sphere::d10;
        _ps[4] = new (nothrow)ParametricSurface3(derivative, sphere::u_min, sphere::u_max, sphere::v_min, sphere::v_max);

        derivative(0,0) = conoid::d00;
        derivative(1,1) = conoid::d01;
        derivative(1,0) = conoid::d10;
        _ps[5] = new (nothrow)ParametricSurface3(derivative, conoid::u_min, conoid::u_max, conoid::v_min, conoid::v_max);

        for(GLint i = 0; i < 6; i++){
            if (!_ps[i])
            {
                // error : either close the application , or handle this exception
                cout << "The generation of the parametric surface was unsuccessful" << std::endl;
                return;

            }
        }

        GLuint div_point_count_u = 200;
        GLuint div_point_count_v = 200;
        GLenum usage_flag = GL_STATIC_DRAW;

        for(GLint i = 0; i < 6; i++){
            _image_of_ps[i] = nullptr;
            _image_of_ps[i] = _ps[choice]->GenerateImage(div_point_count_u, div_point_count_v, usage_flag);
            if(!_image_of_ps[i])
            {
                // error : either close the application , or handle this exception
                cout << "The generation of the parametric surface was unsuccessful" << std::endl;
                return;
            }
            if(!_image_of_ps[i]->UpdateVertexBufferObjects(usage_flag))
            {
                cout << "Could not create the vertex buffer object of the parametric curve !" << endl;
                return;
            }
        }
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

        if (!_shader_directional.InstallShaders("Shaders/directional_ligh.vert ", "Shaders/directional_light.frag", GL_TRUE)) {
           cout<<"Shader error"<<std::endl;
           return;
        }

        if (!_shader_reflection.InstallShaders("Shaders/reflection_lines.vert ", "Shaders/reflection_lines.frag", GL_TRUE)) {
           cout<<"Shader error"<<std::endl;
           return;
        }
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
                paintCyclicCurves();
            break;
            case 1:
                paintCurves();
            break;
            case 2:
                paintModels();
            break;
            default:
                paintSurfaces();
            break;

        }

        // pops the current matrix stack, replacing the current matrix with the one below it on the stack,
        // i.e., the original model view matrix is restored
        glPopMatrix();
    }

//---------------------------------------------------------------------------
//   Drawing
//----------------------------------------------------------------------------
    void GLWidget::paintCurves()
    {
        GLuint div_point_count = 200;
        GLenum usage_flag = GL_STATIC_DRAW;

        _image_of_pc = nullptr;
        if(derivative1>0 && derivative2>0){
             _image_of_pc = _pc[choice]->GenerateImage(div_point_count, usage_flag, 3);
        }else{
            if(derivative1==0 && derivative2==0){
                _image_of_pc = _pc[choice]->GenerateImage(div_point_count, usage_flag, 2);
            }else{

                if(derivative1==0){
                    _image_of_pc = _pc[choice]->GenerateImage(div_point_count, usage_flag, 0);
                }

                if(derivative2==0){
                    _image_of_pc = _pc[choice]->GenerateImage(div_point_count, usage_flag, 1);
                }
            }
        }
        if(!_image_of_pc)
        {
             // error : either close the application , or handle this exception
            cout << "The generation of the cyclic curve was unsuccessful" << std::endl;
            return;
        }

         if(!_image_of_pc->UpdateVertexBufferObjects(usage_flag))
         {
            cout << "Could not create the vertex buffer object of the parametric curve !" << endl;
            return;
         }

         if (_image_of_pc) {
             glColor3f (1.0f, 0.972f, 0.239f);
             _image_of_pc->RenderDerivatives(0, GL_LINE_STRIP);
             glPointSize (5.0f);
             glColor3f (1.0f, 0.517f, 0.239f);
             _image_of_pc->RenderDerivatives(1, GL_LINES);
             _image_of_pc->RenderDerivatives(1, GL_POINTS);
             glColor3f (0.780f, 0.239f, 1.0f);
             _image_of_pc->RenderDerivatives(2, GL_LINES);
             _image_of_pc->RenderDerivatives(2, GL_POINTS);
             glPointSize (1.0f);
         }
    }

    void GLWidget::paintCyclicCurves()
    {
        GLenum usage_flag = GL_STATIC_DRAW;
        GLuint div_point_count = 200;
        _image_of_cc[choice] = _cc[choice]->GenerateImage(2, div_point_count, usage_flag);

        if(!_image_of_cc[choice])
        {
            cout << "The generation of the cyclic curve was unsuccessful" << std::endl;
            return;
        }

        if(!_image_of_cc[choice]->UpdateVertexBufferObjects())
        {
            cout << "The update was unsuccessful" << std::endl;
            return;
        }

        if (_image_of_cc[choice]) {
            glColor3f (1.0f, 0.972f, 0.239f);
            _image_of_cc[choice]->RenderDerivatives(0, GL_LINE_STRIP);
            glPointSize (5.0f);
            glColor3f (1.0f, 0.517f, 0.239f);
            _image_of_cc[choice]->RenderDerivatives(1, GL_LINES);
            _image_of_cc[choice]->RenderDerivatives(1, GL_POINTS);
            glColor3f (0.780f, 0.239f, 1.0f);
            _image_of_cc[choice]->RenderDerivatives(2, GL_LINES);
            _image_of_cc[0]->RenderDerivatives(2, GL_POINTS);
            glPointSize (1.0f);
        }
    }

    void GLWidget::paintModels()
    {
        glEnable(GL_LIGHTING);
        glEnable(GL_NORMALIZE);
        glEnable(GL_LIGHT0);

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

        glDisable (GL_LIGHTING);
        glDisable(GL_NORMALIZE);
        glDisable(GL_LIGHT0);
    }

    void GLWidget::paintSurfaces()
    {
        glEnable(GL_LIGHTING);
        glEnable(GL_NORMALIZE);
        glEnable(GL_LIGHT0);

        DirectionalLight *dl = 0;
        HCoordinate3 direction (0.0, 0.0, 1.0, 0.0);
        Color4 ambient(1.0, 0.4, 0.4, 1.0);
        Color4 diffuse(0.8, 0.8, 0.8, 1.0);
        Color4 specular(1.0, 1.0 , 1.0, 1.0);

        switch(light){
          case 0:
            glEnable(GL_LIGHT0);
            dl = new DirectionalLight (GL_LIGHT0, direction, ambient, diffuse, specular);
            break;
          case 1:
            glEnable(GL_LIGHT1);
            dl = new DirectionalLight (GL_LIGHT1, direction, ambient, diffuse, specular);
            break;
          case 2:
            glEnable(GL_LIGHT2);
            dl = new DirectionalLight (GL_LIGHT2, direction, ambient, diffuse, specular);
            break;
          case 3:
            glEnable(GL_LIGHT3);
            dl = new DirectionalLight (GL_LIGHT3, direction, ambient, diffuse, specular);
            break;
          case 4:
            glEnable(GL_LIGHT4);
            dl = new DirectionalLight (GL_LIGHT4, direction, ambient, diffuse, specular);
            break;
          case 5:
            glEnable(GL_LIGHT5);
            dl = new DirectionalLight (GL_LIGHT5, direction, ambient, diffuse, specular);
            break;
          case 6:
            glEnable(GL_LIGHT6);
            dl = new DirectionalLight (GL_LIGHT6, direction, ambient, diffuse, specular);
            break;
          default:
            glEnable(GL_LIGHT7);
            dl = new DirectionalLight (GL_LIGHT7, direction, ambient, diffuse, specular);
            break;
        }

        try {

            switch(nrshader){
              case 0:
                //none
                break;
               case 1:
                _shader_two_sided.Enable(GL_TRUE);
                break;
               case 2:
                _shader_toon.Enable(GL_TRUE);
                break;
               case 3:
                _shader_directional.Enable(GL_TRUE);
                break;
               default:
                _shader_reflection.Enable(GL_TRUE);
             }

        } catch ( Exception &e)
        {
            cerr << e << endl ;
        }

        if(dl) {
            dl->Enable();
            MatFBTurquoise.Apply();
            _image_of_ps[choice]->Render();
            dl->Disable();
        }

        if (dl) {
            delete dl;
            dl = 0;
        }

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

    void GLWidget::animate()
    {
        GLfloat *vertex=_angel.MapVertexBuffer(GL_READ_WRITE);
        GLfloat *normal = _angel.MapNormalBuffer(GL_READ_ONLY);
        _angle += DEG_TO_RADIAN;
        if(_angle >= TWO_PI) _angle -= TWO_PI;

        GLfloat scale = sin(_angle)/6000.0;
        for(GLuint i = 0; i < _angel.VertexCount(); ++i)
        {
           for(GLuint coordinate = 0; coordinate < 3; ++coordinate , ++vertex , ++normal )
                *vertex += scale * (*normal);
        }
        _angel.UnmapVertexBuffer();
        _angel.UnmapNormalBuffer();
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

    void GLWidget::set_selected_curve(int index)
    {
        choice = index;
        updateGL();
    }

    void GLWidget::set_selected_model(int index)
    {
        choice = index;
        updateGL();
    }

    void GLWidget::set_selected_surface(int index)
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

    void GLWidget::set_checked_box1(int state){
        derivative1 = state;
        updateGL();
    }

    void GLWidget::set_checked_box2(int state){
        derivative2 = state;
        updateGL();
    }

    void GLWidget::set_checked_box3(int state){
        animation = state;
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


    GLWidget ::~GLWidget()
    {
        for(GLint i = 0; i < 8; i++){
            delete _pc[i];
            _pc[i] = nullptr;
        }
        if (_image_of_pc)
            delete _image_of_pc, _image_of_pc = nullptr;

        for(GLint i = 0; i < 8; i++){
            delete _ps[i];
            _ps[i] = nullptr;
        }
        for(GLint i = 0; i < 8; i++){
            if (_image_of_ps[i])
                delete _image_of_ps[i];
                _image_of_ps[i] = nullptr;
        }

        if(_cc[0])
        {
            delete _cc[0];
            _cc[0] = nullptr;
        }

        if(_cc[1])
        {
            delete _cc[1];
            _cc[1] = nullptr;
        }

        if(_image_of_cc[0])
        {
            delete _image_of_cc[0];
            _image_of_cc[0] = nullptr;
        }

        if(_image_of_cc[1])
        {
            delete _image_of_cc[1];
            _image_of_cc[1] = nullptr;
        }

    }

}
