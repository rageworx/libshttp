#ifndef __LIBSHTTP_H__
#define __LIBSHTTP_H__

////////////////////////////////////////////////////////////////////////////////
//
// Simple socket HTTP library.
// ============================================================================
// (C)Copyright 2012-2013 Raphael Kim (rageworx@gmail.com)
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
        void content( const char* src, int srcsize );

    // Action
    public:
        bool request( const char* addr, unsigned short port = 80);

    public:
        HTTPREQMTYPE    httpmethod()    { return _method; }
        const char*     postcontent()   { return _postcontent; }
        const char*     contents()      { return _contents; }

    public:
        int  contentsize();

    protected:
        void splitaddress( const char* addr, std::string &host, std::string&url );
        bool opensocket();
        void closesocket();
        bool makehttpheaderstr( string &out );

    protected:
        unsigned        _postcontenttype;
        char*           _postcontent;
        int             _postcontentsize;
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

    protected:
        std::string     _rcvData;

    protected:
        const char*     _contents;
        int             _contents_size;

#ifdef _WIN32
    public:
        static bool InitWinSock();
        static bool FinalWinSock();
#endif

};


#endif // of __LIBSHTTP_H__
