#ifndef __LIBSHTTPPARSER_H__
#define __LIBSHTTPPARSER_H__

#include <vector>
#include <string>

class HTTPParser
{
    public:
        HTTPParser(const char* bytedata = NULL , int size = 0);
        virtual ~HTTPParser();

    public:
        bool parse( std::string &contents );
        bool parse( const char* bytedata, int size);
        void flush();

    public:
        int  getheaderlines( std::vector<std::string> &header );
        int  getfilename(std::string &fname);
        void getstatuscodemsg( int &code, std::string &msg );
        void getcookie( std::string &ck );
        int  contentsize();

    public:
        const char* contents();

    private:
        char* httpData;
        int   httpDataSize;
        int   httpDataWriteQueue;
        char* bodyStartPtr;

    protected:
        std::vector<std::string>    _headerlines;
};

#endif /// of __LIBSHTTPPARSER_H__
