#pragma once
#if !defined(SERIALCOMWIN_H) && !defined(__linux__)
#define SERIALCOMWIN_H

#include "SerialCom.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <windows.h>

COMPORT openSerialPort(const char* portName) {
    COMPORT serialPort = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (serialPort == INVALID_HANDLE_VALUE)
        exit(fprintf(stderr, "Error opening serial port\n"));
    return serialPort;
}

int closeSerialPort(COMPORT serialPort) {
    return CloseHandle(serialPort) != 0;
}

int configureSerialPort(COMPORT serialPort) {
    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(serialPort, &dcbSerialParams))
        return fprintf(stderr, "Error getting serial port state\n");

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (!SetCommState(serialPort, &dcbSerialParams))
        return fprintf(stderr, "Error setting serial port state\n");

    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = MAXDWORD;   // Maximum time allowed to elapse before the arrival of the next byte.
    timeouts.ReadTotalTimeoutConstant = 100;   // Maximum time allowed to elapse before returning from a read operation.
    timeouts.ReadTotalTimeoutMultiplier = 0;   // Time multiplier for each read operation.
    timeouts.WriteTotalTimeoutConstant = 50;   // Maximum time allowed to elapse before returning from a write operation.
    timeouts.WriteTotalTimeoutMultiplier = 10; // Time multiplier for each write operation.
    if (!SetCommTimeouts(serialPort, &timeouts))
        return fprintf(stderr, "Error setting serial port timeouts\n");

    return 0;
}

int writeToSerialPort(COMPORT serialPort, const char* data) {
    DWORD bytesWritten;
    if (!WriteFile(serialPort, data, (int)strlen(data), &bytesWritten, NULL))
        exit(fprintf(stderr, "Error writing to serial port\n"));

    printf("%lu bytes written to serial port: '%s'\n",
        bytesWritten, data);
    return bytesWritten;
}

int readFromSerialPort(HANDLE serialPort, char* buffer, int bufferSize) {
    DWORD bytesRead;
    if (!ReadFile(serialPort, buffer, bufferSize, &bytesRead, NULL))
        exit(fprintf(stderr, "Error reading from serial port\n"));

    printf("%lu bytes read from serial port: '%.*s'\n",
        bytesRead, (int)bytesRead, buffer);
    return bytesRead;
}

#endif // SERIALCOMWIN_H