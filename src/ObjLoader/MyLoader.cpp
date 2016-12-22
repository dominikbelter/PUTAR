#include "ObjLoader/MyLoader.h"
#include <memory>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <random>

using namespace putar;






/// A single instance of Kinect grabber
MyLoader::Ptr myLoader;

MyLoader::MyLoader(void) : ObjLoader("My Loader", TYPE_OBJ) {

}

const std::string& MyLoader::getName() const {
    return name;
}

/// Returns the current 2D image
void MyLoader::loadObj(){
    //std::cout << filename << "\n";
    //throw std::runtime_error("LoadObj method is not implemented\n");
}

/// Grab image and/or point cloud
void MyLoader::getMesh(obj_type &p_object) const{
    //std::cout << mesh.normals.size() << "\n";
    throw std::runtime_error("getMesh method is not implemented\n");
}

/// Attach visualizer
void MyLoader::attachVisualizer(QGLVisualizer* visualizer) {
    attach(visualizer);
}

void MyLoader:: LoadBitmap(){
        throw std::runtime_error("LoadBitmap method is not implemented\n");
}

void MyLoader::computeMask(const Mat34 cameraPose, cv::Mat& mask){
    std::random_device rd;
    std::mt19937 e2(rd());
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0,0.2);
    Mat34 objPose(Mat34::Identity());
    for (int i=0;i<3;i++)
        objPose(i,3) = distribution(e2);
    notify(objPose);
}

putar::ObjLoader* putar::createMyLoader(void) {
    myLoader.reset(new MyLoader());
    return myLoader.get();
}

putar::ObjLoader* putar::createMyLoader(std::string configFile) {
    myLoader.reset(new MyLoader(configFile));
    return myLoader.get();
}

