#include "cityController.h"
#include <gtest/gtest.h>
#include <memory>

using namespace std;

class FakeCityStorage : public ManagementItemStorageBase<City>
{
public:
    FakeCityStorage()
		: cities(std::list<City> {
			City(1, "New York"),
			City(2, "Los Angeles")
		}),
		ManagementItemStorageBase<City>(DatabaseConnection("nulldb")) {}
    std::list<City> getAllItems() override { return cities;	}
    const std::string &getLastError() const override { return lastError; }
    bool insertItem(const City &city) override { return insertResult; }
    bool updateItem(const City &city) override { return updateResult; }
    QueryResult deleteItem(size_t id) override { return deleteResult; }
	bool isReferentialIntegrityConstraint(size_t id) override { return true; };
	std::string getSelectCommand() const override { return ""; };
    City getItemFromRecord(const IStorageSelectOperation &record) const override { return { 1, "FakeCity" }; };
    std::string getInsertCommand() const override { return ""; };
    std::vector<std::string> getInsertValues(const City &item) const override { return {"", ""}; };
    std::string getUpdateCommand() const override {return ""; };
    std::vector<std::string> getUpdateValues(const City &item) const override { return {"", ""}; };
    std::string getDeleteCommand() const override {return ""; };
    std::vector<std::string> getDeleteValues(size_t id) const override { return {""}; };
    std::string getReferentialIntegrityConstraintsCommand() const override { return ""; };
    std::vector<std::string> getReferentialIntegrityConstraintsValues(size_t id) const override { return {""}; };
	bool insertResult = true;
	bool updateResult = true;
	QueryResult deleteResult = QueryResult::OK;
    std::string lastError;
private:
	std::list<City> cities;
};

class CityControllerTest : public ::testing::Test
{
public:
	CityControllerTest() 
		: fakeStorage { make_unique<FakeCityStorage>()}
	{}

	void prepareController()
	{
		controller = make_unique<CityController>(DatabaseConnection("nulldb"), 
												 std::move(fakeStorage));
	}

	unique_ptr<ManagementItemStorageBase<City>> fakeStorage;								 
	unique_ptr<CityController> controller;
};

TEST(CityController_Constructor, ValidArguments_ReturnSuccess)
{
	CityController controller(DatabaseConnection("nulldb"), unique_ptr<ManagementItemStorageBase<City>>(make_unique<FakeCityStorage>()));
}

TEST_F(CityControllerTest, getCities_Return2Cities)
{
	this->prepareController();
	controller->loadCities();
	ASSERT_EQ(2, controller->getCities().size());
	size_t index {0};
	for(const auto &city : controller->getCities()) {
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
	this->prepareController();
	controller->loadCities();
	ASSERT_EQ(nullptr, controller->findCity(0));
}

TEST_F(CityControllerTest, findCity_WithIdOne_ReturnNewYork) 
{
	this->prepareController();
	controller->loadCities();
	ASSERT_NE(nullptr, controller->findCity(1));
	ASSERT_EQ("New York", controller->findCity(1)->getName());
}

TEST_F(CityControllerTest, findCity_WithIdTwo_ReturnLosAngeles) 
{
	this->prepareController();
	controller->loadCities();
	ASSERT_NE(nullptr, controller->findCity(2));
	ASSERT_EQ("Los Angeles", controller->findCity(2)->getName());
}

TEST_F(CityControllerTest, isNewNameAvailableForAdd_WithNonExistantName_ReturnTrue)
{
	this->prepareController();
	controller->loadCities();
	ASSERT_TRUE(controller->isNewNameAvailableForAdd("Chicago"));
}

TEST_F(CityControllerTest, isNewNameAvailableForAdd_WithExistantName_ReturnFalse)
{
	this->prepareController();
	controller->loadCities();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("New York"));
}

TEST_F(CityControllerTest, isNewNameAvailableForAdd_WithExistantNameDifferentCase_ReturnFalse)
{
	this->prepareController();
	controller->loadCities();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("NEW YoRk"));
}

TEST_F(CityControllerTest, isNewNameAvailableForAdd_WithExistantNameBeginWithSpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadCities();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("   New York"));
}

TEST_F(CityControllerTest, isNewNameAvailableForAdd_WithExistantNameEndWithSpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadCities();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("New York   "));
}

TEST_F(CityControllerTest, isNewNameAvailableForAdd_WithExistantNameBeginAndEndWithSpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadCities();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("   New York   "));
}

TEST_F(CityControllerTest, isNewNameAvailableForAdd_WithEmpty_ReturnFalse)
{
	this->prepareController();
	controller->loadCities();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd(""));
}

TEST_F(CityControllerTest, isNewNameAvailableForAdd_WithOnlySpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadCities();
	ASSERT_FALSE(controller->isNewNameAvailableForAdd("   "));
}

TEST_F(CityControllerTest, isNewNameAvailableForUpdate_WithNonExistantName_ReturnTrue)
{
	this->prepareController();
	controller->loadCities();
	ASSERT_TRUE(controller->isNewNameAvailableForUpdate("Chicago", 1));
}

