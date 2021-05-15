#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libhttp.h"

char *HTTP_version_to_str(HTTP_t *http, version_t version) {
    if ((unsigned)version < ARRAY_SIZE(http->array_str_ver))
        return http->array_str_ver[version];
    else
        return NULL;
}   

size_t HTTP_get_body_size(const HTTP_t *http) {
    if (http->body.size > 0)
        return http->body.size;
    else
        return 0;
}

size_t _HTTP_get_headers_length(HTTPList_t *list) {
    struct HTTPList *tmp = list;
    size_t count = 0;

    while (tmp) {
        count += ((tmp->dict->val_len + 2) + (tmp->dict->key_len + 2)); /* 2 = ': ' and crlf*/
        tmp = tmp->next;   
    }

    return count;
}

size_t HTTP_get_nb_headers(HTTP_t *http) {
    return http->headers_len;;
}

int _HTTP_list_push(HTTPList_t **list, HTTPDict_t *dict) {
    HTTPList_t *curr = *list;
    HTTPList_t *tmp = NULL;

    HTTPList_t *new = malloc(sizeof(**list));
    
    if (!new)
        return HTTP_ERR;

    new->dict = dict;

    while (curr) {
        tmp = curr;
        curr = curr->next;
    }

    new->next = NULL;

    if (tmp) tmp->next = new;
    else *list = new;

    return HTTP_OK;       
}

int HTTP_set_header(HTTP_t *http, const char *strkey, const char *strval) {
    size_t key_len = strlen(strkey);
    size_t val_len = strlen(strval);
    char *key = malloc(key_len+1);
    char *val = malloc(val_len+1);
    int ret = 0;

    if (!key || !val)
        return HTTP_ERR;
    
    bzero(key, key_len+1);
    bzero(val, val_len+1);
   
    if (!memcpy(key, strkey, key_len))
        return HTTP_ERR;

    if (!memcpy(val, strval, val_len))
        return HTTP_ERR;

#if 0
    if (!memcpy(key+key_len, ": ", 2))
        return HTTP_ERR;

    if (!memcpy(val+val_len, CRLF, 2))
        return HTTP_ERR;
#endif

    HTTPDict_t *dict = malloc(sizeof(struct HTTPDict));
    if (!dict) return HTTP_ERR;
    
    bzero(dict, sizeof(struct HTTPList));

    dict->key = key;
    dict->key_len = key_len;
    dict->val = val;
    dict->val_len = val_len;

    ret = _HTTP_list_push(&http->headers, dict);
    if (!ret)
        return HTTP_ERR;

    http->headers_len += 1;
    
    return HTTP_OK;
}

size_t _HTTP_write_header(HTTPList_t *list, void *buf, size_t buf_size) {
    size_t total_size = _HTTP_get_headers_length(list);
    char *buffer = malloc(total_size);
    char *tmp = buffer;
    size_t size_write = 0;

    while (list) {
        if (!memcpy(tmp, list->dict->key, list->dict->key_len))
            return 0;
        
        tmp = (tmp + list->dict->key_len);

        if (!memcpy(tmp, ": ", 2))
            return 0;

        tmp = (tmp + 2);

        if (!memcpy(tmp, list->dict->val, list->dict->val_len))
            return 0;
        
        tmp = (tmp + list->dict->val_len);

        if (!memcpy(tmp, CRLF, 2))
            return 0;

        tmp = (tmp + 2);

        list = list->next;
    }

    size_write = SIZE_WRITE(buf_size, total_size);

    if (!memcpy(buf, buffer, size_write))
        return 0;

    free(buffer);

    return size_write; 
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

size_t _HTTP_parse_version(HTTP_t *http, char *raw) {
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

char *_HTTP_parse_header(HTTP_t *http, char *raw) {
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
            return NULL;    

        bzero(key, key_len + 1);
        if (!memcpy(key, raw, key_len))
            return NULL;
        
        tmp = (strstr(raw, ": ") + 2); /* 2=skip ": " */
        val_len = (end_header - tmp);
        val = malloc(val_len + 1);
    
        if (!val)
           return NULL; 

        bzero(val, val_len + 1);
        if (!memcpy(val, tmp, val_len))
            return NULL;

        HTTP_set_header(http, key, val);
        free(key);
        free(val);
        
        raw = (end_header + 2); /* 2=skip CRLF at end of header */
    }

    return (raw + 2) /* 2=skip last CRLF*/;
}

void HTTP_dict_clear(struct HTTPDict **dict) {
    if ((*dict)->key) free((*dict)->key);
    if ((*dict)->val) free((*dict)->val);

    free(*dict);
}

void HTTP_header_pop(HTTP_t *http) {
    HTTPList_t *tmp = http->headers->next;
    
    if (http->headers_len > 0 && http->headers) {
        HTTP_dict_clear(&http->headers->dict);
        free(http->headers);

        http->headers = tmp;
    
        http->headers_len -= 1;
    }
}

void HTTP_show_header(HTTP_t *http) {
    HTTPList_t *tmp = http->headers;

    while (tmp) {
        printf("%s: ", tmp->dict->key);
        printf("%s\n", tmp->dict->val);
    
        tmp = tmp->next;
    }
}

void HTTP_headers_clear(HTTP_t *http) {
    while (http->headers) {
        HTTP_header_pop(http);
    }
}

HTTP_t *HTTP_init(void) {
    HTTP_t *http = malloc(sizeof(HTTP_t));
    
    if (!http)
        return NULL;
    
    bzero(http, sizeof(HTTP_t));

    http->headers = NULL;
 
    http->req.path = malloc(0);
    if (!http->req.path) 
        return NULL;
   
    http->body.data = malloc(0);
    if (!http->body.data)
        return NULL;

    http->res.str_code = malloc(0);
    if (!http->res.str_code)
        return NULL;

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
        return NULL;

    if (!memcpy(&http->req.array_str_meth, &array_meth, (NB_METHOD * sizeof(char *))))
        return NULL;

    if (!memcpy(&http->array_str_ver, &array_ver, (NB_VERSION * sizeof(char *))))
        return NULL;

    return http;
}

void HTTP_clear(HTTP_t **http) {
    HTTP_headers_clear(*http);
    HTTP_query_clear(*http);

    free((*http)->res.str_code);
    free((*http)->req.path);
    free((*http)->body.data);
    free(*http);
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

HTTPDict_t *HTTP_headers_get_val_with_key(HTTP_t *http, const char *key) {
    HTTPList_t *curr = http->headers;
    HTTPDict_t *dict = malloc(sizeof(HTTPDict_t));
    size_t val_len = 0;
    size_t key_len = 0;
    int ret = -1;

    if (!dict) return NULL;

    while (curr) {
        ret = strncmp(curr->dict->key, key, curr->dict->key_len-2); /* -2=': ' */
        
        if (ret == 0) {
            key_len = (curr->dict->key_len - 2);
            
            dict->key_len = key_len;
            dict->key = malloc(key_len+1);

            if (!dict->key) return NULL;

            bzero(dict->key, key_len+1);

            if(!memcpy(dict->key, key, key_len))
                return NULL;

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

    free(dict);

    return NULL;
}

HTTPBody_t *HTTP_get_body_ptr(HTTP_t *http) {
    return &http->body;  
}

size_t HTTP_header_get_array(HTTP_t *http, HTTPDict_t *array[], size_t array_size) {
    HTTPList_t *tmp = http->headers;
    size_t count;

    for (count = 0; tmp && count < array_size; tmp = tmp->next, count++) {
        array[count] = tmp->dict;
    }

    return count;
}
