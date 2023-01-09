#ifndef SERVER_URLRESOLVERS_H
#define SERVER_URLRESOLVERS_H

#include "src/Includes.h"

namespace regexes
{
    const string id = "[a-zA-Z0-9-]+";
    const string date = "[0-9]{4}-[0-9]{2}-[0-9]{2}T[0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3}Z";
    const string delimiters = "?=/&";
    vector<string> splited;

    void Split(const string& Uri)
    {
        splited.clear();
        boost::split(splited, Uri, boost::is_any_of(delimiters));
    }

    regex Import("/imports"),
            Delete("/delete/" + id + "\\?date=" + date),
            GetNodes("/nodes/" + id),
            GetNodesHistory("/node/" + id + "\\?dateStart=" + date + "&dateEnd=" + date),
            Update("/updates\\?date=" + date);

    void ResolveDelete(const string& Uri, string& Id, string& Date)
    {
        Split(Uri);
        Id = splited[2];
        Date = splited[4];
    }

    void ResolveGetNodes(const string& Uri, string& Id)
    {
        Split(Uri);
        Id = splited[2];
    }

    void ResolveGetNodesHistory(const string& Uri, string& Id, string& DateStart, string& DateEnd)
    {
        Split(Uri);
        Id = splited[2];
        DateStart = splited[4];
        DateEnd = splited[6];
    }

    void ResolveUpdate(const string& Uri, string& Date)
    {
        Split(Uri);
        Date = splited[3];
    }
}

#endif //SERVER_URLRESOLVERS_H
