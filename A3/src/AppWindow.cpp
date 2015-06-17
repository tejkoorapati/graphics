#include <QtWidgets>
#include <iostream>
#include "AppWindow.hpp"

AppWindow::AppWindow() {
    setWindowTitle("488 Assignment Two");

    QGLFormat glFormat;
    glFormat.setVersion(3,3);
    glFormat.setProfile(QGLFormat::CoreProfile);
    glFormat.setSampleBuffers(true);

    QVBoxLayout *layout = new QVBoxLayout;
    m_viewer = new Viewer(glFormat, this);
    layout->addWidget(m_viewer);
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);

    createActions();
    createMenu();
}

AppWindow::setSceneNode(SceneNode *node)
{
    m_viewer->setSceneNode(node);
    node->setViewer(m_viewer);
}

void AppWindow::createActions() {
    // Creates a new action for quiting and pushes it onto the menu actions vector
    QAction* quitAct = new QAction(tr("&Quit"), this);
    m_menu_actions.push_back(quitAct);

    // We set the accelerator keys
    // Alternatively, you could use: setShortcuts(Qt::CTRL + Qt::Key_P);
    quitAct->setShortcuts(QKeySequence::Quit);

    // Set the tip
    quitAct->setStatusTip(tr("Exits the file"));

    // Connect the action with the signal and slot designated
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));


    select_leftUpperArmAct  = new QAction(tr("Select left upper arm"  ),this);
    select_leftLowerArmAct  = new QAction(tr("Select left lower arm"  ),this);
    select_leftHandAct      = new QAction(tr("Select left hand"      ),this);
    select_leftUpperLegAct  = new QAction(tr("Select left upper leg"  ),this);
    select_leftLowerLegAct  = new QAction(tr("Select left lower leg"  ),this);
    select_leftFootAct      = new QAction(tr("Select left foot"      ),this);
    select_rightUpperArmAct = new QAction(tr("Select right upper arm" ),this);
    select_rightLowerArmAct = new QAction(tr("Select right lower arm" ),this);
    select_rightHandAct     = new QAction(tr("Select right hand"     ),this);
    select_rightUpperLegAct = new QAction(tr("Select right upper leg" ),this);
    select_rightLowerLegAct = new QAction(tr("Select right lower leg" ),this);
    select_rightFootAct     = new QAction(tr("Select right foot"     ),this);
    select_neckAct          = new QAction(tr("Select neck"          ),this);
    select_headAct          = new QAction(tr("Select head"         ),this);
    select_jointsAct       = new QAction(tr("Joints (J)"         ),this);
    select_POAct          = new QAction(tr("Position/Orientation (P)"         ),this);
    select_undoAct          = new QAction(tr("Undo (U)"         ),this);
    select_redoAct          = new QAction(tr("Redo (R)"         ),this);

    select_resetALLAct = new QAction(tr("Reset All (A)"),this);
    select_resetPosAct = new QAction(tr("Reset Position (I)"),this);
    select_resetRotAct = new QAction(tr("Reset Orientation (O)"),this);
    select_resetJoiAct = new QAction(tr("Reset Joints (N)"),this);

    select_circleAct    = new QAction(tr("Circle (C)"),this);
    select_zBufferAct   = new QAction(tr("Z-Buffer (Z)"),this);
    select_frontFaceAct = new QAction(tr("Backface cull (B)"),this);
    select_backfaceAct  = new QAction(tr("Frontface cull(F)"),this);

    m_pick_actions.push_back(select_leftUpperArmAct  );
    m_pick_actions.push_back(select_leftLowerArmAct  );
    m_pick_actions.push_back(select_leftHandAct      );
    m_pick_actions.push_back(select_leftUpperLegAct  );
    m_pick_actions.push_back(select_leftLowerLegAct  );
    m_pick_actions.push_back(select_leftFootAct      );
    m_pick_actions.push_back(select_rightUpperArmAct );
    m_pick_actions.push_back(select_rightLowerArmAct );
    m_pick_actions.push_back(select_rightHandAct     );
    m_pick_actions.push_back(select_rightUpperLegAct );
    m_pick_actions.push_back(select_rightLowerLegAct );
    m_pick_actions.push_back(select_rightFootAct     );
    m_pick_actions.push_back(select_neckAct          );
    m_pick_actions.push_back(select_headAct          );
    m_mode_actions.push_back(select_jointsAct          );
    m_mode_actions.push_back(select_POAct          );
    m_edit_actions.push_back(select_undoAct          );
    m_edit_actions.push_back(select_redoAct          );

    m_menu_actions.push_back(select_resetALLAct);
    m_menu_actions.push_back(select_resetPosAct);
    m_menu_actions.push_back(select_resetRotAct);
    m_menu_actions.push_back(select_resetJoiAct);

    m_options_actions.push_back(select_circleAct   );
    m_options_actions.push_back(select_zBufferAct  );
    m_options_actions.push_back(select_frontFaceAct);
    m_options_actions.push_back(select_backfaceAct );

    select_leftUpperArmAct  -> setCheckable(true);
    select_leftLowerArmAct  -> setCheckable(true);
    select_leftHandAct      -> setCheckable(true);
    select_leftUpperLegAct  -> setCheckable(true);
    select_leftLowerLegAct  -> setCheckable(true);
    select_leftFootAct      -> setCheckable(true);
    select_rightUpperArmAct -> setCheckable(true);
    select_rightLowerArmAct -> setCheckable(true);
    select_rightHandAct     -> setCheckable(true);
    select_rightUpperLegAct -> setCheckable(true);
    select_rightLowerLegAct -> setCheckable(true);
    select_rightFootAct     -> setCheckable(true);
    select_neckAct          -> setCheckable(true);
    select_headAct          -> setCheckable(true);
    select_jointsAct        -> setCheckable(true);
    select_POAct            -> setCheckable(true);
    select_circleAct        -> setCheckable(true);
    select_zBufferAct       -> setCheckable(true);
    select_frontFaceAct     -> setCheckable(true);
    select_backfaceAct      -> setCheckable(true);



    connect(select_leftUpperArmAct  , SIGNAL(triggered()), this, SLOT(select_leftUpperArm ()));
    connect(select_leftLowerArmAct  , SIGNAL(triggered()), this, SLOT(select_leftLowerArm() ));
    connect(select_leftHandAct      , SIGNAL(triggered()), this, SLOT(select_leftHand()     ));
    connect(select_leftUpperLegAct  , SIGNAL(triggered()), this, SLOT(select_leftUpperLeg() ));
    connect(select_leftLowerLegAct  , SIGNAL(triggered()), this, SLOT(select_leftLowerLeg() ));
    connect(select_leftFootAct      , SIGNAL(triggered()), this, SLOT(select_leftFoot()     ));
    connect(select_rightUpperArmAct , SIGNAL(triggered()), this, SLOT(select_rightUpperArm()));
    connect(select_rightLowerArmAct , SIGNAL(triggered()), this, SLOT(select_rightLowerArm()));
    connect(select_rightHandAct     , SIGNAL(triggered()), this, SLOT(select_rightHand()    ));
    connect(select_rightUpperLegAct , SIGNAL(triggered()), this, SLOT(select_rightUpperLeg()));
    connect(select_rightLowerLegAct , SIGNAL(triggered()), this, SLOT(select_rightLowerLeg()));
    connect(select_rightFootAct     , SIGNAL(triggered()), this, SLOT(select_rightFoot()    ));
    connect(select_neckAct          , SIGNAL(triggered()), this, SLOT(select_neck()         ));
    connect(select_headAct          , SIGNAL(triggered()), this, SLOT(select_head()         ));
    connect(select_jointsAct          , SIGNAL(triggered()), this, SLOT(select_joints()         ));
    connect(select_POAct          , SIGNAL(triggered()), this, SLOT(select_PO()         ));
    connect(select_undoAct          , SIGNAL(triggered()), this, SLOT(select_undo()         ));
    connect(select_redoAct          , SIGNAL(triggered()), this, SLOT(select_redo()         ));

    connect(select_resetALLAct, SIGNAL(triggered()), this, SLOT(select_resetALL()));
    connect(select_resetPosAct, SIGNAL(triggered()), this, SLOT(select_resetPos()));
    connect(select_resetRotAct, SIGNAL(triggered()), this, SLOT(select_resetRot()));
    connect(select_resetJoiAct, SIGNAL(triggered()), this, SLOT(select_resetJoi()));

    connect(select_circleAct   , SIGNAL(triggered()), this, SLOT(select_circle()));
    connect(select_zBufferAct  , SIGNAL(triggered()), this, SLOT(select_zBuffer()));
    connect(select_frontFaceAct, SIGNAL(triggered()), this, SLOT(select_frontFace()));
    connect(select_backfaceAct , SIGNAL(triggered()), this, SLOT(select_backface()));

    QActionGroup* modeGroup = new QActionGroup(this);

    modeGroup->setExclusive(true);
    modeGroup->addAction(select_jointsAct);
    modeGroup->addAction(select_POAct);
    select_POAct->setChecked(true);


}

