
#include "Defs/defs.h"
#include "ObjLoader/MyLoader.h"
#include "ObjLoader/My3dsLoader.h"
#include "ObjLoader/objLoader.h"
#include "ImageVisualizer/imageVisualizerCV.h"
#include "3rdParty/tinyXML/tinyxml2.h"
#include "Visualizer/Qvisualizer.h"
#include "HMIControl/hmiGamepad.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <QApplication>
#include <iostream>
#include <thread>

using namespace std;
using namespace putar;

//globals
GLuint elephant;
float elephantrot;
char ch='1';


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
            float x,y,z;
            fscanf(file, "%f %f %f\n", &vertex(0), &vertex(1), &vertex(2) );
            temp_vertices.push_back(vertex);
        }else if ( strcmp( lineHeader, "vt" ) == 0 ){
            Eigen::Vector2f uv;
            fscanf(file, "%f %f\n", &uv(0), &uv(1) );
            uv(1) = -uv(1); // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
            temp_uvs.push_back(uv);
        }else if ( strcmp( lineHeader, "vn" ) == 0 ){
            Eigen::Vector3f normal;
            fscanf(file, "%f %f %f\n", &normal(0), &normal(1), &normal(2) );
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

        std::cout <<"\n vertices: " <<mesh.vertices.size() << "\n";
         std::cout <<" uvs: " <<mesh.uvs.size() << "\n";

       std::cout <<"normals: " <<mesh.normals.size() << "\n";




    }
    fclose(file);
    return true;
    }


cv::Mat screenshot()
{
    int height=640;
    int width=840;
cv::Mat img(height, width, CV_8UC3);
glReadBuffer( GL_FRONT );
//use fast 4-byte alignment (default anyway) if possible
glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3) ? 1 : 4);

//set length of one complete row in destination data (doesn't need to equal img.cols)
glPixelStorei(GL_PACK_ROW_LENGTH, img.step/img.elemSize());

glReadPixels(0, 0, img.cols, img.rows, GL_BGR, GL_UNSIGNED_BYTE, img.data);
cv::Mat imgMask(height, width, CV_8UC3);
cv::flip(img, imgMask, 0);
 cv::imshow("MASK",imgMask);
 // cv::imshow("MASK",imgMask);
/*cv::Mat depthMask;

//get the depth Buffor
glEnable(GL_DEPTH_TEST);
glDepthMask(GL_TRUE);
glDepthFunc(GL_ALWAYS); // Change this to whatever kind of depth testing you want
glDepthRange(0.0f, 1.0f);

glReadPixels(0, 0, imgMask.cols, imgMask.rows, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, imgMask.data);
//cv::flip(imgMask, depthMask, 0);
depthMask = imgMask.clone();

for(int x=0; x<depthMask.cols; x++)
{
    for(int y =0; y<depthMask.rows; y++)
    {
        depthMask.at<uchar>(y,x) = depthMask.at<uchar>(y,x)*1000;
    }
}


cv::imshow("DEPTH",depthMask);
//bitwise_not ( imgMask, depthMask );
*/
return imgMask;

}





void loadObje(char *fname)
{
    FILE *fp;
    int read;
    GLfloat x, y, z;
    char ch;
    elephant=glGenLists(1);
    fp=fopen(fname,"r");
    if (!fp)
    {
        printf("can't open file %s\n", fname);
        exit(1);
    }
    glPointSize(2.0);
    glNewList(elephant, GL_COMPILE);
    {
        glPushMatrix();
        glBegin(GL_POINTS);
        while(!(feof(fp)))
        {
            read=fscanf(fp,"%c %f %f %f",&ch,&x,&y,&z);
            if(read==4&&ch=='v')
            {
                glVertex3f(x,y,z);
            }
        }
        glEnd();
    }
    glPopMatrix();
    glEndList();
    // screenshot();
    fclose(fp);
}
//wavefront .obj loader code ends here
//wavefront .obj loader code ends here
void reshape(int w,int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);
    //glOrtho(-25,25,-2,2,0.1,100);
    glMatrixMode(GL_MODELVIEW);
}
void drawElephant()
{
    glPushMatrix();
    glTranslatef(0,-5,-5);
    glColor3f(1.0,0.23,0.27);
    glScalef(1,1,1);
    glRotatef(elephantrot,0,1,0);
    glCallList(elephant);
    glPopMatrix();
    elephantrot=elephantrot+0.6;
    if(elephantrot>360)elephantrot=elephantrot-360;
}
void display(void)
{
   /* glEnable(GL_TEXTURE_2D); // This Enable the Texture mapping

    LoadBitmap("../../resources/Turret.bmp"); // The Function LoadBitmap()


    object.id_texture=num_texture;

    // If the last function returns -1 it means the file was not found so we exit from the program
    if (object.id_texture==-1)
    {
        printf("BMP not found\n");
        //MessageBox(NULL,"Image file: spaceshiptexture.bmp not found", "Zetadeck",MB_OK | MB_ICONERROR);
        exit (0);
    }*/



    //Wczytywanie modelu
    glClearColor (0.0,0.0,0.0,1.0);
     glShadeModel(GL_SMOOTH);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
   // drawElephant();

    glEnable(GL_DEPTH_TEST); // We enable the depth test (also called z buffer)
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)



       Mesh mesh;
       glTranslatef(0,-2,-4);
        glRotatef(20,0,1,0);
         glRotatef(20,1,0,0);
