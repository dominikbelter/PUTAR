
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
        //DB objLoader->computeMask(camPose, mask);

        //visu2D->updateMask(mask, mask);
        //visu2D->updateFrame(rgbImg,depthImg);
    }

}

GLuint loadBMP_custom(const char * imagepath)
{

    printf("Reading image %s\n", imagepath);

    // Data read from the header of the BMP file
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int imageSize;
    unsigned int width, height;
    // Actual RGB data
    unsigned char * data;

    // Open the file
    FILE * file = fopen(imagepath,"rb");
    if (!file)							    {printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); return 0;}

    // Read the header, i.e. the 54 first bytes

    // If less than 54 bytes are read, problem
    if ( fread(header, 1, 54, file)!=54 ){
        printf("Not a correct BMP file\n");
        fclose(file);
        return 0;
    }
    // A BMP files always begins with "BM"
    if ( header[0]!='B' || header[1]!='M' ){
        printf("Not a correct BMP file\n");
        fclose(file);
        return 0;
    }
    std::cout<<*(int*)&(header[0x1C]);
    // Make sure this is a 24bpp file
    if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file\n");    fclose(file); return 0;}
    if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file\n");    fclose(file); return 0;}

    // Read the information about the image
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)      dataPos=54; // The BMP header is done that way

    // Create a buffer
    data = new unsigned char [imageSize];

    // Read the actual data from the file into the buffer
    fread(data,1,imageSize,file);

    // Everything is in memory now, the file wan be closed
    fclose (file);

    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    // OpenGL has now copied the data. Free our own version
    delete [] data;
    fclose(file);

    // Poor filtering, or ...
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // ... nice trilinear filtering.
    /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);*/

    // Return the ID of the texture we just created
    return textureID;
}

cv::Mat screenshot()
{
    int height=640;
    int width=840;
cv::Mat img(height, width, CV_8UC3);

//use fast 4-byte alignment (default anyway) if possible
glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3) ? 1 : 4);

//set length of one complete row in destination data (doesn't need to equal img.cols)
glPixelStorei(GL_PACK_ROW_LENGTH, img.step/img.elemSize());

glReadPixels(0, 0, img.cols, img.rows, GL_BGR, GL_UNSIGNED_BYTE, img.data);
cv::Mat flipped;
cv::flip(img, flipped, 0);

return flipped;

}


void lookAt_Mat34(cv::Mat Poz)
{
    //z Mat34 Poz -> camera


    //na koncu
    //gluLookAt(camera.x, camera.y, camera.z,  lookat.x, lookat.y, lookat.z, 0, 1, 0);

}

void Sterowanie(double x,double y,double z,double skala,double alfa_x,double alfa_y,double alfa_z )
{
    // 1. skalowanie
    glScaled(skala,skala,skala);

    // 2.rotacje
    glRotated(alfa_z,0,0,1);
    glRotated(alfa_y,0,1,0);
    glRotated(alfa_x,1,0,0);

    //  3. przesuwanie
    glTranslated(x,y,z);

}

double odleglosc (double x,double y,double z, double KameraX, double KameraY, double KameraZ)
{

    return sqrt(pow((x-KameraX),2)+pow((y-KameraY),2)+pow((z-KameraZ),2));
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
        //tworzenie czarnego tła
        glClearColor(0, 0,0,0);

        loadOBJ("../../resources/cube.obj", mesh);
        //loadOBJ("/media/user/901E247A1E245B8A/LINUX/PROJEKT/PUTAR/resources/cube.obj", mesh);
        //GLuint numTex;
        //numTex = loadBMP_custom("../../resources/kamien.bmp");


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

         glShadeModel(GL_SMOOTH); // Type of shading for the polygons

         // Viewport transformation
         glViewport(0,0,840,640);

         // Projection transformation
         glMatrixMode(GL_PROJECTION); // Specifies which matrix stack is the target for matrix operations
         glLoadIdentity(); // We initialize the projection matrix as identity
         gluPerspective(45.0f,(GLfloat)840/(GLfloat)640,10.0f,10000.0f); // We define the "viewing volume"

         glEnable(GL_DEPTH_TEST); // We enable the depth test (also called z buffer)
         glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)

         glEnable(GL_TEXTURE_2D); // This Enable the Texture mapping


         glFlush(); // This force the execution of OpenGL commands



         glutCreateWindow("MASKOWNICA");

        cv::Mat shot= screenshot();
        cv::imshow("OKNO",shot);

         //glutMainLoop();
         //cin.get(); //zatrzymanie

std::cout << "Done\n";
}
catch (const std::exception& ex) {
std::cerr << ex.what() << std::endl;
return 1;
}

cv::waitKey();
return 0;
}
