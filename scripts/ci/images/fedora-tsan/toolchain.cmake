set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(_link_flags "-stdlib=libc++ -L /opt/tsan/lib -Wl,-rpath,/opt/tsan/lib -Wno-unused-command-line-argument")
set(_compile_flags "-nostdinc++ -isystem /opt/tsan/include/c++/v1 -fsanitize=thread")
set(CMAKE_EXE_LINKER_FLAGS_INIT ${_link_flags})
set(CMAKE_SHARED_LINKER_FLAGS_INIT ${_link_flags})
set(CMAKE_C_FLAGS_INIT ${_compile_flags})
set(CMAKE_CXX_FLAGS_INIT ${_compile_flags})
set(CMAKE_PREFIX_PATH /opt/tsan)
