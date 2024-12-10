This includes the source code for a library that initializes basic OpenCL elements on a GPU. Compile with an optional macro to create the library. This only supports in order queues on the first available GPU so far. Only work with GCC and Clang compilers.
Macros:
  NO_SETUP_ERRORS: Disables error throwing and error checking for the library
