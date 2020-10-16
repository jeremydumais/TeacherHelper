#include "version.h"
#include <fmt/format.h>
#include <gtest/gtest.h>
#include <climits>

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

TEST(VersionConstructorFromString, EmptyString_ThrowInvalidArgument)
{
	try
	{
		Version version("");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("version cannot be empty.", err.what());
	}
}

TEST(VersionConstructorFromString, WhiteSpacesString_ThrowInvalidArgument)
{
	try
	{
		Version version("   ");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("version cannot be empty.", err.what());
	}
}

TEST(VersionConstructorFromString, OnlyOnePartString_ThrowInvalidArgument)
{
	try
	{
		Version version("1");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("version must have three parts separated with dots.", err.what());
	}
}

TEST(VersionConstructorFromString, OnlyTwoPartsString_ThrowInvalidArgument)
{
	try
	{
		Version version("1.1");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("version must have three parts separated with dots.", err.what());
	}
}

TEST(VersionConstructorFromString, FourPartsString_ThrowInvalidArgument)
{
	try
	{
		Version version("1.1.1.1");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("version must have three parts separated with dots.", err.what());
	}
}

TEST(VersionConstructorFromString, ThreePartsSeparatedWithUnderscoreString_ThrowInvalidArgument)
{
	try
	{
		Version version("1_2_3");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("version must have three parts separated with dots.", err.what());
	}
}

TEST(VersionConstructorFromString, Version123WithDot_ReturnSuccess)
{
	Version version("1.2.3");
	ASSERT_EQ(1, version.getMajor());
	ASSERT_EQ(2, version.getMinor());
	ASSERT_EQ(3, version.getPatch());
}

TEST(VersionConstructorFromString, Version123456WithDot_ReturnSuccess)
{
	Version version("12.34.56");
	ASSERT_EQ(12, version.getMajor());
	ASSERT_EQ(34, version.getMinor());
	ASSERT_EQ(56, version.getPatch());
}

TEST(VersionConstructorFromString, Version000WithDot_ThrowInvalidArgument)
{
	try
	{
		Version version("0.0.0");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("version cannot be empty. (0.0.0)", err.what());
	}
}

TEST(VersionConstructorFromString, VersionA00WithDot_ThrowInvalidArgument)
{
	try
	{
		Version version("A.0.0");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("The part A cannot be converted to integer", err.what());
	}
}

TEST(VersionConstructorFromString, Version22560WithDot_ThrowOutOfRange)
{
	try
	{
		Version version("2.256.0");
		FAIL();
	}
	catch(out_of_range &err) 
	{
        ASSERT_STREQ(fmt::format("The part 256 must be less than {0}", UCHAR_MAX).c_str(), err.what());
	}
}

TEST(VersionConstructorFromString, Version2Minus10WithDot_ThrowOutOfRange)
{
	try
	{
		Version version("2.-1.0");
		FAIL();
	}
	catch(out_of_range &err) 
	{
        ASSERT_STREQ(fmt::format("The part -1 cannot be negative", UCHAR_MAX).c_str(), err.what());
	}
}

TEST(VersionConstructorFromString, VersionDotDot_ThrowInvalidArgument)
{
	try
	{
		Version version("..");
		FAIL();
	}
	catch(invalid_argument &err) 
	{
        ASSERT_STREQ("One of the part is empty.", err.what());
	}
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