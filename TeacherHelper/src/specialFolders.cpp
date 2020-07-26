#include "specialFolders.h"
#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <fmt/format.h>

using namespace std;

const string SpecialFolders::getUserConfigDirectory()
{
    #ifdef _WIN32
        return fmt::format("{0}\\TeacherHelper\\", 
                           getenv("LOCALAPPDATA"));
    #else
        //Linux system
        return fmt::format("{0}/.config/TeacherHelper/",
                           getenv("HOME"));
    #endif

}