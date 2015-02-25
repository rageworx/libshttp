#include "shttptool.h"

using namespace std;

/* URI encode / decode source refer to Jin Qing's code on following site.
   http://www.codeguru.com/cpp/cpp/algorithms/strings/article.php/c12759/URI-Encoding-and-Decoding.htm
*/

const char HEX2DEC_MATRIX[256] =
{
    /*       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F */
    /* 0 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 1 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 2 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 3 */  0, 1, 2, 3,  4, 5, 6, 7,  8, 9,-1,-1, -1,-1,-1,-1,

    /* 4 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 5 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 6 */ -1,10,11,12, 13,14,15,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 7 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

    /* 8 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* 9 */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* A */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* B */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

    /* C */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* D */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* E */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
    /* F */ -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1
};

const char SAFECHAR_MATRIX[256] =
{
    /*      0 1 2 3  4 5 6 7  8 9 A B  C D E F */
    /* 0 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* 1 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* 2 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* 3 */ 1,1,1,1, 1,1,1,1, 1,1,0,0, 0,0,0,0,

    /* 4 */ 0,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1,
    /* 5 */ 1,1,1,1, 1,1,1,1, 1,1,1,0, 0,0,0,0,
    /* 6 */ 0,1,1,1, 1,1,1,1, 1,1,1,1, 1,1,1,1,
    /* 7 */ 1,1,1,1, 1,1,1,1, 1,1,1,0, 0,0,0,0,

    /* 8 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* 9 */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* A */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* B */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,

    /* C */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* D */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* E */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0,
    /* F */ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0
};

const char DEC2HEX_MATRIX[] = "0123456789ABCDEF";

////////////////////////////////////////////////////////////////////////////////

unsigned long long longstrlen( const char* c )
{
    unsigned long long ret = 0;

    while ( *c != NULL )
    {
        c++;
        ret++;
    }

    return ret;
}

////////////////////////////////////////////////////////////////////////////////

const char* SimpleHTTPTool::GetMIME( SHTTPContentType ctype )
{
    unsigned long  mjr = (unsigned int)ctype & 0xFFFF0000;
    unsigned short mnr = (unsigned int)ctype & 0x0000FFFF;

    return GetMIME( (SHTTPCT_MAJOR)mjr, (SHTTPCT_MINOR)mnr );
}

