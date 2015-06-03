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
    // m_menubar = new QMenuBar;
    m_viewer = new Viewer(glFormat, this);
    layout->addWidget(m_viewer);
    setCentralWidget(new QWidget);
    centralWidget()->setLayout(layout);

    createActions();
    createMenu();
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
}

void AppWindow::createMenu() {
    m_menu_app = menuBar()->addMenu(tr("&Application"));

    for (auto& action : m_menu_actions) {
        m_menu_app->addAction(action);
    }
}

void AppWindow::keyPressEvent(QKeyEvent *event) {

    if (event->key() == Qt::Key_O) {
        m_viewer->interactionMode = 0;
    } else if (event->key() == Qt::Key_N) {
        m_viewer->interactionMode = 1;
    } else if (event->key() == Qt::Key_P) {
        m_viewer->interactionMode = 2;
    } else if (event->key() == Qt::Key_R) {
        m_viewer->interactionMode = 3;
    } else if (event->key() == Qt::Key_T) {
        m_viewer->interactionMode = 4;
    } else if (event->key() == Qt::Key_S) {
        m_viewer->interactionMode = 5;
    } else if (event->key() == Qt::Key_A) {
        m_viewer->reset_view();
    } else if (event->key() == Qt::Key_V) {
        if(m_viewer->viewPortMode){
            m_viewer->viewPortMode = false;
        } else{
            m_viewer->viewPortMode = true;
        }
    }



}
