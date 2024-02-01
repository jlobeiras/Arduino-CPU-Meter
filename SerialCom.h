#pragma once
#ifndef SERIALCOM_H
#define SERIALCOM_H

#ifdef __linux__
	#define COMPORT int
#else
	#include <windows.h>
	#define COMPORT HANDLE
#endif

// Open a serial port using OS port name
COMPORT openSerialPort(const char* portName);

// Close serial port specified by handler
int closeSerialPort(COMPORT serialPort);

// Configure serial port (default Arduino parameters)
int configureSerialPort(COMPORT serialPort);

// Send data to serial port
int writeToSerialPort(COMPORT serialPort, const char* data);

int readFromSerialPort(HANDLE serialPort, char* buffer, int bufferSize) {

#endif // SERIALCOM_H