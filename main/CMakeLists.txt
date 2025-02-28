

idf_component_register(SRCS "fros.cpp"
                    INCLUDE_DIRS "." "../payload"
                    REQUIRES "elf-loader" "embed-ros")

add_dependencies(${COMPONENT_LIB} payload)

MESSAGE(WARNING "component names ${COMPONENTS}" )
