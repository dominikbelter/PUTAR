#include "ImageVisualizer/imageVisualizerCV.h"
#include <memory>
#include <vec.h>

using namespace putar;
using namespace std;

ImageVisualizerCV::Ptr imageVisualizerCV;

ImageVisualizerCV::ImageVisualizerCV(std::string configFilename) : config(configFilename){

}


// returns Visualisation of PUTAR in 2D
cv::Mat ImageVisualizerCV::UpdateImage(cv::Mat mask, cv::Mat depthMask, cv::Mat RGB, cv::Mat depthImg)
{
    typedef cv::Vec<uchar,3> Pixel;

    if (!mask.empty() || !depthMask.empty() || !RGB.empty() || !depthImg.empty()){
        std::cout<<"error while loading image"<<std::endl;
    }
    else{
        std::cout<<"error while loading image"<<std::endl;


        for(int k = 1; k<=RGB.channels(); k++)
        {
            for (int j = 0; j < RGB.cols; j++)
            {
                for(int i = 0; i < RGB.rows; i++)
                {
                     if (depthImg.at <unsigned short>(i,j) >depthMask.at < unsigned short>(i, j)){
                        RGB.at<Pixel>(i,j).val[k] = mask.at<Pixel>(i,j).val[k];
                     }
                }
             }

        }

        imshow("Updated Image",RGB);

        cv::waitKey(5);
        return RGB;
    }
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


