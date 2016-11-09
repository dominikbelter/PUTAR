#include "Visualizer/Qvisualizer.h"
//#include <Eigen/Eigenvalues>
#include <memory>
#include <cmath>
#include <stdexcept>
#include <chrono>
#include <GL/glut.h>

using namespace putar;

/// A single instance of Visualizer
QGLVisualizer::Ptr visualizer;

QGLVisualizer::QGLVisualizer(void) : objectPose(Mat34::Identity()){
}

/// Construction
QGLVisualizer::QGLVisualizer(std::string configFilename) : config(configFilename), objectPose(Mat34::Identity()){
}

/// Destruction
QGLVisualizer::~QGLVisualizer(void) {
    //#ifdef DEBUG
    //std::cout << "QGLVisualizer destructor\n";
    //#endif
}

/// draw objects
void QGLVisualizer::draw(){
    double GLmat[16]={objectPose(0,0), objectPose(1,0), objectPose(2,0), 0, objectPose(0,1), objectPose(1,1), objectPose(2,1), 0, objectPose(0,2), objectPose(1,2), objectPose(2,2), 0, objectPose(0,3), objectPose(1,3), objectPose(2,3), 1};
    glPushMatrix();
    glMultMatrixd(GLmat);
    glutSolidTeapot(0.1);
    glPopMatrix();

    drawAxis();
}

/// draw objects
void QGLVisualizer::animate(){
}

/// update object state
void QGLVisualizer::update(const putar::Mat34& objectState){
    objectPose = objectState;
}

/// initialize visualizer
void QGLVisualizer::init(){
    // Light setup
//    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0);
//    GLfloat specular_color[4] = { 0.99f, 0.99f, 0.99f, 1.0f };
//    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  specular_color);

//    //Set global ambient light
//    GLfloat black[] = {0.99f, 0.99f, 0.99f, 1.0f};
//    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, black);

    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);
    // Restore previous viewer state.
    //restoreStateFromFile();

    camera()->setZNearCoefficient(0.00001f);
    camera()->setZClippingCoefficient(100.0);

    setBackgroundColor(config.backgroundColor);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

     // Opens help window
    help();

    startAnimation();
}

/// generate help string
std::string QGLVisualizer::help() const{
    std::string text("S i m p l e V i e w e r");
    text += "Use the mouse to move the camera around the object. ";
    text += "You can respectively revolve around, zoom and translate with the three mouse buttons. ";
    text += "Left and middle buttons pressed together rotate around the camera view direction axis<br><br>";
    text += "Pressing <b>Alt</b> and one of the function keys (<b>F1</b>..<b>F12</b>) defines a camera keyFrame. ";
    text += "Simply press the function key again to restore it. Several keyFrames define a ";
    text += "camera path. Paths are saved when you quit the application and restored at next start.<br><br>";
    text += "Press <b>F</b> to display the frame rate, <b>A</b> for the world axis, ";
    text += "<b>Alt+Return</b> for full screen mode and <b>Control+S</b> to save a snapshot. ";
    text += "See the <b>Keyboard</b> tab in this window for a complete shortcut list.<br><br>";
    text += "Double clicks automates single click actions: A left button double click aligns the closer axis with the camera (if close enough). ";
    text += "A middle button double click fits the zoom of the camera and the right button re-centers the scene.<br><br>";
    text += "A left button double click while holding right button pressed defines the camera <i>Revolve Around Point</i>. ";
    text += "See the <b>Mouse</b> tab and the documentation web pages for details.<br><br>";
    text += "Press <b>Escape</b> to exit the viewer.";
    return text;
}

