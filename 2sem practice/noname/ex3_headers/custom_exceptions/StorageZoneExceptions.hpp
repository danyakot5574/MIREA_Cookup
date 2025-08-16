#ifndef _STORAGE_ZONES_EXCEPTIONS_HPP_
#define _STORAGE_ZONES_EXCEPTIONS_HPP_

#include <exception>
#include <string>

class StorageZoneException : std::exception
{
protected:

    std::string msg;

public:

    StorageZoneException() 
    {
        msg = "Error: unknown storage error";
    }

    virtual const char * what() const noexcept
    { return msg.c_str(); }
    
    ~StorageZoneException() {}
};

class ZonesNotSettedException : public StorageZoneException
{
public:

    ZonesNotSettedException() 
    {
        msg = "Error: to use storage zones features, previously you need to set storage zones\n - howto: SetStorageZones(const std::vector<std::string, 5>*)";
    }

    
    ~ZonesNotSettedException() {}

};

class ZonesAlreadySettedException : public StorageZoneException
{
public:

    ZonesAlreadySettedException()
    {
        msg = "Error: Storage zones is already setted, you can't set storage zones twice!\n";
    }

    ~ZonesAlreadySettedException() {}

};

class UnknownStorageZoneException : public StorageZoneException
{
public:

    UnknownStorageZoneException()
    {
        msg = "Error: Unknown storage zone name setted. Use storage zone names from setted storage zones array";
    }

    ~UnknownStorageZoneException() {}

};

#endif