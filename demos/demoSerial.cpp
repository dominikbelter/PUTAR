#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <string>

//#define USE_LIB_SERIAL
#define USE_BOOST

#ifdef USE_LIB_SERIAL
//install libserial-dev
#include <SerialStream.h>

int main( void )
{
     //
     // Open the serial port.
     //
     using namespace std;
     using namespace LibSerial ;
     SerialStream serial_port ;
     serial_port.Open( "/dev/ttyUSB0" ) ;
     if ( ! serial_port.good() )
     {
         std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
                   << "Error: Could not open serial port."
                   << std::endl ;
         exit(1) ;
     }
     //
     // Set the baud rate of the serial port.
     //
     serial_port.SetBaudRate( SerialStreamBuf::BAUD_115200 ) ;
     if ( ! serial_port.good() ){
         std::cerr << "Error: Could not set the baud rate." << std::endl ;
         exit(1) ;
     }
     //
     // Set the number of data bits.
     //
     serial_port.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 );
     if ( ! serial_port.good() ) {
         std::cerr << "Error: Could not set the character size." << std::endl ;
         exit(1) ;
     }
     //
     // Disable parity.
     //
     serial_port.SetParity( SerialStreamBuf::PARITY_NONE ) ;
     if ( ! serial_port.good() ){
         std::cerr << "Error: Could not disable the parity." << std::endl ;
         exit(1) ;
     }
     //
     // Set the number of stop bits.
     //
     serial_port.SetNumOfStopBits( 1 ) ;
     if ( ! serial_port.good() ) {
         std::cerr << "Error: Could not set the number of stop bits." << std::endl ;
         exit(1) ;
     }
     //
     // Turn off hardware flow control.
     //
     serial_port.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ) ;
     if ( ! serial_port.good() ) {
         std::cerr << "Error: Could not use hardware flow control." << std::endl ;
         exit(1);
     }
     //
     // Do not skip whitespace characters while reading from the
     // serial port.
     //
     // serial_port.unsetf( std::ios_base::skipws ) ;
     //
     // Wait for some data to be available at the serial port.
     //
     //
     // Keep reading data from serial port and print it to the screen.
     //
     // Wait for some data to be available at the serial port.
     //
     while( serial_port.rdbuf()->in_avail() == 0 ){
         usleep(100) ;
     }

     while(1) {
         char next_byte;
         serial_port.get(next_byte);
         std::cerr << next_byte;

     }
     std::cerr << std::endl ;
     return EXIT_SUCCESS ;
}

#endif

#ifdef USE_BOOST
#include <boost/asio.hpp>

class SimpleSerial
{
public:
    /**
     * Constructor.
     * \param port device name, example "/dev/ttyUSB0" or "COM4"
     * \param baud_rate communication speed, example 9600 or 115200
     * \throws boost::system::system_error if cannot open the
     * serial device
     */
    SimpleSerial(std::string port, unsigned int baud_rate)
    : io(), serial(io,port)
    {
        serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    }

    /**
     * Write a string to the serial device.
     * \param s string to write
     * \throws boost::system::system_error on failure
     */
    void writeString(std::string s)
    {
        boost::asio::write(serial,boost::asio::buffer(s.c_str(),s.size()));
    }

    /**
     * Blocks until a line is received from the serial device.
     * Eventual '\n' or '\r\n' characters at the end of the string are removed.
     * \return a string containing the received line
     * \throws boost::system::system_error on failure
     */
    std::string readLine()
    {
        //Reading data char by char, code is optimized for simplicity, not speed
        using namespace boost;
        char c;
        std::string result;
        for(;;)
        {
            asio::read(serial,asio::buffer(&c,1));
            switch(c)
            {
                case '\r':
                    break;
                case '\n':
                    return result;
                default:
                    result+=c;
            }
        }
    }

private:
    boost::asio::io_service io;
    boost::asio::serial_port serial;
};


int main(void)
{
    try {

        SimpleSerial serial("/dev/ttyUSB0",115200);
        while(1){
            std::cout<<serial.readLine()<<std::endl;
        }

    } catch(boost::system::system_error& e)
    {
        std::cout<<"Error: "<<e.what()<<std::endl;
        return 1;
    }
}
#endif
