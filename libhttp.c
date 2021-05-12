#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libhttp.h"

/* @brief
 *  Write header inside buf
 *
 * @param list
 *  Pointer to HTTPList_t
 *
 * @param buf
 *  Pointer to buffer where write headers.
 *
 * @param buf_size
 *  Size of buf
 *
 * @return
 *  Size writted
 * */
static size_t _HTTP_write_header(HTTPList_t *list, void *buf, size_t buf_size);

/* @brief
 *  Parse methode in raw
 *
 * @param http
 *  Pointer to http
 *
 * @param raw
 *  Pointer to buffer with method to parse.
 *
 * @return
 *  method len
 * */
static size_t _HTTP_parse_method(HTTP_t *http, char *raw);

/* @brief
 *  Parse version in raw
 *
 * @param http
 *  Pointer to http
 *
 * @param raw
 *  Pointer to buffer with version to parse.
 *
 * @return
 *  version len
 * */
static size_t _HTTP_parse_version(HTTP_t *http, char *raw);

/* @brief
 *  Parse header in raw
 *
 * @param http
 *  Pointer to http
 *
 * @param raw
 *  Pointer to buffer with headers to parse.
 *
 * @return
 *  Pointer to the end of headers in raw.
 * */
static char *_HTTP_parse_header(HTTP_t *http, char *raw);

/* @brief
 *  Get base code
 *  (Ex: base code of 404 and 302 is 400 and 300)
 *
 * @param code
 *  status_code enum.
 *
 * @return
 *  base code
 * */
static int _HTTP_get_base_code(status_code_t code);

/* @brief
 *  Get index of code
 *  (Ex: index of 404 and 302 is 4 and 2)
 *
 * @param code
 *  status_code enum.
 *
 * @return
 *  index of code.
 * */
static int _HTTP_get_index_array_code(status_code_t code);

/* @brief
 *  Get array of string by base code
 *  
 * @param http
 *  Pointer to http
 *
 * @param base_code
 *  Base code
 *
 * @return
 *  Array of strin.
 * */
static char **_HTTP_get_array_by_base(HTTP_t *http, int base_code);


char *crlf_chomp(char *buff) {
    buff[strcspn(buff, CRLF)] = '\0';
    
    return buff;
}

char *space_chomp(char *buff) {
    buff[strcspn(buff, " ")] = '\0';

    return buff;
}

char *HTTP_method_to_str(HTTP_t *http, method_t method) {
    if ((unsigned)method < ARRAY_SIZE(http->req.array_str_meth))
        return http->req.array_str_meth[method];
    else
        return NULL;
}

char *HTTP_version_to_str(HTTP_t *http, version_t version) {
    if ((unsigned)version < ARRAY_SIZE(http->array_str_ver))
        return http->array_str_ver[version];
    else
        return NULL;
}   

size_t HTTP_get_path_len(const HTTP_t *http) {
    if (http->req.path_len > 0)
        return (http->req.path_len - 2);
    else
        return 0;
}

size_t HTTP_get_body_size(const HTTP_t *http) {
    if (http->body.size > 0)
        return http->body.size;
    else
        return 0;
}

size_t HTTP_get_headers_length(HTTPList_t *list) {
    struct HTTPList *tmp = list;
    size_t count = 0;

    while (tmp) {
        count += (tmp->dict->val_len + tmp->dict->key_len);
        tmp = tmp->next;   
    }

    return count;
}

size_t HTTP_get_nb_headers(HTTP_t *http) {
    return http->headers_len;;
}

size_t long_to_str(long dec, char *buf, size_t buf_size) {
    return snprintf(buf, buf_size, "%ld", dec);
}

