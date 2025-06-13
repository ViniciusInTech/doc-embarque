#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/models/user.h"
#include "include/models/school.h"
#include "include/models/class.h"
#include "include/models/student.h"
#include "include/models/contact.h"

#include "include/use_cases/create_user.h"
#include "include/use_cases/login.h"
#include "include/use_cases/update_user.h"
#include "include/use_cases/delete_user.h"
#include "include/use_cases/create_school.h"
#include "include/use_cases/update_school.h"
#include "include/use_cases/delete_school.h"
#include "include/use_cases/read_school.h"
#include "include/use_cases/create_class.h"
#include "include/use_cases/update_class.h"
#include "include/use_cases/delete_class.h"
#include "include/use_cases/read_class.h"
#include "include/use_cases/create_student.h"
#include "include/use_cases/update_student.h"
#include "include/use_cases/delete_student.h"
#include "include/use_cases/read_student.h"
#include "include/use_cases/create_contact.h"
#include "include/use_cases/read_contact.h"
#include "include/use_cases/update_contact.h"
#include "include/use_cases/delete_contact.h"
#include "include/use_cases/read_user.h"

static void print_menu() {
    printf("\n== DocEmbarque Teste ==\n");
    printf("1 - Criar usuário\n");
    printf("2 - Login\n");
    printf("3 - Atualizar usuário\n");
    printf("4 - Remover usuário\n");
    printf("5 - Criar escola\n");
    printf("6 - Listar escolas\n");
    printf("7 - Atualizar escola\n");
    printf("8 - Remover escola\n");
    printf("9 - Criar turma\n");
    printf("10 - Listar turmas\n");
    printf("11 - Atualizar turma\n");
    printf("12 - Remover turma\n");
    printf("13 - Criar aluno\n");
    printf("14 - Listar alunos\n");
    printf("15 - Atualizar aluno\n");
    printf("16 - Remover aluno\n");
    printf("17 - Criar contato\n");
    printf("18 - Listar contatos\n");
    printf("19 - Atualizar contato\n");
    printf("20 - Remover contato\n");
    printf("21 - Listar usuarios\n");
    printf("0 - Sair\n> ");
}

