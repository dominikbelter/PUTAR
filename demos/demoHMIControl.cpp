#include "Defs/defs.h"
#include "../3rdParty/tinyXML/tinyxml2.h"
#include "Visualizer/Qvisualizer.h"
#include <GL/glut.h>
#include <QApplication>
#include <iostream>
#include <thread>



#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#include <stdlib.h>




#define JOY_DEV "/dev/input/js1"
#define GLUT_JOYSTICK_BUTTON_E 128
#define GLUT_JOYSTICK_BUTTON_F 512
#define GLUT_JOYSTICK_BUTTON_AB 18
#define GLUT_JOYSTICK_BUTTON_BC 3
#define GLUT_JOYSTICK_BUTTON_AD 24
#define GLUT_JOYSTICK_BUTTON_CD 9
#define GLUT_JOYSTICK_BUTTON_G 16



void display () {

    /* clear window */
    glClear(GL_COLOR_BUFFER_BIT);

    /* draw scene */
    glutSolidTeapot(.1);

    /* flush drawing routines to the window */
    glFlush();

}
void joyFunc(unsigned int btn,int x,int y, int z)
{

 if (btn==GLUT_JOYSTICK_BUTTON_A)
 {
 glTranslated(0,-0.02,0);
 }
 if (btn==GLUT_JOYSTICK_BUTTON_B)
 {
 glTranslated(0.02,0,0);
 }
 if (btn==GLUT_JOYSTICK_BUTTON_G)
 {
 glTranslated(0,0.02,0);
 }
 if (btn==GLUT_JOYSTICK_BUTTON_D)
 {
 glTranslated(-0.02,0,0);
 }
 if (btn==GLUT_JOYSTICK_BUTTON_E)
 {
    glTranslated(0,0,0.2);
 }
 if (btn==GLUT_JOYSTICK_BUTTON_F)
 {
    glTranslated(0,0,-0.2);
 }
 if (btn==GLUT_JOYSTICK_BUTTON_AB)
 {
    glTranslated(0.02,0.02,0);
 }
 if (btn==GLUT_JOYSTICK_BUTTON_BC)
 {
    glTranslated(0.02,-0.02,0);
 }
 if (btn==GLUT_JOYSTICK_BUTTON_CD)
 {
    glTranslated(-0.02,-0.02,0);
 }
 if (btn==GLUT_JOYSTICK_BUTTON_AD)
 {
    glTranslated(-0.02,0.02,0);
 }
 if (x>50){
     glRotated(5,0,1,0);
 }
 if (x<-50){
     glRotated(-5,0,1,0);
 }
 if (y>50){
     glRotated(5,1,0,0);
 }
 if (y<-50){
     glRotated(-5,1,0,0);
 }
 if (z>50){
     glRotated(5,0,0,1);
 }
 if (z<-50){
     glRotated(-5,0,0,1);
 }
 std::cout << btn << " -- " << x << "" <<  " -- " << y << " -- " << z << "\n"; //test gampead
 glutPostRedisplay();
}
void Thread2(int argc, char** argv)
{
    /* initialize GLUT, using any commandline parameters passed to the
           program */
        glutInit(&argc,argv);

        /* setup the size, position, and display mode for new windows */
        glutInitWindowSize(1000,800);
        glutInitWindowPosition(0,0);
        glutInitDisplayMode(GLUT_RGB);

        /* create and set up a window */
        glutCreateWindow("hello, teapot!");

        glutDisplayFunc(display);
        glutJoystickFunc(&joyFunc,100);
        /* tell GLUT to wait for events */
        glutMainLoop();



}




void Thread(int joy_fd, struct js_event js, int *axis, char *button, int num_of_axis, int x, int num_of_buttons)
{
    //while( 1 ) 	/* infinite loop */
   // {

            /* read the joystick state */
        read(joy_fd, &js, sizeof(struct js_event));

            /* see what to do with the event */
        switch (js.type & ~JS_EVENT_INIT)
        {
            case JS_EVENT_AXIS:
                axis   [ js.number ] = js.value;
                break;
            case JS_EVENT_BUTTON:
                button [ js.number ] = js.value;
                break;
        }

            /* print the results */
        printf( "X: %6d  Y: %6d  ", axis[0], axis[1] );

        if( num_of_axis > 2 )
            printf("Z: %6d  ", axis[2] );

        if( num_of_axis > 3 )
            printf("R: %6d  ", axis[3] );

        for( x=0 ; x<num_of_buttons ; ++x )
            printf("B%d: %d  ", x, button[x] );

        printf("  \r");
        fflush(stdout);
    //}
}



int main(int argc, char** argv)
{
    int joy_fd, *axis=NULL, num_of_axis=0, num_of_buttons=0, x;
    char *button=NULL, name_of_joystick[80];
    struct js_event js;

    if( ( joy_fd = open( JOY_DEV , O_RDONLY)) == -1 )
    {
        printf( "Couldn't open joystick\n" );
        return -1;
    }

    ioctl( joy_fd, JSIOCGAXES, &num_of_axis );
    ioctl( joy_fd, JSIOCGBUTTONS, &num_of_buttons );
    ioctl( joy_fd, JSIOCGNAME(80), &name_of_joystick );

    axis = (int *) calloc( num_of_axis, sizeof( int ) );
    button = (char *) calloc( num_of_buttons, sizeof( char ) );

    printf("Joystick detected: %s\n\t%d axis\n\t%d buttons\n\n"
        , name_of_joystick
        , num_of_axis
        , num_of_buttons );

    fcntl( joy_fd, F_SETFL, O_NONBLOCK );	/* use non-blocking mode */


    std::thread t(&Thread, joy_fd, js, axis, button, num_of_axis, x, num_of_buttons);
    std::thread t2(&Thread2, argc, argv);
    t.join();
        t2.join();




    close( joy_fd );	/* too bad we never get here */
    return 0;
}
