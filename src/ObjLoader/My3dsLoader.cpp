#include "ObjLoader/My3dsLoader.h"
#include <GL/glu.h>
#include <memory>
#include <stdexcept>
#include <chrono>
#include <thread>
#include <GL/glut.h>
#include <GL/freeglut.h>

using namespace putar;



/// A single instance of 3ds Loader grabber
My3dsLoader::Ptr my3dsLoader;

My3dsLoader::My3dsLoader(void) : ObjLoader("My 3dsLoader", TYPE_3DS) {

}

const std::string& My3dsLoader::getName() const {
    return name;
}

long My3dsLoader::filelength(int f)
{
    struct stat buf;

    fstat(f, &buf);

    return(buf.st_size);
}

/// Load 3ds file
void My3dsLoader::loadObj()
{
    obj_type_ptr p_object(&object);
    //&object;
    std::string path = fileDIR + fileName;

    const char* p_filename = path.c_str();
    int i; //Index variable

    FILE *l_file; //File pointer

    unsigned short l_chunk_id; //Chunk identifier
    unsigned int l_chunk_lenght; //Chunk lenght

    unsigned char l_char; //Char variable
    unsigned short l_qty; //Number of elements in each chunk

    unsigned short l_face_flags; //Flag that stores some face information

    if ((l_file=fopen (p_filename, "rb"))== NULL) exit(0); //Open the file

    while (ftell (l_file) < filelength (fileno (l_file))) //Loop to scan the whole file
    //while(!EOF)
    {
        //getch(); //Insert this command for debug (to wait for keypress for each chuck reading)

        fread (&l_chunk_id, 2, 1, l_file); //Read the chunk header
        //printf("ChunkID: %x\n",l_chunk_id);
        fread (&l_chunk_lenght, 4, 1, l_file); //Read the lenght of the chunk
        //printf("ChunkLenght: %x\n",l_chunk_lenght);

        switch (l_chunk_id)
        {
            //----------------- MAIN3DS -----------------
            // Description: Main chunk, contains all the other chunks
            // Chunk ID: 4d4d
            // Chunk Lenght: 0 + sub chunks
            //-------------------------------------------
            case 0x4d4d:
            break;

            //----------------- EDIT3DS -----------------
            // Description: 3D Editor chunk, objects layout info
            // Chunk ID: 3d3d (hex)
            // Chunk Lenght: 0 + sub chunks
            //-------------------------------------------
            case 0x3d3d:
            break;

            //--------------- EDIT_OBJECT ---------------
            // Description: Object block, info for each object
            // Chunk ID: 4000 (hex)
            // Chunk Lenght: len(object name) + sub chunks
            //-------------------------------------------
            case 0x4000:
                i=0;
                do
                {
                    fread (&l_char, 1, 1, l_file);
                    p_object->name[i]=l_char;
                    i++;
                }while(l_char != '\0' && i<20);
            break;

            //--------------- OBJ_TRIMESH ---------------
            // Description: Triangular mesh, contains chunks for 3d mesh info
            // Chunk ID: 4100 (hex)
            // Chunk Lenght: 0 + sub chunks
            //-------------------------------------------
            case 0x4100:
            break;

            //--------------- TRI_VERTEXL ---------------
            // Description: Vertices list
            // Chunk ID: 4110 (hex)
            // Chunk Lenght: 1 x unsigned short (number of vertices)
            //             + 3 x float (vertex coordinates) x (number of vertices)
            //             + sub chunks
            //-------------------------------------------
            case 0x4110:
                fread (&l_qty, sizeof (unsigned short), 1, l_file);
                p_object->vertices_qty = l_qty;
                //printf("Number of vertices: %d\n",l_qty);
                for (i=0; i<l_qty; i++)
                {
                    fread (&p_object->vertex[i].x, sizeof(float), 1, l_file);
                    //printf("Vertices list x: %f\n",p_object->vertex[i].x);
                    fread (&p_object->vertex[i].y, sizeof(float), 1, l_file);
                    //printf("Vertices list y: %f\n",p_object->vertex[i].y);
                    fread (&p_object->vertex[i].z, sizeof(float), 1, l_file);
                    //printf("Vertices list z: %f\n",p_object->vertex[i].z);
                }
                break;

            //--------------- TRI_FACEL1 ----------------
            // Description: Polygons (faces) list
            // Chunk ID: 4120 (hex)
            // Chunk Lenght: 1 x unsigned short (number of polygons)
            //             + 3 x unsigned short (polygon points) x (number of polygons)
            //             + sub chunks
            //-------------------------------------------
            case 0x4120:
                fread (&l_qty, sizeof (unsigned short), 1, l_file);
                p_object->polygons_qty = l_qty;
                //printf("Number of polygons: %d\n",l_qty);
                for (i=0; i<l_qty; i++)
                {
                    fread (&p_object->polygon[i].a, sizeof (unsigned short), 1, l_file);
                    //printf("Polygon point a: %d\n",p_object->polygon[i].a);
                    fread (&p_object->polygon[i].b, sizeof (unsigned short), 1, l_file);
                    //printf("Polygon point b: %d\n",p_object->polygon[i].b);
                    fread (&p_object->polygon[i].c, sizeof (unsigned short), 1, l_file);
                    //printf("Polygon point c: %d\n",p_object->polygon[i].c);
                    fread (&l_face_flags, sizeof (unsigned short), 1, l_file);
                    //printf("Face flags: %x\n",l_face_flags);
                }
                break;

            //------------- TRI_MAPPINGCOORS ------------
            // Description: Vertices list
            // Chunk ID: 4140 (hex)
            // Chunk Lenght: 1 x unsigned short (number of mapping points)
            //             + 2 x float (mapping coordinates) x (number of mapping points)
            //             + sub chunks
            //-------------------------------------------
            case 0x4140:
                fread (&l_qty, sizeof (unsigned short), 1, l_file);
                for (i=0; i<l_qty; i++)
                {
                    fread (&p_object->mapcoord[i].u, sizeof (float), 1, l_file);
                    //printf("Mapping list u: %f\n",p_object->mapcoord[i].u);
                    fread (&p_object->mapcoord[i].v, sizeof (float), 1, l_file);
                    //printf("Mapping list v: %f\n",p_object->mapcoord[i].v);
                }
                break;

            //----------- Skip unknow chunks ------------
            //We need to skip all the chunks that currently we don't use
            //We use the chunk lenght information to set the file pointer
            //to the same level next chunk
            //-------------------------------------------
            default:
                 fseek(l_file, l_chunk_lenght-6, SEEK_CUR);
        }
    }
    fclose (l_file); // Closes the file stream
}

