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

    return GetMIME( (SimpleHTTPContentType::SHTTPCT_MAJOR)mjr, (SimpleHTTPContentType::SHTTPCT_MINOR)mnr );
}

const char* SimpleHTTPTool::GetMIME( SimpleHTTPContentType::SHTTPCT_MAJOR mjr, SimpleHTTPContentType::SHTTPCT_MINOR mnr )
{
    static string data;
    string major;
    string minor;

    switch( mjr )
    {
        case SimpleHTTPContentType::APPLICATION:
            major = "application";
            break;

        case SimpleHTTPContentType::AUDIO:
            major = "audio";
            break;

        case SimpleHTTPContentType::IMAGE:
            major = "image";
            break;

        case SimpleHTTPContentType::MESSAGE:
            major = "message";
            break;

        case SimpleHTTPContentType::MODEL:
            major = "model";
            break;

        case SimpleHTTPContentType::MULTIPART:
            major = "multipart";
            break;

        case SimpleHTTPContentType::TEXT:
            major = "text";
            break;

        case SimpleHTTPContentType::VIDEO:
            major = "video";
            break;
    }

    switch( mnr )
    {
        case SimpleHTTPContentType::ATOM_FEEDS:
            minor = "atom+xml";
            break;

        case SimpleHTTPContentType::ECMASCRIPT:
            minor = "ecmascript";
            break;

        case SimpleHTTPContentType::EDI_X12:
            minor = "EDI-X12";
            break;

        case SimpleHTTPContentType::EDIFACT:
            minor = "EDIFACT";
            break;

        case SimpleHTTPContentType::JSON:
            minor = "json";
            break;

        case SimpleHTTPContentType::JAVASCRIPT:
            minor = "javascript";
            break;

        case SimpleHTTPContentType::OCTET_STREAM:
            minor = "octet-stream";
            break;

        case SimpleHTTPContentType::OGG:
            minor = "ogg";
            break;

        case SimpleHTTPContentType::PDF:
            minor = "pdf";
            break;

        case SimpleHTTPContentType::POSTSCRIPT:
            minor = "postscript";
            break;

        case SimpleHTTPContentType::RDF_XML:
            minor = "rdf+xml";
            break;

        case SimpleHTTPContentType::RSS_XML:
            minor = "rss+xml";
            break;

        case SimpleHTTPContentType::SOAP_XML:
            minor = "soap+xml";
            break;

        case SimpleHTTPContentType::FONT_WOFF:
            minor = "font-woff";
            break;

        case SimpleHTTPContentType::XHTML_XML:
            minor = "xhtml+xml";
            break;

        case SimpleHTTPContentType::XML:
            minor = "xml";
            break;

        case SimpleHTTPContentType::XML_DTD:
            minor = "xml-dtd";
            break;

        case SimpleHTTPContentType::XOP_XML:
            minor = "xop-xml";
            break;

        case SimpleHTTPContentType::ZIP:
            minor = "zip";
            break;

        case SimpleHTTPContentType::GZIP:
            minor = "gzip";
            break;

        case SimpleHTTPContentType::BASIC:
            minor = "basic";
            break;

        case SimpleHTTPContentType::L24:
            minor = "L24";
            break;

        case SimpleHTTPContentType::MP4:
            minor = "mp4";
            break;

        case SimpleHTTPContentType::MPEG:
            minor = "mpeg";
            break;

        case SimpleHTTPContentType::VORBIS:
            minor = "vorbis";
            break;

        case SimpleHTTPContentType::VND_REALAUDIO:
            minor = "vnd.rn-realaudio";
            break;

        case SimpleHTTPContentType::VND_WAVE:
            minor = "vnd.wave";
            break;

        case SimpleHTTPContentType::WEBM:
            minor = "webm";
            break;

        case SimpleHTTPContentType::QUICKTIME:
            minor = "quicktime";
            break;

        case SimpleHTTPContentType::X_MATROSKA:
            minor = "x-matroska";
            break;

        case SimpleHTTPContentType::X_MS_WMV:
            minor = "x-ms-wmv";
            break;

        case SimpleHTTPContentType::X_FLV:
            minor = "x-flv";
            break;

        case SimpleHTTPContentType::GIF:
            minor = "gif";
            break;

        case SimpleHTTPContentType::JPEG:
            minor = "jpeg";
            break;

        case SimpleHTTPContentType::PJPEG:
            minor = "pjpeg";
            break;

        case SimpleHTTPContentType::PNG:
            minor = "png";
            break;

        case SimpleHTTPContentType::SVG:
            minor = "svg+xml";
            break;

        case SimpleHTTPContentType::TIFF:
            minor = "tiff";
            break;

        case SimpleHTTPContentType::HTTP:
            minor = "http";
            break;

        case SimpleHTTPContentType::IMDN:
            minor = "imdn+xml";
            break;

        case SimpleHTTPContentType::PARTIAL:
            minor = "partial";
            break;

        case SimpleHTTPContentType::RFC822:
            minor = "rfc822";
            break;

        case SimpleHTTPContentType::IGES:
            minor = "iges";
            break;

        case SimpleHTTPContentType::MESH:
            minor = "mesh";
            break;

        case SimpleHTTPContentType::VRML:
            minor = "vrml";
            break;

        case SimpleHTTPContentType::X3D_BINARY:
            minor = "x3d+binary";
            break;

        case SimpleHTTPContentType::X3D_FASTINFOSET:
            minor = "x3d+fastinfoset";
            break;

        case SimpleHTTPContentType::X3D_VRML:
            minor = "x3d-vrml";
            break;

        case SimpleHTTPContentType::X3D_XML:
            minor = "x3d+xml";
            break;

        case SimpleHTTPContentType::MIXED:
            minor = "mixed";
            break;

        case SimpleHTTPContentType::ALTERNATIVE:
            minor = "alternative";
            break;

        case SimpleHTTPContentType::RELATED:
            minor = "related";
            break;

        case SimpleHTTPContentType::FORM_DATA:
            minor = "form-data";
            break;

        case SimpleHTTPContentType::SIGNED:
            minor = "signed";
            break;

        case SimpleHTTPContentType::ENCRYPTED:
            minor = "encrypted";
            break;

        case SimpleHTTPContentType::CMD:
            minor = "cmd";
            break;

        case SimpleHTTPContentType::CSS:
            minor = "css";
            break;

        case SimpleHTTPContentType::SCV:
            minor = "csv";
            break;

        case SimpleHTTPContentType::HTML:
            minor = "html";
            break;

        case SimpleHTTPContentType::PLAIN:
            minor = "plain";
            break;

        case SimpleHTTPContentType::VCARD:
            minor = "vcard";
            break;

        case SimpleHTTPContentType::XFORMURLENCODED:
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
