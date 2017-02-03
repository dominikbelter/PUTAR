#include "Utilities/offlineSLAM.h"
#include <fstream>
#include <iomanip>

OfflineSLAM::OfflineSLAM(std::string configFile) : config(configFile), frameNo(0) {
    loadGT();
}

/// load ground truth trajectory
void OfflineSLAM::loadGT(void){
    std::ifstream infile("../../resources/" + config.groundTruth);
    putar::Vec3 pos;
    putar::Quaternion quat;
    int rowNo;
    while (infile >> rowNo >> pos.x() >> pos.y() >> pos.z() >> quat.x() >> quat.y() >> quat.z() >> quat.w()){
        putar::Mat34 cam;
        cam.matrix().block<3,3>(0,0) = quat.matrix();
        cam.matrix().block<3,1>(0,3) = pos.vector();
        groundtruth.push_back(cam);
        // process pair (a,b)
    }
}

///get frame
void OfflineSLAM::getFrame(cv::Mat& rgb, cv::Mat& depth){
    std::ostringstream rgbFile;
    rgbFile << config.dataset << "rgb_" << std::setfill('0') << std::setw(5)<< frameNo << ".png";
    std::ostringstream depthFile;
    depthFile << config.dataset << "depth_" << std::setfill('0') << std::setw(5)<< frameNo << ".png";
    rgb = cv::imread(rgbFile.str());
    depth = cv::imread(depthFile.str(),CV_LOAD_IMAGE_ANYDEPTH);
}

///get current pose
void OfflineSLAM::getCurrentPose(putar::Mat34& pose){
    pose = groundtruth[frameNo];
    if (frameNo<int(groundtruth.size()-1))
        frameNo++;
}
