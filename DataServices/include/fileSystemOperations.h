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
    const std::string &getLastError() const override;
    bool fileExists(const std::string &fileName) override;
    bool fileCopy(const std::string &source, const std::string &destination) override;
    bool fileCopyWithOverwrite(const std::string &source, const std::string &destination) override;
    std::string extractFolderPath(const std::string &fullPathWithFileName) const override;
private:
    std::string lastError;
};