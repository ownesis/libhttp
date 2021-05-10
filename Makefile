CC=/usr/bin/cc
CFLAGS=-Wall -Wextra
CFLAGS_DYN=-shared -fPIC
AR=/usr/bin/ar
AFLAGS=-rv
VERSION=1

libhttp.o: libhttp.c
	@echo Build libhttp object...
	$(CC) $(CFLAGS) -c libhttp.c
	@echo done.

libhttp.a: libhttp.o
	@echo Make libhttp static library...
	$(AR) $(AFLAGS) libhttp.a libhttp.o
	@echo done.

libhttp.so: libhttp.o
	@echo Make libhttp dynamic library...
	$(CC) $(CFLAGS_DYN) libhttp.o -o libhttp.so.$(VERSION)
	@echo done.

install: libhttp.a libhttp.so
	@echo install libhttp on the system.
	mkdir /usr/include/libhttp
	install libhttp.h /usr/include/libhttp/http.h
	install libhttp.a /usr/lib/libhttp.a
	install libhttp.so.* /usr/lib/
	@echo done.

clean:
	@echo Remove libhttp here.
	rm -rf libhttp.a libhttp.o libhttp.so.*
	@echo done.

remove:
	@echo Remove libhttp of this system.
	rm -rf /usr/include/libhttp
	rm -rf /usr/lib/libhttp.*
	@echo done.
