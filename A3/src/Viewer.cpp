#include <QtWidgets>
#include <QtOpenGL>
#include "Viewer.hpp"
#include <iostream>
#include <math.h>


#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif

Viewer::Viewer(const QGLFormat& format, QWidget *parent) 
    : QGLWidget(format, parent)
    #if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    , mCircleBufferObject(QOpenGLBuffer::VertexBuffer)
    , mVertexArrayObject(this)
    #else
    , mCircleBufferObject(QGLBuffer::VertexBuffer)
    #endif
{

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

void Viewer::setSceneNode(SceneNode *node)
{
    m_root = node;
}

void Viewer::drawCircle(QColor color, QMatrix4x4 trans)
{
    double radius = width() < height() ?
                (float)width() * 0.25 : (float)height() * 0.25;



    float circleData[120];
    for(size_t i=0; i<40; ++i) {
        circleData[i*3] = radius * cos(i*2*M_PI/40);
        circleData[i*3 + 1] = radius * sin(i*2*M_PI/40);
        circleData[i*3 + 2] = 0;
    }

    mSphereBufferObject.allocate(circleData, 40 * 3 * sizeof(float));

    int current_width = width();
    int current_height = height();

    // Set up for orthogonal drawing to draw a circle on screen.
    // You'll want to make the rest of the function conditional on
    // whether or not we want to draw the circle this time around.

    set_colour(color);

    // Set orthographic Matrix
    QMatrix4x4 orthoMatrix;
    orthoMatrix.ortho(0.0, (float)current_width,0.0, (float)current_height, -100, 100);

    // Translate the view to the middle



    // Bind buffer object
    mSphereBufferObject.bind();
    //    trans.setToIdentity();
    mProgram.setUniformValue(mMvpMatrixLocation, getCameraMatrix()* trans);

    // Draw buffer
    glDrawArrays(GL_LINE_LOOP, 0, 40);

}

void Viewer::initializeGL() {
    QGLFormat glFormat = QGLWidget::format();
    if (!glFormat.sampleBuffers()) {
        std::cerr << "Could not enable sample buffers." << std::endl;
        return;
    }

    glShadeModel(GL_SMOOTH);
    glClearColor( 0.4, 0.4, 0.4, 0.0 );
    glEnable(GL_DEPTH_TEST);
    
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
    double radius = width() < height() ?
                (float)width() * 0.25 : (float)height() * 0.25;



    float circleData[120];
    for(size_t i=0; i<40; ++i) {
        circleData[i*3] = radius * cos(i*2*M_PI/40);
        circleData[i*3 + 1] = radius * sin(i*2*M_PI/40);
        circleData[i*3 + 2] = 0.0;


    }


    /*
    float circleData[336];

    int Band_Power = 4;  // 2^Band_Power = Total Points in a band.
    int Band_Points = 16; // 16; = 2^Band_Power
    int Band_Mask = (Band_Points-2);
    float Sections_In_Band = ((Band_Points/2)-1);
    float Total_Points = (Sections_In_Band*Band_Points) ;
    float Section_Arc = (6.28/Sections_In_Band);
    const float R = radius; // radius of 10

    int i;
    float x_angle;
    float y_angle;

    for (i=0;i<Total_Points;i++)
    {
        x_angle=(float)(i&1)+(i>>Band_Power);
        y_angle=(float)((i&Band_Mask)>>1)+((i>>Band_Power)*(Sections_In_Band));

        x_angle*=(float)Section_Arc/2.0f; // remember - 180� x rot not 360
        y_angle*=(float)Section_Arc*-1;

        circleData[i*3]= R*sin(x_angle)*sin(y_angle);
        circleData[i*3+1] = R*cos(x_angle);
        circleData[i*3+2] = R*sin(x_angle)*cos(y_angle);

        printf("[%f,%f,%f],\n",circleData[i*3],circleData[i*3+1],circleData[i*3+2]);


    }
    std::cout<<i<<endl;
*/


#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    mVertexArrayObject.create();
    mVertexArrayObject.bind();

    mCircleBufferObject.create();
    mCircleBufferObject.setUsagePattern(QOpenGLBuffer::StaticDraw);
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

    mCircleBufferObject.create();
    mCircleBufferObject.setUsagePattern(QGLBuffer::StaticDraw);
#endif

    if (!mCircleBufferObject.bind()) {
        std::cerr << "could not bind vertex buffer to the context." << std::endl;
        return;
    }

    mCircleBufferObject.allocate(circleData, 40 * 3 * sizeof(float));

    mProgram.bind();

    mProgram.enableAttributeArray("vert");
    mProgram.setAttributeBuffer("vert", GL_FLOAT, 0, 3);

    mMvpMatrixLocation = mProgram.uniformLocation("mvpMatrix");
    mColorLocation = mProgram.uniformLocation("frag_color");
}

void Viewer::paintGL() {
    // Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up lighting

    // Draw stuff

    draw_trackball_circle();
    QMatrix4x4 temp;
    temp.setToIdentity();
    //    drawCircle(QColor(Qt::GlobalColor::green),temp);

    m_root->walk_gl(false,this);

}

void Viewer::resizeGL(int width, int height) {
    if (height == 0) {
        height = 1;
    }

    mPerspMatrix.setToIdentity();
    mPerspMatrix.perspective(60.0, (float) width / (float) height, 0.001, 1000);

    glViewport(0, 0, width, height);
}



void Viewer::mousePressEvent ( QMouseEvent * event ) {
    buttonPressed = event->button();
    prev_x = event->x();
    prev_y = event->y();
}

void Viewer::mouseReleaseEvent ( QMouseEvent * event ) {
    std::cerr << "Stub: button " << event->button() << " released\n";
}

void Viewer::mouseMoveEvent ( QMouseEvent * event ) {
    int diffx = event->x() - prev_x;
    int signx = (event->x() > prev_x) ? 1 : -1;
    int diffy = event->y() - prev_y;
    int signy = (event->y() < prev_y) ? 1 : -1;
    if(buttonPressed == 1){
        translateWorld(signx*(abs(diffx)),signy*abs(diffy),0);
    }
    else if (buttonPressed == 2){
        translateWorld(0,0,signy*20);
    }
    else if (buttonPressed == 4){
        rotateWorld(signx*abs(diffx)*.5,0,1,0);
    }


    prev_x = event->x();
    prev_y = event->y();
    update();
}

QMatrix4x4 Viewer::getCameraMatrix() {
    // Todo: Ask if we want to keep this.
    QMatrix4x4 vMatrix;

    QMatrix4x4 cameraTransformation;
    QVector3D cameraPosition = cameraTransformation * QVector3D(0, 0, 300.0);
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);

    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

    return mPerspMatrix * vMatrix * mTransformMatrix;
}

