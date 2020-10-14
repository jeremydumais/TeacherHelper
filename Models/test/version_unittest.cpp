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

TEST(VersionGetMajor, Version001_Return0)
{
	Version version(0, 0, 1);
	ASSERT_EQ(0, version.getMajor());
}

TEST(VersionGetMinor, Version001_Return0)
{
	Version version(0, 0, 1);
	ASSERT_EQ(0, version.getMinor());
}

TEST(VersionGetMinor, Version010_Return1)
{
	Version version(0, 1, 0);
	ASSERT_EQ(1, version.getMinor());
}

TEST(VersionGetPatch, Version001_Return1)
{
	Version version(0, 0, 1);
	ASSERT_EQ(1, version.getPatch());
}

TEST(VersionGetPatch, Version110_Return0)
{
	Version version(1, 1, 0);
	ASSERT_EQ(0, version.getPatch());
}

TEST(VersionStr, Version110_ReturnString110)
{
	Version version(1, 1, 0);
	ASSERT_EQ("1.1.0", version.str());
}

TEST(VersionStr, Version1128444_ReturnString1128255)
{
	Version version(1, 128, 255);
	ASSERT_EQ("1.128.255", version.str());
}

TEST(VersionOperatorEqual, Version110Twice_ReturnTrue)
{
	Version version1(1, 1, 0);
	Version version2(1, 1, 0);
	ASSERT_EQ(version1, version2);
}

TEST(VersionOperatorEqual, Version224Twice_ReturnTrue)
{
	Version version1(2, 2, 4);
	Version version2(2, 2, 4);
	ASSERT_EQ(version1, version2);
}

TEST(VersionOperatorEqual, Version110And224_ReturnFalse)
{
	Version version1(1, 1, 0);
	Version version2(2, 2, 4);
	ASSERT_FALSE(version1 == version2);
}

TEST(VersionOperatorNotEqual, Version110Twice_ReturnFalse)
{
	Version version1(1, 1, 0);
	Version version2(1, 1, 0);
	ASSERT_FALSE(version1 != version2);
}

TEST(VersionOperatorNotEqual, Version111And110_ReturnTrue)
{
	Version version1(1, 1, 1);
	Version version2(1, 1, 0);
	ASSERT_NE(version1, version2);
}

TEST(VersionOperatorNotEqual, Version001And110_ReturnTrue)
{
	Version version1(0, 0, 1);
	Version version2(1, 1, 0);
	ASSERT_NE(version1, version2);
}

TEST(VersionOperatorLessThan, Version110Twice_ReturnFalse)
{
	Version version1(1, 1, 0);
	Version version2(1, 1, 0);
	ASSERT_FALSE(version1 < version2);
}

TEST(VersionOperatorLessThan, MajorSmaller_ReturnTrue)
{
	Version version1(0, 1, 0);
	Version version2(1, 1, 0);
	ASSERT_LT(version1, version2);
}

TEST(VersionOperatorLessThan, MajorGreater_ReturnFalse)
{
	Version version1(2, 1, 0);
	Version version2(1, 1, 0);
	ASSERT_FALSE(version1 < version2);
}

TEST(VersionOperatorLessThan, AllFieldsGreaterExceptMajorSmaller_ReturnTrue)
{
	Version version1(0, 12, 20);
	Version version2(1, 1, 0);
	ASSERT_LT(version1, version2);
}

TEST(VersionOperatorLessThan, MinorSmaller_ReturnTrue)
{
	Version version1(1, 0, 0);
	Version version2(1, 1, 0);
	ASSERT_LT(version1, version2);
}

TEST(VersionOperatorLessThan, MinorGreater_ReturnFalse)
{
	Version version1(1, 2, 0);
	Version version2(1, 1, 0);
	ASSERT_FALSE(version1 < version2);
}

TEST(VersionOperatorLessThan, AllFieldsGreaterExceptMinorSmaller_ReturnFalse)
{
	Version version1(2, 0, 20);
	Version version2(1, 1, 0);
	ASSERT_FALSE(version1 < version2);
}

TEST(VersionOperatorLessThan, PatchSmaller_ReturnTrue)
{
	Version version1(1, 1, 0);
	Version version2(1, 1, 1);
	ASSERT_LT(version1, version2);
}

TEST(VersionOperatorLessThan, PatchGreater_ReturnFalse)
{
	Version version1(1, 1, 1);
	Version version2(1, 1, 0);
	ASSERT_FALSE(version1 < version2);
}

TEST(VersionOperatorLessThan, AllFieldsGreaterExceptPatchSmaller_ReturnFalse)
{
	Version version1(2, 12, 0);
	Version version2(1, 1, 1);
	ASSERT_FALSE(version1 < version2);
}

TEST(VersionOperatorLessEqual, Version110Twice_ReturnTrue)
{
	Version version1(1, 1, 0);
	Version version2(1, 1, 0);
	ASSERT_LE(version1, version2);
}

TEST(VersionOperatorLessEqual, Version100And110_ReturnTrue)
{
	Version version1(1, 0, 0);
	Version version2(1, 1, 0);
	ASSERT_LE(version1, version2);
}

TEST(VersionOperatorLessEqual, Version200And110_ReturnFalse)
{
	Version version1(2, 0, 0);
	Version version2(1, 1, 0);
	ASSERT_FALSE(version1 <= version2);
}

