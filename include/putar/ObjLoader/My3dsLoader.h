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
#include <sys/stat.h>

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

            tinyxml2::XMLElement * posXML = config.FirstChildElement( "My3dsLoader" )->FirstChildElement("parameters");

            this->fileName=posXML->Attribute("fileName");

            this->fileDIR=posXML->Attribute("fileDIR");
        }

        /// Name of the grabber
        const std::string& getName() const;

        /// Calculate length of the file
        long filelength(int f);

        /// Returns the current 2D image
        void loadObj();

        /// Grab image and/or point cloud
        void getMesh(obj_type &p_object) const;

        ///Attach visualizer
        void attachVisualizer(QGLVisualizer* visualizer);

        void computeMask(const Mat34 cameraPose,cv::Mat& mask);

    private:
        std::string fileName;
        std::string fileDIR;
        obj_type object;
        Mat34 poseVis;
};

#endif // MY_3DS_LOADER_H_INCLUDED
