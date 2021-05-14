#ifndef _REQUEST_H_
#define _REQUEST_H_

#include "types.h"


/* @brief
 *  Write query into buf
 *
 * @param list
 *  Pointer to linked list of query
 *
 * @param buf
 *  Buffer where write query.
 *
 * @param buf_size
 *  Size of buf
 *
 * @return
 *  Size write
 * */ 
size_t _HTTP_write_query(HTTPList_t *list, void *buf, size_t buf_size);

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
 *  Get length of path.
 *  
 * @param http
 *  Pointer to http.
 *
 * @return
 *  Length of path set.
 * */
size_t HTTP_get_path_len(const HTTP_t *http);

/* @brief
 *  Get total size of request http.
 *
 * @param http
 *  Pointer to http.
 *  
 * @return
 *  Size of http request.
 * */
size_t HTTP_get_request_totalsize(HTTP_t *http);

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
 *  Get request len without body size
 *
 * @param http
 *  Pointer to http
 *
 * @return
 *  Length of request head http
 * */
size_t HTTP_get_req_head_len(HTTP_t *http);

/* @brief
 *  Print key, value query.
 *
 * @param http
 *  Pointer to http.
 * */
void HTTP_show_query(HTTP_t *http);

/* @brief
 *  Add key, value query.
 *  
 * @param http
 *  Pointer to http.
 *
 * @param strkey
 *  Key string.
 *
 * @param strval
 *  Value string.
 *  
 * @return
 *  HTTP_OK or HTTP_ERR
 * */
int HTTP_set_query(HTTP_t *http, const char *strkey, const char *strval);

/* @brief
 *  Get value string of query with specific key.
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
HTTPDict_t *HTTP_query_get_val_with_key(HTTP_t *http, const char *key);

/* @brief
 *  Remove first query set.
 *
 * @param http
 *  Pointer to http.
 * */
void HTTP_query_pop(HTTP_t *http);

/* @brief
 *  Get number of query set
 *
 * @param http
 *  Pointer to http
 *
 * @return
 *  number of query set.
 * */
size_t HTTP_get_nb_query(HTTP_t *http);

/* @brief
 *  Parse query `key=value[&]`
 *  
 * @param http
 *  Pointer to http
 *
 * @param raw
 *  Pointer to data to parse.
 *
 * @return 
 *  HTTP_OK or HTTP_ERR
 * */
int HTTP_parse_query(HTTP_t *http, void *raw);


/* @brief
 *  Clear all query http.
 * 
 * @param http
 *  Pointer to http.
 * */
void HTTP_query_clear(HTTP_t *http);

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



#endif /* _REQUEST_H_ */