void AppWindow::createMenu() {
    m_menu_app = menuBar()->addMenu(tr("&Application"));
    m_menu_pick = menuBar()->addMenu(tr("&Picking"));
    m_menu_mode = menuBar()->addMenu(tr("&Mode"));
    m_menu_edit = menuBar()->addMenu(tr("&Edit"));
    m_menu_options = menuBar()->addMenu(tr("&Options"));

    m_menu_pick->setEnabled(false);

    for (auto& action : m_menu_actions) {
        m_menu_app->addAction(action);
    }
    for (auto& action : m_pick_actions) {
        m_menu_pick->addAction(action);
    }

    for (auto& action : m_mode_actions) {
        m_menu_mode->addAction(action);
    }
    for (auto& action : m_edit_actions) {
        m_menu_edit->addAction(action);
    }
    for (auto& action : m_options_actions) {
        m_menu_options->addAction(action);
    }
}

void AppWindow::select_leftUpperArm (){ m_viewer->leftUpperArm  = !m_viewer->leftUpperArm  ; m_viewer->updateSelectedNodes();}
void AppWindow::select_leftLowerArm() { m_viewer->leftLowerArm  = !m_viewer->leftLowerArm  ; m_viewer->updateSelectedNodes();}
void AppWindow::select_leftHand()     { m_viewer->leftHand      = !m_viewer->leftHand      ; m_viewer->updateSelectedNodes();}
void AppWindow::select_leftUpperLeg() { m_viewer->leftUpperLeg  = !m_viewer->leftUpperLeg  ; m_viewer->updateSelectedNodes();}
void AppWindow::select_leftLowerLeg() { m_viewer->leftLowerLeg  = !m_viewer->leftLowerLeg  ; m_viewer->updateSelectedNodes();}
void AppWindow::select_leftFoot()     { m_viewer->leftFoot      = !m_viewer->leftFoot      ; m_viewer->updateSelectedNodes();}
void AppWindow::select_rightUpperArm(){ m_viewer->rightUpperArm = !m_viewer->rightUpperArm ; m_viewer->updateSelectedNodes();}
void AppWindow::select_rightLowerArm(){ m_viewer->rightLowerArm = !m_viewer->rightLowerArm ; m_viewer->updateSelectedNodes();}
void AppWindow::select_rightHand()    { m_viewer->rightHand     = !m_viewer->rightHand     ; m_viewer->updateSelectedNodes();}
void AppWindow::select_rightUpperLeg(){ m_viewer->rightUpperLeg = !m_viewer->rightUpperLeg ; m_viewer->updateSelectedNodes();}
void AppWindow::select_rightLowerLeg(){ m_viewer->rightLowerLeg = !m_viewer->rightLowerLeg ; m_viewer->updateSelectedNodes();}
void AppWindow::select_rightFoot()    { m_viewer->rightFoot     = !m_viewer->rightFoot     ; m_viewer->updateSelectedNodes();}
void AppWindow::select_neck()         { m_viewer->neck          = !m_viewer->neck          ; m_viewer->updateSelectedNodes();}
void AppWindow::select_head()         { m_viewer->head          = !m_viewer->head          ; m_viewer->updateSelectedNodes();}



void AppWindow::select_resetALL(){ m_viewer->resetAll()   ;}
void AppWindow::select_resetPos(){ m_viewer->resetPos()   ;}
void AppWindow::select_resetRot(){ m_viewer->resetRot()   ;}
void AppWindow::select_resetJoi(){ m_viewer->resetJoints();}

void AppWindow::select_circle()   {m_viewer->circle   = !m_viewer->circle    ;}
void AppWindow::select_zBuffer()  {m_viewer->zBuffer  = !m_viewer->zBuffer   ;}
void AppWindow::select_frontFace(){m_viewer->frontFace= !m_viewer->frontFace ;}
void AppWindow::select_backface() {m_viewer->backFace = !m_viewer->backFace  ;}

void AppWindow::select_joints()         {
    m_viewer->joints = true;
    m_viewer->PO= false;
    m_menu_pick->setEnabled(m_viewer->joints );
    m_viewer->updateSelectedNodes();

}

void AppWindow::select_PO()
{
    m_viewer->joints = false;
    m_viewer->PO= true;
    m_menu_pick->setEnabled(m_viewer->joints );

}

void AppWindow::select_undo()
{
    m_viewer->undo();
}


void AppWindow::select_redo()
{
    m_viewer->redo();
}
