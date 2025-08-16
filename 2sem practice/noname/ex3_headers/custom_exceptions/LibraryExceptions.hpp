#ifndef _LIBRARY_EXCEPTIONS_HPP_
#define _LIBRARY_EXCEPTIONS_HPP_

#include <exception>
#include <string>

class LibraryException : public std::exception
{
protected:

    std::string msg;

public:

    LibraryException()
    {
        msg = "Error: unknown library exception";
    }

    virtual const char * what() const noexcept
    { return msg.c_str(); }

    virtual ~LibraryException() {}
};

class LibraryIsNotSettedException : public LibraryException
{
public:
    
    LibraryIsNotSettedException()
    {
        msg = "Error: Library isn't setted \n - Howto: SetLibrary(std::map<std::string, Book>*)";
    }

    ~LibraryIsNotSettedException() {}
};

class LibraryIsAlreadySetted : public LibraryException
{
public:

    LibraryIsAlreadySetted()
    {
        msg = "Error: Library is already setted. You can't set library's map twice";
    }

    ~LibraryIsAlreadySetted() {}
};

#endif