#ifndef _LIB_HTTP_H_
#define _LIB_HTTP_H_

#include <linux/limits.h>

#define SIZE_WRITE(x, y) \
    ((x > y) ? y : x)

#define ARRAY_SIZE(x) \
    (sizeof(x)/sizeof(x[0]))

#define CRLF "\r\n"

#define HTTP_VERSION_MAX_LEN 8 /* HTTP/x.x */

#define CODE_INFO 100
#define CODE_SUCCESS 200
#define CODE_REDIRECT 300
#define CODE_CLI_ERR 400
#define CODE_SERV_ERR 500

typedef enum Version version_t;
typedef enum status_code status_code_t;
typedef enum Method method_t;
typedef enum Version version_t;
typedef enum HTTPerr HTTPerr_t;

typedef struct HTTP HTTP_t;
typedef struct HTTPBody HTTPBody_t;
typedef struct HTTPDict HTTPDict_t;
typedef struct HTTPList HTTPList_t;

enum HTTPerr {
    HTTP_ERR = 0,
    HTTP_OK,
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
    HTTPv2,
    HTTPv3
};

struct HTTPDict {
    char *key;
    char *val;
    ssize_t key_len;
    ssize_t val_len;
};

struct HTTPList {
    struct HTTPDict *dict;
    struct HTTPList *next; 
};

struct HTTPBody {
    char *data;
    ssize_t size;
};

enum Method {
    UNKNOWN = 0,
    GET,
    PUT,
    POST,
    HEAD,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH,
};

struct _Response {
    enum status_code code;
    char *str_code;
    size_t str_code_len;
};

struct _Request {
    enum Method method;
    char *array_str_meth[10];
    char *path;
    size_t path_len;   
};

struct _array_str {
    char *code_info[4];
    char *code_success[27];
    char *code_redirect[11];
    char *code_cli_err[100];
    char *code_serv_err[28];
};

struct HTTP {
    struct _array_str array_code;
    struct _Request req;
    struct _Response res;
    char version;
    char *array_str_ver[5];   
    
    struct HTTPList *headers;
    ssize_t headers_len;
    struct HTTPBody body;
};

/* @brief
 *  Remove first occurence of CRLF (\r\n) in buff.
 *
 * @param buff
 *  string with CRFL to remove.
 *
 * @return
 *  buff without CRLF.
 * */
char *crlf_chomp(char *buff);


/* @brief
 *  Remove first occurence of space in buff.
 *
 * @param buff
 *  string with space to remove.
 *
 * @return
 *  buff without space.
 * */
char *space_chomp(char *buff);


/* @brief
 *  Get the length of all headers set.     
 *  
 * @param list
 *  Pointer of linked list of headers.
 *
 * @return 
 *  Total lenght of key(s)/value(s) string set in list.
 * */
ssize_t HTTP_get_headers_length(HTTPList_t *list);


/* @brief
 *  Get the string value of method enumeration.
 *  
 * @param http
 *  Pointer of http.
 * 
 * @param method
 *  Method to get at string.
 *  
 * @return
 *  String value of method.
 * */
char *HTTP_method_to_str(HTTP_t *http, method_t method);


/* @brief
 *  Get string value of version enumeration.
 *  
 * @param http
 *  Pointer of http.
 *
 * @param version
 *  Version to get at string.
 *
 * @return
 *  string value of version.
 * */
char *HTTP_version_to_str(HTTP_t *http, version_t version);

    
/* @brief
 *  Get length of path.
 *  
 * @param http
 *  Pointer to http.
 *
 * @return
 *  Length of path set.
 * */
ssize_t HTTP_get_path_len(const HTTP_t *http);


/* @brief
 *  Get size of body data.
 *
 * @param http
 *  Pointer to http.
 *
 * @return
 *  Size of body data set. 
 * */
ssize_t HTTP_get_body_size(const HTTP_t *http);


/* @brief
 *  Get string value of long object.
 * 
 * @param dec
 *  Decimal/long object.
 *
 * @param buf
 *  Pointer to buffer who store string.
 *
 * @param buf_size
 *  Size of buf.
 *
 * @return
 *  Number of char printed in buf.
 * */
