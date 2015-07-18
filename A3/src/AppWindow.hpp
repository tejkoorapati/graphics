#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <vector>
#include "scene.hpp"
#include "Viewer.hpp"

class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    AppWindow();
    void setSceneNode(SceneNode* node);
protected:
    void keyPressEvent(QKeyEvent *event);

private slots:

    void select_leftUpperArm ();
    void select_leftLowerArm();
    void select_leftHand();
    void select_leftUpperLeg();
    void select_leftLowerLeg();
    void select_leftFoot();
    void select_rightUpperArm();
    void select_rightLowerArm();
    void select_rightHand();
    void select_rightUpperLeg();
    void select_rightLowerLeg();
    void select_rightFoot();
    void select_neck();
    void select_head();
    void select_joints();
    void select_PO();
    void select_undo();
    void select_redo();
    void select_resetALL();
    void select_resetPos();
    void select_resetRot();
    void select_resetJoi();
    void select_circle   ();
    void select_zBuffer  ();
    void select_frontFace();
    void select_backface ();

private:
    void createActions();
    void createMenu();

    QAction* select_leftUpperArmAct;
    QAction* select_leftLowerArmAct;
    QAction* select_leftHandAct;
    QAction* select_leftUpperLegAct;
    QAction* select_leftLowerLegAct;
    QAction* select_leftFootAct;
    QAction* select_rightUpperArmAct;
    QAction* select_rightLowerArmAct;
    QAction* select_rightHandAct;
    QAction* select_rightUpperLegAct;
    QAction* select_rightLowerLegAct;
    QAction* select_rightFootAct;
    QAction* select_neckAct;
    QAction* select_headAct;
    QAction* select_undoAct;
    QAction* select_redoAct;
    QAction* select_resetALLAct;
    QAction* select_resetPosAct;
    QAction* select_resetRotAct;
    QAction* select_resetJoiAct;
    QAction* select_jointsAct;
    QAction* select_POAct;
    QAction* select_circleAct   ;
    QAction* select_zBufferAct  ;
    QAction* select_frontFaceAct;
    QAction* select_backfaceAct ;

    // Each menu itself
    QMenu* m_menu_app;
    QMenu* m_menu_pick;
    QMenu* m_menu_mode;
    QMenu* m_menu_edit;
    QMenu* m_menu_options;

    std::vector<QAction*> m_menu_actions;
    std::vector<QAction*> m_pick_actions;
    std::vector<QAction*> m_mode_actions;
    std::vector<QAction*> m_edit_actions;
    std::vector<QAction*> m_options_actions;
    Viewer* m_viewer;
};

#endif
