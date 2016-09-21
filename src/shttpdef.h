#ifndef __LIBSHTTPDEF_H__
#define __LIBSHTTPDEF_H__

#if !defined(_WIN32)
    #define SOCKET  int
#endif

#include <string>

////////////////////////////////////////////////////////////////////////////////
// enum & structures ------
////////////////////////////////////////////////////////////////////////////////

typedef enum
{
    HTTP_REQ_METHOD_GET = 0,
    HTTP_REQ_METHOD_POST,
    HTTP_REQ_METHOD_PUT,
    HTTP_REQ_METHOD_DELETE,
    HTTP_REQ_METHOD_OPTIONS,
    HTTP_REQ_METHOD_HEAD,
    HTTP_REQ_METHOD_TRACE,
    HTTP_REQ_METHOD_CONNECT,
    HTTP_REQ_METHOD_SENDFILE,
    HTTP_REQ_METHOD_GETFILE,
    HTTP_REQ_METHOD_MAX
}HTTPReqMethodType,HTTPREQMTYPE;

////////////////////////////////////////////////////////////////////////////////

typedef struct _HTTPHeaderItem
{
    std::string key;
    std::string value;
}HTTPHeaderItem,HTTPHI;

////////////////////////////////////////////////////////////////////////////////
// classes ---------------
////////////////////////////////////////////////////////////////////////////////

class HTTPEvent
{
    public:
        virtual void OnConnect() = 0;
        virtual void OnDisconnect() = 0;
        virtual void OnRead( const size_t readsize, const size_t maxsize ) = 0;
        virtual void OnError( const char* errorMsg ) = 0;
        virtual void OnRetry( const int retrycount ) = 0;
};

#endif /// of __LIBSHTTPDEF_H__
