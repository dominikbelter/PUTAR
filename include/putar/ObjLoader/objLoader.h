/** @file objLoader.h
 *
 * Obj Loade interface
 *
 */

#ifndef _OBJLOADER_H_
#define _OBJLOADER_H_

#include "../Defs/defs.h"
#include "Utilities/observerAR.h"
#include "Visualizer/Qvisualizer.h"
#include <iostream>
#include <string>
#include <vector>
#include <mutex>

namespace putar {
    /// OBJLOADER interface
    class ObjLoader {
        public:

            /// ObjLoader type
            enum Type {
                    /// OBJ camera
                    TYPE_OBJ,
                    /// 3ds
                    TYPE_3DS
            };


            /// overloaded constructor
            ObjLoader(const std::string _name, Type _type) : name(_name), type(_type) {}

            /// Name of the grabber
            virtual const std::string& getName() const = 0;

            /// Returns the current 2D image
            virtual void loadObj() = 0;

            /// Grab image and/or point cloud
            virtual void getMesh(obj_type &p_object) const = 0;

            virtual void computeMask(const Mat34& cameraPose, const Mat34& objectPose, cv::Mat& mask, cv::Mat& depthMask) = 0;

            /// Attach visualizer
            virtual void attachVisualizer(QGLVisualizer* visualizer) = 0;

            ///Texture loader
            virtual void LoadBitmap() = 0;

            /// Virtual descrutor
            virtual ~ObjLoader() {}

        protected:
            /// Grabber name
            const std::string name;

            /// Grabber type
            Type type;

    };
}

#endif // _OBJLOADER_H_
