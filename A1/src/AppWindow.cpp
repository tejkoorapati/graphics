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
    
    if(m_viewer->gameStatus>=0){
        if (event->key() == Qt::Key_Escape) {
            QCoreApplication::instance()->quit();
        } else if (event->key() == Qt::Key_Right) {
            moveRight();
        } else if (event->key() == Qt::Key_Left) {
            moveLeft();
        } else if (event->key() == Qt::Key_Down) {
            rotateCW();
        } else if (event->key() == Qt::Key_Up) {
            rotateCCW();
        } else if (event->key() == Qt::Key_Space) {
            dropPiece();
        } else if (event->key() == Qt::Key_Shift) {
           shiftPressed();
        }
    }




    if (event->key() == Qt::Key_1) {
        slowSpeed();
        slowSpeedAct->setChecked(true);
    }else if (event->key() == Qt::Key_2) {
        normalSpeed();
        normalSpeedAct->setChecked(true);
    }else if (event->key() == Qt::Key_3) {
        fastSpeed();
        fastSpeedAct->setChecked(true);
    }else if (event->key() == Qt::Key_W) {
        wire_frame();
        wireAct->setChecked(true);
    }else if (event->key() == Qt::Key_F) {
        face();
        faceAct->setChecked(true);
    }else if (event->key() == Qt::Key_M) {
        multi_coloured();
        multiAct->setChecked(true);
    }else if (event->key() == Qt::Key_N) {
        new_game();
    }else if (event->key() == Qt::Key_R) {
        reset();
    }else if (event->key() == Qt::Key_Q) {
        close();
    }else {
        QWidget::keyPressEvent(event);
    }
}

void AppWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Shift) {
           shiftReleased();
        }
}

void AppWindow::createActions() {
    // Creates a new action for quiting and pushes it onto the menu actions vector
    quitAct = new QAction(tr("&Quit"), this);
    m_menu_actions.push_back(quitAct);

    // We set the accelerator keys
    // Alternatively, you could use: setShortcuts(Qt::CTRL + Qt::Key_P);
    quitAct->setShortcuts(QKeySequence::Quit);

    // Set the tip
    quitAct->setStatusTip(tr("Exits the file"));

    // Connect the action with the signal and slot designated
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    newGameAct = new QAction(tr("&New Game (N)"), this);
    m_menu_actions.push_back(newGameAct);
    connect(newGameAct, SIGNAL(triggered()), this, SLOT(new_game()));

    resetAct = new QAction(tr("&Reset (R)"), this);
    m_menu_actions.push_back(resetAct);
    connect(resetAct, SIGNAL(triggered()), this, SLOT(reset()));

    wireAct = new QAction(tr("&Wire-frame (W)"), this);
    m_draw_actions.push_back(wireAct);
    wireAct->setCheckable(true);
    connect(wireAct, SIGNAL(triggered()), this, SLOT(wire_frame()));


    faceAct = new QAction(tr("&Face (F)"), this);
    m_draw_actions.push_back(faceAct);
    faceAct->setCheckable(true);
    connect(faceAct, SIGNAL(triggered()), this, SLOT(face()));


    multiAct = new QAction(tr("&Multicoloured (M)"), this);
    m_draw_actions.push_back(multiAct);
    multiAct->setCheckable(true);
    connect(multiAct, SIGNAL(triggered()), this, SLOT(multi_coloured()));

    QActionGroup* drawGroup = new QActionGroup(this);

    drawGroup->setExclusive(true);
    drawGroup->addAction(wireAct);
    drawGroup->addAction(faceAct);
    drawGroup->addAction(multiAct);

    faceAct->setChecked(true);


    slowSpeedAct = new QAction(tr("&Slow (1)"), this);
    m_speed_actions.push_back(slowSpeedAct);
    slowSpeedAct->setCheckable(true);
    connect(slowSpeedAct, SIGNAL(triggered()), this, SLOT(slowSpeed()));

    normalSpeedAct = new QAction(tr("&Medium (2)"), this);
    m_speed_actions.push_back(normalSpeedAct);
    normalSpeedAct->setCheckable(true);
    connect(normalSpeedAct, SIGNAL(triggered()), this, SLOT(normalSpeed()));

    fastSpeedAct = new QAction(tr("&Fast (3)"), this);
    m_speed_actions.push_back(fastSpeedAct);
    fastSpeedAct->setCheckable(true);
    connect(fastSpeedAct, SIGNAL(triggered()), this, SLOT(fastSpeed()));

    QActionGroup* speedGroup = new QActionGroup(this);

    speedGroup->setExclusive(true);
    speedGroup->addAction(slowSpeedAct);
    speedGroup->addAction(normalSpeedAct);
    speedGroup->addAction(fastSpeedAct);

    normalSpeedAct->setChecked(true);




}

void AppWindow::createMenu() {
    m_menu_app = menuBar()->addMenu(tr("&Application"));
    m_menu_draw = menuBar()->addMenu(tr("&Draw Mode"));
    m_menu_speed = menuBar()->addMenu(tr("&Speed"));

    for (auto& action : m_menu_actions) {
        m_menu_app->addAction(action);
    }
    for (auto& action : m_draw_actions) {
        m_menu_draw->addAction(action);
    }

    for (auto& action : m_speed_actions) {
        m_menu_speed->addAction(action);
    }


}

void AppWindow::new_game(){

    m_viewer->m_game->reset();
}

void AppWindow::reset(){

    m_viewer->resetWorld();
}

void AppWindow::wire_frame(){
    m_viewer->setWireMode();
}
void AppWindow::face(){
    m_viewer->setFaceMode();
}

void AppWindow::multi_coloured(){
    m_viewer->setMultiMode();
}

void AppWindow::slowSpeed()
{
    m_viewer->slowGameSpeed();
}

void AppWindow::normalSpeed()
{
    m_viewer->normalGameSpeed();
}

void AppWindow::fastSpeed()
{
    m_viewer->fastGameSpeed();
}

void AppWindow::moveRight()
{
    m_viewer->m_game->moveRight();
}

void AppWindow::moveLeft()
{
    m_viewer->m_game->moveLeft();
}

void AppWindow::dropPiece()
{
    m_viewer->m_game->drop();
}

void AppWindow::rotateCW()
{
    m_viewer->m_game->rotateCW();
}

void AppWindow::rotateCCW()
{
    m_viewer->m_game->rotateCCW();
}

void AppWindow::shiftPressed()
{
    m_viewer->setShiftState(true);
}

void AppWindow::shiftReleased()
{
    m_viewer->setShiftState(false);
}