size_t HTTP_get_response_totalsize(HTTP_t *http) {
    size_t total_size = 0;

    /* len of version */
    total_size += strlen(HTTP_version_to_str(http, http->version));

    /* size of msg code */
    total_size += http->res.str_code_len;

    /* Total size of each header */
    total_size += HTTP_get_headers_length(http->headers);

    /* size of body */
    total_size += HTTP_get_body_size(http);
  
    /* before and after headers */
    total_size += strlen(CRLF)*2;

    return total_size;
}

size_t HTTP_get_request_totalsize(HTTP_t *http) {
    size_t total_size = 0;

    /* len of method */
    total_size += strlen(HTTP_method_to_str(http, http->req.method));

    /* len of path */
    total_size += HTTP_get_path_len(http)+2;   

    /* len of version */
    total_size += strlen(HTTP_version_to_str(http, http->version));

    /* Total size of each header */
    total_size += HTTP_get_headers_length(http->headers);

    /* size of body */
    total_size += HTTP_get_body_size(http);

    /* before and after headers */
    total_size += strlen(CRLF) * 2;

    return total_size;
}

int HTTP_set_header(HTTP_t *http, const char *strkey, const char *strval) {
    size_t key_len = strlen(strkey);
    size_t val_len = strlen(strval);
    char *key = malloc(key_len+2+1); /* 2=': ' 1=0*/
    char *val = malloc(val_len+2+1); /* 2=crlf 1=0 */

    if (!key || !val)
        return HTTP_ERR;
    
    bzero(key, key_len+3);
    bzero(val, val_len+3);
   
    if (!memcpy(key, strkey, key_len))
        exit(EXIT_FAILURE);

    if (!memcpy(val, strval, val_len))
        exit(EXIT_FAILURE);

    if (!memcpy(key+key_len, ": ", 2))
        exit(EXIT_FAILURE);

    if (!memcpy(val+val_len, CRLF, 2))
        exit(EXIT_FAILURE);

    struct HTTPList *new = malloc(sizeof(struct HTTPList));
    struct HTTPList *curr = http->headers;
    struct HTTPList *tmp = NULL;

    new->dict = malloc(sizeof(struct HTTPDict));
    if (!new->dict) exit(EXIT_FAILURE);
    
    bzero(new->dict, sizeof(struct HTTPList));

    new->dict->key = key;
    new->dict->key_len = key_len+2;
    new->dict->val = val;
    new->dict->val_len = val_len+2;

    while (curr) {
        tmp = curr;
        curr = curr->next;
    }

    new->next = NULL;

    if (tmp) tmp->next = new;
    else http->headers = new;

    http->headers_len += 1;

    return HTTP_OK;
}

static size_t _HTTP_write_header(HTTPList_t *list, void *buf, size_t buf_size) {
    size_t total_size = HTTP_get_headers_length(list);
    char *buffer = malloc(total_size);
    char *tmp = buffer;
    size_t size_write = 0;

    while (list) {
        if (!memcpy(tmp, list->dict->key, list->dict->key_len))
            exit(EXIT_FAILURE);
        
        tmp = (tmp + list->dict->key_len);

        if (!memcpy(tmp, list->dict->val, list->dict->val_len))
            exit(EXIT_FAILURE);
        
        tmp = (tmp + list->dict->val_len);

        list = list->next;
    }

    size_write = SIZE_WRITE(buf_size, total_size);

    if (!memcpy(buf, buffer, size_write))
        exit(EXIT_FAILURE);

    free(buffer);

    return size_write; 
}

