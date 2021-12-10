#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libhttp.h"

char *HTTP_method_to_str(HTTP_t *http, method_t method) {
    if ((unsigned)method < ARRAY_SIZE(http->req.array_str_meth))
        return http->req.array_str_meth[method];
    else
        return NULL;
}

size_t HTTP_get_path_len(const HTTP_t *http) {
    if (http->req.path_len > 0)
        return (http->req.path_len);
    else
        return 0;
}

size_t HTTP_get_nb_query(HTTP_t *http) {
    return http->req.query_len;;
}

size_t HTTP_get_request_totalsize(HTTP_t *http) {
    size_t total_size = 0;
    int tmp = 0;

    /* len of method */
    total_size += strlen(HTTP_method_to_str(http, http->req.method));

    /* len of path */
    total_size += HTTP_get_path_len(http)+2; /* 2=spaces*/  

    /* space after path */
    total_size++;

    /* len of version */
    total_size += strlen(HTTP_version_to_str(http, http->version));

    /* Total size of each header */
    total_size += _HTTP_get_headers_length(http->headers);
    
    /* Total size of each query */
    tmp = _HTTP_get_query_length(http);
        
    if (tmp) {
        total_size += 1; /* '?' start of query */
        total_size += tmp;
    }
    
    /* size of body */
    total_size += HTTP_get_body_size(http);

    /* before and after headers */
    total_size += strlen(CRLF) * 2;

    return total_size;
}

int HTTP_set_query(HTTP_t *http, const char *strkey, const char *strval) {
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

    HTTPDict_t *dict = malloc(sizeof(struct HTTPDict));

    if (!dict)
        return HTTP_ERR;

    bzero(dict, sizeof(struct HTTPList));

    dict->key = key;
    dict->key_len = key_len;
    dict->val = val;
    dict->val_len = val_len;

    ret = _HTTP_list_push(&http->req.query, dict);
    
    if (!ret)
        return HTTP_ERR;

    http->req.query_len += 1;
    
    return HTTP_OK;
}

size_t HTTP_make_raw_request(HTTP_t *header, void *buf, size_t buf_size) {
    size_t method_size = strlen(HTTP_method_to_str(header, header->req.method));
    size_t version_size = strlen(HTTP_version_to_str(header, header->version));;
    size_t body_size = HTTP_get_body_size(header);
    size_t total_size = HTTP_get_request_totalsize(header);
    size_t size_write = 0;

    char *tmp =  NULL; 
    char *buffer = malloc(total_size); 
    
    if (!buffer) return 0;
    
    tmp = buffer;
   
    if (!memcpy(tmp, HTTP_method_to_str(header, header->req.method), method_size))
        return 0;

    tmp = (tmp + method_size);

    tmp[0] = ' ';
    tmp = (tmp + 1);
    
    if (!memcpy(tmp, header->req.path, header->req.path_len))
        return 0;

    tmp = (tmp + header->req.path_len);

    if (header->req.query)
        tmp += _HTTP_write_query(header->req.query, tmp, (_HTTP_get_query_length(header)+1));
   
    tmp[0] = ' ';
    tmp = tmp + 1;

    if (!memcpy(tmp, HTTP_version_to_str(header, header->version), version_size))
        return 0;
     
    tmp = (tmp + version_size);

    if (!memcpy(tmp, CRLF, 2))
        return 0;

    tmp = (tmp + 2);

    tmp += _HTTP_write_header(header->headers, tmp, _HTTP_get_headers_length(header->headers));

    if (!memcpy(tmp, CRLF, 2))
        return 0;
        
    tmp = (tmp + 2);

    if (!memcpy(tmp, header->body.data, body_size))
        return 0;

    size_write = SIZE_WRITE(buf_size, total_size);

    if (!memcpy(buf, buffer, size_write))
        return 0;

    free(buffer);

    return size_write;
}

int HTTP_set_path(HTTP_t *header, char *path) {
    char *tmp = NULL;
    size_t path_len = strlen(path);
    size_t new_size = path_len+1;

    if (!path)
        return HTTP_ERR;

    if (path_len > PATH_MAX)
        return HTTP_ERR;

    header->req.path_len = path_len;
    tmp = realloc(header->req.path, new_size);
    
    if (!tmp)
        return HTTP_ERR;

    header->req.path = tmp;
    bzero(header->req.path, new_size);

    //header->req.path[0] = ' ';

    if (!memcpy(header->req.path, path, path_len))
        return HTTP_ERR;
    
    //header->req.path[path_len+1] = ' ';

    return HTTP_OK;
}

int HTTP_parse_query(HTTP_t *http, void *raw) {
    char *ptr = (char *)raw;
    size_t key_len = 0;
    size_t val_len = 0;
    char *key = NULL;
    char *val = NULL;
    char *tmp = NULL;

    tmp = strchr(ptr, '=');
    if (!tmp) return HTTP_ERR;

    key_len = (tmp - ptr);
        
    key = malloc(key_len+1);
        
    if (!key)
        return HTTP_ERR;

    bzero(key, key_len+1);

    if (!memcpy(key, ptr, key_len))
        return HTTP_ERR;

    ptr = (ptr + key_len + 1); /* 1='=' */

    tmp = strchr(ptr, '&');
    
    if (tmp) {
        val_len = (tmp - ptr);
        val = malloc(val_len+1);

        if (!val) return HTTP_ERR;

        bzero(val, val_len+1);
        
        if (!memcpy(val, ptr, val_len))
            return HTTP_ERR;

        HTTP_set_query(http, key, val);
        
        free(key);
        free(val);

        HTTP_parse_query(http, ptr+val_len+1);

    } else {
        tmp = strchr(ptr, '\0');
        val_len = (tmp - ptr);
        val = malloc(val_len+1);

        if (!val) return HTTP_ERR;

        bzero(val, val_len+1);
        
        if (!memcpy(val, ptr, val_len))
            return HTTP_ERR;

        HTTP_set_query(http, key, val);
        
        free(key);
        free(val);
    }

    return HTTP_OK;
}

