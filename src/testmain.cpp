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
    #include <windows.h>
    #include <winsock2.h>
#else
    #include <unistd.h>
    #include <fcntl.h>
#endif

#include "shttp.h"

int main( int argc, char** argv)
{
    printf("libHTTP test application, 2014-03-25\n");
    printf("\n");

    SimpleHTTP http;
    char       url[] = "http://rageworx.iptime.org/aup/test.txt";

    printf("Requesting for %s ... ", url );

    http.httpmethod( HTTP_REQ_METHOD_GET );
    if ( http.request( url ) == true )
    {
        printf("[ok.]\n");

        printf("headers = %d\n", http.headercount() );
        if ( http.headercount() > 0 )
        {
            for( int cnt=0; cnt<http.headercount(); cnt++ )
            {
                HTTPHI tmpHI;
                http.getheader( cnt, tmpHI );

                printf("[%03d] %s = %s\n", cnt+1, tmpHI.key.c_str(), tmpHI.value.c_str() );
            }
        }


        printf("content = %d bytes.\n", http.contentsize() );

        if ( http.contents() != NULL )
        {
            printf("%s", http.contents() );
        }
    }
    else
    {
        printf("failed.\n");
    }

    return 0;
}
