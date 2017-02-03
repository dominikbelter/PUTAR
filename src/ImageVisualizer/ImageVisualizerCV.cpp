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

    if (mask.empty()){
        std::cout<<"error while loading mask image"<<std::endl;
    }
    if (depthMask.empty()){
        std::cout<<"error while loading depth mask image"<<std::endl;
    }
    if (RGB.empty()){
        std::cout<<"error while loading RGB image"<<std::endl;
    }
    if (depthImg.empty()){
        std::cout<<"error while loading  RGB depth image"<<std::endl;
    }
    else{

        for(int k = 1; k<=RGB.channels(); k++)
        {
            for (int j = 0; j < RGB.cols; j++)
            {
                for(int i = 0; i < RGB.rows; i++)
                {
                    if(depthMask.at <uchar>(i, j) != 156 ){
                        if (double(double(depthImg.at<std::uint16_t>(i,j))/5000.0) >= double(1.0+double(depthMask.at <uchar>(i, j)/100.0))){
                             std::cout << double(double(depthImg.at<std::uint16_t>(i,j))/5000.0) << ", " << double(double(depthMask.at <uchar>(i, j)/100.0)) << ": ";
                            RGB.at<Pixel>(i,j).val[k] = mask.at<Pixel>(i,j).val[k];
                        }
                    }
                }
             }

        }

        imshow("Updated Image",RGB);

        cv::waitKey(0);
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


