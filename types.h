#ifndef _TYPES_H_
#define _TYPES_H_

#if 0
#define CRLF "\r\n"

#define NB_METHOD 10
#define NB_VERSION 3

#define SIZE_CODE_INFO 4
#define SIZE_CODE_SUCCESS 27
#define SIZE_CODE_REDIRECT 11
#define SIZE_CODE_CLI_ERR 100
#define SIZE_CODE_SERV_ERR 28

#define CODE_INFO 100
#define CODE_SUCCESS 200
#define CODE_REDIRECT 300
#define CODE_CLI_ERR 400
#define CODE_SERV_ERR 500

#define PATH_MAX 4096
#endif

#include "macro.h"

typedef enum status_code status_code_t;
typedef enum Method method_t;
typedef enum Version version_t;

typedef struct HTTP HTTP_t;
typedef struct HTTPBody HTTPBody_t;
typedef struct HTTPDict HTTPDict_t;
typedef struct HTTPList HTTPList_t;

enum HTTPerr {
    HTTP_ERR = 0,
    HTTP_OK,
    HTTP_METHOD_ERR,
    HTTP_VERSION_ERR,
    HTTP_STATUS_ERR
};

enum status_code {
    /* 1xx Information */
    CONTINUE = 100,
    SWITCH_PROTOCOL,
    PROCESSING,
    EARLY_HINTS,

    /* 2xx Success */
    OK = 200,
    CREATED,
    ACCEPTED,
    NON_AUTHORITATIVE_INFO,
    NO_CONTENT,
    RESET_CONTENT,
    PARTIAL_CONTENT,
    MULTI_STATUS,
    ALREADY_REPORTED,
    CONTENT_DIFF = 210,
    IM_USED = 226,
    
    /* 3xx Redirect */
    MULTIPLE_CHOICES = 300,
    MOVED_PERMANENTLY,
    FOUND,
    SEE_OTHER,
    NOT_MODIFIED,
    USE_PROXY,
    SWITCH_PROXY,
    TEMPORARY_REDIRECT,
    PERMANENT_REDIRECT,
    TOO_MANY_REDIRECTS = 310,

    /* 4xx Client Error */
    BAD_REQUEST = 400,
    UNAUTHORIZED,
    PAYMENT_REQUIRED,
    FORBIDDEN,
    NOT_FOUND,
    METHOD_NOT_ALLOWED,
    NOT_ACCEPTABLE,
    PROXY_AUTH_REQUIRED,
    REQUEST_TIMEOUT,
    CONFLICT,
    GONE,
    LENGTH_REQUIRED,
    PRECONDITION_FAILED,
    REQUEST_ENTITY_TOO_LARGE,
    REQUEST_URI_TOO_LONG,
    UNSUPORTED_MEDIA_TYPE,
    REQUESTED_RANGE_UNSATISFIABLE,
    EXCPECTATION_FAILED,
    IM_TEAPOT, /* why not :shrug: */
    BAD_MAPPING = 421,
    UNPROCESSABLE_ENTITY,
    LOCKED,
    METHOD_FAILURE,
    TOO_EARLY,
    UPGRADE_REQUIRED,
    PRECONDITION_REQUIRED = 428,
    TOO_MANY_REQUESTS,
    REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
    RETRY_WITH = 449,
    BLOCKED_BY_WINDOWS_PARENTAL_CONTROLS,
    UNVAILABLE_FOR_LEGAL_REASONS,
    UNRECOVERABLE_ERROR = 456,

    /* 4xx Code extend of nginx server */
    NO_RESPONSE = 444,
    SSL_CERTIFICATE_ERROR = 495,
    SSL_CERTIFICATE_REQUIRED,
    HTTP_REQ_SEND_TO_HTTPS_PORT,
    TOKEN_EXPIRED, TOKEN_INVALID = 498,
    CLIENT_CLOSED_REQUEST,


    /* 5xx Server Error */
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED,
    BAD_GATEWAY, PROXY_ERROR = 502,
    SERVICE_UNAVAILABLE,
    GATEWAY_TIME_OUT,
    HTTP_VERSION_NOT_SUPPORTED,
    VARIANT_ALSO_NEGOTIATES,
    INSUFFICIENT_STORAGE,
    LOOP_DETECTED,
    BANDWIDTH_LIMIT_EXCEEDED,
    NOT_EXTENDED,
    NETWORK_AUTH_REQUIRED,
    
    /* 5xx Code extended by cloudflare */
    HTTP_ERROR = 520,
    WEB_SERVER_IS_DOWN,
    CONNECTION_TIMED_OUT,
    ORIGIN_IS_UNREACHABLE,
    A_TIMEOUT_OCCURED,
    SSL_HANDSHAKE_FAILED,
    INVALID_SSL_CERTIFICATE,
    RAILGUN_ERROR
};

enum Version {
    HTTPv0_9 = 0,
    HTTPv1_0,
    HTTPv1_1,
};

struct HTTPDict {
    char *key;
    char *val;
    size_t key_len;
    size_t val_len;
};

struct HTTPList {
    struct HTTPDict *dict;
    struct HTTPList *next; 
};

struct HTTPBody {
    char *data;
    size_t size;
};

enum Method {
    GET = 0,
    PUT,
    POST,
    HEAD,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH,
    M_SEARCH,
};

struct _Response {
    status_code_t code;
    char *str_code;
    size_t str_code_len;
};

struct _Request {
    method_t method;
    HTTPList_t *query;
    size_t query_len;
    char *array_str_meth[NB_METHOD];
    char *path;
    size_t path_len;   
};

struct _array_str {
    char *code_info[SIZE_CODE_INFO];
    char *code_success[SIZE_CODE_SUCCESS];
    char *code_redirect[SIZE_CODE_REDIRECT];
    char *code_cli_err[SIZE_CODE_CLI_ERR];
    char *code_serv_err[SIZE_CODE_SERV_ERR];
};

struct HTTP {
    struct _array_str array_code;
    struct _Request req;
    struct _Response res;
    version_t version;
    char *array_str_ver[NB_VERSION];   
    
    HTTPList_t *headers;
    size_t headers_len;
    HTTPBody_t body;
};

#endif /* _TYPES_H_ */
