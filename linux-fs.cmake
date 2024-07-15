include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

macro   (linux_fs_example)
    add_subdirectory(lib/linux-fs/example)
endmacro()