/// Grab image and/or point cloud
void My3dsLoader::getMesh(obj_type &p_object) const
{
    p_object = object;
}



void My3dsLoader:: LoadBitmap()
{
    FILE * file;
    char temp;
    long i;

    BITMAPINFOHEADER infoheader;

    num_texture++; // The counter of the current texture is increased

    std::string path = bitMapDIR + bitMapName;

    const char* p_filename = path.c_str();

    if( (file = fopen(p_filename, "rb"))==NULL) exit(-1); // Open the file for reading

    fseek(file, 18, SEEK_CUR);  /* start reading width & height */
    fread(&infoheader.biWidth, sizeof(int), 1, file);

    fread(&infoheader.biHeight, sizeof(int), 1, file);

    fread(&infoheader.biPlanes, sizeof(short int), 1, file);
    if (infoheader.biPlanes != 1) {
        printf("Planes from %s is not 1: %u\n", p_filename, infoheader.biPlanes);
        exit(0);
    }

    // read the bpp
    fread(&infoheader.biBitCount, sizeof(unsigned short int), 1, file);
    if (infoheader.biBitCount != 24) {
      printf("Bpp from %s is not 24: %d\n", p_filename, infoheader.biBitCount);
      exit(0);
    }

    fseek(file, 24, SEEK_CUR);

    // read the data.
    infoheader.data = (char *) malloc(infoheader.biWidth * infoheader.biHeight * 3);
    if (infoheader.data == NULL) {
        printf("Error allocating memory for color-corrected image data\n");
        exit(0);
    }

    if ((i = fread(infoheader.data, infoheader.biWidth * infoheader.biHeight * 3, 1, file)) != 1) {
        printf("Error reading image data from %s.\n", p_filename);
        exit(0);
    }

    for (i=0; i<(infoheader.biWidth * infoheader.biHeight * 3); i+=3) { // reverse all of the colors. (bgr -> rgb)
        temp = infoheader.data[i];
        infoheader.data[i] = infoheader.data[i+2];
        infoheader.data[i+2] = temp;
    }


    fclose(file); // Closes the file stream


    glBindTexture(GL_TEXTURE_2D, num_texture); // Bind the ID texture specified by the 2nd parameter

    // The next commands sets the texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // If the u,v coordinates overflow the range 0,1 the image is repeated
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // The magnification function ("linear" produces better results)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); //The minifying function

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // We don't combine the color with the original surface color, use only the texture map.

    // Finally we define the 2d texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, infoheader.biWidth, infoheader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, infoheader.data);

    // And create 2d mipmaps for the minifying function
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, infoheader.biWidth, infoheader.biHeight, GL_RGB, GL_UNSIGNED_BYTE, infoheader.data);

    free(infoheader.data); // Free the memory we used to load the texture
}

