#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "NGLScene.h"

// code modified from this example
// prompt :- how can I create a GUI widget class with qt to control the values in Bird.h in real time?
// chatgpt suggested the following(see the rest of the code in BoidControlPanel.cpp, main.cpp, NGLScene.h and BoidSystem.cpp)
/*
class QSlider;
class QPushButton;
class BoidSystem;

class BoidControlPanel : public QWidget
{
    Q_OBJECT
public:
    explicit BoidControlPanel(BoidSystem* system, QWidget* parent = nullptr);

private slots:
    void updateMaxSpeed(int value);
    void updateCohesionForce(int value);
    void resetSimulation();

private:
    BoidSystem* m_system;
    QSlider* m_maxSpeedSlider;
    QSlider* m_cohesionSlider;
    QPushButton* m_resetButton;
 */

//Code used:

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:

    Ui::MainWindow *m_ui;
    NGLScene *m_gl;

};
//Code ends

#endif