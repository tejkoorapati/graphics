#include <QtWidgets>
#include <QtOpenGL>
#include <iostream>
#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Viewer.hpp"
#include "a2.hpp"
// #include "draw.hpp"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif

using namespace std;

Viewer::Viewer(const QGLFormat& format, QWidget *parent) 
: QGLWidget(format, parent)
    #if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
, mVertexBufferObject(QOpenGLBuffer::VertexBuffer)
, mVertexArrayObject(this)
    #else
, mVertexBufferObject(QGLBuffer::VertexBuffer)
    #endif
{
    reset_view();
}

Viewer::~Viewer() {
    // Nothing to do here right now.
}

QSize Viewer::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize Viewer::sizeHint() const {
    return QSize(300, 300);
}

void Viewer::set_perspective(double fov, double aspect,
 double near_p, double far_p)
{
    double a,c,cotOfFOV ;

    fov = fov *(2 * 3.14159 / 360.0);
    cotOfFOV = 1/tan(fov/2);
    a =  (far_p+near_p)/(far_p-near_p);
    c = (-2 *far_p*near_p)/(far_p-near_p);

    Vector4D r1,r2,r3,r4;
    r1 = Vector4D(cotOfFOV/aspect,0,0,0);
    r2 = Vector4D(0,cotOfFOV,0,0);
    r3 = Vector4D(0,0,a,c);
    r4 = Vector4D(0,0,1,0);

    m_projection = Matrix4x4(r1,r2,r3,r4);

}

void Viewer::reset_view()
{


    interactionMode = 3;
    //    m_projection = Matrix4x4();
    m_Model  = Matrix4x4();
    m_scale = Matrix4x4();
    Vector4D r1,r2,r3,r4;
    r1 = Vector4D( 1,   0,  0, 0 );
    r2 = Vector4D( 0,   1,  0, 0 );
    r3 = Vector4D( 0,   0,  1, 8);
    r4 = Vector4D( 0,   0,  0, 1 );
    m_view = Matrix4x4(r1,r2,r3,r4);




    m_fov = 30;
    m_near = 1;
    m_far = 20;

    bottomLeftView = Point2D(-.95,-.95 );
    bottomRightView = Point2D(.95,-.95);
    topLeftView = Point2D(-.95,.95);
    topRightView = Point2D(.95, .95);



    set_perspective( m_fov, 1, m_near, m_far );
    initAxis();
    initCube();






}

void Viewer::initializeGL() {
    // Do some OpenGL setup
    QGLFormat glFormat = QGLWidget::format();
    if (!glFormat.sampleBuffers()) {
        // std::cerr << "Could not enable sample buffers." << std::endl;
        return;
    }

    glClearColor(0.7, 0.7, 0.7, 0.0);
    
    if (!mProgram.addShaderFromSourceFile(QGLShader::Vertex, "shader.vert")) {
        // std::cerr << "Cannot load vertex shader." << std::endl;
        return;
    }

    if (!mProgram.addShaderFromSourceFile(QGLShader::Fragment, "shader.frag")) {
        // std::cerr << "Cannot load fragment shader." << std::endl;
        return;
    }

    if ( !mProgram.link() ) {
        // std::cerr << "Cannot link shaders." << std::endl;
        return;
    }

#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    mVertexArrayObject.create();
    mVertexArrayObject.bind();

    mVertexBufferObject.create();
    mVertexBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);
#else 

    /*
     * if qt version is less than 5.1, use the following commented code
     * instead of QOpenGLVertexVufferObject. Also use QGLBuffer instead of
     * QOpenGLBuffer.
     */
     uint vao;

     typedef void (APIENTRY *_glGenVertexArrays) (GLsizei, GLuint*);
     typedef void (APIENTRY *_glBindVertexArray) (GLuint);

     _glGenVertexArrays glGenVertexArrays;
     _glBindVertexArray glBindVertexArray;

     glGenVertexArrays = (_glGenVertexArrays) QGLWidget::context()->getProcAddress("glGenVertexArrays");
     glBindVertexArray = (_glBindVertexArray) QGLWidget::context()->getProcAddress("glBindVertexArray");

     glGenVertexArrays(1, &vao);
     glBindVertexArray(vao);

     mVertexBufferObject.create();
     mVertexBufferObject.setUsagePattern(QGLBuffer::DynamicDraw);
#endif

     if (!mVertexBufferObject.bind()) {
        // std::cerr << "could not bind vertex buffer to the context." << std::endl;
        return;
    }

    mProgram.bind();

    mProgram.enableAttributeArray("vert");
    mProgram.setAttributeBuffer("vert", GL_FLOAT, 0, 3);

    mColorLocation = mProgram.uniformLocation("frag_color");
    draw_init();
}

