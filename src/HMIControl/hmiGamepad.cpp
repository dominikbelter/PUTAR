#include "HMIControl/hmiGamepad.h"
#include <memory>
#include <stdexcept>
#include <chrono>
#include <thread>

using namespace putar;

HmiGamepad::Ptr hmiGamepad;

// overloaded constructor
HmiGamepad::HmiGamepad(void){

}

HmiGamepad::HmiGamepad(std::string configFilename) : config(configFilename){

}

void HmiGamepad::updatePose(const Mat34& pose){
    throw std::runtime_error("updatePose method is not implemented");
}

putar::Hmi* putar::createMyHmiGamepad(){
    hmiGamepad.reset(new HmiGamepad());
    return hmiGamepad.get();
}

putar::Hmi* putar::createMyHmiGamepad(std::string configFilename){
    hmiGamepad.reset(new HmiGamepad(configFilename));
    return hmiGamepad.get();
}
