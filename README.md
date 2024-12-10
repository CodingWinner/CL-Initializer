This includes the source code for a library that initializes basic OpenCL elements on a GPU. Compile with any of the optional macros to create the library. This only supports queues on host on the first available GPU so far. Only work with GCC and Clang compilers that support __attribute__((destructor)).
Macros:
  NO_SETUP_ERRORS: Disables error throwing and error checking for the library
  FREE_MEM: Releases device and program after call to initCL()
