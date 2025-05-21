///This code was originally written by Jon Macey
#ifndef NGLSCENE_H_
#define NGLSCENE_H_
#include <ngl/Vec3.h>
#include <ngl/Mat4.h>
#include "WindowParams.h"
// this must be included after NGL includes else we get a clash with gl libs
#include <QOpenGLWindow>
#include "BoidSystem.h"
#include <memory>
#include <QOpenGLWidget>
//----------------------------------------------------------------------------------------------------------------------
/// @file NGLScene.h
/// @brief this class inherits from the Qt OpenGLWindow and allows us to use NGL to draw OpenGL
/// @author Jonathan Macey
/// @version 1.0
/// @date 10/9/13
/// Revision History :
/// This is an initial version used for the new NGL6 / Qt 5 demos
/// @class NGLScene
/// @brief our main glwindow widget for NGL applications all drawing elements are
/// put in this file
//----------------------------------------------------------------------------------------------------------------------

class NGLScene : public QOpenGLWidget
{
    Q_OBJECT
  public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor for our NGL drawing class
    /// @param [in] parent the parent window to the class
    //----------------------------------------------------------------------------------------------------------------------
    NGLScene(QWidget *_parent);
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief dtor must close down ngl and release OpenGL resources
    //----------------------------------------------------------------------------------------------------------------------
    ~NGLScene() override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the initialize class is called once when the window is created and we have a valid GL context
    /// use this to setup any default GL stuff
    //----------------------------------------------------------------------------------------------------------------------
    void initializeGL() override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime we want to draw the scene
    //----------------------------------------------------------------------------------------------------------------------
    void paintGL() override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this is called everytime we resize the window
    //----------------------------------------------------------------------------------------------------------------------
    void resizeGL(int _w, int _h) override;

    // code modified from this example
    // prompt :- how can I create a GUI widget class with qt to control the values in Bird.h in real time?
    // chatgpt suggested the following(see the rest of the code in BoidControlPanel.cpp, BoidControlPanel.h, BoidSystem.cpp and main.cpp)
    /*
    explicit NGLScene(std::shared_ptr<BoidSystem> system)
    : m_system(std::move(system)) {}
    */
    //Code used:
    BoidSystem *getBoidSystem() const
    {
        return m_boidSystem.get();
    };
    //Code ends
    void setNumBirds(int numBirds);

public slots:
    void setMinSpeed(int _value);
    void setMaxSpeed(int _value);
    void setMaxForce(int _value);
    void setVisualRangeDistance(int _value);
    void setVisualRangeAngle(int _value);
    void setAlignmentForce(int _value);
    void setSeparationForce(int _value);
    void setCohesionForce(int _value);
    void setEnvSize (int _value);
    void setNParticles (int _value);
signals:
    void glInitalized();

private:

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Qt Event called when a key is pressed
    /// @param [in] _event the Qt event to query for size etc
    //----------------------------------------------------------------------------------------------------------------------
    void keyPressEvent(QKeyEvent *_event) override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called every time a mouse is moved
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseMoveEvent (QMouseEvent * _event ) override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is pressed
    /// i///End of code by Jon Maceynherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mousePressEvent ( QMouseEvent *_event) override;
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse button is released
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void mouseReleaseEvent ( QMouseEvent *_event ) override;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief this method is called everytime the mouse wheel is moved
    /// inherited from QObject and overridden here.
    /// @param _event the Qt Event structure
    //----------------------------------------------------------------------------------------------------------------------
    void wheelEvent( QWheelEvent *_event) override;

    void timerEvent(QTimerEvent* event) override;
    void keyReleaseEvent(QKeyEvent *_event) override;
    void process_keys();

    ///End of code by Jon Macey

    WinParams m_win;
    //pos for each bird
    ngl::Vec3 m_modelPos;
    std::chrono::steady_clock::time_point m_previousTime;
    std::unique_ptr<BoidSystem> m_boidSystem;
    bool m_animate = true;
    int m_numBirds;
    ngl::Mat4 m_view;
    ngl::Mat4 m_project;
    QSet<Qt::Key> m_keysPressed;

};

#endif
