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

    if( SimpleHTTP::InitWinSock() == false )
    {
        printf("ERROR: Winsock not initialized !\n");
        return 0;
    }

    SimpleHTTP http;
    char       url[] = "http://www.google.com/images/srpr/logo11w.png";

    printf("Requesting for %s ... ", url );

    http.httpmethod( HTTP_REQ_METHOD_GET );
    if ( http.request( url ) == true )
    {
        printf("[ok.]\n");

        string outftype;
        string outfname;

        printf("headers = %d\n", http.headercount() );
        if ( http.headercount() > 0 )
        {
            for( int cnt=0; cnt<http.headercount(); cnt++ )
            {
                HTTPHI tmpHI;
                http.getheader( cnt, tmpHI );

                printf("[%03d] %s: %s \n", cnt+1, tmpHI.key.c_str(), tmpHI.value.c_str() );

                if ( tmpHI.key == "Content-Type" )
                {
                    outftype = tmpHI.value;
                    outfname = "google_logo_w.png";
                }
            }
        }

        printf("content = %d bytes.\n", http.contentsize() );


        if ( http.contents() != NULL )
        {
            if ( outftype.size() > 0 )
            {
                printf("content type = %s\n", outftype.c_str() );

                FILE* fp = fopen( outfname.c_str(), "wb+" );
                if ( fp != NULL )
                {
                    fwrite( http.contents(), 1, http.contentsize() , fp );
                    fclose( fp );
                    printf("file written!\n");
                }
                else
                {
                    printf("file write failed ! \n");
                }
            }
            else
            {
                printf("%s", http.contents() );
            }
        }
    }
    else
    {
        printf("failed.\n");
    }

	SimpleHTTP::FinalWinSock();

    return 0;
}
