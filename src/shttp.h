#ifndef __LIBSHTTP_H__
#define __LIBSHTTP_H__

////////////////////////////////////////////////////////////////////////////////
//
// Simple socket HTTP library.
// ============================================================================
// (C)Copyright 2012-2013 Raphael Kim (rageworx@gmail.com)
//
// update : 2016-09-20
//          added charset= xxx option.
//
////////////////////////////////////////////////////////////////////////////////

#include "shttpdef.h"
#include "shttpobj.h"
#include "shttptool.h"
#include "shttpparser.h"

class SimpleHTTP : public HTTPObject
{
    public:
        SimpleHTTP();
        virtual ~SimpleHTTP();

    // Related in Method.
    public:
        void httpmethod( HTTPREQMTYPE mtype );
        void postcontents( const char* src, long long srcsize );
        void posttype( unsigned t );
        void charset( const char* cs );

    // Action
    public:
        bool request( const char* addr, unsigned short port = 80);

    public:
        HTTPREQMTYPE    httpmethod()    { return _method; }
        const char*     postcontent()   { return _postcontent; }
        const char*     contents()      { return _contents; }
        const char*     lasterror()     { return _lasterrmsg.c_str(); }
        const char*     charset()       { return _charset.c_str(); }
        int             statuscode()    { return _statuscode; }
        const char*     statusmsg()     { return _statusmsg.c_str(); }

    public:
        long long contentsize();

    public:
        void resetcookie();
        const char* cookie();
        void cookie( const char* ck );

    protected:
        void splitaddress( const char* addr, std::string &host, std::string&url );
        bool opensocket();
        void closesocket();
        bool makehttpheaderstr( string &out );

    protected:
        unsigned        _postcontenttype;
        char*           _postcontent;
        long long       _postcontentsize;
        HTTPParser*     _parser;
        bool            _socketinuse;

    protected:
        SOCKET          _sockfd;
        sockaddr_in     _saTarget;
        bool            _socketPossible;
        std::string     _targetaddr;
        std::string     _targeturl;
        unsigned short  _targetport;
        std::string     _targetremappedaddr;
        std::string     _customuseragent;
        std::string     _charset;

    protected:
        std::string     _rcvData;

    protected:
        int             _statuscode;
        std::string     _statusmsg;
        const char*     _contents;
        long long       _contents_size;
        std::string     _singlecookie;

    protected:
        std::string     _lasterrmsg;

#ifdef _WIN32
    public:
        static bool InitWinSock();
        static bool FinalWinSock();
#endif

};


#endif // of __LIBSHTTP_H__
