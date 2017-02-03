#ifndef _IMAGEVISUALIZERCV_H_
#define _IMAGEVISUALIZERCV_H_

#include "../Defs/defs.h"
#include "Utilities/observer.h"
#include <iostream>
#include <string>
#include "opencvCore.h"
#include "imageVisualizer.h"
#include "opencv2/opencv.hpp"
#include "../../3rdParty/tinyXML/tinyxml2.h"


namespace putar{
	
	//create ImageVisualizer object
    ImageVisualizer* createMyImageVisualizer(void);
    ImageVisualizer* createMyImageVisualizer(std::string configFilename);
}

using namespace putar;

// ImageVisualizerCV implementation
class ImageVisualizerCV: public ImageVisualizer{
	public:
	
	// Pointer
    typedef std::unique_ptr<ImageVisualizerCV> Ptr;

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
            tinyxml2::XMLElement * model = config.FirstChildElement( "ImageVisualizer" );
            windowName=model->FirstChildElement( "parameters" )->Attribute("windowName");
        }
    
        public:
            std::string windowName;

    };

	// Constructor
	ImageVisualizerCV(void){}
    // overloaded constructor
    ImageVisualizerCV(std::string configFilename);


    cv::Mat UpdateImage(cv::Mat mask, cv::Mat depthMask, cv::Mat RGB, cv::Mat depthImg);
	
    // Desctructor double(1.0+
	~ImageVisualizerCV(){}
private:
    Config config;

};

#endif
