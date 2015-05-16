#include <QtWidgets>
#include <QGLFormat>
#include <iostream>
#include "AppWindow.hpp"

AppWindow::AppWindow() {
    setWindowTitle("488 Tetrominoes on the Wall");

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
    m_viewer->show();

    createActions();
    createMenu();
}

void AppWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        QCoreApplication::instance()->quit();
    } else if (event->key() == Qt::Key_T) {
        std::cerr << "Hello!" << std::endl;
    } else {
        QWidget::keyPressEvent(event);
    }
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

    QAction* newGameAct = new QAction(tr("&New Game"), this);
    m_menu_actions.push_back(newGameAct);
    newGameAct->setShortcut(QKeySequence(tr("N")));
    connect(newGameAct, SIGNAL(triggered()), this, SLOT(new_game()));

    QAction* resetAct = new QAction(tr("&Reset"), this);
    m_menu_actions.push_back(resetAct);
    resetAct->setShortcut(QKeySequence(tr("R")));
    connect(resetAct, SIGNAL(triggered()), this, SLOT(reset()));

    QAction* wireAct = new QAction(tr("&Wire-frame"), this);
    m_draw_actions.push_back(wireAct);
    wireAct->setChecked(true);
    wireAct->setCheckable(true);
    wireAct->setShortcut(QKeySequence(tr("W")));
    connect(wireAct, SIGNAL(triggered()), this, SLOT(wire_frame()));


    QAction* faceAct = new QAction(tr("&Face"), this);
    m_draw_actions.push_back(faceAct);
    faceAct->setCheckable(true);
    faceAct->setShortcut(QKeySequence(tr("F")));
    connect(faceAct, SIGNAL(triggered()), this, SLOT(face()));


    QAction* multiAct = new QAction(tr("&Multicoloured"), this);
    m_draw_actions.push_back(multiAct);
    multiAct->setCheckable(true);
    multiAct->setShortcut(QKeySequence(tr("M")));
    connect(multiAct, SIGNAL(triggered()), this, SLOT(multi_coloured()));

    QActionGroup* drawGroup = new QActionGroup(this);

    drawGroup->setExclusive(true);
    drawGroup->addAction(wireAct);
    drawGroup->addAction(faceAct);
    drawGroup->addAction(multiAct);



}

void AppWindow::createMenu() {
    m_menu_app = menuBar()->addMenu(tr("&Application"));
    m_menu_draw = menuBar()->addMenu(tr("&Draw Mode"));

    for (auto& action : m_menu_actions) {
        m_menu_app->addAction(action);
    }
    for (auto& action : m_draw_actions) {
        m_menu_draw->addAction(action);
    }


}

void AppWindow::new_game(){

}

void AppWindow::reset(){

}

void AppWindow::wire_frame(){

}
void AppWindow::face(){

}

void AppWindow::multi_coloured(){

}
