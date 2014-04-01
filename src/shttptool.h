#ifndef __LIBSHTTPTOOL_H__
#define __LIBSHTTPTOOL_H__

////////////////////////////////////////////////////////////////////////////////
// A static tool for Content type
// ---------------------------------------------------------------------------
// mime type defined in http://en.wikipedia.org/wiki/MIME_type
////////////////////////////////////////////////////////////////////////////////

#include <string>

class SimpleHTTPTool
{
    typedef unsigned int    SHTTPContentType;

    typedef enum
    {
        APPLICATION = 0x00010000,
        AUDIO       = 0x00020000,
        IMAGE       = 0x00040000,
        MESSAGE     = 0x00080000,
        MODEL       = 0x00100000,
        MULTIPART   = 0x00200000,
        TEXT        = 0x00400000,
        VIDEO       = 0x00800000
    }SHTTPCT_MAJOR;

    typedef enum
    {
        ATOM_FEEDS  = 100,  /// Application ----
        ECMASCRIPT,
        EDI_X12,
        EDIFACT,
        JSON,
        JAVASCRIPT,
        OCTET_STREAM,
        OGG,
        PDF,
        POSTSCRIPT,
        RDF_XML,
        RSS_XML,
        SOAP_XML,
        FONT_WOFF,
        XHTML_XML,
        XML,
        XML_DTD,
        XOP_XML,
        ZIP,
        GZIP,

        BASIC       = 200,  /// Audio, Video ----
        L24,
        MP4,
        MPEG,
        //OGG, = Application-OGG
        VORBIS,
        VND_REALAUDIO,
        VND_WAVE,
        WEBM,
        QUICKTIME,
        X_MATROSKA,
        X_MS_WMV,
        X_FLV,

        GIF         = 300,  /// Image ----
        JPEG,
        PJPEG,
        PNG,
        SVG,
        TIFF,

        HTTP        = 400,  /// Message ----
        IMDN,
        PARTIAL,
        RFC822,

        IGES        = 500,  /// Models ----
        MESH,
        VRML,
        X3D_BINARY,
        X3D_FASTINFOSET,
        X3D_VRML,
        X3D_XML,

        MIXED       = 600,  /// Multipart ---- (Email)
        ALTERNATIVE,
        RELATED,
        FORM_DATA,
        SIGNED,
        ENCRYPTED,

        CMD         = 700,  /// Text ----
        CSS,
        SCV,
        HTML,
        //JAVASCRIPT = Application-JAVASCRIPT
        PLAIN,
        VCARD,
        //XML = Application-XML

    }SHTTPCT_MINOR;

    public:
        static const char* GetMIME( SHTTPContentType ctype );
        static const char* GetMIME( SHTTPCT_MAJOR mjr, SHTTPCT_MINOR mnr );

};

#endif /// of __LIBSHTTPTOOL_H__
