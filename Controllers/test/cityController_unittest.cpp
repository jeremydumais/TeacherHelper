#include "cityController.h"
#include <gtest/gtest.h>
#include <memory>

using namespace std;

class FakeCityStorage : public IManagementItemStorage<City>
{
public:
    FakeCityStorage()
		: cities(std::list<City> {
			City(1, "New York"),
			City(2, "Los Angeles")
		}) {}
    std::list<City> getAllItems() override { return cities;	}
    const std::string &getLastError() const override { return lastError; }
    bool insertItem(const City &city) override { return true; }
    bool updateItem(const City &city) override { return true; }
    bool deleteItem(size_t id) override { return true; }
private:
	std::list<City> cities;
    std::string lastError;
};

class CityControllerTest : public ::testing::Test
{
public:
	CityControllerTest()
	  : controller(DatabaseConnection("nulldb"), 
				   unique_ptr<IManagementItemStorage<City>>(make_unique<FakeCityStorage>()))
	{}
	CityController controller;
};

TEST(CityController_Constructor, ValidArguments_ReturnSuccess)
{
	CityController controller(DatabaseConnection("nulldb"), unique_ptr<IManagementItemStorage<City>>(make_unique<FakeCityStorage>()));
}

TEST_F(CityControllerTest, getCities_Return2Cities)
{
	controller.loadCities();
	ASSERT_EQ(2, controller.getCities().size());
	size_t index {0};
	for(const auto &city : controller.getCities()) {
		if (index == 0) {
			ASSERT_EQ("New York", city.getName());
		}
		else if (index == 1) {
			ASSERT_EQ("Los Angeles", city.getName());
		}
		index++;
	}
}

TEST_F(CityControllerTest, findCity_WithIdZero_ReturnNullPtr) 
{
	controller.loadCities();
	ASSERT_EQ(nullptr, controller.findCity(0));
}

TEST_F(CityControllerTest, findCity_WithIdOne_ReturnNewYork) 
{
	controller.loadCities();
	ASSERT_NE(nullptr, controller.findCity(1));
	ASSERT_EQ("New York", controller.findCity(1)->getName());
}

TEST_F(CityControllerTest, findCity_WithIdTwo_ReturnLosAngeles) 
{
	controller.loadCities();
	ASSERT_NE(nullptr, controller.findCity(2));
	ASSERT_EQ("Los Angeles", controller.findCity(2)->getName());
}

TEST_F(CityControllerTest, isNewNameAvailableForAdd_WithNonExistantName_ReturnTrue)
{
	controller.loadCities();
	ASSERT_TRUE(controller.isNewNameAvailableForAdd("Chicago"));
}

TEST_F(CityControllerTest, isNewNameAvailableForAdd_WithExistantName_ReturnFalse)
{
	controller.loadCities();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("New York"));
}

TEST_F(CityControllerTest, isNewNameAvailableForAdd_WithExistantNameDifferentCase_ReturnFalse)
{
	controller.loadCities();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("NEW YoRk"));
}

TEST_F(CityControllerTest, isNewNameAvailableForAdd_WithExistantNameBeginWithSpaces_ReturnFalse)
{
	controller.loadCities();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("   New York"));
}

TEST_F(CityControllerTest, isNewNameAvailableForAdd_WithExistantNameEndWithSpaces_ReturnFalse)
{
	controller.loadCities();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("New York   "));
}

TEST_F(CityControllerTest, isNewNameAvailableForAdd_WithExistantNameBeginAndEndWithSpaces_ReturnFalse)
{
	controller.loadCities();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("   New York   "));
}

TEST_F(CityControllerTest, isNewNameAvailableForAdd_WithEmpty_ReturnFalse)
{
	controller.loadCities();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd(""));
}

TEST_F(CityControllerTest, isNewNameAvailableForAdd_WithOnlySpaces_ReturnFalse)
{
	controller.loadCities();
	ASSERT_FALSE(controller.isNewNameAvailableForAdd("   "));
}

TEST_F(CityControllerTest, isNewNameAvailableForUpdate_WithNonExistantName_ReturnTrue)
{
	controller.loadCities();
	ASSERT_TRUE(controller.isNewNameAvailableForUpdate("Chicago", 1));
}

TEST_F(CityControllerTest, isNewNameAvailableForUpdate_WithExistantNameButSameRecord_ReturnTrue)
{
	controller.loadCities();
	ASSERT_TRUE(controller.isNewNameAvailableForUpdate("New York", 1));
}

TEST_F(CityControllerTest, isNewNameAvailableForUpdate_WithExistantNameButDifferentRecord_ReturnFalse)
{
	controller.loadCities();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("New York", 2));
}

TEST_F(CityControllerTest, isNewNameAvailableForUpdate_WithExistantNameDifferentCaseDifferentRecord_ReturnFalse)
{
	controller.loadCities();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("NEW YoRk", 2));
}

TEST_F(CityControllerTest, isNewNameAvailableForUpdate_WithExistantNameDifferentCaseSameRecord_ReturnTrue)
{
	controller.loadCities();
	ASSERT_TRUE(controller.isNewNameAvailableForUpdate("NEW YoRk", 1));
}

TEST_F(CityControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginWithSpacesDifferentRecord_ReturnFalse)
{
	controller.loadCities();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("   New York", 2));
}

TEST_F(CityControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginWithSpacesSameRecord_ReturnTrue)
{
	controller.loadCities();
	ASSERT_TRUE(controller.isNewNameAvailableForUpdate("   New York", 1));
}

TEST_F(CityControllerTest, isNewNameAvailableForUpdate_WithExistantNameEndWithSpaces_ReturnFalse)
{
	controller.loadCities();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("New York   ", 2));
}

TEST_F(CityControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginAndEndWithSpaces_ReturnFalse)
{
	controller.loadCities();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("   New York   ", 2));
}

TEST_F(CityControllerTest, isNewNameAvailableForUpdate_WithEmpty_ReturnFalse)
{
	controller.loadCities();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("", 1));
}

TEST_F(CityControllerTest, isNewNameAvailableForUpdate_WithOnlySpaces_ReturnFalse)
{
	controller.loadCities();
	ASSERT_FALSE(controller.isNewNameAvailableForUpdate("   ", 1));
}