size_t long_to_str(long dec, char *buf, size_t buf_size);


/* @brief
 *  Get total size of response http.
 *
 * @param http
 *  Pointer to http.
 *
 * @return
 *  Size of http response.
 * */
ssize_t HTTP_get_response_totalsize(HTTP_t *http);


/* @brief
 *  Get total size of request http.
 *
 * @param http
 *  Pointer to http.
 *  
 * @return
 *  Size of http request.
 * */
ssize_t HTTP_get_request_totalsize(HTTP_t *http);


/* @brief
 *  Set header key value.
 *  
 * @param header
 *  Pointer to http.
 *
 * @param strkey
 *  String of key.
 *
 * @param strval
 *  String of value.
 *
 * @return 
 *  HTTP_OK or HTTP_ERR;
 *
 * */
int HTTP_set_header(HTTP_t *header, const char *strkey, const char *strval);


/* @brief
 *  Create raw request http and copy this in buf.
 * 
 * @param header
 *  Pointer to http.
 *
 * @param buf
 *  Pointer to buff who store raw request HTTP.
 *
 * @param buf_size
 *  Size of buf.
 *
 * @return
 *  size writted in buf.
 * */
size_t HTTP_make_raw_request(HTTP_t *header, void *buf, size_t buf_size);


/* @brief
 *  Create raw response http and copy this in buf.
 * 
 * @param header
 *  Pointer to http.
 *
 * @param buf
 *  Pointer to buff who store raw response HTTP.
 *
 * @param buf_size
 *  Size of buf.
 *
 * @return
 *  Size writted in buf.
 * */
size_t HTTP_make_raw_response(HTTP_t *http, void *buf, size_t buf_size);


/* @brief
 *  Set path in http header
 *
 * @param header
 *  Is http type 
 *  
 * @param path
 *  string of path to set in header.
 *
 * @return
 *  HTTP_OK or HTTP_ERR.
 */
int HTTP_set_path(HTTP_t *header, char *path);


/* @brief
 *  Create an copy of body in header->body
 *  
 * @return
 *  bytes writen.
 *
 * @param header
 *  Is whole header HTTP.
 *
 * @param buf
 *  Pointer to buffer with data to write.
 *
 * @param nbyte
 *  Size of buf.
 */
ssize_t HTTP_write_body(HTTP_t *http, void *buf, ssize_t nbyte);
 

/* @brief
 *  Parse an http request.
 *
 * @param http
 *  Pointer to http.
 *
 * @param raw
 *  Pointer to raw http request.
 *
 * @param size_raw.
 *  Size of raw.
 * 
 * @return
 *  HTTP_OK or HTTP_ERR
 * */
int HTTP_parse_req_raw(HTTP_t *http, void *raw, size_t size_raw); 


/* @brief
 *  Remove first header set.
 *
 * @param http
 *  Pointer to http.
 * */
void HTTP_header_pop(HTTP_t *http);


/* @brief
 *  Show all headers http.
 *
 * @param http
 *  Pointer to http.
 *
 * */
void HTTP_show_header(HTTP_t *http);


/* @brief
 *  Clear all header http.
 * 
 * @param http
 *  Pointer to http.
 * */
void HTTP_headers_clear(HTTP_t *http);


/* @brief
 *  Get string value of status code http.
 *
 * @param http
 *  Pointer to http.
 *
 * @param code
 *  Status code to get.
 *
 * @return 
 *  String value of code.
 * */
char *HTTP_get_str_code(HTTP_t *http, status_code_t code);


/* @brief
 *  Set an status code http.
 *  
 * @param http
 *  Pointer to http.
 *  
 * @param code
 *  Status code to set.
 *
 * @return
 *  HTTP_OK or HTTP_ERR
 * */
int HTTP_set_status_code(HTTP_t *http, status_code_t code);

/* @brief
 *  Init http.
 *  
 * @return
 *  http
 * */
