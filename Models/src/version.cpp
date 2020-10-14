#include "version.h"
#include <fmt/format.h>
#include <stdexcept>

using namespace std;

Version::Version(unsigned char major, unsigned char minor, unsigned char patch)
    : mMajor(major), mMinor(minor), mPatch(patch)
{
    if (mMajor == 0 && mMinor == 0 && mPatch == 0) {
        throw invalid_argument("version cannot be empty. (0.0.0)");
    }
}

unsigned char Version::getMajor() const
{
    return mMajor;
}

unsigned char Version::getMinor() const
{
    return mMinor;
}

unsigned char Version::getPatch() const
{
    return mPatch;
}

std::string Version::str() const
{
    return fmt::format("{0}.{1}.{2}",
                       to_string(mMajor),
                       to_string(mMinor),
                       to_string(mPatch));
}


bool operator==(const Version &lhs, const Version &rhs) 
{
    return (lhs.mMajor == rhs.mMajor &&
            lhs.mMinor == rhs.mMinor &&
            lhs.mPatch == rhs.mPatch);
}

bool operator!=(const Version &lhs, const Version &rhs) 
{
    return !(lhs==rhs);
}

bool operator<(const Version &lhs, const Version &rhs) 
{
    return (lhs.mMajor < rhs.mMajor ||
            (lhs.mMajor == rhs.mMajor && lhs.mMinor < rhs.mMinor) ||
            (lhs.mMajor == rhs.mMajor && lhs.mMinor == rhs.mMinor && lhs.mPatch < rhs.mPatch));
}

bool operator<=(const Version &lhs, const Version &rhs) 
{
    return (lhs == rhs || lhs < rhs);
}

bool operator>(const Version &lhs, const Version &rhs) 
{
    return !(lhs <= rhs);
}

bool operator>=(const Version &lhs, const Version &rhs) 
{
    return !(lhs < rhs);
}