#ifndef __LIBSHTTPPARSER_H__
#define __LIBSHTTPPARSER_H__

#include <vector>
#include <string>

class HTTPParser
{
    public:
        HTTPParser(const char* bytedata = NULL , size_t size = 0);
        virtual ~HTTPParser();

    public:
        bool parse( std::string &contents );
        bool parse( const char* bytedata, size_t size);
        void flush();

    public:
        size_t  getheaderlines( std::vector<std::string> &header );
        int     getfilename(std::string &fname);
        void    getstatuscodemsg( int &code, std::string &msg );
        void    getcookie( std::string &ck );
        size_t  contentsize();
        size_t  datasize();

    public:
        const char* contents();

    private:
        char*   httpData;
        size_t  httpDataSize;
        size_t  httpDataWriteQueue;
        char*   bodyStartPtr;

    protected:
        std::vector<std::string>    _headerlines;
};

#endif /// of __LIBSHTTPPARSER_H__
