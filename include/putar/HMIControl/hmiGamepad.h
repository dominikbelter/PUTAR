#ifndef HMIGAMEPAD_H_INCLUDED
#define HMIGAMEPAD_H_INCLUDED

namespace putar {

    class HmiGamepad : public Hmi {
    public:

        /// overloaded constructor
        HmiGamepad() {}
        
        /// Updates pose
        virtual void updatePose(const Mat34& pose);
        
        /// Virtual descrutor
        virtual ~HmiGamepad() {}
}

#endif
