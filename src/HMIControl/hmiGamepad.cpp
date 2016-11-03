#include "HMIControl/hmiGamepad.h"
#include <memory>
#include <stdexcept>
#include <chrono>
#include <thread>

using namespace putar;
using namespace std;

HmiGamepad::Ptr HMIGamepad;

virtual void updatePose(const Mat34& pose){
        throw runtime_error("updatePose method is not implemented");
}

