add_subdirectory(src)

add_kernel_module    (popcorn-linux-fs)
kernel_module_include(popcorn-linux-fs ${CMAKE_CURRENT_SOURCE_DIR}/include)
kernel_module_source (popcorn-linux-fs popcorn-linux-fs)

kernel_module_include(popcorn-linux-fs ${CMAKE_SOURCE_DIR}/lib/linux-core/include)
kernel_module_include(popcorn-linux-fs ${CMAKE_SOURCE_DIR}/lib/popcorn/include)

kernel_package_module(module popcorn-linux-fs)