size_t HTTP_make_raw_request(HTTP_t *header, void *buf, size_t buf_size) {
    size_t method_size = strlen(HTTP_method_to_str(header, header->req.method));
    size_t version_size = strlen(HTTP_version_to_str(header, header->version));;
    size_t body_size = HTTP_get_body_size(header);
    size_t total_size = HTTP_get_request_totalsize(header);
    size_t size_write = 0;

    char *tmp =  NULL; 
    char *buffer = malloc(total_size); 
    
    if (!buffer) exit(EXIT_FAILURE);
    
    tmp = buffer;
   
    if (!memcpy(tmp, HTTP_method_to_str(header, header->req.method), method_size))
        exit(EXIT_FAILURE);

    tmp = (tmp + method_size);

    if (!memcpy(tmp, header->req.path, header->req.path_len))
        exit(EXIT_FAILURE);

    tmp = (tmp + header->req.path_len);
   
    if (!memcpy(tmp, HTTP_version_to_str(header, header->version), version_size))
        exit(EXIT_FAILURE);
     
    tmp = (tmp + version_size);

    if (!memcpy(tmp, CRLF, 2))
        exit(EXIT_FAILURE);

    tmp = (tmp + 2);

    tmp += _HTTP_write_header(header->headers, tmp, HTTP_get_headers_length(header->headers));

    if (!memcpy(tmp, CRLF, 2))
        exit(EXIT_FAILURE);
        
    tmp = (tmp + 2);

    if (!memcpy(tmp, header->body.data, body_size))
        exit(EXIT_FAILURE);

    size_write = SIZE_WRITE(buf_size, total_size);
    //(buf_size > total_size) ? total_size : buf_size;
    
    if (!memcpy(buf, buffer, size_write))
        exit(EXIT_FAILURE);

    free(buffer);

    return size_write;
}

size_t HTTP_make_raw_response(HTTP_t *http, void *buf, size_t buf_size) {
    size_t total_size = HTTP_get_response_totalsize(http); 
    size_t version_size = strlen(HTTP_version_to_str(http, http->version));
    size_t size_header = HTTP_get_headers_length(http->headers);
    size_t size_write = 0;
    char *tmp = NULL;
    char *buffer = malloc(total_size);

    if (!buffer) 
        exit(EXIT_FAILURE);
    
    bzero(buffer, total_size);

    tmp = buffer;

    if (!memcpy(buffer, HTTP_version_to_str(http, http->version), version_size))
        exit(EXIT_FAILURE);
    
    tmp = tmp+version_size;

    if (!memcpy(tmp, http->res.str_code, http->res.str_code_len))
        exit(EXIT_FAILURE);

    tmp = tmp+http->res.str_code_len;

    if (!memcpy(tmp, CRLF, 2))
        exit(EXIT_FAILURE);

    tmp = tmp+2;

    tmp += _HTTP_write_header(http->headers, tmp, size_header);

    if (!memcpy(tmp, CRLF, 2))
        exit(EXIT_FAILURE);
    
    tmp = tmp+2;

    if (!memcpy(tmp, http->body.data, http->body.size))
        exit(EXIT_FAILURE);


    size_write = SIZE_WRITE(buf_size, total_size);
        
    if (!memcpy(buf, buffer, size_write))
        exit(EXIT_FAILURE);
    
    free(buffer);

    return size_write;
}

int HTTP_set_path(HTTP_t *header, char *path) {
    char *tmp = NULL;
    size_t path_len = strlen(path);
    size_t new_size = path_len+2; /* 2 = space at front and end*/

    if (!path)
        return HTTP_ERR;

    if (path_len > PATH_MAX)
        return HTTP_ERR;

    header->req.path_len = new_size;
    tmp = realloc(header->req.path, new_size);
    
    if (!tmp)
        exit(EXIT_FAILURE);
    
    header->req.path = tmp;

    header->req.path[0] = ' ';
    if (!memcpy(header->req.path+1, path, path_len))
        exit(EXIT_FAILURE);

    header->req.path[path_len+1] = ' ';

    return HTTP_OK;
}

size_t HTTP_write_body(HTTP_t *http, void *buf, size_t nbyte) {
    char *tmp = NULL;
    
    if (!buf)
        return 0;

    bzero(http->body.data, http->body.size);

    http->body.size = nbyte;

    
    tmp = realloc(http->body.data, nbyte);
    if (!tmp)
        return 0;
    
    http->body.data = tmp;

    if (!memcpy(http->body.data, buf, nbyte))
        return 0;

    return nbyte;
}

