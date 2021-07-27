#include "shttpobj.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////

HTTPObject::HTTPObject()
 : _event(NULL),
   _method(HTTP_REQ_METHOD_GET)
{
    resetheaderitem();
}

HTTPObject::~HTTPObject()
{
    resetheaderitem();
}

void HTTPObject::resetheaderitem()
{
    _headeritems.clear();

    addheader( "User-Agent", LIBSHTTPNAME );
}

bool HTTPObject::addheader( string &key, string &value )
{
    size_t  headercount = _headeritems.size();
    bool    isnewitem = true;
    int     existsidx = -1;

    if ( headercount > 0 )
    {
        existsidx = findheader( key );
        if ( existsidx >= 0 )
        {
            isnewitem = false;
        }
    }

    if ( isnewitem == true )
    {
        HTTPHI newitem;
        newitem.key = key;
        newitem.value = value;
        _headeritems.push_back( newitem );
    }
    else
    {
        _headeritems[ existsidx ].value = value;
    }

    return true;
}

bool HTTPObject::addheader( const char* key, const char* value )
{
    string akey = key;
    string aval = value;

    return addheader( akey, aval );
}

int  HTTPObject::findheader( std::string &key )
{
    if ( _headeritems.size() > 0 )
    {
        size_t seekend = _headeritems.size();
        for( size_t cnt=0; cnt<seekend; cnt++ )
        {
            if ( _headeritems[cnt].key == key )
            {
                return (int)cnt;
            }
        }
    }

    return -1;
}

bool HTTPObject::removeheader( int idx )
{
    if ( idx >= 0 )
    {
        if ( ( _headeritems.size() > 0 ) && ( _headeritems.size() > (size_t)idx ) )
        {
            _headeritems.erase( _headeritems.begin() + (size_t)idx );

            return true;
        }
    }

    return false;
}

bool HTTPObject::removeheader( string &key )
{
    int retI = findheader( key );
    if ( retI >= 0 )
    {
        return removeheader( retI );
    }

    return false;
}

bool HTTPObject::getheader( int idx, HTTPHI &out )
{
    if ( idx >= 0 )
    {
        if ( ( _headeritems.size() > 0 ) && ( _headeritems.size() > (size_t)idx ) )
        {
            out.key = _headeritems[ idx ].key;
            out.value = _headeritems[ idx ].value;

            return true;
        }
    }

    return false;
}
