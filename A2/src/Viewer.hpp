#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMatrix4x4>
#include <QtGlobal>
#include <math.h>
#include "algebra.hpp"

#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#else 
#include <QGLBuffer>
#endif

class Viewer : public QGLWidget {
    
    Q_OBJECT

public:
    Viewer(const QGLFormat& format, QWidget *parent = 0);
    virtual ~Viewer();
    
    enum Axis {X,Y,Z};
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    int interactionMode;
    // If you want to render a new frame, call do not call paintGL(),
    // instead, call update() to ensure that the view gets a paint
    // event.

    // *** Fill in these functions (in viewer.cpp) ***

    // Set the parameters of the current perspective projection using
    // the semantics of gluPerspective().
    void set_perspective(double fov, double aspect,
                         double near_p, double far_p);

    // Restore all the transforms and perspective parameters to their
    // original state. Set the viewport to its initial size.
    void reset_view();
    bool viewPortMode;

protected:

    // Events we implement

    // Called when GL is first initialized
    virtual void initializeGL();
    // Called when our window needs to be redrawn
    virtual void paintGL();
    // Called when a mouse button is pressed
    virtual void mousePressEvent ( QMouseEvent * event );
    // Called when a mouse button is released
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    // Called when the mouse moves
    virtual void mouseMoveEvent ( QMouseEvent * event );

    // Draw a line -- call draw_init first!
    void draw_line(const QVector2D& p1, const QVector2D& p2) ;

    // Set the current colour
    void set_colour(const QColor& col);

    // Call this before you begin drawing. Width and height are the width
    // and height of the GL window.
    void draw_init();
private:

#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
    QOpenGLBuffer mVertexBufferObject;
    QOpenGLVertexArrayObject mVertexArrayObject;
#else 
    QGLBuffer mVertexBufferObject;
#endif

    QGLShaderProgram mProgram;

    int mColorLocation;
    

    Matrix4x4 m_projection;
    Matrix4x4 m_view;
    Matrix4x4 m_Model;
    Matrix4x4 m_scale;
    double m_fov;
    double m_near;
    double m_far;

    int prev_x;
    int prev_y;

    Point3D m_cubePoints[8];
    Point3D m_cubeFinal[8];

    Point3D m_cubeAxisPoints[4];
    Point3D m_cubeAxisFinal[4];

    Point3D m_worldAxisPoints[4];
    Point3D m_worldAxisFinal[4];


    Point2D bottomLeftView;
    Point2D topRightView;
    Point2D bottomRightView;
    Point2D topLeftView;

    double mousePressX;
    double mousePressY;

    double mouseReleaseX;
    double mouseReleaseY;



    bool leftPressed;
    bool middlePressed;
    bool rightPressed;
    void initAxis();
    void drawAxis();
    void initCube();
    void drawCube();
    void drawViewPortBorder();
    void setViewPort(Point2D bottomLeft, Point2D bottomRight,Point2D topLeft, Point2D topRight);
    Point3D scaleToViewPort(Point3D point);
    Point3D project(Point3D point);
    void draw3dLine(Point3D start, Point3D end);
    bool clipAgainstPlane(Point3D &start, Point3D &end, Point3D plane, Vector3D normal);

    void drawMouseTracking();

    void rotate(Matrix4x4 &mat, double angle, Axis axis);
    void translate(Matrix4x4 &mat, double x, double y, double z);
    void scale(Matrix4x4 &mat, double x, double y, double z);

};

#endif
