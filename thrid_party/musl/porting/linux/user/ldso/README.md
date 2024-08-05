## Dynamic Linker Namespace

The dynamic linker provides the namespace mechanism which is based on configures and apis. 

Shared libraries can be isolated in different  namespaces so that libraries with same library name but with different symbols won't conflict. On the other hand, the namespace mechanism provides the flexibility so that some shared libraries can be exported by a linker namespace and used by another linker namespace. 


### How does it work

There are two ways to use the linker namespace: 

1. Modify and save the namespace configure file </etc/ld-musl-namespace-arm.ini> or </etc/ld-musl-namespace-aarch64.ini> base on architecture. When it does not exist or is filled in, a default namespace will be created.
   
2. Use the apis of namespace in head file <dlfcn.h>, to create a namespace, to set search paths of library, or to use exported libraries of another namespace by inheriting it.

### Usage example

Test usages show some examples of using the linker namespace. The file <ld-musl-namespace-arm.ini> or <ld-musl-namespace-aarch64.ini> in config directory is configures for test. The file <libc-test/src/functionalext/dlns/> gives usages of how apis and configures work.