TEST_F(CityControllerTest, isNewNameAvailableForUpdate_WithExistantNameButSameRecord_ReturnTrue)
{
	this->prepareController();
	controller->loadCities();
	ASSERT_TRUE(controller->isNewNameAvailableForUpdate("New York", 1));
}

TEST_F(CityControllerTest, isNewNameAvailableForUpdate_WithExistantNameButDifferentRecord_ReturnFalse)
{
	this->prepareController();
	controller->loadCities();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("New York", 2));
}

TEST_F(CityControllerTest, isNewNameAvailableForUpdate_WithExistantNameDifferentCaseDifferentRecord_ReturnFalse)
{
	this->prepareController();
	controller->loadCities();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("NEW YoRk", 2));
}

TEST_F(CityControllerTest, isNewNameAvailableForUpdate_WithExistantNameDifferentCaseSameRecord_ReturnTrue)
{
	this->prepareController();
	controller->loadCities();
	ASSERT_TRUE(controller->isNewNameAvailableForUpdate("NEW YoRk", 1));
}

TEST_F(CityControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginWithSpacesDifferentRecord_ReturnFalse)
{
	this->prepareController();
	controller->loadCities();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("   New York", 2));
}

TEST_F(CityControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginWithSpacesSameRecord_ReturnTrue)
{
	this->prepareController();
	controller->loadCities();
	ASSERT_TRUE(controller->isNewNameAvailableForUpdate("   New York", 1));
}

TEST_F(CityControllerTest, isNewNameAvailableForUpdate_WithExistantNameEndWithSpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadCities();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("New York   ", 2));
}

TEST_F(CityControllerTest, isNewNameAvailableForUpdate_WithExistantNameBeginAndEndWithSpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadCities();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("   New York   ", 2));
}

TEST_F(CityControllerTest, isNewNameAvailableForUpdate_WithEmpty_ReturnFalse)
{
	this->prepareController();
	controller->loadCities();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("", 1));
}

TEST_F(CityControllerTest, isNewNameAvailableForUpdate_WithOnlySpaces_ReturnFalse)
{
	this->prepareController();
	controller->loadCities();
	ASSERT_FALSE(controller->isNewNameAvailableForUpdate("   ", 1));
}

TEST_F(CityControllerTest, insertCity_WithCityThatWillSuccess_ReturnTrue) 
{
	this->prepareController();
	ASSERT_TRUE(controller->insertCity(City { "Chicago" }));
}

TEST_F(CityControllerTest, insertCity_WithCityThatWillFailed_ReturnFailed) 
{
	auto fakeCityStorage = dynamic_cast<FakeCityStorage*>(this->fakeStorage.get());
	fakeCityStorage->insertResult = false;
	fakeCityStorage->lastError = "An insert error occurred";

	this->prepareController();
	ASSERT_FALSE(controller->insertCity(City { "Chicago" }));
	ASSERT_EQ("An insert error occurred", controller->getLastError());
}

TEST_F(CityControllerTest, updateCity_WithCityThatWillSuccess_ReturnTrue) 
{
	this->prepareController();
	ASSERT_TRUE(controller->updateCity(City { "New York" }));
}

TEST_F(CityControllerTest, updateCity_WithCityThatWillFailed_ReturnFailed) 
{
	auto fakeCityStorage = dynamic_cast<FakeCityStorage*>(this->fakeStorage.get());
	fakeCityStorage->updateResult = false;
	fakeCityStorage->lastError = "An update error occurred";

	this->prepareController();
	ASSERT_FALSE(controller->updateCity(City { "Chicago" }));
	ASSERT_EQ("An update error occurred", controller->getLastError());
}

TEST_F(CityControllerTest, deleteCity_WithCityThatWillSuccess_ReturnTrue) 
{
	this->prepareController();
	ASSERT_TRUE(controller->deleteCity(1));
}

TEST_F(CityControllerTest, deleteCity_WithCityThatWillFailedWithConstraintError_ReturnFailed) 
{
	auto fakeCityStorage = dynamic_cast<FakeCityStorage*>(this->fakeStorage.get());
	fakeCityStorage->deleteResult = QueryResult::CONSTRAINTERROR;

	this->prepareController();
	ASSERT_FALSE(controller->deleteCity(1));
	ASSERT_EQ("Unable to delete the city because it is used by another item. (Probably a school)", controller->getLastError());
}

TEST_F(CityControllerTest, deleteCity_WithCityThatWillFailedWithGenericError_ReturnFailed) 
{
	auto fakeCityStorage = dynamic_cast<FakeCityStorage*>(this->fakeStorage.get());
	fakeCityStorage->deleteResult = QueryResult::ERROR;
	fakeCityStorage->lastError = "An generic error occurred";

	this->prepareController();
	ASSERT_FALSE(controller->deleteCity(1));
	ASSERT_EQ("An generic error occurred", controller->getLastError());
}