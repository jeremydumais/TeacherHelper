#include "fileSystemOperations.h"
#include <boost/filesystem.hpp>

using namespace boost;
using namespace std;

const std::string& FileSystemOperations::getLastError() 
{
    return lastError;
}

bool FileSystemOperations::fileExists(const string &fileName) 
{
    return filesystem::exists(fileName);
}

bool FileSystemOperations::fileCopy(const std::string &source, const std::string &destination) 
{
    try {
        filesystem::copy_file(source, destination, filesystem::copy_option::fail_if_exists);
    }
    catch(const boost::filesystem::filesystem_error &err) {
        lastError = err.what();
    }
    return fileExists(destination);
}

bool FileSystemOperations::fileCopyWithOverwrite(const std::string &source, const std::string &destination) 
{
    try {
        filesystem::copy_file(source, destination, filesystem::copy_option::overwrite_if_exists);
    }
    catch(const boost::filesystem::filesystem_error &err) {
        lastError = err.what();
    }
    return fileExists(destination);
}

std::string FileSystemOperations::extractFolderPath(const std::string &fullPathWithFileName) const 
{
    if (fullPathWithFileName.empty()) {
        return ""s;
    }
    return filesystem::path(fullPathWithFileName).parent_path().string();
}
