cmake_minimum_required(VERSION 3.10)

project(doc_embarque C)
set(CMAKE_C_STANDARD 99)

include_directories(include include/models include/services include/use_cases)

add_executable(doc_embarque
        main.c
        src/use_cases/create_user.c
        src/use_cases/login.c
        src/use_cases/create_school.c
)

set_target_properties(doc_embarque PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
