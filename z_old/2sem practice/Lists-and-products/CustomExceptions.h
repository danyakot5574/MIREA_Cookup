#include <iostream>
#include <string>

class DoublyLinkedListError : public std::exception
{
protected:
    std::string msg;

public:

    DoublyLinkedListError(const std::string& error) : msg(error) {}

    const char* what() const noexcept override 
    {
        return msg.c_str();
    }

    virtual ~DoublyLinkedListError() {}
};

class IncorrectIndexDoublyLinkedListError : public DoublyLinkedListError
{
public:
    IncorrectIndexDoublyLinkedListError(const std::string& error) : DoublyLinkedListError(error) {}

    ~IncorrectIndexDoublyLinkedListError() {}
};

class IncomparableTypesLinkedListError : public DoublyLinkedListError
{
public:
    IncomparableTypesLinkedListError(const std::string& error) : DoublyLinkedListError(error) {}

    ~IncomparableTypesLinkedListError() {}
};

class FileDoesNotExistDoublyLinkedListError : public DoublyLinkedListError
{
public:
    FileDoesNotExistDoublyLinkedListError(const std::string& error) : DoublyLinkedListError(error) {}

    ~FileDoesNotExistDoublyLinkedListError() {}
};

class PushToSharedDoublyLinkedListError : public DoublyLinkedListError
{
public:
    PushToSharedDoublyLinkedListError(const std::string& error) : DoublyLinkedListError(error) {}

    ~PushToSharedDoublyLinkedListError() {}
};

class InsertIntoSharedDoublyLinkedListError : public DoublyLinkedListError
{
public:
    InsertIntoSharedDoublyLinkedListError(const std::string& error) : DoublyLinkedListError(error) {}

    ~InsertIntoSharedDoublyLinkedListError() {}
};

class PopFromSharedDoublyLinkedListError : public DoublyLinkedListError
{
public:
    PopFromSharedDoublyLinkedListError(const std::string& error) : DoublyLinkedListError(error) {}

    ~PopFromSharedDoublyLinkedListError() {}
};

class RemoveFromSharedDoublyLinkedListError : public DoublyLinkedListError
{
public:
    RemoveFromSharedDoublyLinkedListError(const std::string& error) : DoublyLinkedListError(error) {}

    ~RemoveFromSharedDoublyLinkedListError() {}
};

class AssignmentOperationSharedDoublyLinkedListError : public DoublyLinkedListError
{
public:
    AssignmentOperationSharedDoublyLinkedListError(const std::string& error) : DoublyLinkedListError(error) {}

    ~AssignmentOperationSharedDoublyLinkedListError() {}
};

class SummationOperationSharedDoublyLinkedListError : public DoublyLinkedListError
{
public:
    SummationOperationSharedDoublyLinkedListError(const std::string& error) : DoublyLinkedListError(error) {}

    ~SummationOperationSharedDoublyLinkedListError() {}
};

class SubtractionOperationSharedDoublyLinkedListError : public DoublyLinkedListError
{
public:
    SubtractionOperationSharedDoublyLinkedListError(const std::string& error) : DoublyLinkedListError(error) {}

    ~SubtractionOperationSharedDoublyLinkedListError() {}
};

