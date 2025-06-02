cmake_minimum_required(VERSION 3.10)

project(doc_embarque C)
set(CMAKE_C_STANDARD 99)

include_directories(include include/models include/services)

file(GLOB_RECURSE SOURCES "src/*.c" "main.c")

add_executable(doc_embarque ${SOURCES}
        include/services/school_service.h
        include/models/school.h
        src/services/school_service.c
)

set_target_properties(doc_embarque PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
