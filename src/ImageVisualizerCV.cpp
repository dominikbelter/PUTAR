#include "ImageVisualizer/imageVisualizerCV.h"
#include <memory>

using namespace putar;

ImageVisualizerCV::Ptr imageVisualizerCV;

// Updates the cloud
void updateCloud(cv::Mat RGBD){
	throw std::runtime_error("updateClouad method is not implemented");
}

// Updates the image by adding the mask
void updateImage(cv::Mat Mask){
	throw std::runtime_error("updateImage method is not implemented");
}
