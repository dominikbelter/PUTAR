#ifndef _OFFLINE_SLAM_H_
#define _OFFLINE_SLAM_H_

#include "Defs/defs.h"
#include "../../3rdParty/tinyXML/tinyxml2.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>

class OfflineSLAM
{
public:

    OfflineSLAM(std::string configFile);
    ~OfflineSLAM(){}

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
            tinyxml2::XMLElement * model = config.FirstChildElement( "OfflineSLAM" );
            dataset = model->FirstChildElement( "parameters" )->Attribute("dataset");
            groundTruth = model->FirstChildElement( "parameters" )->Attribute("groundTruth");
        }

        public:
            std::string dataset;
            std::string groundTruth;

    };
    ///get frame
    void getFrame(cv::Mat& rgb, cv::Mat& depth);
    ///get current pose
    void getCurrentPose(putar::Mat34& pose);

private:
    /// image Number
    int frameNo;
    /// ground truth trajectory
    std::vector<putar::Mat34> groundtruth;
    /// config
    Config config;
    /// load ground truth trajectory
    void loadGT(void);

};

#endif // _OFFLINE_SLAM_H_
