#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libhttp.h"

int main(void) {
    version_t version;
    method_t methode;
    char *path = NULL;
    char data[BUFSIZ] = {0};
    
    char raw_req[] = "POST /yatilunfichierpourtestermalib?a=1&b=2&c=3 HTTP/1.1\r\nHost: toto.com\r\nUser-Agent: curl/7.76.1\r\nAccept: */*\r\n\r\nOn est pas dans body nouuuus ?!";
    
    size_t raw_req_size = strlen(raw_req);

    // init http;
    HTTP_t *http = HTTP_init();

    // parse request
    HTTP_parse_req_raw(http, raw_req, raw_req_size);
    
    methode = HTTP_get_method(http);
    version = HTTP_get_version(http);
    
    // /!\ return a malloc ptr
    path = HTTP_get_path(http);

    printf("Methode: %s\n", HTTP_method_to_str(http, methode));
    printf("Path: %s\n", path);
    printf("Version: %s\n", HTTP_version_to_str(http, version));

    HTTP_show_query(http);

    // free path
    free(path);

    printf("\n------- start header -------\n");
    HTTP_show_header(http);
    printf("----------- end header -------\n");

    HTTP_read_body(http, data, BUFSIZ);
    
    printf("\n-------- start body --------\n");
    printf("%s", data);
    printf("\n---------- end body --------\n");

    printf("%ld\n", HTTP_get_request_totalsize(http));

    // free http
    //HTTP_clear(&http);


//----------------------------------------------------------------------------------

    HTTP_make_raw_request(http, data, BUFSIZ);
    printf("%s", data); 
    printf("\n%ld", strlen(data)); 


    return 0;
}
