#include "HttpClient.hpp"
#include <curl/curl.h>
#include <iostream>

// simple log function using cout, varargs to stderr

void log(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    // add newline
    fprintf(stderr, "\n");
    va_end(args);
}

static size_t writeStringCallback(char* ptr, size_t size, size_t nmemb, void* userdata)
{
    std::string* str = (std::string*)userdata;
    size_t len = size * nmemb;
    str->append(ptr, len);
    return len;
}

static int xferInfoCallback(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
    float* progress = (float*)clientp;
    if (progress) {
        if (dltotal <= 0)
            *progress = 0.f;
        else
            *progress = (float)dlnow / dltotal;
    }
    return 0;
}

static CURL* createCurl(std::optional<std::string> token)
{
    log("Creating curl handle...");
    CURL* curl = curl_easy_init();

    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    std::string userAgent = "Rack/Prefabs";
    curl_easy_setopt(curl, CURLOPT_USERAGENT, userAgent.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);
    // Timeout to wait on initial HTTP connection.
    // This is lower than the typical HTTP timeout of 60 seconds to avoid DAWs from aborting plugin scans.
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);

    // If curl can't resolve a DNS entry, it sends a signal to interrupt the process.
    // However, since we use curl on non-main thread, this crashes the application.
    // So tell curl not to signal.
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

    if (token.has_value()) {
        std::string caPath = token.value();
        curl_easy_setopt(curl, CURLOPT_CAINFO, caPath.c_str());
    }

    return curl;
}

Response::Response() {}

// Returns true if the HTTP status code is 2xx.
bool Response::ok() const
{
    return status >= 200 && status < 300;
}

// Returns the value of the HTTP header.
std::optional<std::string> Response::getHeader(const std::string& key) const
{
    auto it = headers.find(key);
    if (it == headers.end())
        return std::nullopt;
    return it->second;
}

// Returns the value of the HTTP cookie.
std::optional<std::string> Response::getCookie(const std::string& key) const
{
    auto it = cookies.find(key);
    if (it == cookies.end())
        return std::nullopt;
    return it->second;
}

// getParamString returns a url encoded string of the parameters.
std::string getParamString(const std::map<std::string, std::string>& params)
{
    std::string paramsStr;
    for (auto it = params.begin(); it != params.end(); it++) {
        if (it != params.begin())
            paramsStr += "&";
        paramsStr += it->first + "=" + it->second;
    }
    return paramsStr;
}

// getCookieString returns a string of the cookies.
std::string getCookieString(const std::map<std::string, std::string>& cookies)
{
    std::string cookiesStr;
    for (auto it = cookies.begin(); it != cookies.end(); it++) {
        if (it != cookies.begin())
            cookiesStr += "; ";
        cookiesStr += it->first + "=" + it->second;
    }
    return cookiesStr;
}

// getJsonString returns a string representation of the json object.
std::string getJsonString(json_t* json)
{
    char* jsonStr = json_dumps(json, JSON_COMPACT);
    std::string str = jsonStr;
    free(jsonStr);
    return str;
}

// getJson returns a json object from a string.
json_t* getJson(const std::string& str)
{
    json_error_t error;
    json_t* json = json_loads(str.c_str(), 0, &error);
    if (!json) {
        //        WARN("Could not parse json: %s", error.text);
        return NULL;
    }
    return json;
}

// Request represents an HTTP request.
// Its send method returns a Response.

