# LibHTTP
LibHTTP is a C library easy-to-use which implements the base of the HTTP protocol.

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg?style=for-the-badge&logo=appveyor)](https://www.gnu.org/licenses/gpl-3.0)
![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)

# Info's about LibHTTP
LibHTTP is an easy-to-use HTTP library written in C allowing you to parse or create HTTP requests or responses.

There is however some important information to know before using this library:

- Does not necessarily meet HTTP standards and may not comply with its RFCs.
- Does not offer socket management.
- No fuzzer test was performed.

## Documentations
### Installation and more
- [Compilation](Docs/make.md#Compilation)
- [Installation](Docs/make.md#Installation)
- [Uninstallation](Docs/make.md#Uninstallation)
- [Clean](Docs/make.md#Clean)

### Library documentation
-  [LibHTTP doc's](Docs/libhttp.md)

### Example and Usage
- [Some code examples](Examples/)
- [Usage](Examples/usage.md)

## Todo
- [ ] Respect HTTP standards.
- [ ] Fuzzer test
- [ ] Windows compatible.
- [ ] C++ compatible.
