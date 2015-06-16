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
    setSceneNode(SceneNode* node);

private:
    void createActions();
    void createMenu();

    // Each menu itself
    QMenu* m_menu_app;

    std::vector<QAction*> m_menu_actions;
    Viewer* m_viewer;
};

#endif
