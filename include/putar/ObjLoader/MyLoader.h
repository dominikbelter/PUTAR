/** @file MyLoader.h
 *
 * implementation - My Loader
 *
 */
///Adam Kaźmierczak & Arkadiusz Bomba

#ifndef MY_LOADER_H_INCLUDED
#define MY_LOADER_H_INCLUDED

#include "objLoader.h"
#include "../../3rdParty/tinyXML/tinyxml2.h"
#include <iostream>
#include <memory>

namespace putar {
    /// create a single ObjLoader
    ObjLoader* createMyLoader(void);
    ObjLoader* createMyLoader(std::string configFile);
}

using namespace putar;

/// ObjLoader implementation
class MyLoader : public ObjLoader, public Subject {
    public:

        /// Pointer
        typedef std::unique_ptr<MyLoader> Ptr;

        /// Construction
        MyLoader(void);

        /// Construction
        MyLoader(std::string configFilename) : ObjLoader("MyLoader", TYPE_OBJ){
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

            tinyxml2::XMLElement * posXML = config.FirstChildElement( "MyLoader" )->FirstChildElement("parameters");
            std::string fileName;
            fileName=posXML->Attribute("fileName");

            std::cout<<"Loaded obj config file. FileName: " <<fileName <<std::endl;



        }

        /// Name of the grabber
        const std::string& getName() const;

        /// Returns the current 2D image
        void loadObj();

        /// Grab image and/or point cloud
        void getMesh(obj_type &p_object) const;

        ///Attach visualizer
        void attachVisualizer(QGLVisualizer* visualizer);

        void computeMask(const Mat34& cameraPose, const Mat34& objectPose, cv::Mat& mask, cv::Mat& depthMask);

        ///Texture loader
        void LoadBitmap();

    private:
        Mat34 poseVis;
};

#endif // MY_LOADER_H_INCLUDED
