#pragma once

#include "stdafx.h"
#include "http.h"

using namespace std;

class HTTPRequest
{
    Method method_;
    string url_;
    Protocol protocol_;
    string hostname_;
    string useragent_;

    vector<pair<string, string>> headers_;
    string body_;
    string requestpacket_;

    public:
        HTTPRequest();
        ~HTTPRequest();
        void addRequestPacket(char* buf, int& len);
        void addRequestBody(string& str);

        int setMethod(Method argmethod);
        Method getMethod();
        int setURL(string argurl);
        string getURL();
        int setProtocol(Protocol argprotocol);
		Protocol getProtocol();
        int setUserAgent(string arguseragent);
		string getUserAgent();
		int setHTTPHeader(string name, string content);
		string getHTTPHeader(string name);
        int setHTTPHeaderVector(vector<pair<string, string>>* headervector);
		vector<pair<string, string>>* getHTTPHeaderVector();
        int setRequestBody(string argbody);
		string getRequestBody();

        int parseRequestPacket();
		int makeRequest();
		size_t getRequestSize();
		string* getRequestData();
        string updateCursor(size_t& cursorbegin, size_t& cursorend, string target, string obj, size_t next);
};
