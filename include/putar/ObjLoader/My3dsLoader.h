/** @file My3dsLoader.h
 *
 * implementation - My 3DS Loader
 *
 */

#ifndef MY_3DS_LOADER_H_INCLUDED
#define MY_3DS_LOADER_H_INCLUDED

#include "objLoader.h"
#include "../../3rdParty/tinyXML/tinyxml2.h"
#include <iostream>
#include <memory>

namespace putar {
    /// create a single ObjLoader
    ObjLoader* createMy3dsLoader(void);
    ObjLoader* createMy3dsLoader(std::string configFile);
}

using namespace putar;

/// ObjLoader implementation
class My3dsLoader : public ObjLoader, public Subject {
    public:

        /// Pointer
        typedef std::unique_ptr<My3dsLoader> Ptr;

        /// Construction
        My3dsLoader(void);

        /// Construction
        My3dsLoader(std::string configFilename) : ObjLoader("My3dsLoader", TYPE_3DS){
            tinyxml2::XMLDocument config;
            std::string filename = "../../resources/" + configFilename;
            config.LoadFile(filename.c_str());
            if (config.ErrorID())
                std::cout << "unable to load ObjLoader config file.\n";
            /*tinyxml2::XMLElement * posXML = config.FirstChildElement( "pose" );
            double query[4];
            posXML->QueryDoubleAttribute("qw", &query[0]); posXML->QueryDoubleAttribute("qx", &query[1]); posXML->QueryDoubleAttribute("qy", &query[2]); posXML->QueryDoubleAttribute("qz", &query[3]);
            double queryPos[4];
            posXML->QueryDoubleAttribute("x", &queryPos[0]); posXML->QueryDoubleAttribute("y", &queryPos[1]); posXML->QueryDoubleAttribute("z", &queryPos[2]);
            pose = Quaternion (query[0], query[1], query[2], query[3])*Vec3(queryPos[0], queryPos[1], queryPos[2]);
            */
        }

        /// Name of the grabber
        const std::string& getName() const;

        /// Returns the current 2D image
        void loadObj(std::string filename);

        /// Grab image and/or point cloud
        void getMesh(Object3D& mesh) const;

        ///Attach visualizer
        void attachVisualizer(QGLVisualizer* visualizer);

        void computeMask(const Mat34 cameraPose,cv::Mat& mask);

    private:
        Mat34 poseVis;
};

#endif // MY_3DS_LOADER_H_INCLUDED
