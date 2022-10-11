#ifndef YAD_DATEEXTENSION_H
#define YAD_DATEEXTENSION_H

#include "Includes.h"
#include "Configuration.h"

class WrongDateException : std::exception
{
public:
    WrongDateException() = default;
};

class DateExtension
{
public:
    explicit DateExtension(const string& date);
    string date;
    long long date_ms;
private:
    const boost::posix_time::ptime origin = boost::posix_time::time_from_string("2000-01-01 00:00:00.000");
};

#endif //YAD_DATEEXTENSION_H
