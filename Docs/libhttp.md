#Compound type
- version_t : [enum verion](#Version)
- status_code_t : [enum status_code](#status_code)
- method_t : [enum Method](#Method)
- HTTPerr : [enum HTTPerr](#HTTPerr)
- HTTP_t : [struct HTTP](#HTTP)
- HTTPBody_t : [struct HTTPBody](#HTTPBody)
- HTTPDict_t : [struct HTTPDict](#HTTPDict)
- HTTPList_t : [struct HTTPList](#HTTPList)

#Functions prototypes
- [**crlf_chomp**](#crlf_chomp)(char \*);
- [**space_chomp**](#space_chomp)(char \*);
- [**long_to_str**](#long_to_str)(long, char \*, size_t);
- [**HTTP_init**](#HTTP_init)(void);
- [**HTTP_get_str_code**](#HTTP_get_str_code)(HTTP_t \*, status_code_t);
- [**HTTP_get_method**](#HTTP_get_method)(HTTP_t \*);
- [**HTTP_get_status_code**](#HTTP_get_status_code)(HTTP_t \*);
- [**HTTP_get_path**](#HTTP_get_path)(HTTP_t \*);
- [**HTTP_get_version**](#HTTP_get_version)(HTTP_t \*);
- [**HTTP_get_path_len**](#HTTP_get_path_len)(HTTP_t \*);
- [**HTTP_get_body_size**](#HTTP_get_body_size)(HTTP_t \*);
- [**HTTP_get_response_totalsize**](#HTTP_get_response_totalsize)(HTTP_t \*);
- [**HTTP_get_request_totalsize**](#HTTP_get_request_totalsize)(HTTP_t \*);
- [**HTTP_get_res_head_len**](#HTTP_get_res_head_len)(HTTP_t \*);
- [**HTTP_get_req_head_len**](#HTTP_get_req_head_len)(HTTP_t \*);
- [**HTTP_headers_get_val_with_key**](#HTTP_headers_get_val_with_key)(HTTP_t   \*, const char \*);
- [**HTTP_set_status_code**](#HTTP_set_status_code)(HTTP_t \*, status_code_t);
- [**HTTP_set_method**](#HTTP_set_method)(HTTP_t \*, method_t);
- [**HTTP_set_version**](#HTTP_set_version)(HTTP_t \*, version_t);
- [**HTTP_set_header**](#HTTP_set_header)(HTTP_t \*, const char \*, const char \*);
- [**HTTP_set_path**](#HTTP_set_path)(HTTP_t \*, char \*);
- [**HTTP_method_to_str**](#HTTP_method_to_str)(HTTP_t \*, method_t);
- [**HTTP_version_to_str**](#HTTP_version_to_str)(HTTP_t \*, version_t);
- [**HTTP_make_raw_request**](#HTTP_make_raw_request)(HTTP_t \*, void \*, size_t);
- [**HTTP_make_raw_response**](#HTTP_make_raw_response)(HTTP_t \*, void \*, size_t);
- [**HTTP_read_body**](#HTTP_read_body)(HTTP_t \*, void \*, ssize_t);
- [**HTTP_write_body**](#HTTP_write_body)(HTTP_t \*, void \*, ssize_t);
- [**HTTP_get_body_ptr**](#HTTP_get_body_ptr)(HTTP_t \*);
- [**HTTP_parse_req_raw**](#HTTP_parse_req_raw)(HTTP_t \*, void \*, size_t);
- [**HTTP_parse_res_raw**](#HTTP_parse_res_raw)(HTTP_t \*, void \*, size_t);
- [**HTTP_show_header**](#HTTP_show_header)(HTTP_t \*);
- [**HTTP_header_pop**](#HTTP_parse_res_raw)(HTTP_t \*);
- [**HTTP_header_key_val_clear**](#HTTP_header_key_val_clear)(HTTPDict_t \*);
- [**HTTP_headers_clear**](#HTTP_headers_clear)(HTTP_t \*);
- [**HTTP_clear**](#HTTP_clear)(HTTP_t \*\*);
#Enumerations
## Version
```c
enum Version {
    HTTPv0_9 = 0,
    HTTPv1_0,
    HTTPv1_1,
    HTTPv2,
    HTTPv3
};
```
## status_code
```c
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
```
## HTTPerr
```c
enum HTTPerr {
    HTTP_ERR = 0,
    HTTP_OK,
};
```
## Method
```c
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
```

# Structures
## HTTP
```c
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
```

## HTTPBody
```c
struct HTTPBody {
    char *data;
    ssize_t size;
};
```

## HTTPDict
```c
struct HTTPDict {
    char *key;
    char *val;
    ssize_t key_len;
    ssize_t val_len;
};
```
## HTTPList
```c
struct HTTPList {
    struct HTTPDict *dict;
    struct HTTPList *next;
};
```

# Functions
## crlf_chomp 
```c
char *crlf_chomp(char *buff);
```
Remove first occurence of **CRFL** (`\r\n`) in `buff`.
- Param `buff` 
> Pointer to string with **CRLF** to remove.

- Return
> Pointer to `buff` with `\0` instead **CRLF**.

## space_chomp
```c
char *space_chomp(char *buff);
```
Remove first occurence of space in `buff`.
- Param `buff` 
> Pointer to string with space to remove.

- Return
> Pointer to `buff` with `\0` instead space.

## long_to_str
```c
size_t long_to_str(long dec, char *buf, size_t buf_size);
```

Get string value of `dec`.

- Param `dec`.
> Long decimal to get at string.

- Param `buf`.
> Pointer to buffer who store string value.

- Param `buf_size`.
> Size of `buf`.

- Return
> Number of char printed in `buf`.

## HTTP_init
```c
HTTP_t *HTTP_init(void);
```

Make a HTTP context.

- Return
> HTTP_t context instance.

## HTTP_get_str_code
```c
char *HTTP_get_str_code(HTTP_t *http, status_code_t code);
```
Get string code message.

- Param `http`
> Pointer to http context.

- Param `code`
> status_code enumeration.

- Return
> Status code string message.

## HTTP_get_path
```c
char *HTTP_get_path(const HTTP_t *http);
```
Get a copy of path string.
**/!\ This function return a pointer to a malloc, don't forget to free the pointer after use. /!\ **

- Param `http`.
> Pointer to HTTP context.

- Return
> Pointer to a malloc containing string of path.

## HTTP_get_version
```c
version_t HTTP_get_version(const HTTP_t *http);
```
Get version of http.

- Param `http`.
> Pointer to http context.

- Return
> status_code enumeration.

## HTTP_get_path_len
```c
ssize_t HTTP_get_path_len(const HTTP_t *http);
```
Get len of path in http.

- Param `http`.
> Pointer to http context.

- Return
> len of path.

## HTTP_get_body_size
```c
ssize_t HTTP_get_body_size(const HTTP_t *http);
```
 Get size of http body.

- Param `http`.
> Pointer to http context.

- Return
> Size of body.

## HTTP_get_response_totalsize
```c
ssize_t HTTP_get_response_totalsize(HTTP_t *http);
```
Get total size of response http.

- Param `http`.
> Pointer to http context.

- Return
> Total size of response http.

## HTTP_get_request_totalsize
```c
ssize_t HTTP_get_request_totalsize(HTTP_t *http);
```
Get total size of request http.

- Param `http`.
> Pointer to http context.

- Return
> Total size of request http.

## HTTP_get_res_head_len
```c
ssize_t HTTP_get_res_head_len(HTTP_t *http);
```
Get len of head of response HTTP.
Typically is `HTTP_get_res_totalsize()` without body size.

- Param `http`.
> Pointer to http context.

- Return
> Len of head of response http.

## HTTP_get_req_head_len
```c
ssize_t HTTP_get_req_head_len(HTTP_t *http);
```
Get len of head of request HTTP.
Typically is [HTTP_get_req_totalsize](#HTTP_get_req_totalsize) without body size.

- Param `http`.
> Pointer to http context.

- Return
> Len of head of request http.

## HTTP_headers_get_val_with_key
```c
HTTPDict_t *HTTP_headers_get_val_with_key(HTTP_t *http, const char *key);
```
Get a copy of the pointer to the header with key `key`.
**/!\ This function return a pointer to a malloc, don't forget to use [HTTP_header_key_val_clear](#HTTP_header_key_val_clear) and free the pointer after use. /!\ **

- Param `http`.
> Pointer to http context.

- Param `key`.
> String key to find.

- Return
> NULL or a copy of pointer to the HTTPDict struct contains value of `key`. 

##  HTTP_set_status_code
```c
int HTTP_set_status_code(HTTP_t *http, status_code_t code);
```
Set status code HTTP.

- Param `http`.
> Pointer to http context.

- Param `code`.
> status_code enumeration.

- Return
> HTTP_OK or HTTP_ERR.

## HTTP_set_method
```c
int HTTP_set_method(HTTP_t *http, method_t method);
```
Set method HTTP.

- Param `http`.
> Pointer to http context.

- Param `method`.
> method_t enumeration.

- Return
> HTTP_OK or HTTP_ERR

## HTTP_set_version
```c
int HTTP_set_version(HTTP_t *http, version_t version);
```
Set version HTTP.

- Param `http`.
> Pointer to http context.

- Param `version`.
> version_t enumeration.

- Return
> HTTP_OK or HTTP_ERR

## HTTP_set_header
```c
int HTTP_set_header(HTTP_t *header, const char *strkey, const char *strval);
```
Set header HTTP.

- Param `http`.
> Pointer to http context.

- Param `strkey`.
> String key.

- Param `strval`.
> String value.

- Return
> HTTP_OK or HTTP_ERR

## HTTP_set_path
```c
int HTTP_set_path(HTTP_t *header, char *path);
```
Set path http.

- Param `http`.
> Pointer to http context.

- Param `path`.
> String path.

- Return
> HTTP_OK or HTTP_ERR

## HTTP_method_to_str
```c
char *HTTP_method_to_str(HTTP_t *http, method_t method);
```
Get string value of method_t enumeration.

- Param `http`.
> Pointer to http context.

- Param `method`.
> method_t enumeration.

- Return
> string value of `method`.

## HTTP_version_to_str
```c
char *HTTP_version_to_str(HTTP_t *http, version_t version);
```
Get string value of version_t enumeration.

- Param `http`.
> Pointer to http context.

- Param `version`.
> version_t enumeration.

- Return
> string value of `version`.

## HTTP_make_raw_request
```c
size_t HTTP_make_raw_request(HTTP_t *http, void *buf, size_t buf_size);
```
Make raw request HTTP

- Param `http`.
> Pointer to http context.

- Param `buf`.
> Pointer to a buffer who store raw request.

- Param `buf_size`.
> Size of `buf`.

- Return
> Size writted in `buf`.

## HTTP_make_raw_response
```c
size_t HTTP_make_raw_responset(HTTP_t *http, void *buf, size_t buf_size);
```
Make raw response HTTP

- Param `http`.
> Pointer to http context.

- Param `buf`.
> Pointer to a buffer who store raw response.

- Param `buf_size`.
> Size of `buf`.

- Return
> Size written in `buf`.

## HTTP_read_body
```c
ssize_t HTTP_read_body(HTTP_t *http, void *buf, ssize_t nbyte);
```
Copy `nbyte` byte of body http inside `buf`.

- Param `http`.
> Pointer to http context.

- Param `buf`.
> Pointer to buffer who stored body.

- Param `nbytes`.
> Number of byte to copy inside `buf`.

- Return
> number of byte writed.

## HTTP_write_body
```c
ssize_t HTTP_write_body(HTTP_t *http, void *buf, ssize_t nbyte);
```
Copy `nbyte` byte of `buf` inside http body.

- Param `http`.
> Pointer to http context.

- Param `buf`.
> Pointer to buffer who stored body.

- Param `nbytes`.
> Number of byte to copy inside http body.

- Return
> Number of byte writted.

## HTTP_get_body_ptr
```c
HTTPBody_t *HTTP_get_body_ptr(HTTP_t *http);
```
Get pointer to HTTPBody_t struct who contains pointer to body.
**/!\ Do NOT free the pointer ! is used by http context. /!\ **

- Param `http`.
> Pointer to http context.

- Return
> Pointer to HTTPBody_t structure

## HTTP_parse_req_raw
```¢
int HTTP_parse_req_raw(HTTP_t *http, void *raw, size_t size_raw);
```
Parse http raw request.

- Param `http`.
> Pointer to http context.

- Param `raw`.
> Pointer to the buffer who store raw request http.

- Param `size_raw`.
> Size of `raw`.

- Return
> HTTP_OK or HTTP_ERR

## HTTP_parse_res_raw
```¢
int HTTP_parse_res_raw(HTTP_t *http, void *raw, size_t size_raw);
```
Parse http raw response.

- Param `http`.
> Pointer to http context.

- Param `raw`.
> Pointer to the buffer who store raw response http.

- Param `size_raw`.
> Size of `raw`.

- Return
> HTTP_OK or HTTP_ERR

## HTTP_show_header
```c
void HTTP_show_header(HTTP_t *http);
```
Print all key, value header use inside http request/response.

- Param `http`.
> Pointer to http context.

## HTTP_header_pop
```¢
void HTTP_header_pop(HTTP_t *http);
```
Remove (pop) the last header set with [**HTTP_set_header**](#HTTP_set_header).

- Param `http`
> Pointer to http context.

## HTTP_header_key_val_clear
```c
void HTTP_header_key_val_clear(HTTPDict_t *dict);
```
Clear key and value inside HTTPDict_t structure.

- Param `dict`.
> Pointer to HTTPDict_t structure with key, val to free.

## HTTP_headers_clear
```c
void HTTP_headers_clear(HTTP_t *http);
```
Clear all headers setted with [**HTTP_set_header**](#HTTP_set_header) or parsed.

- Param `http`.
> Pointer to http context.

## HTTP_clear
```c
void HTTP_clear(HTTP_t **http);
```
Clear http context.

- Param `http`.
> Address of pointer to http context.