HTTP_t *HTTP_init(void);


/* @brief
 *  Clear http
 *
 * @param http
 *  Pointer to http.
 * */
void HTTP_clear(HTTP_t **http);


/* @brief
 *  Set method http.
 *
 * @param http
 *  Pointer to http.
 *
 * @param method
 *  Http method to set.
 *
 * @return
 *  OK or HTTP_ERR
 * */
int HTTP_set_method(HTTP_t *http, method_t method);


/* @brief
 *  Get method http.
 *
 * @param http
 *  Pointer to http.
 * 
 * @return
 *  Http method enumeration
 *
 * */
method_t HTTP_get_method(HTTP_t *http);


/* @brief
 *  Get status code HTTP.
 * 
 * @param http
 *  Pointer to http.
 *
 * @return
 *  Http Status code enumeration.
 * */
status_code_t HTTP_get_status_code(HTTP_t *http);


/* @brief
 *  get path in header HTTP
 *
 * @return
 *  pointer of path strings
 *
 * @details
 *  WARNING:
 *  This function use malloc, dont forget to free() the ptr returned.
 *
 * @param header
 *  Pointer to http
 */
char *HTTP_get_path(HTTP_t *header);


/* @brief
 *  Set http version.
 *
 * @param http
 *  Pointer to http.
 *
 * @param version
 *  Version enumeration to set.
 *
 * @return
 *  HTTP_OK or HTTP_ERR
 *
 * */
int HTTP_set_version(HTTP_t *header, version_t version);


/* @brief
 *  Get version http.
 *  
 * @param http
 *  Pointer to http.
 *
 * @return
 *  Http version enumeartion.
 * */
version_t HTTP_get_version(const HTTP_t *http);


/*
 * @brief
 *  Read body http into buf.
 *
 * @details
 *  This function read nbyte bytes of body in to buf.
 *  This function set nbyte 0 in to buf automatically.
 *  If nbyte is greater of real body size, only the real size is write in to buf
 *  else nbyte is written.
 *
 * @return
 *  byte written.
 *  0 if buf is null or if memcpy failed.
 *  
 * @param header
 *  Is whole header HTTP
 * 
 * @param *buf
 *  Pointer to buffer who store data.
 *  
 *
 * @param nbyte
 *  Size of *buf param.
 */
ssize_t HTTP_read_body(HTTP_t *http, void *buf, ssize_t nbyte);


/* @brief
 *  Parse response http
 *
 * @param http
 *  Pointer to http.
 * 
 * @param raw
 *  Pointer to raw response http.
 *
 * @param size_raw
 *  Size of raw.
 *
 * @return
 *  HTTP_OK or HTTP_ERR;
 * */
int HTTP_parse_res_raw(HTTP_t *http, void *raw, size_t size_raw);


/* @brief
 *  Get response len without body size
 *
 * @param http
 *  Pointer to http
 *
 * @return
 *  Length of response head http
 * */
ssize_t HTTP_get_res_head_len(HTTP_t *http);


/* @brief
 *  Get request len without body size
 *
 * @param http
 *  Pointer to http
 *
 * @return
 *  Length of request head http
 * */
ssize_t HTTP_get_req_head_len(HTTP_t *http);


/* @brief
 *  Get value string of headers with specific key.
 *
 * @param http
 *  Pointer to http.
 *
 * @param key
 *  Key string to search.
 *
 * @return
 *  Pointer to a malloc of HTTPDict struct.
 *
 * */
HTTPDict_t *HTTP_headers_get_val_with_key(HTTP_t *http, const char *key);


/* @brief
 *  Free key and value dict.
 *  
 * @param dict
 *  Pointer to HTTPDict struct.
 * */
void HTTP_header_key_val_clear(HTTPDict_t *dict);


/* @brief 
 *  Get pointer to http body structure.
 *
 * @param http
 *  Pointer to http.
 *
 * @return
 *  NULL or address of body structure.
 * */
HTTPBody_t *HTTP_get_body_ptr(HTTP_t *http);

#endif /* _LIB_HTTP_H_ */
