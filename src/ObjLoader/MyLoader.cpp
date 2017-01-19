#include "ObjLoader/MyLoader.h"
#include <memory>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <random>

using namespace putar;






/// A single instance of Kinect grabber
MyLoader::Ptr myLoader;

MyLoader::MyLoader(void) : ObjLoader("My Loader", TYPE_OBJ) {

}

const std::string& MyLoader::getName() const {
    return name;
}

/// Returns the current 2D image
void MyLoader::loadObj(){
/*
    Mesh mesh;

    //DB const char path="../../resources/cube.obj";
/*

    printf("Loading OBJ file %s...\n", path);
*/
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<Eigen::Vector3f> temp_vertices;
    std::vector<Eigen::Vector2f> temp_uvs;
    std::vector<Eigen::Vector3f> temp_normals;


    /*DB FILE* file = fopen(path, "r");
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

return mesh;
*/


    //std::cout << filename << "\n";
    //throw std::runtime_error("LoadObj method is not implemented\n");
}

/// Grab image and/or point cloud
void MyLoader::getMesh(obj_type &p_object) const{
/*
    p_object=mesh;
     std::cout <<"\n vertices: " <<mesh.vertices.size() << "\n";
      std::cout <<" uvs: " <<mesh.uvs.size() << "\n";

    std::cout <<"normals: " <<mesh.normals.size() << "\n";*/

   // throw std::runtime_error("getMesh method is not implemented\n");
}

/// Attach visualizer
void MyLoader::attachVisualizer(QGLVisualizer* visualizer) {
    attach(visualizer);
}

void MyLoader:: LoadBitmap(){

/*
    const char imagepath="../../resources/kamien.bmp";


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
*/
    // ... nice trilinear filtering.
    /*
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);*/

    // Return the ID of the texture we just created
    //return textureID;

       // throw std::runtime_error("LoadBitmap method is not implemented\n");
}

void MyLoader::computeMask(const Mat34& cameraPose, const Mat34& objectPose, cv::Mat& mask, cv::Mat& depthMask){
    std::random_device rd;
    std::mt19937 e2(rd());
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0,0.2);
    Mat34 objPose(Mat34::Identity());
    for (int i=0;i<3;i++)
        objPose(i,3) = distribution(e2);
    notify(objPose);

/*
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(0,0);
    glClearColor(0, 0,0,0);
    glutCreateWindow("MASKOWNICA");

    int height=640;
    int width=840;
cv::Mat img(height, width, CV_8UC3);

//use fast 4-byte alignment (default anyway) if possible
glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3) ? 1 : 4);

//set length of one complete row in destination data (doesn't need to equal img.cols)
glPixelStorei(GL_PACK_ROW_LENGTH, img.step/img.elemSize());

glReadPixels(0, 0, img.cols, img.rows, GL_BGR, GL_UNSIGNED_BYTE, img.data);
cv::Mat mask;
cv::flip(img, mask, 0);
cv::imshow("MASKA",mask);

*/



}

putar::ObjLoader* putar::createMyLoader(void) {
    myLoader.reset(new MyLoader());
    return myLoader.get();
}

putar::ObjLoader* putar::createMyLoader(std::string configFile) {
    myLoader.reset(new MyLoader(configFile));
    return myLoader.get();
}

