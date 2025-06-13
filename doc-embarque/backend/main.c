#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/models/user.h"
#include "include/models/school.h"
#include "include/models/class.h"
#include "include/models/student.h"
#include "include/models/contact.h"
#include "include/models/payment.h"
#include "include/models/installment.h"
#include "include/models/boarding.h"
#include "include/models/travel_insurance.h"

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
#include "include/use_cases/create_payment.h"
#include "include/use_cases/read_payment.h"
#include "include/use_cases/update_payment.h"
#include "include/use_cases/delete_payment.h"
#include "include/use_cases/create_installment.h"
#include "include/use_cases/read_installment.h"
#include "include/use_cases/update_installment.h"
#include "include/use_cases/delete_installment.h"
#include "include/use_cases/create_boarding.h"
#include "include/use_cases/read_boarding.h"
#include "include/use_cases/update_boarding.h"
#include "include/use_cases/delete_boarding.h"
#include "include/use_cases/create_travel_insurance.h"
#include "include/use_cases/read_travel_insurance.h"
#include "include/use_cases/update_travel_insurance.h"
#include "include/use_cases/delete_travel_insurance.h"

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
    printf("21 - Criar pagamento\n");
    printf("22 - Listar pagamentos\n");
    printf("23 - Atualizar pagamento\n");
    printf("24 - Remover pagamento\n");
    printf("25 - Criar parcela\n");
    printf("26 - Listar parcelas\n");
    printf("27 - Atualizar parcela\n");
    printf("28 - Remover parcela\n");
    printf("29 - Registrar embarque\n");
    printf("30 - Listar embarques\n");
    printf("31 - Atualizar embarque\n");
    printf("32 - Remover embarque\n");
    printf("33 - Criar seguro viagem\n");
    printf("34 - Listar seguros\n");
    printf("35 - Atualizar seguro\n");
    printf("36 - Remover seguro\n");
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
            case 21: {
                Payment p = create_payment_cli();
                if (p.id != -1)
                    printf("Pagamento registrado com ID %d.\n", p.id);
                break;
            }
            case 22:
                read_payments();
                break;
            case 23: {
                Payment p;
                char buffer[128];
                printf("ID do pagamento: ");
                fgets(buffer, sizeof(buffer), stdin);
                p.id = atoi(buffer);
                printf("ID do aluno (0 para manter): ");
                fgets(buffer, sizeof(buffer), stdin);
                p.student_id = atoi(buffer);
                printf("Valor (0 para manter): ");
                fgets(buffer, sizeof(buffer), stdin);
                p.amount = atof(buffer);
                printf("Metodo (deixe vazio para manter): ");
                fgets(p.method, sizeof(p.method), stdin);
                p.method[strcspn(p.method, "\n")] = '\0';
                if (update_payment(p))
                    printf("Pagamento atualizado.\n");
                else
                    printf("Pagamento não encontrado.\n");
                break;
            }
            case 24: {
                char buf[32];
                printf("ID do pagamento a remover: ");
                fgets(buf, sizeof(buf), stdin);
                if (delete_payment(atoi(buf)))
                    printf("Pagamento removido.\n");
                else
                    printf("Pagamento não encontrado.\n");
                break;
            }
            case 25: {
                Installment i = create_installment_cli();
                if (i.id != -1)
                    printf("Parcela criada com ID %d.\n", i.id);
                break;
            }
            case 26:
                read_installments();
                break;
            case 27: {
                Installment i;
                char buffer[128];
                printf("ID da parcela: ");
                fgets(buffer, sizeof(buffer), stdin);
                i.id = atoi(buffer);
                printf("ID do pagamento (0 para manter): ");
                fgets(buffer, sizeof(buffer), stdin);
                i.payment_id = atoi(buffer);
                printf("Valor (0 para manter): ");
                fgets(buffer, sizeof(buffer), stdin);
                i.amount = atof(buffer);
                printf("Vencimento (deixe vazio para manter): ");
                fgets(i.due_date, sizeof(i.due_date), stdin);
                i.due_date[strcspn(i.due_date, "\n")] = '\0';
                printf("Pago (0/1 ou -1 para manter): ");
                fgets(buffer, sizeof(buffer), stdin);
                i.paid = atoi(buffer);
                if (i.paid != 0 && i.paid != 1) i.paid = -1;
                if (update_installment(i))
                    printf("Parcela atualizada.\n");
                else
                    printf("Parcela não encontrada.\n");
                break;
            }
            case 28: {
                char buf[32];
                printf("ID da parcela a remover: ");
                fgets(buf, sizeof(buf), stdin);
                if (delete_installment(atoi(buf)))
                    printf("Parcela removida.\n");
                else
                    printf("Parcela não encontrada.\n");
                break;
            }
            case 29: {
                Boarding b = create_boarding_cli();
                if (b.id != -1)
                    printf("Embarque registrado com ID %d.\n", b.id);
                break;
            }
            case 30:
                read_boardings();
                break;
            case 31: {
                Boarding b;
                char buffer[128];
                printf("ID do embarque: ");
                fgets(buffer, sizeof(buffer), stdin);
                b.id = atoi(buffer);
                printf("ID do aluno (0 para manter): ");
                fgets(buffer, sizeof(buffer), stdin);
                b.student_id = atoi(buffer);
                printf("Data (deixe vazio para manter): ");
                fgets(b.date, sizeof(b.date), stdin);
                b.date[strcspn(b.date, "\n")] = '\0';
                printf("Embarcou (0/1 ou -1 para manter): ");
                fgets(buffer, sizeof(buffer), stdin);
                b.boarded = atoi(buffer);
                if (b.boarded != 0 && b.boarded != 1) b.boarded = -1;
                if (update_boarding(b))
                    printf("Embarque atualizado.\n");
                else
                    printf("Embarque não encontrado.\n");
                break;
            }
            case 32: {
                char buf[32];
                printf("ID do embarque a remover: ");
                fgets(buf, sizeof(buf), stdin);
                if (delete_boarding(atoi(buf)))
                    printf("Embarque removido.\n");
                else
                    printf("Embarque não encontrado.\n");
                break;
            }
            case 33: {
                TravelInsurance t = create_travel_insurance_cli();
                if (t.id != -1)
                    printf("Seguro criado com ID %d.\n", t.id);
                break;
            }
            case 34:
                read_travel_insurances();
                break;
            case 35: {
                TravelInsurance t;
                char buffer[128];
                printf("ID do seguro: ");
                fgets(buffer, sizeof(buffer), stdin);
                t.id = atoi(buffer);
                printf("ID do aluno (0 para manter): ");
                fgets(buffer, sizeof(buffer), stdin);
                t.student_id = atoi(buffer);
                printf("Numero da apolice (deixe vazio para manter): ");
                fgets(t.policy_number, sizeof(t.policy_number), stdin);
                t.policy_number[strcspn(t.policy_number, "\n")] = '\0';
                printf("Seguradora (deixe vazio para manter): ");
                fgets(t.provider, sizeof(t.provider), stdin);
                t.provider[strcspn(t.provider, "\n")] = '\0';
                if (update_travel_insurance(t))
                    printf("Seguro atualizado.\n");
                else
                    printf("Seguro não encontrado.\n");
                break;
            }
            case 36: {
                char buf[32];
                printf("ID do seguro a remover: ");
                fgets(buf, sizeof(buf), stdin);
                if (delete_travel_insurance(atoi(buf)))
                    printf("Seguro removido.\n");
                else
                    printf("Seguro não encontrado.\n");
                break;
            }
            case 0:
                break;
            default:
                printf("Opção inválida.\n");
        }
    }
    return 0;
}