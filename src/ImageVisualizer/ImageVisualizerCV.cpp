#include "ImageVisualizer/imageVisualizerCV.h"
#include <memory>

using namespace putar;

ImageVisualizerCV::Ptr imageVisualizerCV;

ImageVisualizerCV::ImageVisualizerCV(std::string configFilename) : config(configFilename){


}

// Updates the cloud
void ImageVisualizerCV::updateFrame(cv::Mat RGB, cv::Mat depthImg){
    //throw std::runtime_error("updateClouad method is not implemented");
}

// Updates the image by adding the mask
void ImageVisualizerCV::updateMask(cv::Mat mask, cv::Mat depthMask){
    //throw std::runtime_error("updateImage method is not implemented");
}

putar::ImageVisualizer* putar::createMyImageVisualizer() {
    imageVisualizerCV.reset(new ImageVisualizerCV());
    return imageVisualizerCV.get();
}

putar::ImageVisualizer* putar::createMyImageVisualizer(std::string configFilename)
{
    imageVisualizerCV.reset(new ImageVisualizerCV(configFilename));
    return imageVisualizerCV.get();

}
