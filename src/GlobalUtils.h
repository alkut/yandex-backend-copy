#ifndef SERVER_GLOBALUTILS_H
#define SERVER_GLOBALUTILS_H

#include "Includes.h"
#include "src/Configuration.h"
#include "View/ImportBody.h"

const string val_message = "Validation Failed";
const string item_message = "Item not found";
const size_t max_url_size = 255;

struct CodeMessage
{
    string Serialize() const;

    int code;
    string message;
};

string BadRequestResponse();
string NotFoundResponse();
string GetUpdateResponse(const vector<ImportBodyMessage::ImportBodyItem>& items);
string GetNodeHistoryResponse(const vector<ImportBodyMessage::ImportBodyItem>& items);

void ValidateItem(const ImportBodyMessage::ImportBodyItem& item, int& ResponseCode, string& ResponseMsg);

const boost::posix_time::ptime origin = boost::posix_time::time_from_string("2000-01-01 00:00:00.000");
const boost::posix_time::ptime day_after = boost::posix_time::time_from_string("2000-01-02 00:00:00.000");
const long long day_in_ms = static_cast<long long>((day_after - origin).total_milliseconds());

long long ValidateDate(const string& date, int& ResponseCode, string& ResponseMsg);
#endif //SERVER_GLOBALUTILS_H
