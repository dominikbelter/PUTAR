#ifndef VIS3D_H_INCLUDED
#define VIS3D_H_INCLUDED

namespace putar {

    class Vis3d {
    public:

        /// overloaded constructor
        ObjLoader() {}
        
        /// Updates mesh
        virtual void updateMesh(const Object3D& mesh) const = 0;
        
        /// Updates cloud
        virtual void updateCloud(cv::Mat RGBD) = 0;

        /// Virtual descrutor
        virtual ~ObjLoader() {}
}

#endif
