#include "version.h"
#include <boost/algorithm/string.hpp>
#include <fmt/format.h>
#include <limits>
#include <stdexcept>
#include <vector>

using namespace std;

Version::Version(unsigned char major, unsigned char minor, unsigned char patch)
    : mMajor(major), mMinor(minor), mPatch(patch)
{
    if (mMajor == 0 && mMinor == 0 && mPatch == 0) {
        throw invalid_argument("version cannot be empty. (0.0.0)");
    }
}

Version::Version(const std::string &value) 
{
    if (boost::trim_copy(value)=="") {
        throw invalid_argument("version cannot be empty.");
    }
    //Split the string into parts based on the dot separator
    vector<string> versionParts;
    boost::split(versionParts, value, boost::is_any_of("."));
    if (versionParts.size() != 3) {
        throw invalid_argument("version must have three parts separated with dots.");
    }
    mMajor = getVersionPartFromString(versionParts[0]);
    mMinor = getVersionPartFromString(versionParts[1]);
    mPatch = getVersionPartFromString(versionParts[2]);

    if (mMajor == 0 && mMinor == 0 && mPatch == 0) {
        throw invalid_argument("version cannot be empty. (0.0.0)");
    }
}

unsigned char Version::getVersionPartFromString(const string &part) const
{
    if (boost::trim_copy(part) == "") {
        throw invalid_argument("One of the part is empty.");
    }

    int partConvertedToInt {0};
    try {
        partConvertedToInt = stoi(part);
    }
    catch(invalid_argument &err) {
        throw invalid_argument(fmt::format("The part {0} cannot be converted to integer", part));
    }
    if (partConvertedToInt < 0) {
        throw out_of_range(fmt::format("The part {0} cannot be negative", to_string(partConvertedToInt)));
    }
    if (partConvertedToInt > UCHAR_MAX) {
        throw out_of_range(fmt::format("The part {0} must be less than {1}", to_string(partConvertedToInt), UCHAR_MAX));
    }
    return static_cast<unsigned char>(partConvertedToInt);
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