#ifndef __linux__
    // Windows platform macro definitions
    const char* defaultPort = "COM7";
    #define popen(_data, _mode) _popen(_data, _mode)
    #define pclose(_pipe) _pclose(_pipe)
    #define wait(_msec) Sleep(_msec);
    #pragma warning(disable : 4996)
    #define CPU_LOAD "wmic cpu get loadpercentage /value"
#else
    // Linux platform macro definitions
    const char* defaultPort = "/dev/ttyS7";
    #define wait(_msec) usleep(_msec * 1000)
    #include <unistd.h>
    #define CPU_LOAD "mpstat --dec=0 1 1 | tail -n1 | tr -s ' ' | cut -d' ' -f3 | sed 's/^/LoadPercentage=/'"
#endif

#include "SerialCom.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <list>
#include <string>

const int BUFFER_SIZE = 256;

// Execute command string and return program output as string list
std::list<std::string> execCmd(std::string cmd) {

    // Open a process and create a pipe for reading its output
    FILE* pipe = popen(cmd.data(), "r");
    if (!pipe) exit(fprintf(stderr, "Error at popen\n"));

    // Read the output of the command from the pipe
    std::list<std::string> retVal;
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE - 1, pipe) != NULL)
        retVal.push_back(buffer);

    // Close the pipe and return the program output
    if (pclose(pipe) == -1)
        exit(fprintf(stderr, "Error at pclose\n"));
    return retVal;
}

// Simple test function for echo reply to serial port
void echo(COMPORT serialPort) {
    char dataToSend[BUFFER_SIZE] = "";
    puts("Ready for commands>");
    while (1) {
        if (!fgets(dataToSend, 100, stdin)) break;
        if (strchr("\n\r\0", dataToSend[0])) break;
        writeToSerialPort(serialPort, dataToSend);
        wait(100);
        readFromSerialPort(serialPort);
    }
}

int main(int argc, char* argv[]) {

    // Check arguments and print program usage if needed
    if (argc == 2) defaultPort = argv[1];
    else printf("Usage: %s [port]   (%s by default)\n", "SerialCom", defaultPort);

    // Try to open serial port
    COMPORT serialPort = openSerialPort(defaultPort);
    if (configureSerialPort(serialPort))
        exit(EXIT_FAILURE);

    puts("Waiting for port to be ready...");
    wait(3000); // May be needed for Arduino after port open
    /// echo(serialPort);

    char buffer[BUFFER_SIZE] = "";
    while (1) {
        // Try to get CPU usage using OS command line
        auto output = execCmd(CPU_LOAD);
        int load = -1;
        for (const auto& str : output)
            if (sscanf(str.data(), "LoadPercentage=%i", &load) == 1) break;
        if (load == -1) continue;

        // If data is received send it to Arduino and check reply
        sprintf(buffer, "LoadPercentage=%i\n", load);
        writeToSerialPort(serialPort, buffer);
        wait(100);
        readFromSerialPort(serialPort, buffer, BUFFER_SIZE);
    }

    return closeSerialPort(serialPort);
}
