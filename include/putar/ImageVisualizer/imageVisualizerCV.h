#ifndef _IMAGEVISUALIZERCV_H_
#define _IMAGEVISUALIZERCV_H_

#include "../Defs/defs.h"
#include "Utilities/observer.h"
#include <iostream>
#include <string>
#include "ImageVisualizer.h"


namespace putar{
	
	//create ImageVisualizer object
	ImageVisualizer* myImageVisualizer(void);
}

using namespace putar;

// ImageVisualizerCV implementation
class ImageVisualizerCV: public ImageVisualizer{
	public:
	
	// Pointer
	typedef unique_ptr<ImageVisualizerCV> Ptr;
	
	// Constructor
	ImageVisualizerCV(void){}
	
	// update the Cloud
	void updateCloud(cv::Mat RGBD)
	
	// update Mask
	void updateImage(cv::Mat Mask)
	
	// Desctructor
	~ImageVisualizerCV(){}
}

#endif
