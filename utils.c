#include <stdio.h>
#include <string.h>

#include "macro.h"

char *crlf_chomp(char *buff) {
    buff[strcspn(buff, CRLF)] = '\0';
    
    return buff;
}

char *space_chomp(char *buff) {
    buff[strcspn(buff, " ")] = '\0';

    return buff;
}

size_t long_to_str(long dec, char *buf, size_t buf_size) {
    return snprintf(buf, buf_size, "%ld", dec);
}
