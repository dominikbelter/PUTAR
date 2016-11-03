#ifndef HMIGAMEPAD_H_INCLUDED
#define HMIGAMEPAD_H_INCLUDED

#include "hmi.h"

namespace putar {

    class HmiGamepad : public Hmi {
    public:

        /// Pointer
        typedef std::unique_ptr<HmiGamepad> Ptr;

        /// overloaded constructor
        HmiGamepad() {}
        
        /// Updates pose
        virtual void updatePose(const Mat34& pose);
        
        /// Virtual descrutor
        virtual ~HmiGamepad() {}
};

#endif