static size_t _HTTP_parse_method(HTTP_t *http, char *raw) {
    size_t method_size = 0; 

    for (size_t i = 0; i < NB_METHOD; i++) {
        method_size = strlen(http->req.array_str_meth[i]);

        if (strcmp(raw, http->req.array_str_meth[i]) == 0) {
            http->req.method = i;
            
            return method_size;
        }
    }

    return 0;
}

static size_t _HTTP_parse_version(HTTP_t *http, char *raw) {
    size_t version_size = 0;

    for (size_t i = 0; i < NB_VERSION; i++) {
        version_size = strlen(http->array_str_ver[i]);
       
        if (strcmp(raw, http->array_str_ver[i]) == 0) {
            http->version = i;

            return version_size;
        }
    }

    return 0;
}

static char *_HTTP_parse_header(HTTP_t *http, char *raw) {
    char *key = NULL;
    char *val = NULL;
    size_t val_len = 0;
    size_t key_len = 0;
    char *tmp = NULL;
    char *end_header = NULL;

    while (raw[0] != '\r' && raw[1] != '\n') {
        end_header = strstr(raw, CRLF);
        key_len = (strstr(raw, ": ") - raw);
        
        key = malloc(key_len+1);
      
        if (!key)
            exit(EXIT_FAILURE);    

        bzero(key, key_len + 1);
        if (!memcpy(key, raw, key_len))
            exit(EXIT_FAILURE);
        
        tmp = (strstr(raw, ": ") + 2); /* 2=skip ": " */
        val_len = (end_header - tmp);
        val = malloc(val_len + 1);
    
        if (!val)
           exit(EXIT_FAILURE); 

        bzero(val, val_len + 1);
        if (!memcpy(val, tmp, val_len))
            exit(EXIT_FAILURE);

        HTTP_set_header(http, key, val);
        free(key);
        free(val);
        
        raw = (end_header + 2); /* 2=skip CRLF at end of header */
    }

    return (raw + 2) /* 2=skip last CRLF*/;
}

int HTTP_parse_req_raw(HTTP_t *http, void *raw, size_t size_raw) { 
    char *ptr = (char *)raw;
    size_t body_size = 0;
    int ret = 0;

    char meth[10] = {0};
    char path[PATH_MAX] = {0};
    char ver[9] = {0};
    size_t meth_len;
    size_t version_len;
    size_t path_len;

    ret = sscanf(raw, "%9s %4095s %8s\r\n", meth, path, ver);
    if (ret != 3)
        return HTTP_ERR;

    meth_len = _HTTP_parse_method(http, meth);
    version_len = _HTTP_parse_version(http, ver);
    
    if (!meth_len || !version_len)
        return HTTP_ERR;

    HTTP_set_path(http, path);
    
    path_len = http->req.path_len;

    ptr = (raw
            + meth_len
            + path_len
            + version_len
            + 2);

    ptr = _HTTP_parse_header(http, ptr); 
   
    body_size = (size_raw - (ptr - (char *)raw));

    if (body_size > 0) {
        if (!HTTP_write_body(http, ptr, body_size))
            return HTTP_ERR;
    }

    return HTTP_OK;
}

void HTTP_header_key_val_clear(struct HTTPDict *dict) {
    if (dict->key) free(dict->key);
    if (dict->val) free(dict->val);
}

void HTTP_header_pop(HTTP_t *http) {
    HTTPList_t *tmp = http->headers->next;
    
    if (http->headers_len > 0 && http->headers) {
        HTTP_header_key_val_clear(http->headers->dict);
        free(http->headers);

        http->headers = tmp;
    
        http->headers_len -= 1;
    }
}

