#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef _WIN32
    #include <windows.h>
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <unistd.h>
    #include <fcntl.h>
    #include <netdb.h>
#endif

#include <string>
#include <vector>
using namespace std;

#include "shttp.h"

////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
static WSADATA _wsData      = {0};
#endif

////////////////////////////////////////////////////////////////////////////////


#if !defined(_WIN32)
static int strnicmp( char const * a, char const * b, int n )
{
    return strncasecmp( a, b, n );
}
#endif

SimpleHTTP::SimpleHTTP()
 : _postcontent(NULL),
   _postcontentsize(0),
   _parser(NULL),
   _sockfd(-1),
   _socketinuse(false),
   _socketPossible(true),
   _contents(NULL),
   _contents_size(0)
{
    _parser = new HTTPParser();
}

SimpleHTTP::~SimpleHTTP()
{
    if ( _parser != NULL )
    {
        delete _parser;
        _parser = NULL;
    }

    if ( _postcontent != NULL )
    {
        delete[] _postcontent;
        _postcontent = NULL;
        _postcontentsize = 0;
    }
}

void SimpleHTTP::httpmethod( HTTPREQMTYPE mtype )
{
    if ( _socketinuse == false )
    {
        _method = mtype;
    }
}

bool SimpleHTTP::request( const char* addr, unsigned short port )
{
    if ( addr == NULL )
        return false;

    if ( port == 0 )
        return false;

    string host;
    string url;

    splitaddress( addr, host, url );

    if ( host.size() == 0 )
    {
        return false;
    }

    _targetaddr = host;
    _targeturl  = url;
    _targetport = port;

    if ( opensocket() == false )
    {
        return false;
    }

    string httprequestheader;

    if ( makehttpheaderstr( httprequestheader ) == false )
    {
        return false;
    }

    int retI = send( _sockfd, httprequestheader.data(), httprequestheader.size(), 0 );
    if ( retI != httprequestheader.size() )
    {
        return false;
    }

    char rcvBuff[1024] = {0};

    // read socket until it ends ...
    while( true )
    {
        memset( rcvBuff, 0, 1024 );

        retI = recv( _sockfd, rcvBuff, 1024, 0 );
        if ( retI > 0 )
        {
            for( int cnt=0; cnt<retI; cnt++ )
            {
                _rcvData.push_back( rcvBuff[cnt] );
            }
        }
        else
        if ( retI <= 0 )
        {
            _rcvData.push_back( 0 );
            closesocket();
            break;
        }
    }

    if ( _parser != NULL )
    {
        _parser->flush();

        if ( _parser->parse( _rcvData ) == true )
        {
            std::vector<std::string> headers;
            _parser->getheaderlines( headers );

            if ( headers.size() > 0 )
            {
                resetheaderitem();

                for( int cnt=0; cnt<headers.size(); cnt++ )
                {
                    string::size_type posDiv = headers[cnt].find(": ");
                    if ( posDiv != string::npos )
                    {
                        string hdrkey = headers[cnt].substr( 0, posDiv );
                        string hdrval = headers[cnt].substr( posDiv + 1 );
                        addheader( hdrkey, hdrval );
                    }
                }
            }

            _contents = _parser->contents();
            _contents_size = _parser->contentsize();
        }
        else
        {
            _contents = NULL;
            _contents_size = 0;
        }

    }

    return true;
}

int  SimpleHTTP::contentsize()
{
    return _contents_size;
}

void SimpleHTTP::splitaddress( const char* addr, std::string &host, std::string &url )
{
    string tmpstr = addr;
    string::size_type  pos1st = tmpstr.find("http://");

    if ( pos1st != string::npos )
    {
        pos1st += 7;

        string::size_type posnext = tmpstr.find("/", pos1st );

        if ( posnext != string::npos )
        {
            host = tmpstr.substr( pos1st, posnext - pos1st );
            pos1st = posnext;
            posnext = tmpstr.find("/", pos1st);

            if ( posnext != string::npos )
            {
                url = tmpstr.substr( pos1st );
            }
        }
        else
        {
            host = tmpstr.substr( pos1st );
#ifdef DEBUG
            printf("debug.host = %s\n", host.c_str() );
#endif
            url.clear();
        }
    }
}

