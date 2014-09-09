project(libagl_zlib)

add_library(agl_zlib SHARED Deflator.cpp Inflator.cpp)
include_directories(../../include)