void HTTP_show_header(HTTP_t *http) {
    HTTPList_t *tmp = http->headers;

    while (tmp) {
        printf("%s", tmp->dict->key);
        printf("%s", tmp->dict->val);
    
        tmp = tmp->next;
    }
}

void HTTP_headers_clear(HTTP_t *http) {
    while (http->headers) {
        HTTP_header_pop(http);
    }
}

static int _HTTP_get_base_code(status_code_t code) {
    int base_code = -1;
    code = (unsigned)code;

    if (code >= CODE_INFO && code < CODE_SUCCESS)
        base_code = CODE_INFO;
    else if (code >= CODE_SUCCESS && code < CODE_REDIRECT)
        base_code = CODE_SUCCESS;
    else if (code >= CODE_REDIRECT && code < CODE_CLI_ERR)
        base_code = CODE_REDIRECT;
    else if (code >= CODE_CLI_ERR && code < CODE_SERV_ERR)
        base_code = CODE_CLI_ERR;
    else if (code >= CODE_SERV_ERR && code < CODE_SERV_ERR+100)
        base_code = CODE_SERV_ERR;

    return base_code;
}

static int _HTTP_get_index_array_code(status_code_t code) {
    int base_code = -1;
    code = (unsigned)code;

    base_code = _HTTP_get_base_code(code);
  
    if (base_code == -1)
        return -1;

    return (code - base_code);
}

static char **_HTTP_get_array_by_base(HTTP_t *http, int base_code) {
    switch (base_code) {
        case CODE_INFO:     return http->array_code.code_info;
        case CODE_SUCCESS:  return http->array_code.code_success;
        case CODE_REDIRECT: return http->array_code.code_redirect;
        case CODE_CLI_ERR:  return http->array_code.code_cli_err;
        case CODE_SERV_ERR: return http->array_code.code_serv_err;
        default:            return NULL;
    }    
}

char *HTTP_get_str_code(HTTP_t *http, status_code_t code) {
    int base_code = 0; 
    int index = -1;
    char **array = NULL;
    char *str = NULL;

    code = (unsigned)code;

    base_code = _HTTP_get_base_code(code);
    
    index = _HTTP_get_index_array_code(code);
    if (index == -1)
        exit(EXIT_FAILURE);

    array = _HTTP_get_array_by_base(http, base_code);
    
    if(!array)
        exit(EXIT_FAILURE);

    str = array[index];

    return str;
}

int HTTP_set_status_code(HTTP_t *http, status_code_t code) {
    char str_code[4] = {0};
    char *tmp = NULL;
    char *code_msg = NULL; 
    size_t total_size = 0;
    size_t len_strcode = 0;
    size_t len_msg_code = 0;
    int base_code = _HTTP_get_base_code(code);

    if (base_code == -1)
        return HTTP_ERR;
    
    http->res.code = code;

    code_msg = HTTP_get_str_code(http, code);
    
    if (!code_msg) 
        exit(EXIT_FAILURE);

    len_strcode = long_to_str(code, str_code, 4);
    len_msg_code = strlen(code_msg);
    total_size = (len_strcode + len_msg_code + 2);

    tmp = realloc(http->res.str_code, total_size);
    
    if (!tmp)
        exit(EXIT_FAILURE);

    if (!memcpy(tmp+1, str_code, len_strcode))
        exit(EXIT_FAILURE);

    tmp[0] = ' ';
    tmp[len_strcode+1] = ' ';
    
    http->res.str_code = tmp;

    tmp = tmp+(len_strcode + 2);

    if (!memcpy(tmp, code_msg, len_msg_code))
        exit(EXIT_FAILURE);

    http->res.str_code_len = total_size;

    return HTTP_OK;
}

