#include "SerialCom.h"

#define BUFFER_SIZE 256

#ifdef __linux__
	#include "SerialComLinux.h"
#else
	#include "SerialComWin.h"
#endif
