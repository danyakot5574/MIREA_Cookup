#ifndef _GENRE_STORAGE_HPP_
#define _GENRE_STORAGE_HPP_

#include <set>
#include <string>

#include "Book.hpp"
#include "custom_exceptions/GenreStorageExceptions.hpp"

// ==========

class GenreStorage
{
    std::set<std::string> genreStorage;

public:

    void AddGenre(const std::string& genreName)
    {
        genreStorage.insert(genreName);
    }

    void AddGenre(const Book& book)
    {
        genreStorage.insert(book.GetGenre());
    }

    void RemoveGenre(const std::string& genreName)
    {
        if(genreStorage.count(genreName) == 0)
            return;

        genreStorage.erase(genreName);
    }

    void RemoveGenre(const Book& book)
    {
        if(genreStorage.count(book.GetGenre()) == 0)
            return;

        genreStorage.erase(book.GetGenre());
    }

    bool Contains(const std::string& genreName)
    {
        return genreStorage.count(genreName);
    }

    std::string GetStringOfGenres() const
    {
        std::string newString = "";

        for(std::string x : genreStorage)
            newString += x + ' ';

        return newString;
    }
};


#endif