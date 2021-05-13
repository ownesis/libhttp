# LibHTTP
LibHTTP is a C library easy-to-use which implements the base of the HTTP protocol.

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg?style=for-the-badge&logo=appveyor)](https://www.gnu.org/licenses/gpl-3.0)
![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)

# Info's about LibHTTP
LibHTTP is an easy-to-use HTTP library written in C allowing you to parse or create HTTP requests or responses.

There is however some important information to know before using this library:

- Only HTTP/1.1 and lower are supported 
- Does not necessarily meet HTTP standards and may not comply with its RFCs.
- Does not offer socket management.[

## Documentation
The documentation on the use of the library, the compilation and the installation is located on the [wiki](https://github.com/ownesis/libhttp/wiki).

## Todo
- [x] Add query `?[...]&[...]` parser/setter.
- [ ] Respect HTTP standards.
- [x] Fuzzer test
- [ ] Windows compatible.
- [ ] C++ compatible.
