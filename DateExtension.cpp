#include "DateExtension.h"

DateExtension::DateExtension(const string &date) : date(date)
{
    string s = date;
    std::replace(s.begin(), s.end(), 'T', ' ');
    s.resize(std::remove(s.begin(), s.end(), 'Z') - s.begin());
    try {
        auto delta = boost::posix_time::time_from_string(s) - origin;
        date_ms = static_cast<long long>(delta.total_milliseconds());
    }
    catch(const std::exception& ex)
    {
        LOG(ERROR) << "Date "<< date << "is not valid";
        throw WrongDateException();
    }
}