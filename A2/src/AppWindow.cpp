#include <QtWidgets>
#include <iostream>
#include <QStatusBar>
#include "AppWindow.hpp"

AppWindow::AppWindow() {
    setWindowTitle("488 Assignment Two");

    QGLFormat glFormat;
    glFormat.setVersion(3,3);
    glFormat.setProfile(QGLFormat::CoreProfile);
    glFormat.setSampleBuffers(true);

    QVBoxLayout *layout = new QVBoxLayout;
    // m_menubar = new QMenuBar;
    m_viewer = new Viewer(glFormat, this);
    layout->addWidget(m_viewer);
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);

    QLabel* label = new QLabel(this);

    statusBar()->addPermanentWidget(label);
    m_viewer->setLabel(label);
    

    createActions();
    createMenu();
}

void AppWindow::createActions() {
    // Creates a new action for quiting and pushes it onto the menu actions vector
    menuBar()->setNativeMenuBar(false);
    QAction* quitAct = new QAction(tr("&Quit"), this);
    m_menu_actions.push_back(quitAct);

    // We set the accelerator keys
    // Alternatively, you could use: setShortcuts(Qt::CTRL + Qt::Key_P);
    quitAct->setShortcuts(QKeySequence::Quit);

    // Set the tip
    quitAct->setStatusTip(tr("Exits the file"));

    // Connect the action with the signal and slot designated
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    QAction* resetAct = new QAction(tr("Reset (A)"), this);
    m_menu_actions.push_back(resetAct);
    connect(resetAct, SIGNAL(triggered()), this, SLOT(resetView()));



     viewRotateAct = new QAction(tr("View Rotate (O)"), this);
    m_mode_actions.push_back(viewRotateAct);
    viewRotateAct->setCheckable(true);
    connect(viewRotateAct, SIGNAL(triggered()), this, SLOT(viewRotate()));

     viewTranslateAct = new QAction(tr("View Translate (N)"), this);
    m_mode_actions.push_back(viewTranslateAct);
    viewTranslateAct->setCheckable(true);
    connect(viewTranslateAct, SIGNAL(triggered()), this, SLOT(viewTranslate()));


     viewPerspectiveAct = new QAction(tr("View Perspective (P)"), this);
    m_mode_actions.push_back(viewPerspectiveAct);
    viewPerspectiveAct->setCheckable(true);
    connect(viewPerspectiveAct, SIGNAL(triggered()), this, SLOT(viewPerspective()));

     modelRotateAct = new QAction(tr("Model Rotate (R)"), this);
    m_mode_actions.push_back(modelRotateAct);
    modelRotateAct->setCheckable(true);
    connect(modelRotateAct, SIGNAL(triggered()), this, SLOT(modelRotate()));

     modelTranslateAct = new QAction(tr("Model Translate (T)"), this);
    m_mode_actions.push_back(modelTranslateAct);
    modelTranslateAct->setCheckable(true);
    connect(modelTranslateAct, SIGNAL(triggered()), this, SLOT(modelTranslate()));

     modelScaleAct = new QAction(tr("Model Scale (S)"), this);
    m_mode_actions.push_back(modelScaleAct);
    modelScaleAct->setCheckable(true);
    connect(modelScaleAct, SIGNAL(triggered()), this, SLOT(modelScale()));


     viewPortModeAct = new QAction(tr("Viewport Mode (V)"), this);
    m_mode_actions.push_back(viewPortModeAct);
    viewPortModeAct->setCheckable(true);
    connect(viewPortModeAct, SIGNAL(triggered()), this, SLOT(viewPortMode()));


    QActionGroup* modeGroup = new QActionGroup(this);

    modeGroup->setExclusive(true);
    modeGroup->addAction(viewRotateAct);
    modeGroup->addAction(viewTranslateAct);
    modeGroup->addAction(viewPerspectiveAct);
    modeGroup->addAction(modelRotateAct);
    modeGroup->addAction(modelTranslateAct);
    modeGroup->addAction(modelScaleAct);
    modeGroup->addAction(viewPortModeAct);

    modelRotate();
    modelRotateAct->setChecked(true);


}

void AppWindow::createMenu() {
    m_menu_app = menuBar()->addMenu(tr("&Application"));

    m_menu_mode = menuBar()->addMenu(tr("&Mode"));

    for (auto& action : m_menu_actions) {
        m_menu_app->addAction(action);
    }

    for (auto& action : m_mode_actions) {
        m_menu_mode->addAction(action);
    }
}

void AppWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        QCoreApplication::instance()->quit();
    } else if (event->key() == Qt::Key_O) {
     viewRotate();
     viewRotateAct->setChecked(true);
 } else if (event->key() == Qt::Key_N) {
    viewTranslate();
    viewTranslateAct->setChecked(true);
} else if (event->key() == Qt::Key_P) {
    viewPerspective();
    viewPerspectiveAct->setChecked(true);
} else if (event->key() == Qt::Key_R) {
    modelRotate();
    modelRotateAct->setChecked(true);
} else if (event->key() == Qt::Key_T) {
    modelTranslate();
    modelTranslateAct->setChecked(true);
} else if (event->key() == Qt::Key_S) {
    modelScale();
    modelScaleAct->setChecked(true);
} else if (event->key() == Qt::Key_A) {
    resetView();
} else if (event->key() == Qt::Key_V) {
    viewPortMode();
    viewPortModeAct->setChecked(true);
}
}


void AppWindow::viewRotate(){
    m_viewer->interactionMode = 0;
     m_viewer->viewPortMode = false;
}

void AppWindow::viewTranslate(){
    m_viewer->interactionMode = 1;
     m_viewer->viewPortMode = false;
}

void AppWindow::viewPerspective(){
    m_viewer->interactionMode =2;
     m_viewer->viewPortMode = false;
}

void AppWindow::modelRotate(){
    m_viewer->interactionMode = 3;
     m_viewer->viewPortMode = false;
}

void AppWindow::modelTranslate(){
    m_viewer->interactionMode = 4;
     m_viewer->viewPortMode = false;
}

void AppWindow::modelScale(){
    m_viewer->interactionMode = 5;
     m_viewer->viewPortMode = false;
}

void AppWindow::viewPortMode(){
     m_viewer->viewPortMode = true;
}

void AppWindow::resetView(){
    modelRotateAct->setChecked(true);
   m_viewer->reset_view();
    m_viewer->viewPortMode = false;
}