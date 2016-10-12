/** @file defs.h
*
* Simulator definitions
*
*/

#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include <cstdint>
#include <vector>
#include <memory>
#include <cmath>
#include "opencvCore.h"
#include "eigen3.h"

/// putslam name space
namespace putar {

    /// 3 element vector class
    typedef Eigen::Translation<double,3> Vec3;

    /// Matrix representation of SO(3) group of rotations
    typedef Eigen::Matrix<double,3,3> Mat33;

    /// Quaternion representation of SO(3) group of rotations
    typedef Eigen::Quaternion<double> Quaternion;

	/// Homogeneous representation of SE(3) rigid body transformations
    typedef Eigen::Transform<double, 3, Eigen::Affine> Mat34;

    /// Image
    typedef cv::Mat Image;

    /// 3D object
    class Object3D {
        public:
            /// set of nodes
            typedef std::vector<Object3D> Seq;

            /// vertices
            std::vector<Vec3> vertices;
            /// texture coordinates
            std::vector<Vec3> uvs;
            /// normal vectors
            std::vector<Vec3> normals;
    };
}

#endif // DEFS_H_INCLUDED
