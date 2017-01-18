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

            tinyxml2::XMLElement * kinectPar = config.FirstChildElement( "Model" );
            kinectPar->FirstChildElement( "focalLength" )->QueryDoubleAttribute("fu", &kinectModel.focalLength[0]);
            kinectPar->FirstChildElement( "focalLength" )->QueryDoubleAttribute("fv", &kinectModel.focalLength[1]);
            kinectPar->FirstChildElement( "focalAxis" )->QueryDoubleAttribute("Cu", &kinectModel.focalAxis[0]);
            kinectPar->FirstChildElement( "focalAxis" )->QueryDoubleAttribute("Cv", &kinectModel.focalAxis[1]);
            kinectPar->FirstChildElement( "variance" )->QueryDoubleAttribute("sigmaU", &kinectModel.varU);
            kinectPar->FirstChildElement( "variance" )->QueryDoubleAttribute("sigmaV", &kinectModel.varV);
            kinectPar->FirstChildElement( "varianceDepth" )->QueryDoubleAttribute("c3", &kinectModel.c3);
            kinectPar->FirstChildElement( "varianceDepth" )->QueryDoubleAttribute("c2", &kinectModel.c2);
            kinectPar->FirstChildElement( "varianceDepth" )->QueryDoubleAttribute("c1", &kinectModel.c1);
            kinectPar->FirstChildElement( "varianceDepth" )->QueryDoubleAttribute("c0", &kinectModel.c0);

            PHCPModel <<
        1/kinectModel.focalLength[0],0,-kinectModel.focalAxis[0]/kinectModel.focalLength[0],
                          0,1/kinectModel.focalLength[1],
        -kinectModel.focalAxis[1]/kinectModel.focalLength[1],
                          0,0,1;
           /* Ruvd << kinectModel.varU, 0, 0,
                    0, kinectModel.varV, 0,
                    0, 0, 0;*/
        }

        public:
            /// verbose
            bool verbose;
            /// Background color
            QColor backgroundColor;
            struct{
                double focalLength[2];
                double focalAxis[2];
                double varU;
                double varV;
                double c0;
                double c1;
                double c2;
                double c3;
            } kinectModel;

            /// pin-hole camera projection model
            Mat33 PHCPModel;
    };

    /// Construction
    QGLVisualizer(void);

    /// Construction
    QGLVisualizer(std::string configFilename);

    /// update object state
    void update(const putar::Mat34& objectState);

    /// Updates mesh
    void updateMesh(const Object3D& mesh);

    /// Updates cloud
    void updateCloud(cv::Mat RGB, cv::Mat D);

    void getPoint(unsigned int u, unsigned int v, float depth, Eigen::Vector3d& point3D);
    void depth2cloud(cv::Mat& depthImage, cv::Mat RGB);

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

    /// object pose
    Mat34 objectPose;

    struct Point3D{
        float x;
        float y;
        float z;
        unsigned char red;
        unsigned char green;
        unsigned char blue;
    };

    std::vector<Point3D> PointCloud;
};

#endif // QVISUALIZER_H_INCLUDED
