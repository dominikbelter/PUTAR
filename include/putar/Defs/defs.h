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

    #define MAX_VERTICES 8000 // Max number of vertices (for each object)
    #define MAX_POLYGONS 8000 // Max number of polygons (for each object)

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


    // Our vertex type
    typedef struct{
        float x,y,z;
    }vertex_type;

    // The polygon (triangle), 3 numbers that aim 3 vertices
    typedef struct{
        int a,b,c;
    }polygon_type;

    // The mapcoord type, 2 texture coordinates for each vertex
    typedef struct{
        float u,v;
    }mapcoord_type;

    // The object type
    typedef struct {
        char name[20];

        int vertices_qty;
        int polygons_qty;

        vertex_type vertex[MAX_VERTICES];
        polygon_type polygon[MAX_POLYGONS];
        mapcoord_type mapcoord[MAX_VERTICES];
        int id_texture;
    } obj_type, *obj_type_ptr;


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

    /// Mesh
    class Mesh {
        public:
            /// vertices
            std::vector<Eigen::Vector3f> vertices;
            /// texture coordinates
            std::vector<Eigen::Vector2f>  uvs;
            /// normal vectors
            std::vector<Eigen::Vector3f>  normals;
    };
}

#endif // DEFS_H_INCLUDED
