#include "HMIControl/hmiGamepad.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#include <stdlib.h>
#include <unistd.h>
#define JOY_DEV "/dev/input/js1"

using namespace putar;

HmiGamepad::Ptr hmiGamepad;

// overloaded constructor
HmiGamepad::HmiGamepad(void){

}

HmiGamepad::HmiGamepad(std::string configFilename) : config(configFilename){

    if( ( joy_fd = open( JOY_DEV , O_RDONLY)) == -1 )
    {
        printf( "Couldn't open joystick\n" );
        //return -1;
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

    pose(0,0)=1;
    pose(0,1)=0;
    pose(0,2)=0;
    pose(0,3)=0;
    pose(1,0)=0;
    pose(1,1)=1;
    pose(1,2)=0;
    pose(1,3)=0;
    pose(2,0)=0;
    pose(2,1)=0;
    pose(2,2)=1;
    pose(2,3)=2;
    std::cout<<"pozycja poczatkowa"<<std::endl;
    std::cout<<pose.matrix()<<std::endl;



}

void HmiGamepad::updatePose(Mat34& pose){
    //throw std::runtime_error("updatePose method is not implemented");
    while( 1 ) 	/* infinite loop */
    {
        struct js_event js;

            /* read the joystick state */
        read(joy_fd, &js, sizeof(struct js_event));

            /* see what to do with the event */
        switch (js.type & ~JS_EVENT_INIT)
        {
            case JS_EVENT_AXIS:
                axis   [ js.number ] = js.value;
                break;
            case JS_EVENT_BUTTON:
                button [ js.number ] = (char)js.value;
                break;
        }
        if(button[4]==1)
        {
            printf("gora\n");
            zmienna(0,0)=1;
            zmienna(0,1)=0;
            zmienna(0,2)=0;
            zmienna(0,3)=0;
            zmienna(1,0)=0;
            zmienna(1,1)=1;
            zmienna(1,2)=0;
            zmienna(1,3)=0.02;
            zmienna(2,0)=0;
            zmienna(2,1)=0;
            zmienna(2,2)=1;
            zmienna(2,3)=0;
            //std::cout<<zmienna.matrix()<<std::endl;
            pose=pose*zmienna;
            std::cout<<pose.matrix()<<std::endl;
        }
        if(button[0]==1)
        {
            printf("dol\n");
            zmienna(0,0)=1;
            zmienna(0,1)=0;
            zmienna(0,2)=0;
            zmienna(0,3)=0;
            zmienna(1,0)=0;
            zmienna(1,1)=1;
            zmienna(1,2)=0;
            zmienna(1,3)=-0.02;
            zmienna(2,0)=0;
            zmienna(2,1)=0;
            zmienna(2,2)=1;
            zmienna(2,3)=0;
            //std::cout<<zmienna.matrix()<<std::endl;
            pose=pose*zmienna;
            std::cout<<pose.matrix()<<std::endl;
        }
        if(button[1]==1)
        {
            printf("prawo\n");
            zmienna(0,0)=1;
            zmienna(0,1)=0;
            zmienna(0,2)=0;
            zmienna(0,3)=0.02;
            zmienna(1,0)=0;
            zmienna(1,1)=1;
            zmienna(1,2)=0;
            zmienna(1,3)=0;
            zmienna(2,0)=0;
            zmienna(2,1)=0;
            zmienna(2,2)=1;
            zmienna(2,3)=0;
            //std::cout<<zmienna.matrix()<<std::endl;
            pose=pose*zmienna;
            std::cout<<pose.matrix()<<std::endl;
        }
        if(button[3]==1)
        {
            printf("lewo\n");
            zmienna(0,0)=1;
            zmienna(0,1)=0;
            zmienna(0,2)=0;
            zmienna(0,3)=-0.02;
            zmienna(1,0)=0;
            zmienna(1,1)=1;
            zmienna(1,2)=0;
            zmienna(1,3)=0;
            zmienna(2,0)=0;
            zmienna(2,1)=0;
            zmienna(2,2)=1;
            zmienna(2,3)=0;
            //std::cout<<zmienna.matrix()<<std::endl;
            pose=pose*zmienna;
            std::cout<<pose.matrix()<<std::endl;
        }
        if(button[9]==1)
        {
            printf("translacja  z+\n");
            zmienna(0,0)=1;
            zmienna(0,1)=0;
            zmienna(0,2)=0;
            zmienna(0,3)=0;
            zmienna(1,0)=0;
            zmienna(1,1)=1;
            zmienna(1,2)=0;
            zmienna(1,3)=0;
            zmienna(2,0)=0;
            zmienna(2,1)=0;
            zmienna(2,2)=1;
            zmienna(2,3)=0.02;
            //std::cout<<zmienna.matrix()<<std::endl;
            pose=pose*zmienna;
            std::cout<<pose.matrix()<<std::endl;
        }
        if(button[7]==1)
        {
            printf("translacja z-\n");
            zmienna(0,0)=1;
            zmienna(0,1)=0;
            zmienna(0,2)=0;
            zmienna(0,3)=0;
            zmienna(1,0)=0;
            zmienna(1,1)=1;
            zmienna(1,2)=0;
            zmienna(1,3)=0;
            zmienna(2,0)=0;
            zmienna(2,1)=0;
            zmienna(2,2)=1;
            zmienna(2,3)=-0.02;
            //std::cout<<zmienna.matrix()<<std::endl;
            pose=pose*zmienna;
            std::cout<<pose.matrix()<<std::endl;
        }
        if(axis[0]>50)
        {
            printf("rotacja wokol x +\n");
            zmienna(0,0)=1;
            zmienna(0,1)=0;
            zmienna(0,2)=0;
            zmienna(0,3)=0;
            zmienna(1,0)=0;
            zmienna(1,1)=0.28366;
            zmienna(1,2)=0.95892;
            zmienna(1,3)=0;
            zmienna(2,0)=0;
            zmienna(2,1)=-0.95892;
            zmienna(2,2)=0.28366;
            zmienna(2,3)=0;
            //std::cout<<zmienna.matrix()<<std::endl;
            pose=pose*zmienna;
            std::cout<<pose.matrix()<<std::endl;
        }
        if(axis[0]<-50)
        {
            printf("rotacja wokol x -\n");
            zmienna(0,0)=1;
            zmienna(0,1)=0;
            zmienna(0,2)=0;
            zmienna(0,3)=0;
            zmienna(1,0)=0;
            zmienna(1,1)=0.28366;
            zmienna(1,2)=-0.95892;
            zmienna(1,3)=0;
            zmienna(2,0)=0;
            zmienna(2,1)=0.95892;
            zmienna(2,2)=0.28366;
            zmienna(2,3)=0;
            //std::cout<<zmienna.matrix()<<std::endl;
            pose=pose*zmienna;
            std::cout<<pose.matrix()<<std::endl;
        }
        if(axis[1]>50)
        {
            printf("rotacja wokol y +\n");
            zmienna(0,0)=0.28366;
            zmienna(0,1)=0;
            zmienna(0,2)=-0.95892;
            zmienna(0,3)=0;
            zmienna(1,0)=0;
            zmienna(1,1)=1;
            zmienna(1,2)=0;
            zmienna(1,3)=0;
            zmienna(2,0)=0.95892;
            zmienna(2,1)=0;
            zmienna(2,2)=0.28366;
            zmienna(2,3)=0;
            //std::cout<<zmienna.matrix()<<std::endl;
            pose=pose*zmienna;
            std::cout<<pose.matrix()<<std::endl;
        }
        if(axis[1]<-50)
        {
            printf("rotacja wokol y -\n");
            zmienna(0,0)=0.28366;
            zmienna(0,1)=0;
            zmienna(0,2)=0.95892;
            zmienna(0,3)=0;
            zmienna(1,0)=0;
            zmienna(1,1)=1;
            zmienna(1,2)=0;
            zmienna(1,3)=0;
            zmienna(2,0)=-0.95892;
            zmienna(2,1)=0;
            zmienna(2,2)=0.28366;
            zmienna(2,3)=0;
            //std::cout<<zmienna.matrix()<<std::endl;
            pose=pose*zmienna;
            std::cout<<pose.matrix()<<std::endl;
        }
        if(axis[2]>50)
        {
            printf("rotacja wokol z +\n");
            zmienna(0,0)=0.28366;
            zmienna(0,1)=0.95892;
            zmienna(0,2)=0;
            zmienna(0,3)=0;
            zmienna(1,0)=-0.95892;
            zmienna(1,1)=0.28366;
            zmienna(1,2)=0;
            zmienna(1,3)=0;
            zmienna(2,0)=0;
            zmienna(2,1)=0;
            zmienna(2,2)=1;
            zmienna(2,3)=0;
            //std::cout<<zmienna.matrix()<<std::endl;
            pose=pose*zmienna;
            std::cout<<pose.matrix()<<std::endl;
        }
        if(axis[2]<-50)
        {
            printf("rotacja wokol z -\n");
            zmienna(0,0)=0.28366;
            zmienna(0,1)=-0.95892;
            zmienna(0,2)=0;
            zmienna(0,3)=0;
            zmienna(1,0)=0.95892;
            zmienna(1,1)=0.28366;
            zmienna(1,2)=0;
            zmienna(1,3)=0;
            zmienna(2,0)=0;
            zmienna(2,1)=0;
            zmienna(2,2)=1;
            zmienna(2,3)=0;
            //std::cout<<zmienna.matrix()<<std::endl;
            pose=pose*zmienna;
            std::cout<<pose.matrix()<<std::endl;
        }
        printf("  \r");
        usleep(100000);
        fflush(stdout);

    }
}

putar::Hmi* putar::createMyHmiGamepad(){
    hmiGamepad.reset(new HmiGamepad());
    return hmiGamepad.get();
}

putar::Hmi* putar::createMyHmiGamepad(std::string configFilename){
    hmiGamepad.reset(new HmiGamepad(configFilename));
    return hmiGamepad.get();
}
