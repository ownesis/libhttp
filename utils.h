#ifndef _UTILS_H_
#define _UTILS_H_

#define bzero(x, len) \
    (memset((x), 0, (len)))

#define SIZE_WRITE(x, y) \
    ((x > y) ? y : x)

#define ARRAY_SIZE(x) \
    (sizeof(x)/sizeof(x[0]))

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



#endif /* _UTILS_H_ */
