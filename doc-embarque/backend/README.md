# 🧠 Backend - DocEmbarque

Este diretório contém a implementação do backend da aplicação **DocEmbarque**, responsável por gerenciar cadastros de alunos, escolas e viagens. Ele também controla pagamentos, embarque e seguro viagem.

## 📁 Estrutura de Pastas

```
backend/
├── bin/                  # Binários compilados (.out)
├── build/                # Arquivos temporários de build
├── data/                 # Arquivos .txt simulando persistência dos dados
├── include/              # Arquivos de cabeçalho (.h)
│   ├── models/           # Structs que representam as entidades do sistema
│   └── use_cases/        # Declarações das funções de manipulação e lógica
├── src/                  # Implementações em C (.c)
│   ├── models/           # Definição das entidades
│   └── use_cases/        # Implementação das funções de negócio (CRUD, busca, etc.)
├── main.c                # Arquivo principal para testes e execução
├── Makefile              # Script de compilação do projeto
└── README.md             # Documentação do backend
```

## 🧱 Entidades Modeladas

- `User`: cadastro e autenticação dos usuários.
- `School`: escolas cadastradas pela empresária.
- `ContactPerson`: contatos associados a cada escola.
- `Classroom`: turmas de cada escola.
- `Student`: alunos inscritos nas excursões.
- `Payment` & `Installment`: controle de pagamento único ou parcelado.
- `Boarding`: confirmação de embarque.
- `TravelInsurance`: dados do seguro viagem.

## 📌 Objetivo do Backend

- Gerenciar as entidades e regras de negócio da excursão.
- Manipular dados em arquivos `.txt`, simulando um banco de dados.
- Fornecer funções reutilizáveis para o frontend (interface gráfica).


## 🗄️ Backend

O repositório inclui um backend escrito em **C** localizado em `doc-embarque/backend`.
Ele simula a persistência de dados em arquivos `.txt` e disponibiliza diversos casos de uso para a manipulação de usuários e escolas. Alguns exemplos:

- `create_user_cli` e `login_cli` para cadastro e autenticação de usuários;
- `update_user_cli` e `delete_user_cli` para edição e remoção;
- `get_user_by_id` para consultas pontuais;
- `create_school_cli` para registrar escolas vinculadas a um usuário.
- `create_class_cli` para cadastro de turmas.
- `create_student_cli` para inserir alunos em uma turma.

### Requisitos

- GCC e `make`
- `cmake`

### Compilação

```bash
cd doc-embarque/backend
mkdir build && cd build
cmake .. && make
```

Após o processo o executável ficará disponível em `build/bin/doc_embarque`.
## 🚧 Em Desenvolvimento

Este backend será integrado à interface gráfica desenvolvida separadamente na pasta `frontend/`