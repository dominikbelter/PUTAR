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
void Thread2(int argc, char** argv)
{
    tinyxml2::XMLDocument config;
    config.LoadFile("../../resources/configGlobal.xml");
    if (config.ErrorID())
        std::cout << "unable to load config file.\n";
    std::string visualizerConfig(config.FirstChildElement("Configuration")->FirstChildElement("Visualizer")->FirstChildElement("config")->GetText());
    std::string visualizerType(config.FirstChildElement("Configuration")->FirstChildElement("Visualizer")->FirstChildElement("type")->GetText());
    std::string Loader3dsConfig(config.FirstChildElement("Configuration")->FirstChildElement("Loader3ds")->FirstChildElement("config")->GetText());

    QApplication application(argc,argv);
    setlocale(LC_NUMERIC,"C");
    glutInit(&argc, argv);

    QGLVisualizer visu(visualizerConfig);
    visu.setWindowTitle("Simulator viewer");
    visu.show();


    application.exec();
    std::cout << "Finished\n";
}

void Thread(int joy_fd, struct js_event js, int *axis, char *button, int num_of_axis, int x, int num_of_buttons, int argc, char** argv)
{
    while( 1 ) 	/* infinite loop */
    {

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
    }
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


    std::thread t(&Thread, joy_fd, js, axis, button, num_of_axis, x, num_of_buttons,argc ,argv);
    std::thread t2(&Thread2, argc, argv);
    t.join();
        t2.join();




    close( joy_fd );	/* too bad we never get here */
    return 0;
}
