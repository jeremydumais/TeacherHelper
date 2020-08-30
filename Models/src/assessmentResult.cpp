#include "assessmentResult.h"
#include <stdexcept>

using namespace std;

AssessmentResult::AssessmentResult(const Student &student, const float result, const std::string &comments)
    : AssessmentResult(0, student, result, comments)
{
}

AssessmentResult::AssessmentResult(size_t id, 
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

bool operator==(const AssessmentResult &lhs, const AssessmentResult &rhs)
{
    return lhs.id == rhs.id &&
           lhs.student == rhs.student;
}

size_t AssessmentResult::getId() const
{
    return id;
}

const Student& AssessmentResult::getStudent() const
{
    return student;
}

float AssessmentResult::getResult() const
{
    return result;
}

const std::string& AssessmentResult::getComments() const
{
    return comments;
}

void AssessmentResult::setStudent(const Student &student) 
{
    this->student = student;
}

void AssessmentResult::setResult(const float result) 
{
    if (result < 0) {
        throw invalid_argument("result must be a positive number.");
    } 
    this->result = result;
}

void AssessmentResult::setComments(const std::string &comments) 
{
    if (comments.length() > 256) {
        throw invalid_argument("comments must not be larger then 256 chars.");
    }  
    this->comments = comments;
}
