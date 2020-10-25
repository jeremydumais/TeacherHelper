#pragma once

#include "IFileSystemOperations.h"
#include <string>

#ifdef _WIN32
    #ifdef DATASERVICES_EXPORTS  
        #define FILESYSTEMOPERATIONS_API __declspec(dllexport)   
    #else  
        #define FILESYSTEMOPERATIONS_API __declspec(dllimport)   
    #endif
#else
    #define FILESYSTEMOPERATIONS_API
#endif

class FILESYSTEMOPERATIONS_API FileSystemOperations : public IFileSystemOperations
{
public:
    bool fileExists(const std::string &fileName) override;
private:
    std::string lastError;
};