void My3dsLoader::computeMask(const Mat34& cameraPose, const Mat34& objectPose, cv::Mat& mask, cv::Mat& depthMask)
{
    glClearColor(0.0, 0.0, 0.0, 0.0); // This clear the background color to black
    glShadeModel(GL_SMOOTH); // Type of shading for the polygons

    // Viewport transformation
    glViewport(0,0,screen_width,screen_height);

    // Projection transformation
    glMatrixMode(GL_PROJECTION); // Specifies which matrix stack is the target for matrix operations
    glLoadIdentity(); // We initialize the projection matrix as identity
    gluPerspective(45.0f,(GLfloat)screen_width/(GLfloat)screen_height,10.0f,10000.0f); // We define the "viewing volume"

    glEnable(GL_DEPTH_TEST); // We enable the depth test (also called z buffer)
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)

    glEnable(GL_TEXTURE_2D); // This Enable the Texture mapping

    object.id_texture=num_texture;

    // If the last function returns -1 it means the file was not found so we exit from the program
    if (object.id_texture==-1)
    {
        printf("Image file: spaceshiptexture.bmp not found\n");
        //MessageBox(NULL,"Image file: spaceshiptexture.bmp not found", "Zetadeck",MB_OK | MB_ICONERROR);
        exit (0);
    }


    int l_index;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // This clear the background color to dark blue
    glMatrixMode(GL_MODELVIEW); // Modeling transformation
    glLoadIdentity(); // Initialize the model matrix as identity


    glTranslatef(0.0,0.0,-400); // We move the object forward (the model matrix is multiplied by the translation matrix)

    glRotatef(0,1.0,0.0,0.0); // Rotations of the object (the model matrix is multiplied by the rotation matrices)
    glRotatef(0,0.0,1.0,0.0);
    glRotatef(0,0.0,0.0,1.0);


    glBindTexture(GL_TEXTURE_2D, object.id_texture); // We set the active texture

    glBegin(GL_TRIANGLES); // glBegin and glEnd delimit the vertices that define a primitive (in our case triangles)
    for (l_index=0;l_index<object.polygons_qty;l_index++)
    {
        //----------------- FIRST VERTEX -----------------
        // Texture coordinates of the first vertex
        glTexCoord2f( object.mapcoord[ object.polygon[l_index].a ].u,
                      object.mapcoord[ object.polygon[l_index].a ].v);
        // Coordinates of the first vertex
        glVertex3f( object.vertex[ object.polygon[l_index].a ].x,
                    object.vertex[ object.polygon[l_index].a ].y,
                    object.vertex[ object.polygon[l_index].a ].z); //Vertex definition

        //----------------- SECOND VERTEX -----------------
        // Texture coordinates of the second vertex
        glTexCoord2f( object.mapcoord[ object.polygon[l_index].b ].u,
                      object.mapcoord[ object.polygon[l_index].b ].v);
        // Coordinates of the second vertex
        glVertex3f( object.vertex[ object.polygon[l_index].b ].x,
                    object.vertex[ object.polygon[l_index].b ].y,
                    object.vertex[ object.polygon[l_index].b ].z);

        //----------------- THIRD VERTEX -----------------
        // Texture coordinates of the third vertex
        glTexCoord2f( object.mapcoord[ object.polygon[l_index].c ].u,
                      object.mapcoord[ object.polygon[l_index].c ].v);
        // Coordinates of the Third vertex
        glVertex3f( object.vertex[ object.polygon[l_index].c ].x,
                    object.vertex[ object.polygon[l_index].c ].y,
                    object.vertex[ object.polygon[l_index].c ].z);
    }
    glEnd();

    glFlush(); // This force the execution of OpenGL commands


    cv::Mat img(screen_height, screen_width, CV_8UC3);
    cv::Mat imgMask;
    imgMask = cv::Mat::zeros(screen_height, screen_width, CV_8UC1);

    mask.create(img.size(), img.type());
    //use fast 4-byte alignment (default anyway) if possible
    glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3) ? 1 : 4);

    //set length of one complete row in destination data (doesn't need to equal img.cols)
    glPixelStorei(GL_PACK_ROW_LENGTH, img.step/img.elemSize());

    //get the RGB Buffor
    glReadPixels(0, 0, img.cols, img.rows, GL_BGR, GL_UNSIGNED_BYTE, img.data);
    cv::flip(img, mask, 0);

    //get the depth Buffor
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_ALWAYS); // Change this to whatever kind of depth testing you want
    glDepthRange(0.0f, 1.0f);

    glReadPixels(0, 0, imgMask.cols, imgMask.rows, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, imgMask.data);
    cv::flip(imgMask, imgMask, 0);

    for(int x =0; x<imgMask.cols; x++)
    {
        for(int y = 0; y<imgMask.rows; y++)
        {
            imgMask.at<uchar>(y,x)=100* imgMask.at<uchar>(y,x);
        }
    }

    depthMask = imgMask.clone();
    notify(objectPose);
    notifyCamera(cameraPose);
    //bitwise_not ( imgMask, depthMask );
}

/// Attach visualizer
void My3dsLoader::attachVisualizer(QGLVisualizer* visualizer) {
    attach(visualizer);
}

putar::ObjLoader* putar::createMy3dsLoader(void) {
    my3dsLoader.reset(new My3dsLoader());
    return my3dsLoader.get();
}

putar::ObjLoader* putar::createMy3dsLoader(std::string configFile) {
    my3dsLoader.reset(new My3dsLoader(configFile));
    return my3dsLoader.get();
}

