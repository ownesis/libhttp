#ifndef _LIB_HTTP_H_
#define _LIB_HTTP_H_

#include "utils.h"
#include "request.h"
#include "response.h"
#include "macro.h"
#include "types.h"

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
size_t _HTTP_write_header(HTTPList_t *list, void *buf, size_t buf_size);

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
size_t _HTTP_parse_method(HTTP_t *http, char *raw);

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
char *_HTTP_parse_header(HTTP_t *http, char *raw);

/* @brief
 *  Push HTTPDict struct pointer in list.
 *
 * @param list
 *  Adresse of pointer to list.
 *
 * @param dict
 *  Pointer to dict.
 *
 * @return
 *  HTTP_OK or HTTP_ERR
 * */
int _HTTP_list_push(HTTPList_t **list, HTTPDict_t *dict);

/* @brief
 *  Get size of query, len of key, value and more.
 *
 * @param http
 *  Pointer to http
 *
 * @return
 *  Size of query
 * */
size_t _HTTP_get_query_length(HTTP_t *http);

/* @brief
 *  Get length  of all headers set, len of key and value.
 *
 * @param list
 *  Pointer to linked list
 *
 * @return
 *  Total length pg key(s)/value(s) string set in list
 * */
size_t _HTTP_get_headers_length(HTTPList_t *list);

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
int _HTTP_get_base_code(status_code_t code);

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
int _HTTP_get_index_array_code(status_code_t code);

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
char **_HTTP_get_array_by_base(HTTP_t *http, int base_code);

/* @brief
 *  Get size of array of string by base_code
 *
 * @param base_code
 *  Base code
 *
 * @return size or HTTP_ERR
 * */
int _HTTP_get_size_status_array(int base_code);

/* @brief
 *  Get size of query, len of key, value and more.
 *
 * @param http
 *  Pointer to http
 *
 * @return
 *  Size of query
 * */
size_t _HTTP_get_query_length(HTTP_t *http);





/* @brief
 *  Get length  of all headers set, len of key and value.
 *
 * @param list
 *  Pointer to linked list
 *
 * @return
 *  Total length pg key(s)/value(s) string set in list
 * */
size_t _HTTP_get_headers_length(HTTPList_t *list);

/* @brief
 *  Push HTTPDict struct pointer in list.
 *
 * @param list
 *  Adresse of pointer to list.
 *
 * @param dict
 *  Pointer to dict.
 *
 * @return
 *  HTTP_OK or HTTP_ERR
 * */
int _HTTP_list_push(HTTPList_t **list, HTTPDict_t *dict);

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
size_t _HTTP_parse_version(HTTP_t *http, char *raw);

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
char *_HTTP_parse_header(HTTP_t *http, char *raw);

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
 *  Get size of body data.
 *
 * @param http
 *  Pointer to http.
 *
 * @return
 *  Size of body data set. 
 * */
size_t HTTP_get_body_size(const HTTP_t *http);

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
size_t HTTP_write_body(HTTP_t *http, void *buf, size_t nbyte);
 
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
size_t HTTP_read_body(HTTP_t *http, void *buf, size_t nbyte);


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
 *  NULL or Pointer to a malloc of HTTPDict struct.
 * */
HTTPDict_t *HTTP_headers_get_val_with_key(HTTP_t *http, const char *key);


/* @brief
 *  Free key and value dict.
 *  
 * @param dict
 *  Address of pointer to HTTPDict struct.
 * */
void HTTP_dict_clear(HTTPDict_t **dict);


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

/* @brief
 *  Get number of headers set
 *
 * @param http
 *  Pointer to http
 *
 * @return
 *  number of headers set.
 * */
size_t HTTP_get_nb_headers(HTTP_t *http);

#endif /* _LIB_HTTP_H_ */
