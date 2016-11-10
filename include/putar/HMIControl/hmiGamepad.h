#ifndef _HMIGAMEPAD_H_
#define _HMIGAMEPAD_H_

#include "hmi.h"
#include "../Defs/defs.h"
#include "Utilities/observer.h"
#include <iostream>
#include <string>
#include "opencvCore.h"

namespace putar {
    Hmi* createMyHmiGamepad(void);
}

using namespace putar;


class HmiGamepad : public Hmi {
    public:

        // Pointer
        typedef std::unique_ptr<HmiGamepad> Ptr;

        // overloaded constructor
        HmiGamepad() {}
        
        // Updates pose
        void updatePose(const Mat34& pose);
        
        // Virtual descrutor
        ~HmiGamepad() {}
};

#endif
