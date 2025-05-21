/// This code was originally written by Jon Macey
/// Modified by Alonso Plaza on 10th May
/// The following sections/functions are modified by me based on Jon Macey's original code:
#ifndef NGLSCENE_CPP
#define NGLSCENE_CPP

#include <QMouseEvent>
#include <QGuiApplication>
#include "BoidSystem.h"
#include "NGLScene.h"
#include <ngl/NGLInit.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/ShaderLib.h>
#include <ngl/Util.h>
#include <iostream>

/// Function 1
/// The following function was modified:

// NGLScene::NGLScene(QWidget *_parent) :QOpenGLWidget(_parent)
// {
//   // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
//   setTitle("Boid System");
// }

/// Modified: New member attribute of the NGLScene class added for the number of particles. setTitle deleted

NGLScene::NGLScene(QWidget *_parent) :QOpenGLWidget(_parent), m_numBirds(1500)
{
  // re-size the widget to that of the parent (in this case the GLFrame passed in on construction)
}

/// end of Citation



/// Function 2
/// The following function was created:
void NGLScene::setNumBirds(int numBirds)
{
  m_numBirds = numBirds;
  // Recreate the BoidSystem with the updated number of birds
  m_boidSystem = std::make_unique<BoidSystem>(m_numBirds);
}
/// end of Citation


NGLScene::~NGLScene()
{
  std::cout<<"Shutting down NGL, removing VAO's and Shaders\n";
}


void NGLScene::resizeGL(int _w , int _h)
{
  m_win.width  = static_cast<int>( _w * devicePixelRatio() );
  m_win.height = static_cast<int>( _h * devicePixelRatio() );

  m_project = ngl::perspective(45.0f, float(m_win.width)/float(m_win.height), 0.01f, 300.0f);
}

/// Function 3
/// The following function was modified:
void NGLScene::initializeGL()
{
  // // we must call that first before any other GL commands to load and link the
  // // gl commands from the lib, if that is not done program will crash
  // ngl::NGLInit::initialize();
  // glClearColor(0.7f, 0.7f, 0.7f, 1.0f);			   // Grey Background
  // // enable depth testing for drawing
  // glEnable(GL_DEPTH_TEST);
  // // enable multisampling for smoother drawing
  // glEnable(GL_MULTISAMPLE);
  // ngl::VAOPrimitives::createSphere("sphere", 1.0f, 20);
  // ngl::VAOPrimitives::createLineGrid("floor",100,100,50);
  // m_Emitter=std::make_unique<BoidSystem>(1000); //Create a set amount of particles
  // ngl::ShaderLib::loadShader("ParticleShader", "shaders/ParticleVertex.glsl", "shaders/ParticleFragment.glsl");
  // ngl::ShaderLib::use("ParticleShader");
  // m_view = ngl::lookAt({0,60,60},{0,0,0},{0,1,0});
  // m_previousTime=std::chrono::steady_clock::now();
  // startTimer(10);
  // emit glInitalized();

  /// Modified: Changed some of the lookAt values and the amount of particles created

  // we must call that first before any other GL commands to load and link the
  // gl commands from the lib, if that is not done program will crash
  ngl::NGLInit::initialize();
  glClearColor(0.7f, 0.7f, 0.7f, 1.0f);			   // Grey Background
  // enable depth testing for drawing
  glEnable(GL_DEPTH_TEST);
  // enable multisampling for smoother drawing
  glEnable(GL_MULTISAMPLE);
  ngl::VAOPrimitives::createSphere("sphere", 1.0f, 20);
  ngl::VAOPrimitives::createLineGrid("floor",100,100,50);
  m_boidSystem=std::make_unique<BoidSystem>(m_numBirds); //Create a set amount of particles
  ngl::ShaderLib::loadShader("ParticleShader", "shaders/ParticleVertex.glsl", "shaders/ParticleFragment.glsl");
  ngl::ShaderLib::use("ParticleShader");
  m_view = ngl::lookAt({0,60,90},{0,0,0},{0,1,0});
  m_previousTime=std::chrono::steady_clock::now();
  startTimer(10);
  emit glInitalized();
}
/// end of Citation

