#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMatrix4x4>
#include <QtGlobal>
#include "scene.hpp"
#include <vector>

#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include <QOpenGLFunctions>
#else 
#include <QGLBuffer>
#endif

class SceneNode;
class JointNode;

class Viewer : public QGLWidget {
    
    Q_OBJECT

public:
    Viewer(const QGLFormat& format, QWidget *parent = 0);
    virtual ~Viewer();
    bool picking;
    bool joints;
    bool PO;

    bool backFace;
    bool frontFace;
    bool zBuffer;
    bool circle;


    bool leftUpperArm ;
    bool leftLowerArm;
    bool leftHand;
    bool leftUpperLeg;
    bool leftLowerLeg;
    bool leftFoot;
    bool rightUpperArm;
    bool rightLowerArm;
    bool rightHand;
    bool rightUpperLeg;
    bool rightLowerLeg;
    bool rightFoot;
    bool neck;
    bool head;
    std::vector<SceneNode*> selectedNodes;
    std::vector< std::vector<SceneNode*> > undoNodes;
    std::vector< std::vector<SceneNode*> > redoNodes;

    bool MMB;
    bool RMB;


    void undo();
    void redo();
    void resetJoints();
    void resetAll();
    void resetPos();
    void resetRot();
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void setSceneNode(SceneNode* node);

    void drawSphere(QColor color, QMatrix4x4 trans, std::string name);

//    JointNode* curNode;

    // If you want to render a new frame, call do not call paintGL(),
    // instead, call update() to ensure that the view gets a paint 
    // event.
    void updateSelectedNodes();
  
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
    
    // Draw a circle for the trackball, with OpenGL commands.
    // Assumes the context for the viewer is active.
    void draw_trackball_circle();

private:

    QMatrix4x4 getCameraMatrix();
    void translateWorld(float x, float y, float z);
    void rotateWorld(float angle,float x, float y, float z);
    void scaleWorld(float x, float y, float z);
    void set_colour(const QColor& col);

    void vCalcRotVec(float fNewX, float fNewY,
                     float fOldX, float fOldY,
                     float fDiameter,
                     float *fVecX, float *fVecY, float *fVecZ);
    QMatrix4x4 vAxisRotMatrix(float fVecX, float fVecY, float fVecZ);





#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    QOpenGLBuffer mCircleBufferObject;
    QOpenGLBuffer mSphereBufferObject;
    QOpenGLVertexArrayObject mVertexArrayObject;
#else 
    QGLBuffer mCircleBufferObject;
#endif
    
    int mMvpMatrixLocation;
    int mColorLocation;
    int buttonPressed;
    int prev_x;
    int prev_y;
    float circleData[40*3];
    float sphereData[36*37*6];
    QMatrix4x4 mPerspMatrix;
    QMatrix4x4 mRotationMatrix;
    QMatrix4x4 mTransformMatrix;
    QGLShaderProgram mProgram;


    SceneNode* m_root;
};


#endif
