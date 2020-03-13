//==============================================================================
// Test for libHTTP
//==============================================================================

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>
#include <vector>

using namespace std;

#if defined(_WIN32)
    #include <winsock2.h>
    #include <windows.h>
#else
    #include <unistd.h>
    #include <fcntl.h>
#endif

#include "shttp.h"

void do_test1()
{
    SimpleHTTP http;

    char url[] = "http://rageworx.info";

    printf("Requesting for %s ... ", url );

    http.httpmethod( HTTP_REQ_METHOD_GET );

    if ( http.request( url, 80 ) == true )
    {
        printf("[ok.]\n");

        string outftype;
        string outfname;

        // contentsize() is just returns parsed value of http header "Content-size".
        // Need to check real content existed or not with content() return.
        printf("content = %lld bytes.\n", http.contentsize() );

        if ( http.contents() != NULL )
        {
            outftype = http.contents();

            if ( outftype.size() > 0 )
            {
                printf( "%s\n", outftype.c_str() );
            }
        }
    }
    else
    {
        printf("failed.\n");
    }

    http.closeconnection();
}

void do_test2()
{
    SimpleHTTP http;

    char url[] = "http://google.com";

    printf("Requesting for %s ... ", url );

    http.httpmethod( HTTP_REQ_METHOD_GET );

    if ( http.request( url, 80 ) == true )
    {
        printf("[ok.]\n");

        string outftype;
        string outfname;

        // contentsize() is just returns parsed value of http header "Content-size".
        // Need to check real content existed or not with content() return.
        printf("content = %lld bytes.\n", http.contentsize() );

        if ( http.contents() != NULL )
        {
            outftype = http.contents();

            if ( outftype.size() > 0 )
            {
                printf("%s\n", outftype.c_str() );
            }
        }
    }
    else
    {
        printf("failed.\n");
    }

    http.closeconnection();
}

int main( int argc, char** argv)
{
    if( SimpleHTTP::InitWinSock() == false )
    {
        printf("ERROR: Winsock not initialized !\n");
        return 0;
    }

    do_test1();
    printf( "////////////////////////////////////////////////////////\n" );
    do_test2();

	SimpleHTTP::FinalWinSock();

    return 0;
}
