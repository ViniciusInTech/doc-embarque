cmake_minimum_required(VERSION 3.10)
project(doc_embarque_frontend C)
set(CMAKE_C_STANDARD 99)

# Find GTK3
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(CAIRO REQUIRED cairo)

include_directories(${GTK3_INCLUDE_DIRS} ${CAIRO_INCLUDE_DIRS} ../backend/include ../backend/include/models ../backend/include/use_cases include)
link_directories(${GTK3_LIBRARY_DIRS} ${CAIRO_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER} ${CAIRO_CFLAGS_OTHER})

set(BACKEND_SOURCES
        ../backend/src/use_cases/create_user.c
        ../backend/src/use_cases/login.c
        ../backend/src/use_cases/create_school.c
        ../backend/src/use_cases/read_school.c
        ../backend/src/use_cases/update_school.c
        ../backend/src/use_cases/delete_school.c
        ../backend/src/use_cases/create_class.c
        ../backend/src/use_cases/read_class.c
        ../backend/src/use_cases/update_class.c
        ../backend/src/use_cases/delete_class.c
        ../backend/src/use_cases/create_student.c
        ../backend/src/use_cases/read_student.c
        ../backend/src/use_cases/update_student.c
        ../backend/src/use_cases/delete_student.c
        ../backend/src/use_cases/create_payment.c
        ../backend/src/use_cases/read_payment.c
        ../backend/src/use_cases/create_installment.c
        ../backend/src/use_cases/read_installment.c
        ../backend/src/use_cases/create_travel.c
        ../backend/src/use_cases/read_travel.c
        ../backend/src/use_cases/check_student_payment.c
        ../backend/src/utils.c
)

add_executable(doc_embarque_frontend
        src/main.c
        src/login_window.c
        src/register_window.c
        src/home_window.c
        src/dashboard_page.c
        src/schools_page.c
        src/classes_page.c
        src/travels_page.c
        src/payments_page.c
        src/boarding_page.c
        src/session.c
        ${BACKEND_SOURCES}
)

target_link_libraries(doc_embarque_frontend ${GTK3_LIBRARIES} ${CAIRO_LIBRARIES})

set_target_properties(doc_embarque_frontend PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
# Copiar CSS e imagens automaticamente
add_custom_target(copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/style.css
        ${CMAKE_BINARY_DIR}/bin/style.css
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_BINARY_DIR}/bin/assets
)

add_dependencies(doc_embarque_frontend copy_assets)
