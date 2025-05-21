/// The following section is from :-
/// Qt Documentation (2015). QOpenGLWindow Class [online]. [Accessed 10 May 2025].
/// Modified by Jon Macey.
/// Available from: "https://contribute.qt-project.org/doc/qt-5.0/qtgui/openglwindow.html".
#ifndef MAIN_CPP
#define MAIN_CPP

#include <QApplication>
#include "NGLScene.h"
#include "MainWindow.h"

// code modified from this example
// prompt :- how can I implement the GUI I made to my scene on main?
// chatgpt suggested the following(see the rest of the code in BoidControlPanel.h, BoidControlPanel.cpp, BoidSystem.cpp and NGLScene.h)
/*
int main(int argc, char **argv)
{
    QApplication app(argc, argv);  // Use QApplication to support widgets

    // Setup OpenGL format
    QSurfaceFormat format;
    format.setSamples(4);
#if defined(__APPLE__)
    format.setMajorVersion(4);
    format.setMinorVersion(1);
#else
    format.setMajorVersion(4);
    format.setMinorVersion(3);
#endif
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);

    // Create the Boid simulation logic
    auto* system = new BoidSystem();

    // Create NGLScene (inherits QOpenGLWindow)
    auto* glScene = new NGLScene(system);
    glScene->setFormat(format);

    // Wrap NGLScene in a QWidget so we can place it in a layout
    QWidget* glWidget = QWidget::createWindowContainer(glScene);
    glWidget->setMinimumSize(1024, 720);

    // Create control panel
    auto* controlPanel = new BoidControlPanel(system);

    // Layout for OpenGL scene + control panel
    QWidget* centralWidget = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(glWidget);
    layout->addWidget(controlPanel);
    centralWidget->setLayout(layout);

    // Set up the main application window
    QMainWindow* mainWin = new QMainWindow();
    mainWin->setCentralWidget(centralWidget);
    mainWin->resize(1280, 720);
    mainWin->show();

    std::cout << "Profile is " << format.majorVersion() << "." << format.minorVersion() << "\n";

    return app.exec();
}
*/

//Code used:

int main(int argc, char **argv)
{
    QSurfaceFormat format;
    // set the number of samples for multisampling
    // will need to enable glEnable(GL_MULTISAMPLE); once we have a context
    format.setSamples(4);
#if defined( __APPLE__)
    // at present mac osx Mountain Lion only supports GL3.2
    // the new mavericks will have GL 4.x so can change
    format.setMajorVersion(4);
    format.setMinorVersion(1);
#else
    // with luck we have the latest GL version so set to this
    format.setMajorVersion(4);
    format.setMinorVersion(3);
#endif
    // now we are going to set to CoreProfile OpenGL so we can't use and old Immediate mode GL
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
    format.setDepthBufferSize(24);

    QApplication app(argc, argv);
    // create an OpenGL format specifier

    MainWindow w;

    //w.resize(1280, 720);
    w.show();

    return app.exec();
}

#endif
//Code ends
/// end of Citation

