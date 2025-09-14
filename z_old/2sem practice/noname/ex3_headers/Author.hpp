#ifndef _AUTHOR_HPP_
#define _AUTHOR_HPP_

#include <string>

class Author
{
    std::string firstname, lastname, pantronymic;

public:

    Author() : firstname("FirstName"), lastname("LastName"), pantronymic("Pantronymic") {}
    
    Author(const std::string& firstname, const std::string& lastname)
    : firstname(firstname), lastname(lastname), pantronymic("Pantronymic") {}

    Author(const std::string& firstname, const std::string& lastname, const std::string& pantronymic)
    : firstname(firstname), lastname(lastname), pantronymic(pantronymic) {}

    Author(const Author& other) 
    : firstname(other.firstname), lastname(other.lastname), pantronymic(other.pantronymic) {}

    Author(Author&& move)
    : firstname(move.firstname), lastname(move.lastname), pantronymic(move.pantronymic) {}

    // ==========

    void SetFirstname(const std::string& newFirstname)
    { firstname = newFirstname; }

    void SetLastname(const std::string& newLastname)
    { lastname = newLastname; }

    void SetPantronymic(const std::string& newPantronymic)
    { pantronymic = newPantronymic; }

    // ==========

    std::string GetFirstname() const noexcept
    { return firstname; }

    std::string GetLastname() const noexcept
    { return lastname; }

    std::string GetPantronymic() const noexcept
    { return pantronymic; }

    // ==========

    Author operator = (const Author& other)
    {
        firstname = other.firstname;
        lastname = other.lastname;
        pantronymic = other.pantronymic;

        return *this;
    }

    Author operator = (Author&& move) 
    {
        firstname = move.firstname;
        lastname = move.lastname;
        pantronymic = move.pantronymic;

        return *this;
    }

    // ==========

    // ==========

    ~Author() {}

};

#endif