void Viewer::translateWorld(float x, float y, float z) {
    // Todo: Ask if we want to keep this.
    mTransformMatrix.translate(x, y, z);
}

void Viewer::rotateWorld(float angle, float x, float y, float z) {
    // Todo: Ask if we want to keep this.
    mTransformMatrix.rotate(angle, x, y, z);
}

void Viewer::scaleWorld(float x, float y, float z) {
    // Todo: Ask if we want to keep this.
    mTransformMatrix.scale(x, y, z);
}

void Viewer::set_colour(const QColor& col)
{
    mProgram.setUniformValue(mColorLocation, col.red(), col.green(), col.blue());
}

void Viewer::draw_trackball_circle()
{
    int current_width = width();
    int current_height = height();

    // Set up for orthogonal drawing to draw a circle on screen.
    // You'll want to make the rest of the function conditional on
    // whether or not we want to draw the circle this time around.

    set_colour(QColor(0.0, 0.0, 0.0));

    // Set orthographic Matrix
    QMatrix4x4 orthoMatrix;
    orthoMatrix.ortho(0.0, (float)current_width,
                      0.0, (float)current_height, -0.1, 0.1);

    // Translate the view to the middle
    QMatrix4x4 transformMatrix;
    transformMatrix.translate(width()/2.0, height()/2.0, 0.0);

    // Bind buffer object
    mCircleBufferObject.bind();
    mProgram.setUniformValue(mMvpMatrixLocation, orthoMatrix * transformMatrix);

    // Draw buffer
    glDrawArrays(GL_LINE_LOOP, 0, 40);

}


/*******************************************************
 *
 * void vCalcRotVec(float fNewX, float fNewY,
 *                  float fOldX, float fOldY,
 *                  float fDiameter,
 *                  float *fVecX, float *fVecY, float *fVecZ);
 *
 *    Calculates a rotation vector based on mouse motion over
 *    a virtual trackball.
 *
 *    The fNew and fOld mouse positions
 *    should be in 'trackball' space. That is, they have been
 *    transformed into a coordinate system centered at the middle
 *    of the trackball. fNew, fOld, and fDiameter must all be specified
 *    in the same units (pixels for example).
 *
 * Parameters: fNewX, fNewY - New mouse position in trackball space.
 *                            This is the second point along direction
 *                            of rotation.
 *             fOldX, fOldY - Old mouse position in trackball space.
 *                            This is the first point along direction
 *                            of rotation.
 *             fDiameter - Diameter of the trackball. This should
 *                         be specified in the same units as fNew and fOld.
 *                         (ie, usually pixels if fNew and fOld are transformed
 *                         mouse positions)
 *             fVec - The output rotation vector. The length of the vector
 *                    is proportional to the angle of rotation.
 *
 *******************************************************/