void Viewer::paintGL() {

    draw_init();
    drawCube();
    drawAxis();
    drawViewPortBorder();

    QString mode = "";
    if(viewPortMode){
      mode  = "Viewport Mode";
  }
  else{


      switch (interactionMode){
        case 0 :
        mode  = "View Rotate Mode";
        break;

        case 1:
        mode  = "View Translate Mode";
        break;

        case 2:
        mode  = "View Perspective Mode";
        break;

        case 3:
        mode  = "Model Rotate Mode";
        break;

        case 4:
        mode  = "Model Translate Mode";
        break;

        case 5:
        mode  = "View Scale Mode";
        break;
    }
}

    QString far = QString::number(m_far);
    QString near = QString::number(m_near);

    QString complete= "Far: "+ far + " Near: "+ near + ", " + mode;  


label->setText(complete);

}

void Viewer::mousePressEvent ( QMouseEvent * event ) {
    // std::cerr << "Stub: button " << event->button() << " pressed\n";

    prev_x = event->x();
    prev_y = event->y();

    int button = event->button();
    switch(button){
        case 1 :
        leftPressed = true;
        break;
        case 2:
        rightPressed = true;
        break;
        case 4:
        middlePressed = true;
        break;
    }


    if(viewPortMode){
        if(!leftPressed) return;
        mousePressX = event->x();
        mousePressY = event->y();
        return;
    }

}

void Viewer::mouseReleaseEvent ( QMouseEvent * event ) {
    // std::cerr << "Stub: button " << event->button() << " released\n";
    int button = event->button();
    switch(button){
        case 1 :
        leftPressed = false;
        break;
        case 2:
        rightPressed = false;
        break;
        case 4:
        middlePressed = false;
        break;
    }

    if(viewPortMode){
        if(!leftPressed) return;
        mouseReleaseX = event->x();
        mouseReleaseY = event->y();
        drawMouseTracking();
        return;
    }



}

void Viewer::mouseMoveEvent ( QMouseEvent * event ) {
    //std::cerr << "Stub: Motion at " << event->x() << ", " << event->y() << std::endl;



    if(viewPortMode){
        if(!leftPressed) return;
        mouseReleaseX = event->x();
        mouseReleaseY = event->y();
        drawMouseTracking();
        return;
    }

    int diffx = event->x() - prev_x;
    int signx = (event->x() > prev_x) ? 1 : -1;

    double translateDistance = signx*0.05;
    double rotationAngle = signx*abs(diffx)*.01;
    double scaleRatio = 1.01;
    if(signx < 0){
        scaleRatio = 0.99;
    }

    switch (interactionMode){
        case 0 :
        if(leftPressed)
            rotate(m_view,rotationAngle,1,1);
        if(middlePressed)
            rotate(m_view,rotationAngle,2,1);
        if(rightPressed)
            rotate(m_view,rotationAngle,3,1);
        break;

        case 1:
        if(leftPressed)
            translate(m_view,translateDistance,0,0);
        if(middlePressed)
            translate(m_view,0,translateDistance,0);
        if(rightPressed)
            translate(m_view,0,0,translateDistance);
        break;

        case 2:
        if(leftPressed){
            if (signx < 0 && m_fov <= 5){
                return;
            }
            else if (signx > 0 && m_fov >= 160){
                return;
            }
            m_fov += signx*0.1;
        }
        if(middlePressed)
            m_near += signx*1;
        if(rightPressed)
            m_far += signx*1;
        set_perspective(m_fov,1,m_near,m_far);
        break;

        case 3:
        if(leftPressed)
            rotate(m_Model,rotationAngle,1,2);
        if(middlePressed)
            rotate(m_Model,rotationAngle,2,2);
        if(rightPressed)
            rotate(m_Model,rotationAngle,3,2);
        break;

        case 4:
        if(leftPressed)
            translate(m_Model,translateDistance,0,0);
        if(middlePressed)
            translate(m_Model,0,translateDistance,0);
        if(rightPressed)
            translate(m_Model,0,0,translateDistance);
        break;

        case 5:
        if(leftPressed)
            scale(m_scale,scaleRatio,1,1);
        if(middlePressed)
            scale(m_scale,1,scaleRatio,1);
        if(rightPressed)
            scale(m_scale,1,1,scaleRatio);
        break;

    }


    prev_x = event->x();
    prev_y = event->y();


    update();
}



Point3D Viewer::project(Point3D point)
{
    Point3D projectedPoint = m_projection * point;

    Point3D finalPoint = Point3D(projectedPoint[0]/point[2],
        projectedPoint[1]/point[2],
        projectedPoint[2]/point[2]);

    return finalPoint;

}