bool SimpleHTTP::opensocket()
{
    if ( _sockfd >= 0 )
    {
        closesocket();
    }

    _sockfd = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

    if ( _sockfd >= 0 )
    {

        int valOn = 1;
        setsockopt( _sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&valOn, sizeof(int) );

        struct hostent* hstent = gethostbyname( _targetaddr.c_str() );
        if ( hstent == NULL )
        {
            _socketPossible = false;
            return false;
        }

        _targetremappedaddr         = inet_ntoa( *(struct in_addr*)hstent->h_addr_list[0] );
        _saTarget.sin_family        = AF_INET;
        _saTarget.sin_addr.s_addr   = inet_addr( _targetremappedaddr.c_str() );
        _saTarget.sin_port          = htons( _targetport );

        int retI = connect( _sockfd, (sockaddr*)&_saTarget, sizeof( _saTarget ) );
        if ( retI >= 0 )
        {
            _socketPossible = true;
        }
    }

    return _socketPossible;
}

void SimpleHTTP::closesocket()
{
    if ( _sockfd >= 0 )
    {
#ifdef _WIN32
        ::closesocket( _sockfd );
#else
        close( _sockfd );
#endif

        if ( _socketPossible == true )
        {
            _socketPossible = false;
        }

    }
}

bool SimpleHTTP::makehttpheaderstr( string &out )
{
    if ( _headeritems.size() > 0 )
    {
        out.clear();

        string hdr;
        string ct;
        string cl;
        string fm;

        // set method type in header.
        switch( _method )
        {
            case HTTP_REQ_METHOD_GET:
                hdr = "GET ";
                break;

            case HTTP_REQ_METHOD_POST:
                {
                    hdr = "POST ";

                    char tmpstr[80] = {0};

                    sprintf( tmpstr, "From: %s\r\n", _targeturl.c_str() );
                    fm = tmpstr;
                }
                break;

            case HTTP_REQ_METHOD_SENDFILE:
                hdr = "FILE ";
                break;

            case HTTP_REQ_METHOD_GETFILE:
                hdr = "FILE ";
                break;
        }

        // set contents size if it exists.
        if ( ( _postcontent != NULL ) && ( _postcontentsize > 0 ) )
        {
            char tmpstr[80] = {0};

            unsigned short mj = _postcontenttype & 0xFFFF0000;
            unsigned short mn = _postcontenttype & 0x0000FFFF;

            sprintf( tmpstr, "Content-Type: %s\r\n", SimpleHTTPTool::GetMIME( mj, mn ) );
            ct = tmpstr;

            sprintf( tmpstr, "Content-Length: %d\r\n", _postcontentsize );
            cl = tmpstr;
        }

        out  = hdr;
        out += _targeturl;
        out += " HTTP/1.0\r\n";
        out += "User-Agent: ";

        if ( _customuseragent.size() > 0 )
        {
            out += _customuseragent;
        }
        else
        {
            out += "SHTTPLIB/0.1";
        }

        out += "\r\n";
        out += fm;  /// From:
        out += ct;  /// Content-Type:
        out += cl;  /// Content-Length:
        out += "\r\n";
    }
}

#ifdef _WIN32
bool SimpleHTTP::InitWinSock()
{
    if ( _wsData.wVersion == 0x0202 )
        return true;

    if ( WSAStartup( 0x0202, &_wsData ) != 0 )
    {
        WSACleanup();
        return false;
    }

    return true;
}
#endif /// of _WIN32

#ifdef _WIN32
bool SimpleHTTP::FinalWinSock()
{
    WSACleanup();
    memset( &_wsData, 0, sizeof( WSADATA ) );
    return true;
}
#endif /// of _WIN32
