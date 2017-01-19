#include "HMIControl/hmiGamepad.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>
#include <stdlib.h>
#include <unistd.h>
#include <thread>
//#define JOY_DEV "/dev/input/js1"

using namespace putar;

HmiGamepad::Ptr hmiGamepad;

// overloaded constructor
HmiGamepad::HmiGamepad(void){

}

HmiGamepad::HmiGamepad(std::string configFilename) : config(configFilename){

    std::string device_path = config.devicename;
    const char *JOY_DEV = device_path.c_str();

    if( ( joy_fd = open( JOY_DEV , O_RDONLY)) == -1 )
    {
        throw std::runtime_error("Couldn't open joystick");
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

    macierz(0,0)=1;
    macierz(0,1)=0;
    macierz(0,2)=0;
    macierz(0,3)=0;
    macierz(1,0)=0;
    macierz(1,1)=1;
    macierz(1,2)=0;
    macierz(1,3)=0;
    macierz(2,0)=0;
    macierz(2,1)=0;
    macierz(2,2)=1;
    macierz(2,3)=2;
    std::cout<<"pozycja poczatkowa"<<std::endl;
    std::cout<<macierz.matrix()<<std::endl;

    gamepadThread.reset(new std::thread(&HmiGamepad::gamepadProcess,this));
}
void HmiGamepad::gamepadProcess()
{
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
            zmienna(1,3)=config.increment;
            zmienna(2,0)=0;
            zmienna(2,1)=0;
            zmienna(2,2)=1;
            zmienna(2,3)=0;
            //std::cout<<zmienna.matrix()<<std::endl;
            macierz=macierz*zmienna;
            std::cout<<macierz.matrix()<<std::endl;
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
            zmienna(1,3)=-config.increment;
            zmienna(2,0)=0;
            zmienna(2,1)=0;
            zmienna(2,2)=1;
            zmienna(2,3)=0;
            //std::cout<<zmienna.matrix()<<std::endl;
            macierz=macierz*zmienna;
            std::cout<<macierz.matrix()<<std::endl;
        }
        if(button[1]==1)
        {
            printf("prawo\n");
            zmienna(0,0)=1;
            zmienna(0,1)=0;
            zmienna(0,2)=0;
            zmienna(0,3)=config.increment;
            zmienna(1,0)=0;
            zmienna(1,1)=1;
            zmienna(1,2)=0;
            zmienna(1,3)=0;
            zmienna(2,0)=0;
            zmienna(2,1)=0;
            zmienna(2,2)=1;
            zmienna(2,3)=0;
            //std::cout<<zmienna.matrix()<<std::endl;
            macierz=macierz*zmienna;
            std::cout<<macierz.matrix()<<std::endl;
        }
        if(button[3]==1)
        {
            printf("lewo\n");
            zmienna(0,0)=1;
            zmienna(0,1)=0;
            zmienna(0,2)=0;
            zmienna(0,3)=-config.increment;
            zmienna(1,0)=0;
            zmienna(1,1)=1;
            zmienna(1,2)=0;
            zmienna(1,3)=0;
            zmienna(2,0)=0;
            zmienna(2,1)=0;
            zmienna(2,2)=1;
            zmienna(2,3)=0;
            //std::cout<<zmienna.matrix()<<std::endl;
            macierz=macierz*zmienna;
            std::cout<<macierz.matrix()<<std::endl;
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
            zmienna(2,3)=config.increment;
            //std::cout<<zmienna.matrix()<<std::endl;
            macierz=macierz*zmienna;
            std::cout<<macierz.matrix()<<std::endl;
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
            zmienna(2,3)=-config.increment;
            //std::cout<<zmienna.matrix()<<std::endl;
            macierz=macierz*zmienna;
            std::cout<<macierz.matrix()<<std::endl;
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
            macierz=macierz*zmienna;
            std::cout<<macierz.matrix()<<std::endl;
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
            macierz=macierz*zmienna;
            std::cout<<macierz.matrix()<<std::endl;
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
            macierz=macierz*zmienna;
            std::cout<<macierz.matrix()<<std::endl;
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
            macierz=macierz*zmienna;
            std::cout<<macierz.matrix()<<std::endl;
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
            macierz=macierz*zmienna;
            std::cout<<macierz.matrix()<<std::endl;
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
            macierz=macierz*zmienna;
            std::cout<<macierz.matrix()<<std::endl;
        }
        printf("  \r");
        usleep(config.delay*1000);
        fflush(stdout);

    }

}

HmiGamepad::~HmiGamepad(){
    gamepadThread->join();
}


void HmiGamepad::updatePose(Mat34& pose){
    pose = macierz;
}

putar::Hmi* putar::createMyHmiGamepad(){
    hmiGamepad.reset(new HmiGamepad());
    return hmiGamepad.get();
}

putar::Hmi* putar::createMyHmiGamepad(std::string configFilename){
    hmiGamepad.reset(new HmiGamepad(configFilename));
    return hmiGamepad.get();
}