Point3D Viewer::scaleToViewPort(Point3D point)
{

    //    return Point2D(point[0]+1 *(topRightView[0] - topLeftView[0])/2 + topLeftView[0],
    //            point[1] + 1 * (bottomRightView[1]-topRightView[1])/2 + topRightView[1]);

    double scaleFactorWidth = (bottomRightView[0] - bottomLeftView[0])/2;
    double scaleFactorHeigth = (topRightView[1] - bottomRightView[1])/2;


    return( Point3D( scaleFactorWidth * (point[0] + 1) + bottomLeftView[0],
        scaleFactorHeigth * (point[1] + 1) + bottomLeftView[1],point[2]) );


}

//void

void Viewer::draw3dLine(Point3D start, Point3D end)
{

    if(!(clipAgainstPlane(start,end,Point3D(0.0, 0.0, m_far),Vector3D(0.0, 0.0, -1)) &&
     clipAgainstPlane(start,end,Point3D(0.0, 0.0, m_near),Vector3D(0.0, 0.0, 1)) ) ){
        return;
}


start = scaleToViewPort(project(start));
end = scaleToViewPort(project(end));

if(!(clipAgainstPlane(start,end,Point3D(topLeftView[0], 0.0, 0),Vector3D(1, 0.0, 0)) &&
 clipAgainstPlane(start,end,Point3D(topRightView[0], 0.0, 0),Vector3D(-1, 0.0, 0))&&
 clipAgainstPlane(start,end,Point3D(0, bottomLeftView[1], 0),Vector3D(0, 1, 0)) &&
 clipAgainstPlane(start,end,Point3D(0, topLeftView[1], 0),Vector3D(0, -1, 0))) ){
    return;
}


draw_line(QVector2D(start[0],start[1]),QVector2D(end[0],end[1]));

}

bool Viewer::clipAgainstPlane(Point3D &start, Point3D &end, Point3D plane, Vector3D normal)
{
    double startToPlane = (start - plane).dot(normal);
    double endToPlane = (end - plane).dot(normal);

    if(startToPlane < 0 && endToPlane < 0)
        return false;

    if(startToPlane > 0 && endToPlane > 0){
        return true;
    }

    double t = startToPlane / (startToPlane - endToPlane);

    if(startToPlane < 0){
        start = start + t * (end - start);
    }
    else{
        end = start + t * (end - start);
    }
    return true;


}

void Viewer::drawMouseTracking()
{
    if(mousePressX > width()) mousePressX = width();
    if(mousePressX < 0) mousePressX = 0;
    if(mousePressY > height()) mousePressY = height();
    if(mousePressY < 0) mousePressY = 0;


    if(mouseReleaseX > width()) mouseReleaseX = width();
    if(mouseReleaseX < 0) mouseReleaseX = 0;
    if(mouseReleaseY > height()) mouseReleaseY = height();
    if(mouseReleaseY < 0) mouseReleaseY = 0;

    //x1,y1 = topleft x2,y2 = bottomright

    double x1 = min(mousePressX,mouseReleaseX);
    double y1 = min(mousePressY,mouseReleaseY);

    double x2 = max(mousePressX,mouseReleaseX);
    double y2 = max(mousePressY,mouseReleaseY);

    Point2D topLeft = Point2D(x1,y1);
    Point2D topRight = Point2D(x2,y1);
    Point2D bottomLeft = Point2D(x1,y2);
    Point2D bottomRight = Point2D(x2,y2);

    ////cout<<topLeft<<", "<<bottomRight<<endl;
    setViewPort(bottomLeft,bottomRight,topLeft,topRight);

}



void Viewer::drawCube()
{
    for(int i = 0 ; i < 8; i ++){
        m_cubeFinal[i] = m_view * m_Model * m_scale * m_cubePoints[i];
    }
    set_colour(QColor(1, 1, 1));
    draw3dLine(m_cubeFinal[0],m_cubeFinal[3]);
    draw3dLine(m_cubeFinal[0],m_cubeFinal[1]);
    draw3dLine(m_cubeFinal[0],m_cubeFinal[5]);
    draw3dLine(m_cubeFinal[1],m_cubeFinal[2]);
    draw3dLine(m_cubeFinal[1],m_cubeFinal[6]);
    draw3dLine(m_cubeFinal[2],m_cubeFinal[3]);
    draw3dLine(m_cubeFinal[2],m_cubeFinal[7]);
    draw3dLine(m_cubeFinal[3],m_cubeFinal[4]);
    draw3dLine(m_cubeFinal[4],m_cubeFinal[5]);
    draw3dLine(m_cubeFinal[4],m_cubeFinal[7]);
    draw3dLine(m_cubeFinal[5],m_cubeFinal[6]);
    draw3dLine(m_cubeFinal[6],m_cubeFinal[7]);


}

