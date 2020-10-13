#include "version.h"
#include <stdexcept>

using namespace std;

Version::Version(unsigned char major, unsigned char minor, unsigned char patch)
    : major(major), minor(minor), patch(patch)
{
    if (major == 0 && minor == 0 && patch == 0) {
        throw invalid_argument("version cannot be empty. (0.0.0)");
    }
}

unsigned char Version::getMajor() const
{
    return major;
}
