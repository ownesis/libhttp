# Compilation
For compile LibHTTP file object:

```shell
make
```

This options build `libhttp.o` object.

# Installation
For compile static and dynamic LibHTTP library and install this on your system:

```shell
make install
```

This options create the directory `libhttp` in `/usr/include` and install `libhttp.h` at `http.h` inside. Also install `libhttp.so`, `libhttp.a`, after their compilations,  inside `/usr/lib/`.

# Uninstallation
For uninstall LibHTTP on your system:

```shell
make remove
```

This option remove the directory `libhttp` inside `/usr/include` and its content. Also remove `libhttp.a`, `libhttp.so` inside `/usr/lib/`.

# Clean
for cleaning binaries in current repertory:

```shell
make clean
```

This option remove `libhttp.o`, `libhttp.a` and `libhttp.so` in current directory
