#include "ObjLoader/My3dsLoader.h"
#include <memory>
#include <stdexcept>
#include <chrono>
#include <thread>

using namespace putar;

/// A single instance of Kinect grabber
My3dsLoader::Ptr my3dsLoader;

My3dsLoader::My3dsLoader(void) : ObjLoader("My Loader", TYPE_3DS) {

}

const std::string& My3dsLoader::getName() const {
    return name;
}

/// Returns the current 2D image
void My3dsLoader::loadObj(std::string filename){
    std::cout << filename << "\n";
    throw std::runtime_error("LoadObj method is not implemented\n");
}

/// Grab image and/or point cloud
void My3dsLoader::getMesh(Object3D& mesh) const{
    std::cout << mesh.normals.size() << "\n";
    throw std::runtime_error("getMesh method is not implemented\n");
}

void My3dsLoader::computeMask(const Mat34 cameraPose,cv::Mat& mask){
    std::cout << mask.rows << "\n";
    throw std::runtime_error("compute Mask method is not implemented\n");
}

putar::ObjLoader* putar::createMy3dsLoader(void) {
    my3dsLoader.reset(new My3dsLoader());
    return my3dsLoader.get();
}

putar::ObjLoader* putar::createMy3dsLoader(std::string configFile) {
    my3dsLoader.reset(new My3dsLoader(configFile));
    return my3dsLoader.get();
}