void Viewer::drawViewPortBorder()
{


    set_colour(QColor(0, 0, 0));
    draw_line(QVector2D(bottomLeftView[0],bottomLeftView[1]),QVector2D(bottomRightView[0],bottomRightView[1]));
    draw_line(QVector2D(topLeftView[0],topLeftView[1]),QVector2D(topRightView[0],topRightView[1]));
    draw_line(QVector2D(bottomLeftView[0],bottomLeftView[1]),QVector2D(topLeftView[0],topLeftView[1]));
    draw_line(QVector2D(bottomRightView[0],bottomRightView[1]),QVector2D(topRightView[0],topRightView[1]));
    update();

}


void Viewer::drawAxis()
{
    for(int i = 0 ; i < 4; i ++){
        m_cubeAxisFinal[i] = m_view * m_Model * m_cubeAxisPoints[i];
    }

    for(int i = 0 ; i < 4; i ++){
        m_worldAxisFinal[i] = m_view * m_worldAxisPoints[i];
    }
    set_colour(QColor(255, 0, 0));
    draw3dLine(m_cubeAxisFinal[0],m_cubeAxisFinal[1]);
    draw3dLine(m_cubeAxisFinal[0],m_cubeAxisFinal[2]);
    draw3dLine(m_cubeAxisFinal[0],m_cubeAxisFinal[3]);
    set_colour(QColor(0, 255, 0));
    draw3dLine(m_worldAxisFinal[0],m_worldAxisFinal[2]);
    draw3dLine(m_worldAxisFinal[0],m_worldAxisFinal[1]);
    draw3dLine(m_worldAxisFinal[0],m_worldAxisFinal[3]);


}

void Viewer::initCube()
{
    m_cubePoints[0]=(Point3D(1.0,1.0,1.0));
    m_cubePoints[1]=(Point3D(1.0,1.0,-1.0));
    m_cubePoints[2]=(Point3D(-1.0,1.0,-1.0));
    m_cubePoints[3]=(Point3D(-1.0,1.0,1.0));
    m_cubePoints[4]=(Point3D(-1.0,-1.0,1.0));
    m_cubePoints[5]=(Point3D(1.0,-1.0,1.0));
    m_cubePoints[6]=(Point3D(1.0,-1.0,-1.0));
    m_cubePoints[7]=(Point3D(-1.0,-1.0,-1.0));
}

void Viewer::initAxis()
{
    m_cubeAxisPoints[0]=(Point3D(0.0,0.0,0.0));
    m_cubeAxisPoints[1]=(Point3D(0.5,0.0,0.0));
    m_cubeAxisPoints[2]=(Point3D(0.0,0.5,0.0));
    m_cubeAxisPoints[3]=(Point3D(0.0,0.0,0.5));

    for(int i = 0 ; i < 4 ; i ++){
        m_worldAxisPoints[i] = m_cubeAxisPoints[i];
    }
}

void Viewer::setViewPort(Point2D bottomLeft, Point2D bottomRight, Point2D topLeft, Point2D topRight)
{
    double halfWidth = width()/2;
    double halfHeight = height()/2;
    bottomLeftView = Point2D( (bottomLeft[0] / halfWidth) - 1, 1 - (bottomLeft[1] / halfHeight));
    bottomRightView = Point2D( (bottomRight[0] / halfWidth) - 1, 1 - (bottomRight[1] / halfHeight));
    topRightView = Point2D( (topRight[0] / halfWidth) - 1, 1 - (topRight[1] / halfHeight));
    topLeftView = Point2D( (topLeft[0] / halfWidth) - 1, 1 - (topLeft[1] / halfHeight));
    drawViewPortBorder();
}

void Viewer::setLabel(QLabel* statusLabel){
    label = statusLabel;
}
// Drawing Functions
// ******************* Do Not Touch ************************ // 

void Viewer::draw_line(const QVector2D& p1, const QVector2D& p2) {

    const GLfloat lineVertices[] = {
        p1.x(), p1.y(), 1.0,
        p2.x(), p2.y(), 1.0
    };

    mVertexBufferObject.allocate(lineVertices, 3 * 2 * sizeof(float));

    glDrawArrays(GL_LINES, 0, 2);
}

void Viewer::set_colour(const QColor& col)
{
    mProgram.setUniformValue(mColorLocation, col.red(), col.green(), col.blue());
}

void Viewer::draw_init() {
    glClearColor(0.7, 0.7, 0.7, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (double)width(), 0.0, (double)height());
    glViewport(0, 0, width(), height());

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(1.0);
}





