#ifndef HMIGAMEPAD_H_INCLUDED
#define HMIGAMEPAD_H_INCLUDED

namespace putar {

    class HmiGamepad {
    public:

        /// overloaded constructor
        HmiGamepad() {}
        
        /// Updates pose
        virtual void updatePose(const Object3D& pose) const = 0;
        
        /// Virtual descrutor
        virtual ~HmiGamepad() {}
}

#endif
