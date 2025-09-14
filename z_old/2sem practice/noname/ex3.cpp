#include <iostream>

#include <string>
#include <array>
#include <set>
#include <map>

#include "ex3_headers/Author.hpp"
#include "ex3_headers/Book.hpp"
#include "ex3_headers/GenreStorage.hpp"``
#include "ex3_headers/Library.hpp"

int main()
{
    const std::array<std::string, 5> customZones {
        "earlyReads",  
        "youngMinds",  
        "teenBooks",  
        "matureReads",  
        "adultBooks"  
    };

    std::set<std::string> genres;
    std::map<std::string, Book> libeb;

    Author coolGuy = {"Charles", "Petzold"};

    Book programming = {"CODE", "Programming", coolGuy};

    SetStorageZones(&customZones);
    SetGenreStorage(genreStorage);
    SetLibrary(lib);

    AddGenreToStorage("Sigmo");
    AddGenreToStorage(programming);

    AddBookToLibrary(programming);

    RemoveBookFromLibrary("Super Book");

    std::cout << GetStringOfGenres() << std::endl;

    // programming.SetZone("adultBooks");

    // try
    // {
    //     SetStorageZones(&customZones);
    // }
    // catch(const StorageZoneException& e)
    // {
    //     std::cerr << e.what() << '\n';
    // }

    std::cout << programming.GetZone() << std::endl;

    return 0;
}

/*
=Classes=

--- Book
- String genre
- String title
- Author author

--- Author
- String firstName
- String lastName
- String pantronymic


=Functions=

*/