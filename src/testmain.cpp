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
    char       url[]    = "http://167.88.112.71/test.php";
    //char       posts[]  = "user=developer&passwd=developerpass&req_action=none&req_param=none";
    char       posts[]  = "user=developer&passwd=developerpass&req_action=echo&req_param=testme_string_ok";


    printf("Requesting for %s ... ", url );

    http.httpmethod( HTTP_REQ_METHOD_POST );
    http.postcontents( posts, strlen(posts) );

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