HTTP_t *HTTP_init(void) {
    HTTP_t *http = malloc(sizeof(HTTP_t));
    
    if (!http)
        exit(EXIT_FAILURE);
    
    bzero(http, sizeof(HTTP_t));

    http->headers = NULL;
 
    http->req.path = malloc(0);
    if (!http->req.path) 
        exit(EXIT_FAILURE);
   
    http->body.data = malloc(0);
    if (!http->body.data)
        exit(EXIT_FAILURE);

    http->res.str_code = malloc(0);
    if (!http->res.str_code)
        exit(EXIT_FAILURE);

    char *array_meth[NB_METHOD] = {
        "GET", "PUT", "POST", 
        "HEAD", "DELETE", "CONNECT", 
        "OPTIONS", "TRACE", "PATCH", "M-SEARCH"
    }; 

    char *array_ver[NB_VERSION] = {"HTTP/0.9", "HTTP/1.0", "HTTP/1.1"};

    http->version = -1;
    http->req.method = -1;
    http->res.code = -1;

    struct _array_str code = {
        .code_info = {
            "Continue", "Switch protocol", 
            "Processing", "Early hints"
        },

        .code_success = {
            "OK", "Created", "Accepted", 
            "Non authoritative info", 
            "No content", "Reset content", 
            "Partial content", 
            "Multi status", "Already reported",
            [10] = "Content diff", 
            [26] = "Im used"
        },

        .code_redirect = {
            "Multiple choice", "Moved permanently", 
            "Found", "See other", "Not modified",
            "Use proxy", "Switch proxy", 
            "Temporary redirect", "Permanent redirect", 
            [10] = "Too many redirects"
        },

        .code_cli_err = {
            "Bad request", "Unauthorized", "Payment required",
            "Forbidden", "Not found", "Method not allowed",
            "Not acceptable", "Proxy auth required", "Request timeout",
            "Conflict", "Gone", "Length required", "Precondition failed",
            "Request entity too large", "Request uri too long",
            "Unsuported media type", "requested range unsatisfiable",
            "Expectation failed", "Im teapot", 
            [21] = "Bad mapping",
            "Unprocessable entity", "Locked", "Method failure",
            "Too early", "Upgrade required", 
            [28] = "Precondition required",
            "Too many requests",
            [31] = "Request header fields too large",
            [44] = "No response",
            [49] = "Retry with",
            "Blocked by windows parental controls",
            "Unvailable for legal reasons",
            [56] = "Unrecoverable error",
            [95] = "SSL Certificate error",
            "SSL Certificate required", "HTTP request send to HTTPS port",
            [98] = "Token invalid", 
            "Client closed request"
        },

        .code_serv_err = {
            "Internal server error", "Not implemented", "Bad gateway",
            "Service unavailable", "Gateway time out", 
            "HTTP version not supported", "Variant also negotiates",
            "Insufficient storage", "Loop detected",
            "Bandwith limit exceeded", "Not extended", "Network authentication required",
            [20] = "Unknown error",
            "Web server is down", "Connection timed out", "Origin is unreachable",
            "A timeout occured", "SSL handshake failed", "Invalid SSL certificate",
            "Railgun error"
        }
    };

    if (!memcpy(&http->array_code, &code, sizeof(struct _array_str)))
        exit(EXIT_FAILURE);

    if (!memcpy(&http->req.array_str_meth, &array_meth, (10*sizeof(char *))))
        exit(EXIT_FAILURE);
    
    if (!memcpy(&http->array_str_ver, &array_ver, (5*sizeof(char *))))
        exit(EXIT_FAILURE);

    return http;
}

void HTTP_clear(HTTP_t **http) {
    HTTP_headers_clear(*http);
    
    free((*http)->res.str_code);
    free((*http)->req.path);
    free((*http)->body.data);
    free(*http);
}

int HTTP_set_method(HTTP_t *http, method_t method) {
    if ((unsigned)method < ARRAY_SIZE(http->req.array_str_meth))
        http->req.method = method;
    else
        return HTTP_ERR;
    
    return HTTP_OK;
}

method_t HTTP_get_method(HTTP_t *http) {
    return http->req.method;
}

