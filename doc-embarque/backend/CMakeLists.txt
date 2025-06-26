cmake_minimum_required(VERSION 3.10)

project(doc_embarque C)
set(CMAKE_C_STANDARD 99)

include_directories(include include/models include/services include/use_cases)

add_executable(doc_embarque
        main.c
        src/use_cases/create_user.c
        src/use_cases/login.c
        src/use_cases/create_school.c
        src/utils.c
        src/use_cases/update_user.c
        src/use_cases/get_user.c
        src/use_cases/delete_user.c
        src/use_cases/create_class.c
        src/use_cases/create_student.c
        src/use_cases/update_student.c
        src/use_cases/delete_student.c
        src/use_cases/read_student.c
        src/use_cases/delete_class.c
        src/use_cases/update_class.c
        src/use_cases/read_class.c
        include/use_cases/delete_school.h
        src/use_cases/delete_school.c
        src/use_cases/update_school.c
        src/use_cases/delete_school.c
        src/use_cases/read_school.c
        src/use_cases/create_contact.c
        src/use_cases/read_contact.c
        src/use_cases/update_contact.c
        src/use_cases/delete_contact.c
        src/use_cases/create_payment.c
        src/use_cases/read_payment.c
        src/use_cases/update_payment.c
        src/use_cases/delete_payment.c
        src/use_cases/create_installment.c
        src/use_cases/read_installment.c
        src/use_cases/update_installment.c
        src/use_cases/delete_installment.c
        src/use_cases/create_boarding.c
        src/use_cases/read_boarding.c
        src/use_cases/update_boarding.c
        src/use_cases/delete_boarding.c
        src/use_cases/create_travel.c
        src/use_cases/create_travel_insurance.c
        src/use_cases/read_travel.c
        src/use_cases/read_travel_insurance.c
        src/use_cases/update_travel_insurance.c
        src/use_cases/delete_travel_insurance.c
        src/use_cases/read_user.c
)

set_target_properties(doc_embarque PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
