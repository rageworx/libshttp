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
    while ( src.length() > 0 && ( src[src.length() - 1] == ' ') )
    {
        src.erase( src.length() - 1, 1 );
    }

    while ( src.length() > 0 && ( src[0] == ' ' ) )
    {
        src.erase( 0, 1 );
    }
}

////////////////////////////////////////////////////////////////////////////////

HTTPParser::HTTPParser(const char* bytedata, int size)
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

bool HTTPParser::parse( const char* bytedata, int size )
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
        _headerlines.clear();

        int   curQueue     = 0;
        char* tempHeadLine = NULL;
        char* tokenStart   = httpData;
        char* tokenNext    = NULL;

        tokenNext = strstr(tokenStart, tokenDilim);

        while ( true )
        {
            int tokenSize = tokenNext - tokenStart;
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

int HTTPParser::getheaderlines(vector<string> &header)
{
    int cnt = 0;

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
        for( int cnt=0; cnt<_headerlines.size(); cnt++ )
        {
            string::size_type clpos = _headerlines[cnt].find("HTTP/");
            if ( clpos != string::npos )
            {
                vector< string > httpstatuslines = httpparsertool_split( _headerlines[cnt], 0x20 );

                if ( httpstatuslines.size() >= 2 )
                {
                    code = atoi( httpstatuslines[1].c_str() );

                    if ( httpstatuslines.size() > 2 )
                    {
                        msg.clear();

                        for( int cnt=2; cnt<httpstatuslines.size(); cnt++ )
                        {
                            msg += httpstatuslines[ cnt] ;
                            if ( cnt + 1 < cnt<httpstatuslines.size() )
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
        for( int cnt=0; cnt<_headerlines.size(); cnt++ )
        {
            string::size_type clpos = _headerlines[cnt].find("Set-Cookie: ");
            if ( clpos != string::npos )
            {
                string::size_type subPos = clpos + 11;
                string::size_type subLen = _headerlines[cnt].size() - subPos;

                ck = _headerlines[cnt].substr( subPos, subLen );
                httpparsertool_trimspace( ck );
            }
        }
    }
}

int HTTPParser::contentsize()
{
    if ( _headerlines.size() > 0 )
    {
        for( int cnt=0; cnt<_headerlines.size(); cnt++ )
        {
            string::size_type clpos = _headerlines[cnt].find("Content-Length: ");
            if ( clpos != string::npos )
            {
                string::size_type subPos = clpos + 16;
                string::size_type subLen = _headerlines[cnt].size() - subPos;

                string tmpCL = _headerlines[cnt].substr( subPos, subLen );
                if ( tmpCL.size() > 0 )
                {
                    return atoi(tmpCL.c_str());
                }
            }
        }
    }

    return 0;
}