TEST(VersionOperatorLessEqual, MinorGreater_ReturnFalse)
{
	Version version1(1, 2, 0);
	Version version2(1, 1, 0);
	ASSERT_FALSE(version1 <= version2);
}

TEST(VersionOperatorLessEqual, PatchGreater_ReturnFalse)
{
	Version version1(1, 1, 1);
	Version version2(1, 1, 0);
	ASSERT_FALSE(version1 <= version2);
}

TEST(VersionOperatorLessEqual, AllFieldsGreater_ReturnFalse)
{
	Version version1(2, 2, 1);
	Version version2(1, 1, 0);
	ASSERT_FALSE(version1 <= version2);
}

TEST(VersionOperatorLessEqual, AllFieldsSmaller_ReturnTrue)
{
	Version version1(1, 1, 1);
	Version version2(2, 2, 2);
	ASSERT_LE(version1, version2);
}

TEST(VersionOperatorGreaterThan, Version110Twice_ReturnFalse)
{
	Version version1(1, 1, 0);
	Version version2(1, 1, 0);
	ASSERT_FALSE(version1 > version2);
}

TEST(VersionOperatorGreaterThan, MajorSmaller_ReturnFalse)
{
	Version version1(0, 1, 1);
	Version version2(1, 1, 1);
	ASSERT_FALSE(version1 > version2);
}

TEST(VersionOperatorGreaterThan, MinorSmaller_ReturnFalse)
{
	Version version1(1, 0, 1);
	Version version2(1, 1, 1);
	ASSERT_FALSE(version1 > version2);
}

TEST(VersionOperatorGreaterThan, PatchSmaller_ReturnFalse)
{
	Version version1(1, 1, 0);
	Version version2(1, 1, 1);
	ASSERT_FALSE(version1 > version2);
}

TEST(VersionOperatorGreaterThan, AllFieldsSmaller_ReturnFalse)
{
	Version version1(1, 1, 1);
	Version version2(2, 2, 2);
	ASSERT_FALSE(version1 > version2);
}

TEST(VersionOperatorGreaterThan, MajorGreater_ReturnTrue)
{
	Version version1(2, 1, 1);
	Version version2(1, 1, 1);
	ASSERT_GT(version1, version2);
}

TEST(VersionOperatorGreaterThan, MinorGreater_ReturnTrue)
{
	Version version1(1, 2, 1);
	Version version2(1, 1, 1);
	ASSERT_GT(version1, version2);
}

TEST(VersionOperatorGreaterThan, PatchGreater_ReturnTrue)
{
	Version version1(1, 1, 2);
	Version version2(1, 1, 1);
	ASSERT_GT(version1, version2);
}

TEST(VersionOperatorGreaterThan, AllFieldsGreater_ReturnTrue)
{
	Version version1(2, 2, 2);
	Version version2(1, 1, 1);
	ASSERT_GT(version1, version2);
}

TEST(VersionOperatorGreaterThan, MinorAndPatchSmallerAndMajorGreater_ReturnTrue)
{
	Version version1(2, 1, 1);
	Version version2(1, 13, 13);
	ASSERT_GT(version1, version2);
}

TEST(VersionOperatorGreaterEqual, Version110Twice_ReturnTrue)
{
	Version version1(1, 1, 0);
	Version version2(1, 1, 0);
	ASSERT_GE(version1, version2);
}

TEST(VersionOperatorGreaterEqual, MajorSmaller_ReturnFalse)
{
	Version version1(0, 1, 1);
	Version version2(1, 1, 1);
	ASSERT_FALSE(version1 >= version2);
}

TEST(VersionOperatorGreaterEqual, MinorSmaller_ReturnFalse)
{
	Version version1(1, 0, 1);
	Version version2(1, 1, 1);
	ASSERT_FALSE(version1 >= version2);
}

TEST(VersionOperatorGreaterEqual, PatchSmaller_ReturnFalse)
{
	Version version1(1, 1, 0);
	Version version2(1, 1, 1);
	ASSERT_FALSE(version1 >= version2);
}

TEST(VersionOperatorGreaterEqual, AllFieldsSmaller_ReturnFalse)
{
	Version version1(1, 1, 1);
	Version version2(2, 2, 2);
	ASSERT_FALSE(version1 >= version2);
}

TEST(VersionOperatorGreaterEqual, MajorGreater_ReturnTrue)
{
	Version version1(2, 1, 1);
	Version version2(1, 1, 1);
	ASSERT_GT(version1, version2);
}

TEST(VersionOperatorGreaterEqual, MinorGreater_ReturnTrue)
{
	Version version1(1, 2, 1);
	Version version2(1, 1, 1);
	ASSERT_GT(version1, version2);
}

TEST(VersionOperatorGreaterEqual, PatchGreater_ReturnTrue)
{
	Version version1(1, 1, 2);
	Version version2(1, 1, 1);
	ASSERT_GT(version1, version2);
}

TEST(VersionOperatorGreaterEqual, AllFieldsGreater_ReturnTrue)
{
	Version version1(2, 2, 2);
	Version version2(1, 1, 1);
	ASSERT_GT(version1, version2);
}

TEST(VersionOperatorGreaterEqual, MinorAndPatchSmallerAndMajorGreater_ReturnTrue)
{
	Version version1(2, 1, 1);
	Version version2(1, 13, 13);
	ASSERT_GT(version1, version2);
}