std::vector<float> vert;
gluLookAt(10, 10, 10,  0, 0, 0, 0, 1, 0);
//glBindTexture(GL_TEXTURE_2D, object.id_texture);
     loadOBJ("../../resources/Hulk.obj", mesh);
    //DrawCube();
       glColor3f(0,1,0);
     /*  for (int i = 0; i < mesh.vertices.size(); i += 3)
       {
           int t0 = i + 0;
           int t1 = i + 1;
           int t2 = i + 2;
           vert.push_back(mesh.vertices[t0][0]);
           vert.push_back(mesh.vertices[t0][1]);
           vert.push_back(mesh.vertices[t0][2]);
           vert.push_back(mesh.vertices[t1][0]);
           vert.push_back(mesh.vertices[t1][1]);
           vert.push_back(mesh.vertices[t1][2]);
           vert.push_back(mesh.vertices[t2][0]);
           vert.push_back(mesh.vertices[t2][1]);
           vert.push_back(mesh.vertices[t2][2]);

           // [...] Same for colors and texture coords.
       }
glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size());*/


    glBegin(GL_TRIANGLES); // grot na osi OX

   for (int i=0;i<=mesh.vertices.size();i=i+3)
{

       glTexCoord2f(mesh.uvs[i][0], mesh.uvs[i][1]);
       glNormal3f(mesh.normals[i][0], mesh.normals[i][1], mesh.normals[i][2]);
       glVertex3f(mesh.vertices[i][0], mesh.vertices[i][1], mesh.vertices[i][2]);
        glTexCoord2f(mesh.uvs[i+1][0], mesh.uvs[i+1][1]);
         glNormal3f(mesh.normals[i+1][0], mesh.normals[i+1][1], mesh.normals[i+1][2]);
        glVertex3f(mesh.vertices[i+1][0], mesh.vertices[i+1][1], mesh.vertices[i+1][2]);
        glTexCoord2f(mesh.uvs[i+2][0], mesh.uvs[i+2][1]);
         glNormal3f(mesh.normals[i+2][0], mesh.normals[i+2][1], mesh.normals[i+2][2]);
        glVertex3f(mesh.vertices[i+2][0], mesh.vertices[i+2][1], mesh.vertices[i+2][2]);

 }



  /* glVertex3f(-1.0f,-0.25f,0.0f);//triangle one first vertex
        glVertex3f(-0.5f,-0.25f,0.0f);//triangle one second vertex
        glVertex3f(-0.75f,0.25f,0.0f);//triangle one third vertex
        //drawing a new triangle
        glVertex3f(0.5f,-0.25f,0.0f);//triangle two first vertex
        glVertex3f(1.0f,-0.25f,0.0f);//triangle two second vertex
        glVertex3f(0.75f,0.25f,0.0f);//triangle two third vertex

/*
    glTranslatef(0,0,-5);
     glRotatef(20,0,1,0);
      glRotatef(20,1,0,0);
*/
   glEnd();
  glFlush(); // This force the execution of OpenGL commands
      screenshot();
    glutSwapBuffers(); //swap the buffers

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

        cv::Mat shot;

        Mesh mesh;



        glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
        glutInitWindowSize(800,450);
        glutInitWindowPosition(20,20);
        glutCreateWindow("ObjLoader");
        glutReshapeFunc(reshape);
        glutDisplayFunc(display);
        glutIdleFunc(display);
        //loadOBJ("../../resources/cube.obj", mesh);
        // DrawGLScene();
       // DrawCube();


       // loadObje("../../resources/Hulk.obj");//replace elepham.obj withp orsche.obj or radar.obj or any other .obj to display it
      //  shot= screenshot();
      //  cv::imshow("OKNO",shot);

        glutMainLoop();




std::cout << "Done\n";
}
catch (const std::exception& ex) {
std::cerr << ex.what() << std::endl;
return 1;
}

cv::waitKey();
return 0;
}
