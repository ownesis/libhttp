CC=/usr/bin/cc
CFLAGS=-Wall -Wextra
CFLAGS_DYN=-shared -fPIC
AR=/usr/bin/ar
AFLAGS=-rv
VERSION=1
OBJ=libhttp.o response.o request.o utils.o 

INCLUDE_DIR=/usr/include/libhttp/
LIB_DIR=/usr/lib/

libhttp.a: $(OBJ)
	@echo Make libhttp static library...
	$(AR) $(AFLAGS) libhttp.a $(OBJ)
	@echo done.

libhttp.o: libhttp.c
	@echo Build libhttp object...
	$(CC) $(CFLAGS) -c libhttp.c
	@echo done.

response.o: response.c
	@echo Build response object...
	$(CC) $(CFLAGS) -c response.c
	@echo done.

request.o: request.c
	@echo Build request object...
	$(CC) $(CFLAGS) -c request.c
	@echo done.

utils.o: utils.c
	@echo Build utils object...
	$(CC) $(CFLAGS) -c utils.c
	@echo done.

libhttp.so: libhttp.o
	@echo Make libhttp dynamic library...
	$(CC) $(CFLAGS_DYN) $(OBJ) -o libhttp.so.$(VERSION)
	@echo done.

install: libhttp.a libhttp.so
	@echo install libhttp on the system.
	mkdir $(INCLUDE_DIR)
	install libhttp.h $(INCLUDE_DIR)/http.h
	install utils.h $(INCLUDE_DIR)
	install macro.h $(INCLUDE_DIR)
	install types.h $(INCLUDE_DIR)
	install request.h $(INCLUDE_DIR)
	install response.h $(INCLUDE_DIR)
	install libhttp.a $(LIB_DIR)libhttp.a
	install libhttp.so.* $(LIB_DIR)
	@echo done.

clean:
	@echo Remove libhttp here.
	rm -rf libhttp.a *.o libhttp.so.*
	@echo done.

remove:
	@echo Remove libhttp of this system.
	rm -rf $(INCLUDE_DIR)
	rm -rf $(LIB_DIR)libhttp.*
	@echo done.
