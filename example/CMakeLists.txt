add_kernel_feature   (popcorn-fs example)
kernel_feature_source(popcorn-fs example popcorn-linux-fs)

add_kernel_source (char char.c)
add_kernel_feature(char example char)

kernel_feature_include(popcorn-fs example ${CMAKE_CURRENT_SOURCE_DIR}/../include)
kernel_feature_include(char       example ${CMAKE_CURRENT_SOURCE_DIR}/../include)
kernel_feature_include(popcorn-fs example ${ROOT_LIB}/linux-core/include)
kernel_feature_include(char       example ${ROOT_LIB}/linux-core/include)
kernel_feature_include(popcorn-fs example ${ROOT_LIB}/popcorn/include)
kernel_feature_include(char       example ${ROOT_LIB}/popcorn/include)
kernel_feature_source (char       example char)