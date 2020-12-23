#include "Http.h"
#include <codecvt>

Http::Http(std::wstring serverName) {
    _hSession = WinHttpOpen(
        L"HIBP-Policy-Evaluator",
        WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0);

    if (!_hSession)
        throw "WinHTTP: Could not open session.";

    _hConnect = WinHttpConnect(
        _hSession,
        serverName.c_str(),
        INTERNET_DEFAULT_HTTPS_PORT,
        0);

    if (!_hConnect)
        throw "WinHTTP: Could not connect to server.";
}

Http::~Http() {
    if (_hConnect)
        WinHttpCloseHandle(_hConnect);
    if (_hSession)
        WinHttpCloseHandle(_hSession);
}

std::wstring Http::LoadHashBucket(std::wstring requestPath, std::wstring hexHash, int prefixLength) {
    HINTERNET hRequest;
    bool hasResult = false;

    std::wstring query = requestPath + L"/" + (hexHash.substr(0, prefixLength));

    hRequest = WinHttpOpenRequest(_hConnect,
        NULL, // Default: GET
        query.c_str(),
        NULL, // Default: HTTP/1.1
        WINHTTP_NO_REFERER,
        NULL, // Default: text/*
        WINHTTP_FLAG_SECURE);

    // Send a request.
    if (hRequest)
        hasResult = WinHttpSendRequest(hRequest,
            WINHTTP_NO_ADDITIONAL_HEADERS,
            0, WINHTTP_NO_REQUEST_DATA, 0,
            0, 0);


    // End the request.
    if (hasResult)
        hasResult = WinHttpReceiveResponse(hRequest, NULL);

    if (!hasResult)
        throw "WinHTTP: Could not receive response.";

    //Read the result
    std::string httpResult = ReadResult(hRequest);
    std::wstring result = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(httpResult);

    if (hRequest)
        WinHttpCloseHandle(hRequest);

    return result;
}

std::string Http::ReadResult(HINTERNET hRequest) {
    unsigned long dataSize, downloadedSize;

    std::string result = std::string();

    do
    {
        // Check for available data.
        dataSize = 0;
        if (!WinHttpQueryDataAvailable(hRequest, &dataSize))
            throw "WinHTTP: QueryDataAvailable was false.";

        if (dataSize == 0)
            break;

        // Allocate space for the buffer.
        char* outBuffer = new char[dataSize + 1];
        if (!outBuffer)
        {
            throw "WinHTTP: Could not allocate buffer for request result.";
        }
            // Read the data.
        ZeroMemory(outBuffer, dataSize + 1);

        if (!WinHttpReadData(hRequest, outBuffer, dataSize, &downloadedSize))
            throw "WinHTTP: Could not read data from WinHTTP.";

        result = result + outBuffer;

        // Free the memory allocated to the buffer.
        delete[] outBuffer;
    } while (dataSize > 0);

    return result;
}
