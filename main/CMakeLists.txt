

idf_component_register(SRCS "fros.c"
                    INCLUDE_DIRS "." "../payload"
                    REQUIRES "elf-loader")

add_dependencies(${COMPONENT_LIB} payload)

