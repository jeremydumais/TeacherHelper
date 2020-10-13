#include "version.h"
#include <gtest/gtest.h>

using namespace std;

TEST(VersionConstructor, Version000_ThrowInvalidArgument)
{
	try
	{
		Version version(0, 0, 0);
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("version cannot be empty. (0.0.0)", err.what());
	}
}

TEST(VersionConstructor, Version100_ReturnSuccess)
{
	Version version(1, 0, 0);
}

TEST(VersionGetMajor, Version100_Return1)
{
	Version version(1, 0, 0);
	ASSERT_EQ(1, version.getMajor());
}

TEST(VersionGetMajor, Version001_Return1)
{
	Version version(0, 0, 1);
	ASSERT_EQ(0, version.getMajor());
}