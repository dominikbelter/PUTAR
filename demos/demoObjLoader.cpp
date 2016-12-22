
#include "Defs/defs.h"
#include "ObjLoader/MyLoader.h"
#include "ObjLoader/My3dsLoader.h"
#include "ImageVisualizer/imageVisualizerCV.h"
#include "3rdParty/tinyXML/tinyxml2.h"
#include "Visualizer/Qvisualizer.h"
#include "HMIControl/hmiGamepad.h"
#include <GL/glut.h>
#include <QApplication>
#include <iostream>
#include <thread>

using namespace std;
using namespace putar;

bool loadOBJ(const char* path, Mesh& mesh)
{
    printf("Loading OBJ file %s...\n", path);

    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<Eigen::Vector3f> temp_vertices;
    std::vector<Eigen::Vector2f> temp_uvs;
    std::vector<Eigen::Vector3f> temp_normals;


    FILE* file = fopen(path, "r");
    if( file == NULL ){
        printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
        getchar();
        return false;
    }

    while( 1 ){

        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        // else : parse lineHeader

        if ( strcmp( lineHeader, "v" ) == 0 ){
            Eigen::Vector3f vertex;
           //WUNUSED? BŁĄD? float x,y,z;
           //WUNUSED? BŁĄD? fscanf(file, "%f %f %f\n", &vertex(0), &vertex(1), &vertex(2) );
            temp_vertices.push_back(vertex);
        }else if ( strcmp( lineHeader, "vt" ) == 0 ){
            Eigen::Vector2f uv;
           //WUNUSED? BŁĄD? fscanf(file, "%f %f\n", &uv(0), &uv(1) );
            uv(1) = -uv(1); // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
            temp_uvs.push_back(uv);
        }else if ( strcmp( lineHeader, "vn" ) == 0 ){
            Eigen::Vector3f normal;
           //WUNUSED? BŁĄD? fscanf(file, "%f %f %f\n", &normal(0), &normal(1), &normal(2) );
            temp_normals.push_back(normal);
        }else if ( strcmp( lineHeader, "f" ) == 0 ){
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            if (matches != 9){
                printf("File can't be read by our simple parser :-( Try exporting with other options\n");
                fclose(file);
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices    .push_back(uvIndex[0]);
            uvIndices    .push_back(uvIndex[1]);
            uvIndices    .push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);




        }else{
            // Probably a comment, eat up the rest of the line
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }

    }


// For each vertex of each triangle
for( unsigned int i=0; i<vertexIndices.size(); i++ ){

    // Get the indices of its attributes
    unsigned int vertexIndex = vertexIndices[i];
    unsigned int uvIndex = uvIndices[i];
    unsigned int normalIndex = normalIndices[i];

    // Get the attributes thanks to the index
    Eigen::Vector3f vertex = temp_vertices[ vertexIndex-1 ];
    Eigen::Vector2f uv = temp_uvs[ uvIndex-1 ];
    Eigen::Vector3f normal = temp_normals[ normalIndex-1 ];

    // Put the attributes in buffers
    mesh.vertices.push_back(vertex);
    mesh.uvs.push_back(uv);
    mesh.normals.push_back(normal);

}
fclose(file);
return true;
}

void processPUTAR(ObjLoader* objLoader, ImageVisualizer* visu2D){
    while(1){
        Mat34 camPose;
        //slam.getPose(camPose);
        cv::Mat rgbImg;
        cv::Mat depthImg;
        //slam.getFrame(rgbImg, depthImg);
        cv::Mat mask;
        objLoader->computeMask(camPose, mask);

        visu2D->updateMask(mask, mask);
        visu2D->updateFrame(rgbImg,depthImg);
    }

}



int main(int argc, char** argv)
{

    try {
        tinyxml2::XMLDocument config;
        config.LoadFile("../../resources/configGlobal.xml");
        if (config.ErrorID())
            std::cout << "unable to load config file.\n";
        std::string visualizerConfig(config.FirstChildElement("Configuration")->FirstChildElement("Visualizer")->FirstChildElement("config")->GetText());
        std::string visualizerType(config.FirstChildElement("Configuration")->FirstChildElement("Visualizer")->FirstChildElement("type")->GetText());
        std::string Loader3dsConfig(config.FirstChildElement("Configuration")->FirstChildElement("Loader3ds")->FirstChildElement("config")->GetText());
         std::string LoaderObjConfig(config.FirstChildElement("Configuration")->FirstChildElement("LoaderObj")->FirstChildElement("config")->GetText());

        QApplication application(argc,argv);
        setlocale(LC_NUMERIC,"C");
        glutInit(&argc, argv);

        QGLVisualizer visu(visualizerConfig);
        visu.setWindowTitle("Simulator viewer");
        visu.show();

        Mesh mesh;
        loadOBJ("/media/user/901E247A1E245B8A/LINUX/PROJEKT/PUTAR/resources/cube.obj", mesh);
         cout<<"Object loadin process ended"<<endl;
//ObjLoader* objLoader;// = putar::createMyLoader();
//if (1)
//    objLoader = putar::createMyLoader();
//else{
//    objLoader = putar::createMy3dsLoader(Loader3dsConfig);
//}
//objLoader->attachVisualizer(&visu);

//objLoader->loadObj("stone.obj");

//------------------CREATING MASK----------------------

         glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
         glutInitWindowSize(640,480);
         glutInitWindowPosition(0,0);
         glutCreateWindow("MASKOWNICA");

 cv::waitKey(0);


std::cout << "Done\n";
}
catch (const std::exception& ex) {
std::cerr << ex.what() << std::endl;
return 1;
}

return 0;
}