int main() {
    int option = -1;
    while (option != 0) {
        print_menu();
        if (scanf("%d", &option) != 1) {
            printf("Entrada inválida.\n");
            return 0;
        }
        getchar();
        switch (option) {
            case 1: {
                User u = create_user_cli();
                printf("Usuário '%s' cadastrado com ID %d.\n", u.name, u.id);
                break;
            }
            case 2: {
                User logged;
                if (login_cli(&logged)) {
                    printf("Login realizado com sucesso. Bem-vindo, %s!\n", logged.name);
                } else {
                    printf("Falha no login.\n");
                }
                break;
            }
            case 3:
                update_user_cli();
                break;
            case 4:
                delete_user_cli();
                break;
            case 5: {
                School s = create_school_cli();
                printf("Escola '%s' cadastrada com ID %d.\n", s.name, s.id);
                break;
            }
            case 6:
                read_schools();
                break;
            case 7: {
                School s;
                char buffer[128];
                printf("ID da escola: ");
                fgets(buffer, sizeof(buffer), stdin);
                s.id = atoi(buffer);
                printf("Novo nome (deixe vazio para manter): ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                if (strlen(buffer) > 0) strcpy(s.name, buffer);
                printf("Novo endereço (deixe vazio para manter): ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                if (strlen(buffer) > 0) strcpy(s.address, buffer);
                if (update_school(s))
                    printf("Escola atualizada.\n");
                else
                    printf("Escola não encontrada.\n");
                break;
            }
            case 8: {
                char buf[32];
                printf("ID da escola a remover: ");
                fgets(buf, sizeof(buf), stdin);
                if (delete_school(atoi(buf)))
                    printf("Escola removida.\n");
                else
                    printf("Escola não encontrada.\n");
                break;
            }
            case 9: {
                Class c = create_class_cli();
                if (c.class_id != -1)
                    printf("Turma '%s' criada com ID %d.\n", c.name, c.class_id);
                break;
            }
            case 10:
                read_classes();
                break;
            case 11: {
                Class c;
                char buffer[128];
                printf("ID da turma: ");
                fgets(buffer, sizeof(buffer), stdin);
                c.class_id = atoi(buffer);
                printf("ID da escola (0 para manter): ");
                fgets(buffer, sizeof(buffer), stdin);
                c.school_id = atoi(buffer);
                printf("Nome da turma (deixe vazio para manter): ");
                fgets(c.name, sizeof(c.name), stdin);
                c.name[strcspn(c.name, "\n")] = '\0';
                printf("Total de alunos (0 para manter): ");
                fgets(buffer, sizeof(buffer), stdin);
                c.students = atoi(buffer);
                printf("Alunos confirmados (0 para manter): ");
                fgets(buffer, sizeof(buffer), stdin);
                c.confirmed_students = atoi(buffer);
                update_class(c);
                break;
            }
            case 12: {
                char buf[32];
                printf("ID da turma a remover: ");
                fgets(buf, sizeof(buf), stdin);
                delete_class(atoi(buf));
                break;
            }
            case 13: {
                Student s = create_student_cli();
                if (s.id != -1)
                    printf("Aluno '%s' cadastrado com ID %d.\n", s.name, s.id);
                break;
            }
            case 14:
                read_students();
                break;
            case 15: {
                Student s;
                char buffer[128];
                printf("ID do aluno: ");
                fgets(buffer, sizeof(buffer), stdin);
                s.id = atoi(buffer);
                printf("ID da turma (0 para manter): ");
                fgets(buffer, sizeof(buffer), stdin);
                s.classroom_id = atoi(buffer);
                printf("Nome (deixe vazio para manter): ");
                fgets(s.name, sizeof(s.name), stdin);
                s.name[strcspn(s.name, "\n")] = '\0';
                printf("RG (deixe vazio para manter): ");
                fgets(s.rg, sizeof(s.rg), stdin);
                s.rg[strcspn(s.rg, "\n")] = '\0';
                printf("CPF (deixe vazio para manter): ");
                fgets(s.cpf, sizeof(s.cpf), stdin);
                s.cpf[strcspn(s.cpf, "\n")] = '\0';
                printf("Data de nascimento (deixe vazio para manter): ");
                fgets(s.birth_date, sizeof(s.birth_date), stdin);
                s.birth_date[strcspn(s.birth_date, "\n")] = '\0';
                if (update_student(s))
                    printf("Aluno atualizado.\n");
                else
                    printf("Aluno não encontrado.\n");
                break;
            }
            case 16: {
                char buf[32];
                printf("ID do aluno a remover: ");
                fgets(buf, sizeof(buf), stdin);
                if (delete_student(atoi(buf)))
                    printf("Aluno removido.\n");
                else
                    printf("Aluno não encontrado.\n");
                break;
            }
            case 17: {
                ContactPerson c = create_contact_cli();
                if (c.id != -1)
                    printf("Contato '%s' cadastrado com ID %d.\n", c.name, c.id);
                break;
            }
            case 18:
                read_contacts();
                break;
            case 19: {
                ContactPerson c;
                char buffer[128];
                printf("ID do contato: ");
                fgets(buffer, sizeof(buffer), stdin);
                c.id = atoi(buffer);
                printf("ID da escola (0 para manter): ");
                fgets(buffer, sizeof(buffer), stdin);
                c.school_id = atoi(buffer);
                printf("Nome (deixe vazio para manter): ");
                fgets(c.name, sizeof(c.name), stdin);
                c.name[strcspn(c.name, "\n")] = '\0';
                printf("Telefone (deixe vazio para manter): ");
                fgets(c.phone, sizeof(c.phone), stdin);
                c.phone[strcspn(c.phone, "\n")] = '\0';
                printf("Email (deixe vazio para manter): ");
                fgets(c.email, sizeof(c.email), stdin);
                c.email[strcspn(c.email, "\n")] = '\0';
                if (update_contact(c))
                    printf("Contato atualizado.\n");
                else
                    printf("Contato não encontrado.\n");
                break;
            }
            case 20: {
                char buf[32];
                printf("ID do contato a remover: ");
                fgets(buf, sizeof(buf), stdin);
                if (delete_contact(atoi(buf)))
                    printf("Contato removido.\n");
                else
                    printf("Contato não encontrado.\n");
                break;
            }
            case 21:
                read_users();
                break;
            case 0:
                break;
            default:
                printf("Opção inválida.\n");
        }
    }
    return 0;
}