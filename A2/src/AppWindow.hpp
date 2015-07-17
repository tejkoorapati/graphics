#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <vector>
#include "Viewer.hpp"

class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    AppWindow();

private slots:

void viewRotate();
void viewTranslate();
void viewPerspective();

void modelRotate();
void modelTranslate();
void modelScale();

void viewPortMode();

void resetView();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    void createActions();
    void createMenu();
    QAction* viewRotateAct;
    QAction* viewTranslateAct;
    QAction* viewPerspectiveAct;
    QAction* modelRotateAct;
    QAction* modelTranslateAct;
    QAction* modelScaleAct;
    QAction* viewPortModeAct;


    // Each menu itself
    QMenu* m_menu_app;
    QMenu* m_menu_mode;

    std::vector<QAction*> m_menu_actions;
     std::vector<QAction*> m_mode_actions;
    Viewer* m_viewer;
};

#endif
