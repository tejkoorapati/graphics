#include <QtWidgets>
#include <QtOpenGL>
#include <iostream>
#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Viewer.hpp"
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

    cotOfFOV = 1/tan(3.14159 * fov / 360);
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

//    m_projection = Matrix4x4();
    m_Model  = Matrix4x4();
    Vector4D r1,r2,r3,r4;
    r1 = Vector4D( 1,   0,  0, 0 );
    r2 = Vector4D( 0,   1,  0, 0 );
    r3 = Vector4D( 0,   0,  1, 8);
    r4 = Vector4D( 0,   0,  0, 1 );
    m_view = Matrix4x4(r1,r2,r3,r4);

    m_fov = 40;
    m_near = 4;
    m_far = 20;

    set_perspective( m_fov, 1, m_near, m_far );
    initCube();




}

void Viewer::initializeGL() {
    // Do some OpenGL setup
    QGLFormat glFormat = QGLWidget::format();
    if (!glFormat.sampleBuffers()) {
        std::cerr << "Could not enable sample buffers." << std::endl;
        return;
    }

    glClearColor(0.7, 0.7, 0.7, 0.0);
    
    if (!mProgram.addShaderFromSourceFile(QGLShader::Vertex, "shader.vert")) {
        std::cerr << "Cannot load vertex shader." << std::endl;
        return;
    }

    if (!mProgram.addShaderFromSourceFile(QGLShader::Fragment, "shader.frag")) {
        std::cerr << "Cannot load fragment shader." << std::endl;
        return;
    }

    if ( !mProgram.link() ) {
        std::cerr << "Cannot link shaders." << std::endl;
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
        std::cerr << "could not bind vertex buffer to the context." << std::endl;
        return;
    }

    mProgram.bind();

    mProgram.enableAttributeArray("vert");
    mProgram.setAttributeBuffer("vert", GL_FLOAT, 0, 3);

    mColorLocation = mProgram.uniformLocation("frag_color");
}

void Viewer::paintGL() {    
    draw_init();

    // Here is where your drawing code should go.
    
    /* A few of lines are drawn below to show how it's done. */
//    set_colour(QColor(1.0, 1.0, 1.0));

//    draw_line(QVector2D(-0.9, -0.9),
//              QVector2D(0.9, 0.9));
//    draw_line(QVector2D(0.9, -0.9),
//              QVector2D(-0.9, 0.9));

//    draw_line(QVector2D(-0.9, -0.9),
//              QVector2D(-0.4, -0.9));
//    draw_line(QVector2D(-0.9, -0.9),
//              QVector2D(-0.9, -0.4));

    drawCube();
}

void Viewer::mousePressEvent ( QMouseEvent * event ) {
    std::cerr << "Stub: button " << event->button() << " pressed\n";
}

void Viewer::mouseReleaseEvent ( QMouseEvent * event ) {
    std::cerr << "Stub: button " << event->button() << " released\n";
}

void Viewer::mouseMoveEvent ( QMouseEvent * event ) {
    std::cerr << "Stub: Motion at " << event->x() << ", " << event->y() << std::endl;
}



Point3D Viewer::project(Point3D point)
{
    Point3D projectedPoint = m_projection * point;

    Point3D finalPoint = Point3D(projectedPoint[0]/point[2],
            projectedPoint[1]/point[2],
            projectedPoint[2]/point[2]);

    return finalPoint;

}

Point2D Viewer::normalize(Point3D point)
{
    return Point2D(point[0]/point[2],point[1]/point[2]);

}

void Viewer::draw3dLine(Point3D start, Point3D end)
{
    Point2D startF = normalize(project(start));
    Point2D endF = normalize(project(end));

    draw_line(QVector2D(startF[0],startF[1]),QVector2D(endF[0],endF[1]));

}


void Viewer::drawCube()
{
    for(int i = 0 ; i < 8; i ++){
        m_cubeFinal[i] = m_view * m_Model * m_cubeOriginal[i];
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

void Viewer::initCube()
{
    m_cubeOriginal[0]=(Point3D(1.0,1.0,1.0));
    m_cubeOriginal[1]=(Point3D(1.0,1.0,-1.0));
    m_cubeOriginal[2]=(Point3D(-1.0,1.0,-1.0));
    m_cubeOriginal[3]=(Point3D(-1.0,1.0,1.0));
    m_cubeOriginal[4]=(Point3D(-1.0,-1.0,1.0));
    m_cubeOriginal[5]=(Point3D(1.0,-1.0,1.0));
    m_cubeOriginal[6]=(Point3D(1.0,-1.0,-1.0));
    m_cubeOriginal[7]=(Point3D(-1.0,-1.0,-1.0));
}



