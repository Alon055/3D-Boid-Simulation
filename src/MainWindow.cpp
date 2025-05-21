#ifndef MAINWINDOW_CPP
#define MAINWINDOW_CPP

#include "MainWindow.h"
#include "BoidSystem.h"
#include "ui_MainWindow.h"


// code modified from this example
// prompt :- how can I create a GUI widget class with qt to control the values in Bird.h in real time?
// chatgpt suggested the following(see the rest of the code in BoidControlPanel.h, main.cpp, BoidSystem.cpp and NGLScene.h)
/*
BoidControlPanel::BoidControlPanel(BoidSystem* system, QWidget* parent)
    : QWidget(parent), m_system(system)
{
    m_maxSpeedSlider = new QSlider(Qt::Horizontal);
    m_maxSpeedSlider->setRange(1, 100);
    m_maxSpeedSlider->setValue(static_cast<int>(system->getBirds()[0].maxSpeed * 10));
    connect(m_maxSpeedSlider, &QSlider::valueChanged, this, &BoidControlPanel::updateMaxSpeed);

    m_cohesionSlider = new QSlider(Qt::Horizontal);
    m_cohesionSlider->setRange(0, 50);
    m_cohesionSlider->setValue(static_cast<int>(system->getBirds()[0].cohesionForce * 10));
    connect(m_cohesionSlider, &QSlider::valueChanged, this, &BoidControlPanel::updateCohesionForce);

    m_resetButton = new QPushButton("Reset");
    connect(m_resetButton, &QPushButton::clicked, this, &BoidControlPanel::resetSimulation);

    auto* layout = new QVBoxLayout();
    layout->addWidget(new QLabel("Max Speed"));
    layout->addWidget(m_maxSpeedSlider);
    layout->addWidget(new QLabel("Cohesion Force"));
    layout->addWidget(m_cohesionSlider);
    layout->addWidget(m_resetButton);
    setLayout(layout);
}

void BoidControlPanel::updateMaxSpeed(int value)
{
    float newSpeed = value / 10.0f;
    for (auto& bird : m_system->getBirds())
    {
        bird.maxSpeed = newSpeed;
    }
}

void BoidControlPanel::updateCohesionForce(int value)
{
    float newForce = value / 10.0f;
    for (auto& bird : m_system->getBirds())
    {
        bird.cohesionForce = newForce;
    }
}

void BoidControlPanel::resetSimulation()
{
    m_system->reset();  // You’ll need to implement reset() in BoidSystem
}
*/

//Code used:

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    m_gl = new  NGLScene(this);

    m_ui->gridLayout->addWidget(m_gl,0,2,1,1);

    connect(m_ui->m_minSpeed, SIGNAL(valueChanged(int)),
            m_gl, SLOT(setMinSpeed(int)));
    connect(m_ui->m_maxSpeed, SIGNAL(valueChanged(int)),
        m_gl, SLOT(setMaxSpeed(int)));
    connect(m_ui->m_maxForce, SIGNAL(valueChanged(int)),
    m_gl, SLOT(setMaxForce(int)));
    connect(m_ui->m_FOVRange, SIGNAL(valueChanged(int)),
    m_gl, SLOT(setVisualRangeDistance(int)));
    connect(m_ui->m_FOVAngle, SIGNAL(valueChanged(int)),
            m_gl, SLOT(setVisualRangeAngle(int)));
    connect(m_ui->m_alignment, SIGNAL(valueChanged(int)),
            m_gl, SLOT(setAlignmentForce(int)));
    connect(m_ui->m_separation, SIGNAL(valueChanged(int)),
            m_gl, SLOT(setSeparationForce(int)));
    connect(m_ui->m_cohesion, SIGNAL(valueChanged(int)),
            m_gl, SLOT(setCohesionForce(int)));
    connect(m_ui->m_envSize, SIGNAL(valueChanged(int)),
            m_gl, SLOT(setEnvSize(int)));
    connect(m_ui->m_nParticles, SIGNAL(valueChanged(int)),
        m_gl, SLOT(setNParticles(int)));


}

MainWindow::~MainWindow()
{
    delete m_ui;
}

#endif

