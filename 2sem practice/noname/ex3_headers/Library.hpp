#ifndef _LIBRARY_HPP_
#define _LIBRARY_HPP_

#include <iostream>
#include <map>

#include "Book.hpp"

#include "StorageZones.hpp"
#include "GenreStorage.hpp"

#include "custom_exceptions/LibraryExceptions.hpp"

/*
Надо написать:

логику для взаимодействия библиотеки и хранилища жанров 
    - При добавлении книги, добавить её жанр в хранилище

логику для взаимодействия библитеки и хранилища зон:
    - При добавлении книги, проверка, есть ли в библиотеке зона для неё, если нет, то изменить зону книги на null
    - При изменении названия зоны, зоны всех книг текущей библиотеки меняются либо на null либо на название этой зоны (задать параметром bool в функции)
    - Возможно стоит сделать класс StorageZones приватным подклассом Library
*/

class Library
{
    std::map<std::string, Book> library;

    GenreStorage genres;
    StorageZones zones;

public:

    Library(const StorageZones& newStorageZones) : library(), zones(newStorageZones) {}

    GenreStorage& GetGenreStorage() 
    { return genres; }

    StorageZones& GetStorageZones()
    { return zones; }

    void AddBook(const Book& newBook)
    {
        Book bookCopy = newBook;

        if(bookCopy.GetZone() != "null" && !zones.Contains(bookCopy.GetZone()))
        {
            bookCopy.SetZone("null");
            std::cout << "Warning: You tried to add book with unavailable zone. Book's zone setted into \"null\"";
        }

        genres.AddGenre(bookCopy);

        library.insert({bookCopy.GetTitle(), bookCopy});
    }

    void RemoveBook(const std::string& bookTitle)
    {
        try
        {
            library.at(bookTitle);
        }
        catch(const std::exception& e)
        {
            std::cerr << "Warning: You tried to delete a non-existent element from library. The process will continue" << '\n';
            return;
        }

        library.erase(bookTitle);
    }

    void RemoveBook(const Book& book)
    {
        try
        {
            library.at(book.GetTitle());
        }
        catch(const std::exception& e)
        {
            std::cerr << "Warning: You tried to delete a non-existent element from library. The process will continue" << '\n';
            return;
        }

        library.erase(book.GetTitle());
    }

    const Book& TakeBook(const std::string& bookTitle)
    {
        Book* currentBook;

        try
        {
            currentBook = &library.at(bookTitle);
        }
        catch(const std::exception& e)
        {
            std::cerr << "Warning: You tried to \"take\" a non-existent element from library. The process will continue" << '\n';
            return Book();
        } 

        if(currentBook->GetAvailability())
            return *currentBook;
        else
        {
            std::cout << "Warning: You tried to \"take\" a non-available book. The process will continue" << '\n';
            return Book();
        }
    }

    const Book& TakeBook(const Book& book)
    {
        Book* currentBook;

        try
        {
            currentBook = &library.at(book.GetTitle());
        }
        catch(const std::exception& e)
        {
            std::cerr << "Warning: You tried to \"take\" a non-existent element from library. The process will continue" << '\n';
            return Book();
        } 

        if(currentBook->GetAvailability())
            return *currentBook;
        else
        {
            std::cout << "Warning: You tried to \"take\" a non-available book. The process will continue" << '\n';
            return Book();
        }
    }

    void Organize()
    {
        for(auto& [name, book] : library)
        {
            genres.AddGenre(book.GetGenre());

            if(!zones.Contains(book.GetZone()))
                book.SetZone("null");
        }
    }

    std::string GetStringOfLibrary() const
    {
        std::string result = "";

        for(auto& [name, book] : library)
            result += name + ":\t" + book.GetAuthor().GetFirstname() + ' ' + book.GetAuthor().GetLastname() + ' ' + book.GetAuthor().GetPantronymic() + '\t' + book.GetGenre() + '\t' + book.GetZone() + '\n';
        
        return result;
    }

};

#endif