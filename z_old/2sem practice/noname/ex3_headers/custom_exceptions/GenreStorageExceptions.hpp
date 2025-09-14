#ifndef _GENRE_STORAGE_EXCEPTIONS_HPP_
#define _GENRE_STORAGE_EXCEPTIONS_HPP_

#include <exception>
#include <string>

class GenreStorageException : public std::exception
{
protected:

    std::string msg;

public:

    GenreStorageException()
    {
        msg = "Error: unknown genre storage exception";
    }

    virtual const char * what() const noexcept
    { return msg.c_str(); }

    ~GenreStorageException() {}
};

class GenreStorageIsAlreadySetted : public GenreStorageException
{
public:

    GenreStorageIsAlreadySetted()
    {
        msg = "Error: Genre storage is already setted. You can't set genre storage twice";
    }

    ~GenreStorageIsAlreadySetted() {}
};

class GenreStorageIsNotSetted : public GenreStorageException
{
public:

    GenreStorageIsNotSetted()
    {
        msg = "Error: Genre storage is't setted. \n - howto: SetGenreStorage(std::set<std::string>*)";
    }

    ~GenreStorageIsNotSetted() {}
};

#endif