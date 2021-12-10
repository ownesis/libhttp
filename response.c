#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libhttp.h"

size_t HTTP_get_response_totalsize(HTTP_t *http) {
    size_t total_size = 0;

    /* len of version */
    total_size += strlen(HTTP_version_to_str(http, http->version));

    /* size of msg code */
    total_size += http->res.str_code_len;

    /* Total size of each header */
    total_size += _HTTP_get_headers_length(http->headers);

    /* size of body */
    total_size += HTTP_get_body_size(http);
  
    /* before and after headers */
    total_size += strlen(CRLF)*2;

    return total_size;
}

size_t HTTP_make_raw_response(HTTP_t *http, void *buf, size_t buf_size) {
    size_t total_size = HTTP_get_response_totalsize(http); 
    size_t version_size = strlen(HTTP_version_to_str(http, http->version));
    size_t size_header = _HTTP_get_headers_length(http->headers);
    size_t size_write = 0;
    char *tmp = NULL;
    char *buffer = malloc(total_size);

    if (!buffer) 
        return 0;
    
    bzero(buffer, total_size);

    tmp = buffer;

    if (!memcpy(buffer, HTTP_version_to_str(http, http->version), version_size))
        return 0;
    
    tmp = tmp+version_size;

    if (!memcpy(tmp, http->res.str_code, http->res.str_code_len))
        return 0;

    tmp = tmp+http->res.str_code_len;

    if (!memcpy(tmp, CRLF, 2))
        return 0;

    tmp = tmp+2;

    tmp += _HTTP_write_header(http->headers, tmp, size_header);

    if (!memcpy(tmp, CRLF, 2))
        return 0;
    
    tmp = tmp+2;

    if (!memcpy(tmp, http->body.data, http->body.size))
        return 0;

    size_write = SIZE_WRITE(buf_size, total_size);
        
    if (!memcpy(buf, buffer, size_write))
        return 0;
    
    free(buffer);

    return size_write;
}

int _HTTP_get_base_code(status_code_t code) {
    int base_code = -1;
    unsigned int ucode = (unsigned)code;
    unsigned int x, y;

    for (x = CODE_INFO, y = CODE_SUCCESS; 
            x <= CODE_SERV_ERR; 
            x += 100, y += 100) {
    
        if (ucode >= x && ucode < y)
            return x;
    }
    
    return base_code;
}

int _HTTP_get_index_array_code(status_code_t code) {
    int base_code = -1;
    unsigned int ucode = (unsigned)code;

    base_code = _HTTP_get_base_code(ucode);
  
    if (base_code == -1)
        return -1;

    return (ucode - base_code);
}

char *_HTTP_array_code_info(unsigned int index) {
    char *code_info[] = {
        "Continue", "Switch protocol", 
        "Processing", "Early hints"
    };

    if (index >= SIZE_CODE_INFO);
        return NULL;

    return code_info[index];
}

char *_HTTP_array_code_success(unsigned int index) {
    char *code_success[SIZE_CODE_SUCCESS] = {
        "OK", "Created", "Accepted", 
        "Non authoritative info", 
        "No content", "Reset content", 
        "Partial content", 
        "Multi status", "Already reported",
        [10] = "Content diff", 
        [26] = "Im used"
    };

    if (index >= SIZE_CODE_SUCCESS)
        return NULL;

    return code_success[index];
}

char *_HTTP_array_code_redirect(unsigned int index) {
    char *code_redirect[SIZE_CODE_REDIRECT] = {
        "Multiple choice", "Moved permanently", 
        "Found", "See other", "Not modified",
        "Use proxy", "Switch proxy", 
        "Temporary redirect", "Permanent redirect", 
        [10] = "Too many redirects"
    };
    
    if (index >= SIZE_CODE_REDIRECT)
        return NULL;

    return code_redirect[index];
}

char *_HTTP_array_code_cli_err(unsigned int index) {
    char *code_cli_err[SIZE_CODE_CLI_ERR] = {
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
    };

    if (index >= SIZE_CODE_CLI_ERR)
        return NULL;

    return code_cli_err[index];
}

char *_HTTP_array_code_serv_err(unsigned int index) {
    char *code_serv_err[SIZE_CODE_SERV_ERR] = {
        "Internal server error", "Not implemented", "Bad gateway",
        "Service unavailable", "Gateway time out", 
        "HTTP version not supported", "Variant also negotiates",
        "Insufficient storage", "Loop detected",
        "Bandwith limit exceeded", "Not extended", "Network authentication required",
        [20] = "Unknown error",
        "Web server is down", "Connection timed out", "Origin is unreachable",
        "A timeout occured", "SSL handshake failed", "Invalid SSL certificate",
        "Railgun error"
    };

    if (index >= SIZE_CODE_SERV_ERR)
        return NULL;

    return code_serv_err[index];
}

char *_HTTP_status_get_msg(int base_code, unsigned int index) {
    switch (base_code) {
        case CODE_INFO:     return _HTTP_array_code_info(index);
        case CODE_SUCCESS:  return _HTTP_array_code_success(index);
        case CODE_REDIRECT: return _HTTP_array_code_redirect(index);
        case CODE_CLI_ERR:  return _HTTP_array_code_cli_err(index);
        case CODE_SERV_ERR: return _HTTP_array_code_serv_err(index);
        default:            return NULL;
    }    
}

int _HTTP_get_size_status_array(int base_code) {
    switch (base_code) {
        case CODE_INFO:     return SIZE_CODE_INFO;
        case CODE_SUCCESS:  return SIZE_CODE_SUCCESS;
        case CODE_REDIRECT: return SIZE_CODE_REDIRECT;
        case CODE_CLI_ERR:  return SIZE_CODE_CLI_ERR;
        case CODE_SERV_ERR: return SIZE_CODE_SERV_ERR;
        default:            return HTTP_ERR;
    }    
}

char *HTTP_get_str_code(HTTP_t *http, status_code_t code) {
    int base_code = 0; 
    int index = -1;
    int ret = 0;
    char **array = NULL;
    char *str = NULL;
    unsigned int ucode = (unsigned)code;

    base_code = _HTTP_get_base_code(ucode);
 
    ret = _HTTP_get_size_status_array(base_code);
    if (!ret)
        return NULL;   

    index = _HTTP_get_index_array_code(ucode);
    if (index == -1)
        return NULL;

    if (index >= ret)
        return NULL;

    str = _HTTP_status_get_msg(base_code, (unsigned)index);
    
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
        return HTTP_ERR;

    len_strcode = long_to_str(code, str_code, 4);
    len_msg_code = strlen(code_msg);
    total_size = (len_strcode + len_msg_code + 2);

    tmp = realloc(http->res.str_code, total_size);
    
    if (!tmp)
        return HTTP_ERR;

    if (!memcpy(tmp+1, str_code, len_strcode))
        return HTTP_ERR;

    tmp[0] = ' ';
    tmp[len_strcode+1] = ' ';
    
    http->res.str_code = tmp;

    tmp = tmp+(len_strcode + 2);

    if (!memcpy(tmp, code_msg, len_msg_code))
        return HTTP_ERR;

    http->res.str_code_len = total_size;

    return HTTP_OK;
}

status_code_t HTTP_get_status_code(HTTP_t *http) {
    return http->res.code;
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
        return HTTP_VERSION_ERR;
    
    ret = HTTP_set_status_code(http, code);
    
    if (!ret)
        return HTTP_STATUS_ERR;

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