const char* SimpleHTTPTool::GetMIME( SHTTPCT_MAJOR mjr, SHTTPCT_MINOR mnr )
{
    static string data;
    string major;
    string minor;

    switch( mjr )
    {
        case APPLICATION:
            major = "application";
            break;

        case AUDIO:
            major = "audio";
            break;

        case IMAGE:
            major = "image";
            break;

        case MESSAGE:
            major = "message";
            break;

        case MODEL:
            major = "model";
            break;

        case MULTIPART:
            major = "multipart";
            break;

        case TEXT:
            major = "text";
            break;

        case VIDEO:
            major = "video";
            break;
    }

    switch( mnr )
    {
        case ATOM_FEEDS:
            minor = "atom+xml";
            break;

        case ECMASCRIPT:
            minor = "ecmascript";
            break;

        case EDI_X12:
            minor = "EDI-X12";
            break;

        case EDIFACT:
            minor = "EDIFACT";
            break;

        case JSON:
            minor = "json";
            break;

        case JAVASCRIPT:
            minor = "javascript";
            break;

        case OCTET_STREAM:
            minor = "octet-stream";
            break;

        case OGG:
            minor = "ogg";
            break;

        case PDF:
            minor = "pdf";
            break;

        case POSTSCRIPT:
            minor = "postscript";
            break;

        case RDF_XML:
            minor = "rdf+xml";
            break;

        case RSS_XML:
            minor = "rss+xml";
            break;

        case SOAP_XML:
            minor = "soap+xml";
            break;

        case FONT_WOFF:
            minor = "font-woff";
            break;

        case XHTML_XML:
            minor = "xhtml+xml";
            break;

        case XML:
            minor = "xml";
            break;

        case XML_DTD:
            minor = "xml-dtd";
            break;

        case XOP_XML:
            minor = "xop-xml";
            break;

        case ZIP:
            minor = "zip";
            break;

        case GZIP:
            minor = "gzip";
            break;

        case BASIC:
            minor = "basic";
            break;

        case L24:
            minor = "L24";
            break;

        case MP4:
            minor = "mp4";
            break;

        case MPEG:
            minor = "mpeg";
            break;

        case VORBIS:
            minor = "vorbis";
            break;

        case VND_REALAUDIO:
            minor = "vnd.rn-realaudio";
            break;

        case VND_WAVE:
            minor = "vnd.wave";
            break;

        case WEBM:
            minor = "webm";
            break;

        case QUICKTIME:
            minor = "quicktime";
            break;

        case X_MATROSKA:
            minor = "x-matroska";
            break;

        case X_MS_WMV:
            minor = "x-ms-wmv";
            break;

        case X_FLV:
            minor = "x-flv";
            break;

        case GIF:
            minor = "gif";
            break;

        case JPEG:
            minor = "jpeg";
            break;

        case PJPEG:
            minor = "pjpeg";
            break;

        case PNG:
            minor = "png";
            break;

        case SVG:
            minor = "svg+xml";
            break;

        case TIFF:
            minor = "tiff";
            break;

        case HTTP:
            minor = "http";
            break;

        case IMDN:
            minor = "imdn+xml";
            break;

        case PARTIAL:
            minor = "partial";
            break;

        case RFC822:
            minor = "rfc822";
            break;

        case IGES:
            minor = "iges";
            break;

        case MESH:
            minor = "mesh";
            break;

        case VRML:
            minor = "vrml";
            break;

        case X3D_BINARY:
            minor = "x3d+binary";
            break;

        case X3D_FASTINFOSET:
            minor = "x3d+fastinfoset";
            break;

        case X3D_VRML:
            minor = "x3d-vrml";
            break;

        case X3D_XML:
            minor = "x3d+xml";
            break;

        case MIXED:
            minor = "mixed";
            break;

        case ALTERNATIVE:
            minor = "alternative";
            break;

        case RELATED:
            minor = "related";
            break;

        case FORM_DATA:
            minor = "form-data";
            break;

        case SIGNED:
            minor = "signed";
            break;

        case ENCRYPTED:
            minor = "encrypted";
            break;

        case CMD:
            minor = "cmd";
            break;

        case CSS:
            minor = "css";
            break;

        case SCV:
            minor = "csv";
            break;

        case HTML:
            minor = "html";
            break;

        case PLAIN:
            minor = "plain";
            break;

        case VCARD:
            minor = "vcard";
            break;

        case XFORMURLENCODED:
            minor = "x-www-form-urlencoded";
            break;
    }

    if ( ( major.size() > 0 ) && ( minor.size() > 0 ) )
    {
        data  = major;
        data += "/";
        data += minor;
    }
    else
    {
        data.clear();
    }

    return data.c_str();
}

void SimpleHTTPTool::ConvertStringToURI( const char* str, std::string &uri )
{
    if ( str != NULL )
    {
        unsigned long long strlength = longstrlen( str );

        uri.clear();

        for( unsigned long long cnt=0; cnt<strlength; cnt++ )
        {
            if ( SAFECHAR_MATRIX[ str[cnt] ] >= 0 )
            {
                char conv[4] = {NULL};

                conv[0] = '%';
                conv[1] = DEC2HEX_MATRIX[ str[cnt] >> 4 ];
                conv[2] = DEC2HEX_MATRIX[ str[cnt] & 0x0F ];

                uri += conv;
            }
            else
            {
                uri += str[cnt];
            }
        }
    }
}

void SimpleHTTPTool::ConvertURIToString( const char* uri, std::string &str )
{
    if ( uri != NULL )
    {
        unsigned long long urilength = longstrlen(uri);

        str.clear();

        for( unsigned long long cnt=0; cnt<urilength; cnt++ )
        {
            if ( ( uri[cnt] == '%' ) && ( cnt+2 < urilength ) )
            {
                char dec1 = HEX2DEC_MATRIX[ uri[cnt + 1] ];
                char dec2 = HEX2DEC_MATRIX[ uri[cnt + 2] ];
                if ( ( dec1 != -1 ) && ( dec2 != -1 ) )
                {
                    char out = ( dec1 << 4 ) + dec2;
                    str += out;
                }
                else
                {
                    str += uri[cnt];
                }
            }
            else
            {
                str += uri[cnt];
            }
        }
    }
}