void vCalcRotVec(float fNewX, float fNewY,
                 float fOldX, float fOldY,
                 float fDiameter,
                 float *fVecX, float *fVecY, float *fVecZ) {
    long  nXOrigin, nYOrigin;
    float fNewVecX, fNewVecY, fNewVecZ,        /* Vector corresponding to new mouse location */
            fOldVecX, fOldVecY, fOldVecZ,        /* Vector corresponding to old mouse location */
            fLength;

    /* Vector pointing from center of virtual trackball to
    * new mouse position
    */
    fNewVecX    = fNewX * 2.0 / fDiameter;
    fNewVecY    = fNewY * 2.0 / fDiameter;
    fNewVecZ    = (1.0 - fNewVecX * fNewVecX - fNewVecY * fNewVecY);

    /* If the Z component is less than 0, the mouse point
    * falls outside of the trackball which is interpreted
    * as rotation about the Z axis.
    */
    if (fNewVecZ < 0.0) {
        fLength = sqrt(1.0 - fNewVecZ);
        fNewVecZ  = 0.0;
        fNewVecX /= fLength;
        fNewVecY /= fLength;
    } else {
        fNewVecZ = sqrt(fNewVecZ);
    }

    /* Vector pointing from center of virtual trackball to
    * old mouse position
    */
    fOldVecX    = fOldX * 2.0 / fDiameter;
    fOldVecY    = fOldY * 2.0 / fDiameter;
    fOldVecZ    = (1.0 - fOldVecX * fOldVecX - fOldVecY * fOldVecY);

    /* If the Z component is less than 0, the mouse point
    * falls outside of the trackball which is interpreted
    * as rotation about the Z axis.
    */
    if (fOldVecZ < 0.0) {
        fLength = sqrt(1.0 - fOldVecZ);
        fOldVecZ  = 0.0;
        fOldVecX /= fLength;
        fOldVecY /= fLength;
    } else {
        fOldVecZ = sqrt(fOldVecZ);
    }

    /* Generate rotation vector by calculating cross product:
    *
    * fOldVec x fNewVec.
    *
    * The rotation vector is the axis of rotation
    * and is non-unit length since the length of a crossproduct
    * is related to the angle between fOldVec and fNewVec which we need
    * in order to perform the rotation.
    */
    *fVecX = fOldVecY * fNewVecZ - fNewVecY * fOldVecZ;
    *fVecY = fOldVecZ * fNewVecX - fNewVecZ * fOldVecX;
    *fVecZ = fOldVecX * fNewVecY - fNewVecX * fOldVecY;
}

/*******************************************************
 * void vAxisRotMatrix(float fVecX, float fVecY, float fVecZ, Matrix mNewMat)
 *
 *    Calculate the rotation matrix for rotation about an arbitrary axis.
 *
 *    The axis of rotation is specified by (fVecX,fVecY,fVecZ). The length
 *    of the vector is the amount to rotate by.
 *
 * Parameters: fVecX,fVecY,fVecZ - Axis of rotation
 *             mNewMat - Output matrix of rotation in column-major format
 *                       (ie, translation components are in column 3 on rows
 *                       0,1, and 2).
 *
 *******************************************************/
QMatrix4x4 vAxisRotMatrix(float fVecX, float fVecY, float fVecZ) {
    float fRadians, fInvLength, fNewVecX, fNewVecY, fNewVecZ;

    /* Find the length of the vector which is the angle of rotation
     * (in radians)
     */
    fRadians = sqrt(fVecX * fVecX + fVecY * fVecY + fVecZ * fVecZ);

    /* If the vector has zero length - return the identity matrix */
    if (fRadians > -0.000001 && fRadians < 0.000001) {
        QMatrix4x4 mNewMat;
        mNewMat.setToIdentity();
        return mNewMat;
    }

    QMatrix4x4 mNewMat;
    mNewMat.setToIdentity();

    /* Normalize the rotation vector now in preparation for making
     * rotation matrix.
     */
    fInvLength = 1 / fRadians;
    fNewVecX   = fVecX * fInvLength;
    fNewVecY   = fVecY * fInvLength;
    fNewVecZ   = fVecZ * fInvLength;

    /* Create the arbitrary axis rotation matrix */
    double dSinAlpha = sin(fRadians);
    double dCosAlpha = cos(fRadians);
    double dT = 1 - dCosAlpha;

    mNewMat = QMatrix4x4(
                dCosAlpha + fNewVecX*fNewVecX*dT            ,
                fNewVecX*fNewVecY*dT + fNewVecZ*dSinAlpha   ,
                fNewVecX*fNewVecZ*dT - fNewVecY*dSinAlpha   ,
                0,

                fNewVecX*fNewVecY*dT - dSinAlpha*fNewVecZ  ,
                dCosAlpha + fNewVecY*fNewVecY*dT           ,
                fNewVecY*fNewVecZ*dT + dSinAlpha*fNewVecX  ,
                0,

                fNewVecZ*fNewVecX*dT + dSinAlpha*fNewVecY,
                fNewVecZ*fNewVecY*dT - dSinAlpha*fNewVecX,
                dCosAlpha + fNewVecZ*fNewVecZ*dT         ,
                0,

                0,
                0,
                0,
                1);



    return mNewMat;
}