// GET are sent as url encoded parameters in the URL.
// POST are sent as JSON in the body.
Response Request::send()
{
    // use the old curl code as reference on how to use libcurl

    Response response;

    std::string fullUrl = url;

    json_t* body = json_object();
    char* bodyStr = NULL;

    // setup curl
    CURL* curl = createCurl(cert);

    // set the URL
    log("Setting URL: %s", url.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // set the method
    if (method == "GET") {
        log("Setting GET method");

        // set the params
        if (!params.empty()) {
            std::string paramsStr = getParamString(params);
            log("Setting params: %s", paramsStr.c_str());
            fullUrl += "?" + paramsStr;
            curl_easy_setopt(curl, CURLOPT_URL, fullUrl.c_str());
        }
    }
    else if (method == "POST") {
        log("Setting POST method");
        curl_easy_setopt(curl, CURLOPT_POST, true);

        // set the body
        if (!params.empty()) {
            for (auto it = params.begin(); it != params.end(); it++) {
                log("Setting param: %s: %s", it->first.c_str(), it->second.c_str());
                json_object_set_new(body, it->first.c_str(), json_string(it->second.c_str()));
            }

            bodyStr = json_dumps(body, JSON_COMPACT);
            log("Setting body: %s", bodyStr);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, bodyStr);
        }
    }

    // set the headers
    struct curl_slist* headerList = NULL;

    if (!headers.empty()) {
        for (auto it = headers.begin(); it != headers.end(); it++) {
            log("Setting header: %s: %s", it->first.c_str(), it->second.c_str());
            std::string header = it->first + ": " + it->second;
            headerList = curl_slist_append(headerList, header.c_str());
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);
    }

    // set the cookies
    if (!cookies.empty()) {
        log("Setting cookies: %s", getCookieString(cookies).c_str());
        curl_easy_setopt(curl, CURLOPT_COOKIE, getCookieString(cookies).c_str());
    }

    std::string resText;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeStringCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &resText);

    // perform the request
    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        log("Could not download %s: %s", url.c_str(), curl_easy_strerror(res));
        return response;
    }

    // get the status code
    long httpCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    // get the headers
    struct curl_slist* responseHeaders = NULL;
    curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &responseHeaders);
    for (struct curl_slist* header = responseHeaders; header; header = header->next) {
        std::string headerStr = header->data;
        size_t colonPos = headerStr.find(":");
        if (colonPos == std::string::npos)
            continue;
        std::string key = headerStr.substr(0, colonPos);
        std::string value = headerStr.substr(colonPos + 1);
        response.headers[key] = value;
    }

    // get the body
    response.body = json_loads(resText.c_str(), 0, &response.json_error);
    log("Got response: %s", resText.c_str());

    // cleanup
    if (bodyStr)
        free(bodyStr);

    curl_easy_cleanup(curl);
    if (!headers.empty()) {
        curl_slist_free_all(headerList);
    }

    return response;
}

DownloadResponse DownloadRequest::send()
{
    DownloadResponse response;

    // setup curl
    CURL* curl = createCurl(cert);

    FILE* file = std::fopen(filename.c_str(), "wb");
    if (!file) {
        log("Could not open file %s for writing", filename.c_str());
        response.status = -1;
        response.error = "Could not open file for writing";
        return response;
    }

    float progress = 0.f;

    log("Setting URL: %s", url.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, xferInfoCallback);
    curl_easy_setopt(curl, CURLOPT_XFERINFODATA, progress);
    // Fail on 4xx and 5xx HTTP codes
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, true);

    // set the headers
    if (!headers.empty()) {
        struct curl_slist* headerList = NULL;
        for (auto it = headers.begin(); it != headers.end(); it++) {
            log("Setting header: %s: %s", it->first.c_str(), it->second.c_str());
            std::string header = it->first + ": " + it->second;
            headerList = curl_slist_append(headerList, header.c_str());
        }
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);
    }

    // set the cookies
    if (!cookies.empty()) {
        log("Setting cookies: %s", getCookieString(cookies).c_str());
        curl_easy_setopt(curl, CURLOPT_COOKIE, getCookieString(cookies).c_str());
    }

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

    // perform the request
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    std::fclose(file);

    if (res != CURLE_OK) {
        log("Could not download %s: %s", url.c_str(), curl_easy_strerror(res));
        response.status = -1;
        response.error = curl_easy_strerror(res);
        return response;
    }

    // get the status code
    long httpCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    // get the headers
    struct curl_slist* headers = NULL;
    curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &headers);
    for (struct curl_slist* header = headers; header; header = header->next) {
        std::string headerStr = header->data;
        size_t colonPos = headerStr.find(":");
        if (colonPos == std::string::npos)
            continue;
        std::string key = headerStr.substr(0, colonPos);
        std::string value = headerStr.substr(colonPos + 1);
        response.headers[key] = value;
    }

    // get the cookies
    for (struct curl_slist* header = headers; header; header = header->next) {
        std::string headerStr = header->data;
        size_t colonPos = headerStr.find(":");
        if (colonPos == std::string::npos)
            continue;
        std::string key = headerStr.substr(0, colonPos);
        std::string value = headerStr.substr(colonPos + 1);
        response.cookies[key] = value;
    }

    return response;
}