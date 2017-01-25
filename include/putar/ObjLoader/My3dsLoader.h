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
class My3dsLoader : public ObjLoader, public SubjectAR {
    public:

        /// Pointer
        typedef std::unique_ptr<My3dsLoader> Ptr;

        /**
         * @brief My3dsLoader constructor
         */
        My3dsLoader(void);

        /**
         * @brief My3dsLoader overloaded constructor
         * @param configFilename
         */
        My3dsLoader(std::string configFilename) : ObjLoader("My3dsLoader", TYPE_3DS){
            tinyxml2::XMLDocument config;
            std::string filename = "../../resources/" + configFilename;
            config.LoadFile(filename.c_str());
            if (config.ErrorID())
                std::cout << "unable to load ObjLoader config file.\n";

            tinyxml2::XMLElement * posXML = config.FirstChildElement( "My3dsLoader" )->FirstChildElement("file3ds");
            this->fileName=posXML->Attribute("fileName");
            this->fileDIR=posXML->Attribute("fileDIR");

            posXML = config.FirstChildElement( "My3dsLoader" )->FirstChildElement("bitMap");
            this->bitMapName=posXML->Attribute("bitMapName");
            this->bitMapDIR=posXML->Attribute("bitMapDIR");

            this->num_texture=-1;
        }


        /**
        * @brief Load 3ds file. Set Path and FileName in configFile
        */
        void loadObj();

        /**
         * @brief Grab image and/or point cloud
         * @param p_object
         */
        void getMesh(obj_type &p_object) const;

        /**
         * @brief Attach Visualizer
         * @param visualizer
         */
        void attachVisualizer(QGLVisualizer* visualizer);

        /**
         * @brief Compute 2D RGB and Depth masks
         * @param cameraPose
         * @param objectPose
         * @param mask
         * @param depthMask
         */
        void computeMask(const Mat34& cameraPose, const Mat34& objectPose, cv::Mat& mask, cv::Mat& depthMask);


    private:
        /// Name of the grabber
        const std::string& getName() const;

        /// Calculate length of the file
        long filelength(int f);

        ///Texture loader
        void LoadBitmap();

        std::string fileName;
        std::string fileDIR;
        std::string bitMapName;
        std::string bitMapDIR;
        obj_type object;
        Mat34 poseVis;
        int screen_width=640;
        int screen_height=480;
        int num_texture;
};

#endif // MY_3DS_LOADER_H_INCLUDED
