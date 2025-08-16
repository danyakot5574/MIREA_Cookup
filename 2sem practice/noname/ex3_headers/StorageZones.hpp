#ifndef _STORAGE_ZONES_HPP_
#define _STORAGE_ZONES_HPP_

#include <array>
#include <string>

class StorageZones
{
    std::array<std::string, 5> storageZones;

    class Item
    {
        StorageZones* current {nullptr};
        short index {-1};

    public:
        
        Item(StorageZones* object, short index) : current(object), index(index) {}

        operator std::string() const
        {
            if(index > 4 || index < 0) return "null";

            return current->storageZones[index];
        }

        void operator = (const std::string& newZone)
        {
            if(index > 4 || index < 0) return;

            current->storageZones[index] = newZone;
        }
    };

public:

    StorageZones(const std::array<std::string, 5>& newStorage) : storageZones(newStorage) {}
    StorageZones(const std::string& zone1, const std::string& zone2, const std::string& zone3, const std::string& zone4, const std::string& zone5) : storageZones({zone1, zone2, zone3, zone4, zone5}) {}

    bool Contains(const std::string& zone)
    {
        for(int i = 0; i < storageZones.size(); ++i)
            if(storageZones[i] == zone)
                return true;
        
        return false;
    }

    std::string GetStringOfZones() const
    {
        std::string result = "";

        for(std::string x : storageZones)
            result += x + ' ';

        return result;
    }

    Item operator [] (short index)
    { return Item(this, index); }
};

#endif