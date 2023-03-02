#pragma once

#include <jansson.h>
#include <map>
#include <optional>
#include <string>

// Response holds the result of an HTTP request.
class Response {
   public:
    // The HTTP status code.
    int status = 0;

    // The HTTP headers.
    std::map<std::string, std::string> headers;

    // The HTTP cookies.
    std::map<std::string, std::string> cookies;

    // The body of the response.
    json_t* body;

    json_error_t json_error;

    Response();

    // Returns true if the HTTP status code is 2xx.
    bool ok() const;

    // Returns the value of the HTTP header.
    std::optional<std::string> getHeader(const std::string& key) const;

    // Returns the value of the HTTP cookie.
    std::optional<std::string> getCookie(const std::string& key) const;
};

// getParamString returns a url encoded string of the parameters.
std::string getParamString(const std::map<std::string, std::string>& params);

// getCookieString returns a string of the cookies.
std::string getCookieString(const std::map<std::string, std::string>& cookies);

// Request represents an HTTP request.
// Its send method returns a Response.

// GET are sent as url encoded parameters in the URL.
// POST are sent as JSON in the body.

class Request {
   public:
    std::string url;

    std::string method = "GET";

    std::map<std::string, std::string> headers;

    std::map<std::string, std::string> cookies;

    std::map<std::string, std::string> params;

    std::optional<std::string> cert;

    Response send();
};

class DownloadResponse {
   public:
    // The HTTP status code.
    int status = 0;

    // The filename
    std::string filename;

    // The HTTP headers.
    std::map<std::string, std::string> headers;

    // The HTTP cookies.
    std::map<std::string, std::string> cookies;

    std::string error;

    // Returns true if the HTTP status code is 2xx.
    bool ok() const;

    // Returns the value of the HTTP header.
    std::optional<std::string> getHeader(const std::string& key) const;

    // Returns the value of the HTTP cookie.
    std::optional<std::string> getCookie(const std::string& key) const;
};

class DownloadRequest {
   public:
    std::string url;

    std::string filename;

    std::map<std::string, std::string> headers;

    std::map<std::string, std::string> cookies;

    std::map<std::string, std::string> params;

    std::optional<std::string> cert;

    DownloadResponse send();
};
