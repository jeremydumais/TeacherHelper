#include "fileSystemOperations.h"
#include <boost/filesystem.hpp>

using namespace boost;
using namespace std;

bool FileSystemOperations::fileExists(const string &fileName) 
{
    return filesystem::exists(fileName);
}
