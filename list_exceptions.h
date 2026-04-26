#ifndef LIST_EXCEPTIONS_H
#define LIST_EXCEPTIONS_H

#include <stdexcept>

class ListInvalidArgument : public std::invalid_argument
{
public:
    explicit ListInvalidArgument(const char* message) : std::invalid_argument(message)
    {
    }
};

class ListIndexOutOfRange : public std::out_of_range
{
public:
    explicit ListIndexOutOfRange(const char* message) : std::out_of_range(message)
    {
    }
};

#endif // LIST_EXCEPTIONS_H
