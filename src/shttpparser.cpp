#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "shttpparser.h"

#include <sstream>

using namespace std;

////////////////////////////////////////////////////////////////////////////////

const char tokenDilim[]     = { 0x0D, 0x0A, 0x00 };

////////////////////////////////////////////////////////////////////////////////

void httpparsertool_split(const string &src, char delim, vector< string > &elems)
{
    stringstream sstrm;
    sstrm.str( src );

    string item;

    while( getline(sstrm, item, delim ) )
    {
        elems.push_back(item);
    }
}

vector< string > httpparsertool_split(const string &src, char delim)
{
    vector< string > elems;
    httpparsertool_split( src, delim, elems );

    return elems;
}

void httpparsertool_trimspace( string &src )
{
    while ( ( src.length() > 0 ) && ( src[src.length()-1] == ' ' ) )
    {
        src.erase( src.length() - 1, 1 );
    }

    while ( src.length() > 0 && ( src[0] == ' ' ) )
    {
        src.erase( 0, 1 );
    }
}

////////////////////////////////////////////////////////////////////////////////

HTTPParser::HTTPParser(const char* bytedata, size_t size)
 : httpData(NULL),
   httpDataSize(0),
   bodyStartPtr(NULL)
{
    if ( ( bytedata != NULL ) && ( size > 0 ) )
    {
        httpData = new char[size];
        if ( httpData != NULL )
        {
            memcpy( httpData, bytedata , size );
            httpDataSize = size;
        }
    }
}

HTTPParser::~HTTPParser()
{
    flush();
}

bool HTTPParser::parse( std::string &contents )
{
    return parse( contents.c_str(), contents.size() );
}

bool HTTPParser::parse( const char* bytedata, size_t size )
{
    if ( ( bytedata != NULL ) && ( size > 0 ) )
    {
        httpData = new char[size];
        if ( httpData != NULL )
        {
            memcpy( httpData, bytedata , size );
            httpDataSize = size;
        }
    }

    if ( ( httpData != NULL ) && ( httpDataSize > 0 ) )
    {
        if ( _headerlines.size() > 0 )
            _headerlines.clear();

        size_t  curQueue     = 0;
        char*   tempHeadLine = NULL;
        char*   tokenStart   = httpData;
        char*   tokenNext    = NULL;

        tokenNext = strstr(tokenStart, tokenDilim);

        while ( true )
        {
            size_t tokenSize = tokenNext - tokenStart;
            curQueue += tokenSize;

            if ( curQueue > httpDataSize )
            {
                break;
            }

            if ( tokenSize > 0 )
            {
                tempHeadLine = new char[tokenSize + 1];
                if ( tempHeadLine != NULL )
                {
                    memset(tempHeadLine, 0, tokenSize + 1);
                    memcpy(tempHeadLine, tokenStart, tokenSize);

                    _headerlines.push_back(tempHeadLine);

                    delete[] tempHeadLine;
                }
            }
            else
            {
                // if http corrupted, this calculation failure.
                if ( curQueue + 2 >= httpDataSize )
                {
                    bodyStartPtr = NULL;
                    break;
                }

                // if double \r\n means end of header ..
                // and it means done to parse !

                bodyStartPtr  = tokenNext + 2;

                break;
            }

            // increase 0x0D,0x0A 2bytes;
            tokenStart = tokenNext + 2;
            tokenNext  = strstr(tokenStart, tokenDilim);
        }

        return true;
    }

    return false;
}

size_t HTTPParser::getheaderlines(vector<string> &header)
{
    size_t cnt = 0;

    for ( cnt=0; cnt<_headerlines.size(); cnt++ )
    {
        header.push_back( _headerlines[cnt] );
    }

    return cnt;
}

const char* HTTPParser::contents()
{
    return bodyStartPtr;
}

void HTTPParser::flush()
{
    if ( httpData != NULL )
    {
        delete[] httpData;
        httpData = NULL;
    }

    httpDataSize = 0;

    _headerlines.clear();
    bodyStartPtr = NULL;
}

int HTTPParser::getfilename(string &fname)
{
    if ( _headerlines.size() > 0 )
    {
        for( int cnt=0; cnt<_headerlines.size(); cnt++ )
        {
            string::size_type fnpos = _headerlines[cnt].find("filename=");
            if ( fnpos != string::npos )
            {
                string::size_type subPos = fnpos + 9;
                string::size_type subLen = _headerlines[cnt].size() - subPos;

                string tmpFN = _headerlines[cnt].substr( subPos, subLen );
                if ( tmpFN.size() > 0 )
                {
                    // remove " ...
                    while( true )
                    {
                        string::size_type fndpos = tmpFN.find("\"");
                        if ( fndpos == string::npos )
                        {
                            break;
                        }

                        tmpFN.replace( fndpos, 1 , "");
                    }

                }

                fname = tmpFN;
                return tmpFN.size();
            }
        }
    }

    return 0;
}

void HTTPParser::getstatuscodemsg( int &code, std::string &msg )
{
    if ( _headerlines.size() > 0 )
    {
        for( size_t cnt=0; cnt<_headerlines.size(); cnt++ )
        {
            string::size_type clpos = _headerlines[cnt].find("HTTP/");
            if ( clpos != string::npos )
            {
                vector< string > httpstatuslines = httpparsertool_split( _headerlines[cnt], 0x20 );

                if ( httpstatuslines.size() > 1 )
                {
                    code = atoi( httpstatuslines[1].c_str() );

                    if ( httpstatuslines.size() > 2 )
                    {
                        msg.clear();

                        for( size_t cntl=2; cnt<httpstatuslines.size(); cnt++ )
                        {
                            msg += httpstatuslines[cntl] ;
                            if ( cntl + 1 < cnt<httpstatuslines.size() )
                            {
                                msg += " ";
                            }
                        }
                    }
                }
            }
        }
    }
}

void HTTPParser::getcookie( std::string &ck )
{
    if ( _headerlines.size() > 0 )
    {
        string outstr;
        size_t ckcnt = 0;

        for( size_t cnt=0; cnt<_headerlines.size(); cnt++ )
        {
            string::size_type clpos = _headerlines[cnt].find("Set-Cookie: ");
            if ( clpos != string::npos )
            {
                string::size_type subPos = clpos + 11;
                string::size_type subEnd = _headerlines[cnt].find_first_of( ";", subPos );
                string::size_type subLen = string::npos;

                if ( subEnd != string::npos )
                {
                    subLen = subEnd - subPos;
                }
                else
                {
                    subLen = _headerlines[cnt].size() - subPos;
                }

                string tmpstr = _headerlines[cnt].substr( subPos, subLen );
                httpparsertool_trimspace( tmpstr );

                if ( ckcnt > 0 )
                {
                    outstr += "; ";
                }

                outstr += tmpstr;
                ckcnt++;
            }
        }

        ck = outstr;
    }
}

size_t HTTPParser::contentsize()
{
    if ( _headerlines.size() > 0 )
    {
        for( size_t cnt=0; cnt<_headerlines.size(); cnt++ )
        {
            string::size_type clpos = _headerlines[cnt].find("Content-Length: ");
            if ( clpos != string::npos )
            {
                string::size_type subPos = clpos + 16;
                string::size_type subLen = _headerlines[cnt].size() - subPos;

                string tmpCL = _headerlines[cnt].substr( subPos, subLen );
                if ( tmpCL.size() > 0 )
                {
                    char* strend = NULL;
                    return strtoull( tmpCL.c_str(), &strend, 10 );
                }
            }
        }
    }

    return 0;
}

size_t HTTPParser::datasize()
{
    return httpDataSize;
}

