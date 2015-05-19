#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#define PI 3.14159265

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMatrix4x4>
#include <QtGlobal>
#include <math.h>
#include "game.hpp"

#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#else 
#include <QGLBuffer>
#endif
using namespace std;
class Viewer : public QGLWidget {
    
    Q_OBJECT

public:
    Viewer(const QGLFormat& format, QWidget *parent = 0);
    virtual ~Viewer();
    
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    Game* m_game;

    // If you want to render a new frame, call do not call paintGL(),
    // instead, call update() to ensure that the view gets a paint 
    // event.
    void increaseGameSpeed();
    void decreaseGameSpeed();
    void setWireMode();
    void setFaceMode();
    void setMultiMode();
    void setShiftState(bool state);
    void resetWorld();
public slots:
    void gameTick();
    void trackSpeed();
  
protected:

    // Events we implement

    // Called when GL is first initialized
    virtual void initializeGL();
    // Called when our window needs to be redrawn
    virtual void paintGL();
    // Called when the window is resized (formerly on_configure_event)
    virtual void resizeGL(int width, int height);
    // Called when a mouse button is pressed
    virtual void mousePressEvent ( QMouseEvent * event );
    // Called when a mouse button is released
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    // Called when the mouse moves
    virtual void mouseMoveEvent ( QMouseEvent * event );

private:

    QMatrix4x4 getCameraMatrix();
    void translateWorld(float x, float y, float z);
    void rotateWorld(float angle, float x, float y, float z);
    void scaleWorld(float x, float y, float z);
    void drawCubeAt(int row, int col, QColor color);
    void makeWell();




#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    QOpenGLBuffer mVertexBufferObject;
    QOpenGLVertexArrayObject mVertexArrayObject;
#else 
    QGLBuffer mVertexBufferObject;
#endif

    int mVertexLocation;
    int mMvpMatrixLocation;
    int colorLocation;

    int buttonPressed;
    int gameSpeed=3;
    bool multMode = false;
    int drawMode = GL_TRIANGLES;

    QMatrix4x4 mPerspMatrix;
    QMatrix4x4 mModelMatrices[12];
    QMatrix4x4 mTransformMatrix;
    
    QTimer* mTimer;
    QTimer* gameTimer;
    QTimer* rotateTimer;

    QGLShaderProgram mProgram;

    int prev_x =-1;
    int prev_y = -1;
    QColor colorList[12]  = {Qt::red, Qt::green ,Qt::blue,
                             Qt::magenta,Qt::yellow,Qt::darkGray,
                             Qt::cyan,Qt::white,Qt::darkRed,
                            Qt::darkBlue,Qt::darkGreen,Qt::darkYellow};

    bool shiftState = false;
    float worldScale = 1;

    bool autoRotate;
    int rotateSign;
    int speed_x=0;
    int speed_y=0;
    int xBeforeTick;
    int yBeforeTick;
    char lastRotateAxis;

};

#endif
