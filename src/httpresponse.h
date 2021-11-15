#pragma once

#include "stdafx.h"
#include "http.h"

using namespace std;

class HTTPResponse
{
    Protocol protocol_;
    size_t statuscode_;
    string reasonphrase_;
    vector<pair<string, string>> headers_;
    string body_;
    string responsepacket_;

	public:
		HTTPResponse();
		~HTTPResponse();
		void addResponsePacket(char* buf, int& len);
	
        int setProtocol(Protocol argprotocol);
		Protocol getProtocol();
        int setStatusCode(size_t argstatuscode);
		size_t getStatusCode();
		int setReasonPhrase();
		string getReasonPhrase();
		int setHTTPHeader(string name, string content);
		string getHTTPHeader(string name);
        int setHTTPHeaderVector(vector<pair<string, string>>* headervector);
		vector<pair<string, string>>* getHTTPHeaderVector();
        int setResponseBody(string argbody);
		string getResponseBody();

		int makeResponse();
		int parseResponsePacket();
		size_t getResponseSize();
		string* getResponseData();
        string updateCursor(size_t& cursorbegin, size_t& cursorend, string target, string obj, size_t next);
};
