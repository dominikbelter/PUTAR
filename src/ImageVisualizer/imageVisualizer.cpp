#include "ImageVisualizer/imageVisualizer.h"
#include <memory>

using namespace putar;

ImageVisualizer::Ptr imageVisualizer;

virtual void updateCloud(cv::Mat RGBD){
	throw std::runtime_error("updateClouad method is not implemented");
}

virtual void updateImage(cv::Mat Mask){
	throw std::runtime_error("updateImage method is not implemented");
}