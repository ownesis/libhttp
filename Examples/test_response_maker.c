#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libhttp/http.h"

int main(void) {
    version_t version;
    char buffer[BUFSIZ] = {0};
    char content_length[101] = {0};

    char data[] = "<html><body><h1>Mon putain de site web gros !<h1></body></html>";
    size_t data_size = strlen(data);
    
    // init http;
    HTTP_t *http = HTTP_init();

    HTTP_set_version(http, HTTPv1_1);
    HTTP_set_status_code(http, OK);

    long_to_str(data_size, content_length, 101);

    HTTP_set_header(http, "Content-Length", content_length);
    HTTP_set_header(http, "Cookie", "tiens_batard_mange");

    // write data
    HTTP_write_body(http, data, data_size);
   
    // make raw response
    HTTP_make_raw_response(http, buffer, BUFSIZ);
    printf("%s", buffer);

    // free http
    HTTP_clear(&http);
  
    return 0;
}
