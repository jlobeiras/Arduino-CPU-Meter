#pragma once
#if !defined(SERIALCOMLINUX_H) && defined(__linux__)
#define SERIALCOMLINUX_H

#include "SerialCom.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

COMPORT openSerialPort(const char* portName) {
    COMPORT serialPort = open(portName, O_RDWR | O_NOCTTY | O_NDELAY);
    if (serialPort == -1)
        exit(fprintf(stderr, "Error opening serial port\n"));
    return serialPort;
}

int closeSerialPort(COMPORT serialPort) {
    return close(serialPort);
}

int configureSerialPort(COMPORT serialPort) {
    struct termios serialSettings;
    if (tcgetattr(serialPort, &serialSettings))
        return fprintf(stderr, "Error at tcgetattr\n");

    cfsetispeed(&serialSettings, B9600);  // Set baud rate to 9600
    cfsetospeed(&serialSettings, B9600);

    serialSettings.c_cflag &= ~PARENB;    // No parity
    serialSettings.c_cflag &= ~CSTOPB;    // 1 stop bit
    serialSettings.c_cflag &= ~CSIZE;     // Mask character size bits
    serialSettings.c_cflag |= CS8;        // 8 data bits

    if (tcsetattr(serialPort, TCSANOW, &serialSettings))
        return fprintf(stderr, "Error at tcsetattr\n");

    return 0;
}

int writeToSerialPort(COMPORT serialPort, const char* data) {
    size_t dataSize = strlen(data);
    ssize_t bytesWritten = write(serialPort, data, dataSize);
    if (bytesWritten < 0)
        exit(fprintf(stderr, "Error writing to serial port\n"));

    printf("%zd bytes written to serial port: '%s'\n",
        bytesWritten, data);
    return bytesWritten;
}

int readFromSerialPort(COMPORT serialPort, char* buffer, int bufferSize) {
    ssize_t bytesRead = read(serialPort, buffer, bufferSize);
    if (bytesRead < 0)
        exit(fprintf(stderr, "Error reading from serial port\n"));

    printf("%zd bytes read from serial port: %.*s\n",
        bytesRead, (int)bytesRead, buffer);
    return bytesRead;
}

#endif // SERIALCOMLINUX_H