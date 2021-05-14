#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libhttp.h"

int main(void) {
    char buffer[BUFSIZ] = {0};
    int ret = 0;
    ssize_t data_write = 0;

    char data[] = "Nous forgerons comme de vrai forgeron (seulement du http)";
    size_t data_size = strlen(data);
    
    // init http;
    HTTP_t *http = HTTP_init();

    ret = HTTP_set_version(http, HTTPv1_1);
    if (!ret)
        exit(EXIT_FAILURE);
    
    ret = HTTP_set_path(http, "/voltaire_a_terre");
    if (!ret)
        exit(EXIT_FAILURE);
 
    ret = HTTP_set_method(http, PUT);
    if (!ret)
        exit(EXIT_FAILURE);
 
    ret = HTTP_set_header(http, "User-Agent", "MacBook woula");
    if (!ret)
        exit(EXIT_FAILURE);
 
    ret = HTTP_set_header(http, "Cookie", "atoutmangé");
    if (!ret)
        exit(EXIT_FAILURE);
 
    ret = HTTP_set_header(http, "Boule", "Bill");
    if (!ret)
        exit(EXIT_FAILURE);
    
    HTTP_set_query(http, "TOTO", "TATA");
    HTTP_set_query(http, "TUTU", "TETE");

   HTTP_set_query(http, "TOTO", "TATA");
    HTTP_set_query(http, "TUTU", "TETE");

   HTTP_set_query(http, "TOTO", "TATA");
    HTTP_set_query(http, "TUTU", "TETE");

   HTTP_set_query(http, "TOTO", "TATA");
    HTTP_set_query(http, "TUTU", "TETE");

   HTTP_set_query(http, "TOTO", "TATA");
    HTTP_set_query(http, "TUTU", "TETE");

    // write data
    data_write = HTTP_write_body(http, data, data_size);
    if (!data_write)
        exit(EXIT_FAILURE);

    printf("[%ld]\n", HTTP_get_path_len(http));

    // make raw request
    data_write = HTTP_make_raw_request(http, buffer, BUFSIZ);
    if (data_write)
        printf("%s", buffer);

    // free http
    HTTP_clear(&http);
  
    return 0;
}
