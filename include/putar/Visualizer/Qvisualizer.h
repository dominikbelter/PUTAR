/** @file QVisualizer.h
 *
 * implementation - QGLVisualizer
 *
 */

#ifndef QVISUALIZER_H_INCLUDED
#define QVISUALIZER_H_INCLUDED

#include "Defs/defs.h"
#include "Utilities/observer.h"
#include "../../3rdParty/tinyXML/tinyxml2.h"
#include <QGLViewer/qglviewer.h>
#include <iostream>
#include <thread>
#include <mutex>
using namespace putar;

/// Map implementation
class QGLVisualizer: public QGLViewer, public Observer{
public:
    /// Pointer
    typedef std::unique_ptr<QGLVisualizer> Ptr;

    class Config
    {

      public:
        Config(){
        }

        Config(std::string configFilename){
            load(configFilename);
        }

        void load(std::string configFilename) {
            tinyxml2::XMLDocument config;
            std::string filename = "../../resources/" + configFilename;
            config.LoadFile(filename.c_str());
            if (config.ErrorID())
                std::cout << "unable to load Visualizer config file: " + filename << std::endl;

            tinyxml2::XMLElement * model = config.FirstChildElement( "VisualizerConfig" );
            model->FirstChildElement( "parameters" )->QueryBoolAttribute("verbose", &verbose);
            double rgba[4]={0,0,0,0};
            model->FirstChildElement( "background" )->QueryDoubleAttribute("red", &rgba[0]);
            model->FirstChildElement( "background" )->QueryDoubleAttribute("green", &rgba[1]);
            model->FirstChildElement( "background" )->QueryDoubleAttribute("blue", &rgba[2]);
            model->FirstChildElement( "background" )->QueryDoubleAttribute("alpha", &rgba[3]);

            backgroundColor.setRedF(rgba[0]); backgroundColor.setGreenF(rgba[1]);
            backgroundColor.setBlueF(rgba[2]); backgroundColor.setAlphaF(rgba[3]);
        }
    
        public:
            /// verbose
            bool verbose;
            /// Background color
            QColor backgroundColor;
    };

    /// Construction
    QGLVisualizer(void);

    /// Construction
    QGLVisualizer(std::string configFilename);

    /// update object state
    void update(const putar::Mat34& objectState);
    
    /// Updates mesh
    virtual void updateMesh(const Object3D& mesh) = 0;
    
    /// Updates cloud
    virtual void updateCloud(cv::Mat RGBD) = 0;

    /// Destruction
    ~QGLVisualizer(void);

private:
    ///config
    Config config;

    /// draw objects
    void draw();

    /// draw objects
    void animate();

    ///render
    void renderScene();

    /// initialize visualizer
    void init();

    /// generate help string
    std::string help() const;

    bool shadowFlag = false;

};

#endif // QVISUALIZER_H_INCLUDED