status_code_t HTTP_get_status_code(HTTP_t *http) {
    return http->res.code;
}

char *HTTP_get_path(HTTP_t *header) {
    char *tmp = NULL;
    size_t len = HTTP_get_path_len(header);
    
    tmp = malloc(len);
    
    if (!tmp) exit(EXIT_FAILURE);

    if (!memcpy(tmp, header->req.path+1, len))
        exit(EXIT_FAILURE);

    tmp[len] = '\0';

    return tmp;
}

int HTTP_set_version(HTTP_t *http, version_t version) {
    if ((unsigned)version < ARRAY_SIZE(http->array_str_ver))
        http->version = version;
    else
        return HTTP_ERR;
    
    return HTTP_OK;
}

version_t HTTP_get_version(const HTTP_t *http) {
    return (http->version);
}

size_t HTTP_read_body(HTTP_t *http, void *buf, size_t nbyte) {
    if (!buf)
        return 0;

    size_t body_size = http->body.size;
   
    nbyte = SIZE_WRITE(nbyte, body_size);

    if (!memcpy(buf, http->body.data, nbyte))
        return 0;

    return nbyte;
}

int HTTP_parse_res_raw(HTTP_t *http, void *raw, size_t size_raw) {
    char *ptr = (char *)raw;
    char ver[9] = {0};
    
    size_t body_size = 0;
    size_t ver_len = 0;
    int ret = 0;
    int code = 0;

    ret = sscanf(raw, "%8s %d", ver, &code);
    
    if (ret != 2)
        return HTTP_ERR;

    ver_len = _HTTP_parse_version(http, ver);
    
    if (!ver_len)
        return HTTP_ERR;
    
    HTTP_set_status_code(http, code);

    ptr = strstr(ptr, "\r\n")+2;
    
    ptr = _HTTP_parse_header(http, ptr);
    body_size = (size_raw - (ptr - (char *)raw));

    if (body_size > 0) {
        if (!HTTP_write_body(http, ptr, body_size))
            return HTTP_ERR;
    }

    return HTTP_OK;
}

size_t HTTP_get_res_head_len(HTTP_t *http) {
    size_t body_size = 0;
    size_t total_size = 0;

    body_size = HTTP_get_body_size(http);
    total_size = HTTP_get_response_totalsize(http);

    return (total_size - body_size);
}

size_t HTTP_get_req_head_len(HTTP_t *http) {
    size_t body_size = 0;
    size_t total_size = 0;

    body_size = HTTP_get_body_size(http);
    total_size = HTTP_get_request_totalsize(http);

    return (total_size - body_size);
}

HTTPDict_t *HTTP_headers_get_val_with_key(HTTP_t *http, const char *key) {
    HTTPList_t *curr = http->headers;
    HTTPDict_t *dict = malloc(sizeof(HTTPDict_t));
    size_t val_len = 0;
    size_t key_len = strlen(key);
    int ret = -1;

    if (!dict) return NULL;

    dict->key = malloc(key_len+1);

    if (!dict->key) return NULL;

    bzero(dict->key, key_len+1);

    if(!memcpy(dict->key, key, key_len))
        return NULL;

    dict->key_len = key_len;

    while (curr) {
        ret = strncmp(curr->dict->key, key, curr->dict->key_len-2); /* -2=': ' */
        
        if (ret == 0) {
            val_len = (curr->dict->val_len - 2); /* -2=crlf */
           
            dict->val_len = val_len;
            dict->val = malloc(val_len + 1); /* 1=nullbyte */
            
            if (!dict->val) return NULL;

            bzero(dict->val, val_len+1);

            if (!memcpy(dict->val, curr->dict->val, val_len))
                return NULL;
            
            return dict;
        }
        
        curr = curr->next;
    }

    return NULL;
}

HTTPBody_t *HTTP_get_body_ptr(HTTP_t *http) {
  return &http->body;  
}
