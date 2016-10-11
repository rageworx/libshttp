#ifndef __LIBSHTTPSTATUS_H__
#define __LIBSHTTPSTATUS_H__

// Refer to http://en.wikipedia.org/wiki/List_of_HTTP_status_codes

typedef enum
{
    HTTPSTAT_NONE           = 0,

    // -- Informations
    HTTPSTAT_CONTINUE       = 100,
    HTTPSTAT_SP,                    /// == Switching Protocols.
    HTTPSTAT_PROCESSING,

    // -- Success
    HTTPSTAT_OK             = 200,
    HTTPSTAT_CREATED,
    HTTPSTAT_ACCEPTED,
    HTTPSTAT_NAI,                   /// == Non-Authorized Information ( HTTP/1.1 )
    HTTPSTAT_NOCONENTS,
    HTTPSTAT_RESETCONTENTS,
    HTTPSTAT_PARTIALCONTENTS,
    HTTPSTAT_MULTISTATUS,
    HTTPSTAT_ALREADYREPORTED,
    HTTPSTAT_IMUSED         = 226,

    // -- Redirection
    HTTPSTAT_MC             = 300,  /// == Multiple choices
    HTTPSTAT_MP,                    /// == Moved Permanetly
    HTTPSTAT_FOUND,
    HTTPSTAT_SEEOTHER,              /// == See Other ( HTTP/1.1 )
    HTTPSTAT_NOTMODIFIED,
    HTTPSTAT_USEPROXY,              /// == Use Proxy ( HTTP/1.1 )
    HTTPSTAT_SPROXY,                /// == Switch Proxy
    HTTPSTAT_TMPREDIR,              /// == Temporary Redirect ( HTTP/1.1 )
    HTTPSTAT_PERMREDIR,             /// == Permanent Redirect

    // --- Client Error
    HTTPSTAT_BADREQ         = 400,  /// == Bad Request
    HTTPSTAT_UNAUTHORIZED,
    HTTPSTAT_PAYMENTREQ,
    HTTPSTAT_FORBIDDEN,
    HTTPSTAT_NOTFOUND,
    HTTPSTAT_MNA,                   /// == Method Not Allowed
    HTTPSTAT_NOTACCEPT,
    HTTPSTAT_PROXYAR,               /// == Proxy Authentication Required.
    HTTPSTAT_REQTIMEOUT,
    HTTPSTAT_CONFLICT,
    HTTPSTAT_GONE,
    HTTPSTAT_LENGTHREQ,
    HTTPSTAT_PRECONFAIL,            /// == Precondition failed.
    HTTPSTAT_RETL,                  /// == Request Entity Too Large
    HTTPSTAT_RUTL,                  /// == Request URL Too Long
    HTTPSTAT_UMT,                   /// == Unsupported Media Type
    HTTPSTAT_RRNS,                  /// == Requested Range Not Satisfiable
    HTTPSTAT_EXCEPTIONFAILED,
    HTTPSTAT_IAMTEAPOT,
    HTTPSTAT_AUTHTIMEDOUT,
    HTTPSTAT_METHODFAIL,            /// Same with Enhance your claim ( 420 )

    HTTPSTAT_UENTITY        = 422,  /// == Unprocessable Entitly
    HTTPSTAT_LOCKED,
    HTTPSTAT_FAILEDDEP,             /// == Failed dependency
    HTTPSTAT_UC,                    /// == Unordered collection ( Internet draft)
    HTTPSTAT_UPGRADEREQ,
    HTTPSTAT_PRECONREQ,             /// == Precondition required.
    HTTPSTAT_TMR,                   /// == Too many requests

    HTTPSTAT_RHFTL          = 431,  /// == Request Header Fields Too Large
    HTTPSTAT_LOGINTO        = 440,  /// == LogIn Timeout (Microsoft)
    HTTPSTAT_NORESP         = 444,  /// == No response (Nginx)
    HTTPSTAT_RETRYW         = 449,  /// == Retry Width (Microsoft)
    HTTPSTAT_BBWPC,                 /// == Blocked by Windows Parental Controls (Microsoft)
    HTTPSTAT_REDIRECTMS,            /// == Redirect (Microsoft)

    HTTPSTAT_RHTL           = 494,  /// == Request header too large (NGinx)
    HTTPSTAT_CERTERR,
    HTTPSTAT_NOCERT,
    HTTPSTAT_HTTP2HTTPS,
    HTTPSTAT_CCREQ,                 /// == Client Closed Request (Nginx)

    // --- Server Error
    HTTPSTAT_INTSVRERR      = 500,  /// == Internal Server Error
    HTTPSTAT_NOTIMPL,               /// == Not implemented
    HTTPSTAT_BADGW,                 /// == Bad gateway
    HTTPSTAT_SVRUNAVL,              /// == Server unavailable
    HTTPSTAT_GWTIMEDOUT,            /// == Gateway timed out
    HTTPSTAT_HTTPVERNS,             /// == HTTP version not supported
    HTTPSTAT_VARAN,                 /// == Variant also negotiates
    HTTPSTAT_INSFSTOR,              /// == Insufficient Storage
    HTTPSTAT_LOOPDETECTED,
    HTTPSTAT_BANDWLE,               /// == Bandwidth limit exceeded
    HTTPSTAT_NOTEXTENDED,
    HTTPSTAT_NETAUTHREQ,            /// == Netword Authentication Requred

    HTTPSTAT_ORIGINERR      = 520,  /// == Origin error (Cloudflare)
    HTTPSTAT_CONNTO         = 522,  /// == Connection timed out (Cloudflare)
    HTTPSTAT_PDREQ,                 /// == Proxy declined request (Cloudflare)
    HTTPSTAT_ATO,                   /// == A timeout occurred (Cloudflare)

    HTTPSTAT_NETREADTOE     = 598,  /// == Network read timeout error (Unknown)
    HTTPSTAT_NETCONNTOE,            /// == Network connect timeout error (Unknown)

    HTTPSTAT_MAX

}SHTTPstatusType;

#endif /// of __LIBSHTTPSTATUS_H__