int HTTP_parse_req_raw(HTTP_t *http, void *raw, size_t size_raw) { 
    char *ptr = (char *)raw;
    char *tmp = NULL;
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

    tmp = strchr(path, '?');
    if (tmp)
        HTTP_parse_query(http, tmp+1);

    if (!meth_len) 
        return HTTP_METHOD_ERR;

    if (!version_len)
        return HTTP_VERSION_ERR;

    HTTP_set_path(http, path);
    
    path_len = http->req.path_len;

    ptr = (raw
            + meth_len
            + path_len
            + version_len
            + 2); /* spaces of path */

    ptr = (ptr + 2); /* CRLF */

    ptr = _HTTP_parse_header(http, ptr); 
   
    body_size = (size_raw - (ptr - (char *)raw));

    if (body_size > 0) {
        if (!HTTP_write_body(http, ptr, body_size))
            return HTTP_ERR;
    }

    return HTTP_OK;
}

void HTTP_query_pop(HTTP_t *http) {
    HTTPList_t *tmp = http->req.query->next;
    
    if (http->req.query_len > 0 && http->req.query) {
        HTTP_dict_clear(&http->req.query->dict);
        free(http->req.query);

        http->req.query = tmp;
    
        http->req.query_len -= 1;
    }
}

void HTTP_show_query(HTTP_t *http) {
    HTTPList_t *tmp = http->req.query;

    while (tmp) {
        printf("%s = ", tmp->dict->key);
        printf("%s\n", tmp->dict->val);
    
        tmp = tmp->next;
    }
}

void HTTP_query_clear(HTTP_t *http) {
    while (http->req.query) {
        HTTP_query_pop(http);
    }
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

char *HTTP_get_path(HTTP_t *http) {
    return http->req.path;
}

size_t HTTP_get_req_head_len(HTTP_t *http) {
    size_t body_size = 0;
    size_t total_size = 0;

    body_size = HTTP_get_body_size(http);
    total_size = HTTP_get_request_totalsize(http);

    return (total_size - body_size);
}

HTTPDict_t *HTTP_query_get_val_with_key(HTTP_t *http, const char *key) {
    HTTPList_t *curr = http->req.query;
    HTTPDict_t *dict = malloc(sizeof(HTTPDict_t));
    size_t key_len = 0;
    size_t val_len = 0;
    int ret = -1;

    if (!dict) return NULL;

    while (curr) {
        ret = strncmp(curr->dict->key, key, curr->dict->key_len);
        
        if (ret == 0) {
            key_len = curr->dict->key_len;
            
            dict->key_len = key_len;
            dict->key = malloc(key_len+1);

            if (!dict->key) return NULL;

            bzero(dict->key, key_len+1);

            if(!memcpy(dict->key, key, key_len))
                return NULL;

            val_len = curr->dict->val_len;
            dict->val_len = val_len;
            dict->val = malloc(val_len + 1);
            
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

size_t _HTTP_parse_method(HTTP_t *http, char *raw) {
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

size_t _HTTP_write_query(HTTPList_t *list, void *buf, size_t buf_size) {
    char *buffer = malloc(buf_size);
    char *tmp = buffer;
    size_t size_write = 0;

    if (!buffer)
        return 0; 

    bzero(buffer, buf_size);

    if (list) {
        tmp[0] = '?';
        tmp = tmp + 1;
        size_write += 1;
    }

    while (list) {
        if (!memcpy(tmp, list->dict->key, list->dict->key_len))
            return 0;
        
        size_write += list->dict->key_len;
        tmp = tmp + list->dict->key_len;

        tmp[0] = '=';
        tmp = tmp + 1;
        size_write += 1;

        if (!memcpy(tmp, list->dict->val, list->dict->val_len))
            return 0;
        
        size_write += list->dict->val_len;
        tmp = tmp + list->dict->val_len;

        if (list->next) {
            tmp[0] = '&';
            tmp = tmp + 1;
            size_write += 1;
        }

        list = list->next;
    }

    if (!memcpy(buf, buffer, size_write))
        return 0;

    free(buffer);

    return size_write;
}

size_t _HTTP_get_query_length(HTTP_t *http) {
    struct HTTPList *tmp = http->req.query;
    size_t count = 0;
    size_t nb = HTTP_get_nb_query(http);

    if (nb) {
        count += nb; /* nb of '=' */
        count += (nb - 1); /* nb of '&' */
    }

    while (tmp) {
        count += (tmp->dict->val_len + tmp->dict->key_len);
        tmp = tmp->next;   
    }

    return count;
}

size_t HTTP_query_get_array(HTTP_t *http, HTTPDict_t *array[], size_t array_size) {
    HTTPList_t *tmp = http->req.query;
    size_t count;

    for (count = 0; tmp && count < array_size; tmp = tmp->next, count++) {
        array[count] = tmp->dict;
    }

    return count;
}
