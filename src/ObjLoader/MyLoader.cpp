#include "ObjLoader/MyLoader.h"
#include <memory>
#include <stdexcept>
#include <chrono>
#include <thread>

using namespace putar;

/// A single instance of Kinect grabber
MyLoader::Ptr myLoader;

MyLoader::MyLoader(void) : ObjLoader("My Loader", TYPE_OBJ) {

}

const std::string& MyLoader::getName() const {
    return name;
}

/// Returns the current 2D image
void MyLoader::loadObj(std::string filename){
    std::cout << filename << "\n";
    throw std::runtime_error("LoadObj method is not implemented\n");
}

/// Grab image and/or point cloud
void MyLoader::getMesh(Object3D& mesh) const{
    std::cout << mesh.normals.size() << "\n";
    throw std::runtime_error("getMesh method is not implemented\n");
}

void MyLoader::computeMask(const Mat34 cameraPose, cv::Mat& mask){
    std::cout << mask.rows << "\n";
    throw std::runtime_error("compute Mask method is not implemented\n");
}

putar::ObjLoader* putar::createMyLoader(void) {
    myLoader.reset(new MyLoader());
    return myLoader.get();
}

putar::ObjLoader* putar::createMyLoader(std::string configFile) {
    myLoader.reset(new MyLoader(configFile));
    return myLoader.get();
}

