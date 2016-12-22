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

// returns Visualisation of PUTAR in 2D
cv::Mat ImageVisualizerCV::UpdateImage(/*cv::Mat mask, cv::Mat depthMask, cv::Mat RGB, cv::Mat depthImg*/)
{
    cv::Mat mask, depthMask, RGB, depthImg;
    mask =cv::imread("/home/bw/PUTAR/PUTAR/resources/IMGVisualizer/rgb_00849.png");
    //depthMask = cv::imread("/home/bw/PUTAR/PUTAR/resources/IMGVisualizer/depth_00849.png", CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR | 0);
    //RGB = cv::imread("/home/bw/PUTAR/PUTAR/resources/IMGVisualizer/rgb_00752.png");
    //depthImg = cv::imread("/home/bw/PUTAR/PUTAR/resources/IMGVisualizer/depth_00752.png", CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR | 0);


   imshow("mask",mask);
   /*
   unsigned short dst, dst1;
    cv::imshow("Image", RGB);
    cv::imshow("Mask", mask);
    for (int i = 0; i < RGB.rows; i++){
        for (int j = 0; j < RGB.cols; j++){

            dst = depthMask.at < unsigned short>(i, j);
            dst1 = depthImg.at <unsigned short>(i,j);
            if (dst > dst1){
                RGB.at<uchar>(i,j) = mask.at<uchar>(i,j);

            }
        }

      }
      */
    imshow("Updated Image",RGB);
    return RGB;
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


