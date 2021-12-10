#ifndef _RESPONSE_H_
#define _RESPONSE_H_

#include "types.h"

/* @brief
 *  Get total size of response http.
 *
 * @param http
 *  Pointer to http.
 *
 * @return
 *  Size of http response.
 * */
size_t HTTP_get_response_totalsize(HTTP_t *http);

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
size_t HTTP_get_res_head_len(HTTP_t *http);

#endif /* _RESPONSE_H_ */
