#include "testResult.h"
#include <stdexcept>

using namespace std;

TestResult::TestResult(const Student &student, const float result, const std::string &comments)
    : TestResult(0, student, result, comments)
{
}

TestResult::TestResult(size_t id, 
                     const Student &student, 
                     const float result, 
                     const std::string &comments)
    : id(id), 
      student(student), 
      result(result), 
      comments(comments)
{
    if (result < 0) {
        throw invalid_argument("result must be a positive number.");
    } 
    if (comments.length() > 256) {
        throw invalid_argument("comments must not be larger then 256 chars.");
    }   
}

size_t TestResult::getId() const
{
    return id;
}

const Student& TestResult::getStudent() const
{
    return student;
}

const float TestResult::getResult() const
{
    return result;
}

const std::string& TestResult::getComments() const
{
    return comments;
}

void TestResult::setStudent(const Student &student) 
{
    this->student = student;
}

void TestResult::setResult(const float result) 
{
    if (result < 0) {
        throw invalid_argument("result must be a positive number.");
    } 
    this->result = result;
}

void TestResult::setComments(const std::string &comments) 
{
    if (comments.length() > 256) {
        throw invalid_argument("comments must not be larger then 256 chars.");
    }  
    this->comments = comments;
}