void NGLScene::paintGL()
{
  //clear the screen and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0,0,m_win.width,m_win.height);
  auto rotX = ngl::Mat4::rotateX(m_win.spinXFace);
  auto rotY = ngl::Mat4::rotateY(m_win.spinYFace);
  auto mouseRotation = rotX * rotY;
  mouseRotation.m_m[3][0]=m_modelPos.m_x;
  mouseRotation.m_m[3][1]=m_modelPos.m_y;
  mouseRotation.m_m[3][2]=m_modelPos.m_y;

  glPointSize(3);
  ngl::ShaderLib::use("ParticleShader");
  ngl::ShaderLib::setUniform("MVP", m_project*m_view*mouseRotation);
  m_boidSystem->render();

  ngl::ShaderLib::use(ngl::nglColourShader);
  ngl::ShaderLib::setUniform("MVP", m_project*m_view*mouseRotation);
  ngl::ShaderLib::setUniform("Colour", 1.0f, 1.0f, 1.0f);
  ngl::VAOPrimitives::draw("floor");
  ngl::ShaderLib::use(ngl::nglTextShader);

}

//----------------------------------------------------------------------------------------------------------------------

void NGLScene::keyReleaseEvent(QKeyEvent *_event)
{
  m_keysPressed -=(Qt::Key)_event->key();
}

void NGLScene::keyPressEvent(QKeyEvent *_event)
{
  // this method is called every time the main window recives a key event.
  // we then switch on the key value and set the camera in the GLWindow
  switch (_event->key())
  {
  // escape key to quite
  case Qt::Key_Escape : QGuiApplication::exit(EXIT_SUCCESS); break;
  case Qt::Key_Space :
      m_win.spinXFace=0;
      m_win.spinYFace=0;
      m_modelPos.set(ngl::Vec3::zero());

  break;
  case Qt::Key_A : m_animate^=true; break;
  default : break;
  }
  // finally update the GLWindow and re-draw

    update();
}



/// Function 4
/// The following function was modified:
void NGLScene::timerEvent(QTimerEvent *_event)
{
  // if(m_animate)
  // {
  //   m_emitter->update();
  // }
  // //std::cout<<"Update\n";
  // //m_emitter->update();
  // update();

  /// Modified: Changed from Emitter function to boidSystem

  auto now = std::chrono::steady_clock::now();
  //auto delta = std::chrono::duration<float,std::chrono::seconds::period>(now-m_previousTime);
  m_previousTime=now;

  if(m_animate)
  {
    m_boidSystem->update();
  }
  //std::cout<<"Update\n";
  //m_emitter->update();
  update();
}
/// end of Citation
///End of code by Jon Macey

void NGLScene::setMinSpeed(int _value)
{
  m_boidSystem->setMinSpeed(_value);
  update();
}

void NGLScene::setMaxSpeed(int _value)
{
  m_boidSystem->setMaxSpeed(_value);
  update();
}

void NGLScene::setMaxForce(int _value)
{
  m_boidSystem->setMaxForce(_value);
  update();
}

void NGLScene::setVisualRangeDistance(int _value)
{
  m_boidSystem->setVisualRangeDistance(_value);
  update();
}

void NGLScene::setVisualRangeAngle(int _value)
{
  m_boidSystem->setVisualRangeAngle(_value);
  update();
}

void NGLScene::setAlignmentForce(int _value)
{
  m_boidSystem->setAlignmentForce(_value);
  update();
}

void NGLScene::setSeparationForce(int _value)
{
  m_boidSystem->setSeparationForce(_value);
  update();
}

void NGLScene::setCohesionForce(int _value)
{
  m_boidSystem->setCohesionForce(_value);
  update();
}

void NGLScene::setEnvSize(int _value)
{
  m_boidSystem->setEnvSize(_value);
  update();
}

void NGLScene::setNParticles(int _value)
{
  m_boidSystem->setNParticles(_value);
  update();
}

#endif



