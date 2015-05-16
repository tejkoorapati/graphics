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

private:
    void createActions();
    void createMenu();

    // Each menu itself
    QMenu* m_menu_app;
    QMenu* m_menu_draw;

    vector<QAction*> m_menu_actions;
    vector<QAction*> m_draw_actions;
    Viewer* m_viewer;

private slots:
    void new_game();
    void reset();
    void wire_frame();
    void face();
    void multi_coloured();
};

#endif
