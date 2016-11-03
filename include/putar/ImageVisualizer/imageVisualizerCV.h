#ifndef _IMAGEVISUALIZERCV_H_
#define _IMAGEVISUALIZERCV_H_

#include "../Defs/defs.h"
#include "Utilities/observer.h"
#include <iostream>
#include <string>
#include "opencvCore.h"
#include "imageVisualizer.h"


namespace putar{
	
	//create ImageVisualizer object
    ImageVisualizer* createMyImageVisualizer(void);
}

using namespace putar;

// ImageVisualizerCV implementation
class ImageVisualizerCV: public ImageVisualizer{
	public:
	
	// Pointer
    typedef std::unique_ptr<ImageVisualizerCV> Ptr;
	
	// Constructor
	ImageVisualizerCV(void){}
	
	// update the Cloud
    void updateFrame(cv::Mat RGBD, cv::Mat depthImg);
	
	// update Mask
    void updateMask(cv::Mat Mask);
	
	// Desctructor
	~ImageVisualizerCV(){}
};

#endif
