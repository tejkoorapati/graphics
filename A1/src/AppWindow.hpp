#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <vector>
#include <QKeySequence>
#include "Viewer.hpp"
using namespace std;
class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    AppWindow();


protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    void createActions();
    void createMenu();


    // Each menu itself
    QMenu* m_menu_app;
    QMenu* m_menu_draw;
    QMenu* m_menu_speed;

    vector<QAction*> m_menu_actions;
    vector<QAction*> m_draw_actions;
    vector<QAction*> m_speed_actions;
    Viewer* m_viewer;

    void moveRight();
    void moveLeft();
    void dropPiece();
    void rotateCW();
    void rotateCCW();
    void shiftPressed();
    void shiftReleased();

private slots:
    void new_game();
    void reset();
    void wire_frame();
    void face();
    void multi_coloured();
    void slowSpeed();
    void normalSpeed();
    void fastSpeed();


};

#endif
