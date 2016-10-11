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

int main( int argc, char** argv)
{
    printf("DRTECH HASP TIME PERIOD KEY MAKER, TEST HTTP REQUEST ---\n");
    printf("(C)2016 Raph.K.\n\n");

    if( SimpleHTTP::InitWinSock() == false )
    {
        printf("ERROR: Winsock not initialized !\n");
        return 0;
    }

    SimpleHTTP http;

    char url[]   = "http://rageworx.ddns.net";
    char content[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<test value=30241 />";

    printf("Requesting for %s ... ", url );

    http.cookie( "JSESSIONID=2B3EE2FE56868BD9588A7B55C405974B; Path=/;" );
    http.httpmethod( HTTP_REQ_METHOD_GET );
    //http.postcontents( content, strlen(content) );
    //http.posttype( SimpleHTTPTool::APPLICATION | SimpleHTTPTool::TEXT );
    //http.charset( "utf-8" );

    if ( http.request( url, 80 ) == true )
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

        printf("content = %lld bytes.\n", http.contentsize() );


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
                //printf("%s", http.contents() );
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
