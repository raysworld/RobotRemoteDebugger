// stdafx.h : Standard Application Framework Extensions

#pragma once

/******************************************************/
// If you use Qt to 
#define USE_QT	(1)

#include <stdio.h>
#include <tchar.h>

// C++ STL
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>

#ifndef  USE_QT
	// Using Windows Socket
	#include <Winsock2.h>
	#pragma comment(lib, "ws2_32.lib")
#else
	// Using Qt Socket
	#include <QCoreApplication>
	#include <QTcpSocket>
#endif

// User-defined
#include "RobotClient/Helper.h"
