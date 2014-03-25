#ifndef __LIBSHTTPOBJ_H__
#define __LIBSHTTPOBJ_H__

#include <vector>
#include <string>
#include "shttpdef.h"

class HTTPObject
{
    public:
        HTTPObject();
        virtual ~HTTPObject();

    public:
        void        httpevent( HTTPEvent* e )   { _event = e; }
        HTTPEvent*  httpevent()                 { return _event; }
        int         headercount()               { return _headeritems.size(); }

    public:
        bool addheader( std::string &key, std::string &value );
        bool addheader( const char* key, const char* value );
        int  findheader( std::string &key );
        bool removeheader( int idx );
        bool removeheader( std::string &key );
        bool getheader( int idx, HTTPHI &out );

    public:
        const HTTPHI* header( int idx );

    protected:
        void resetheaderitem();


    protected:
        HTTPReqMethodType       _method;
        HTTPEvent*              _event;
        std::vector<HTTPHI>     _headeritems;
};

#endif /// of __LIBSHTTPOBJ_H__
