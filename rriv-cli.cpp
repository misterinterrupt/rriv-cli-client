/**
 *  @example example_project.cpp
 */

#include <libserial/SerialPort.h>
#include <libserial/SerialStream.h>

#include <iostream>
#include <cstring>
#include "lib/Cmd.h"
#include <unistd.h>

LibSerial::SerialStream serial_stream ;


void help(int arg_cnt, char**args)
{
  char commands[] = "Command List:\n"
  "version\n"
  "show-warranty\n"
  "get-config\n"
  "set-config\n"
  "set-slot-config\n"
  "clear-slot\n"
  "set-rtc\n"
  "get-rtc\n"
  "restart\n"
  "set-site-name\n"
  "set-deployment-identifier\n"
  "set-interval\n"
  "set-burst-number\n"
  "set-start-up-delay\n"
  "set-burst-delay\n"
  "calibrate\n"
  "set-user-note\n"
  "set-user-value\n"
  "start-logging\n"
  "deploy-now\n"
  "interactive\n"
  "trace\n"
  "check-memory\n"
  "scan-ic2\n";

  std::cout << commands << std::endl;
}

void relay(int arg_cnt, char **args)
{
    for(int i=0; i< arg_cnt; i++)
    {
        serial_stream << args[i];
    }
    serial_stream << std::endl;
}


int main()
{
    std::cin.sync_with_stdio(false);
    // serial_stream.sync_with_stdio(false);

    char port[50] = "/dev/ttyACM0";
    std::cout << "opening " << port << std::endl;

    serial_stream.Open( port ) ;
    std::cout << "opened" << std::endl;

    // Set the baud rates.
    using LibSerial::BaudRate ;
    serial_stream.SetBaudRate( BaudRate::BAUD_115200 ) ;

    char buffer[200];

    std::cout << "connecting" << std::endl;

    cmdInit(&std::cin, &std::cout);
    cmdAdd("help", help);
    cmdAdd("relay", relay);

    std::cout << "cmd ready" << std::endl;

    // With SerialStream objects you can read/write to the port using iostream operators.
    while(true)
    {

        while (serial_stream.rdbuf()->in_avail() > 0)
        {

            // serial_stream.getline(buffer, 200, '\r');
            std::streamsize size = 6;
            std::cout << serial_stream.rdbuf()->in_avail()  << std::endl;

            if(serial_stream.rdbuf()->in_avail() < size)
            {
                size = serial_stream.rdbuf()->in_avail();
            }
            std::cout << size  << std::endl;

            serial_stream.get(buffer, size) ;
            buffer[size] = '\0';

            // if (strlen(buffer) > 0)
            // {
                std::cout << ".> " << strlen(buffer) << buffer << std::endl;
            // }    

            usleep(1000);

        }

        cmdPoll();

        // if(strcmp(buffer, "\u200B\u200B\u200B\u200B\r") == 0)
        // {
        //     std::cout << "will read" << std::endl;
        //     std::cin.getline(buffer, 200);
        //     std::cout << "did read" << std::endl;
        //     // if(strlen(wbuffer) > 0)
        //     // {
        //         std::cout << "done read" << std::endl;
        //         std::cout << buffer << std::endl;
        //     // }
        //     // else
        //     // {
        //         std::cout << "gotnothing" << std::endl;
        //     // }
        // }
        usleep(100000);
    }
    serial_stream.Close() ;
}
