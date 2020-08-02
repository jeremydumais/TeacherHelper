#pragma once

#include "test.h"
#include "IManagementItemStorage.h"
#include "databaseConnection.h"
#include <list>
#include <memory>

#ifdef _WIN32
    #ifdef CONTROLLERS_EXPORTS  
        #define TESTCONTROLLER_API __declspec(dllexport)   
    #else  
        #define TESTCONTROLLER_API __declspec(dllimport)   
    #endif
#else
    #define TESTCONTROLLER_API
#endif

class TESTCONTROLLER_API TestController
{
};