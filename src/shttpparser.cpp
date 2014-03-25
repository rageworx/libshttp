#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "shttpparser.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////

const char tokenDilim[]     = { 0x0D, 0x0A, 0x00 };

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
