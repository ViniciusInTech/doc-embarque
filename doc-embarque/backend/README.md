# 🧠 Backend - Projeto Lumaster Turismo

Este diretório contém a implementação do backend da aplicação **Lumaster Turismo**, responsável por gerenciar os dados da excursão escolar, incluindo cadastro de alunos, pagamentos, embarque e seguro viagem.

## 📁 Estrutura de Pastas

```
backend/
├── bin/                  # Binários compilados (.out)
├── build/                # Arquivos temporários de build
├── data/                 # Arquivos .txt simulando persistência dos dados
├── include/              # Arquivos de cabeçalho (.h)
│   ├── models/           # Structs que representam as entidades do sistema
│   └── services/         # Declarações das funções de manipulação e lógica
├── src/                  # Implementações em C (.c)
│   ├── models/           # Definição das entidades
│   └── services/         # Implementação das funções de negócio (CRUD, busca, etc.)
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

## 🚧 Em Desenvolvimento

Este backend será integrado à interface gráfica desenvolvida separadamente na pasta `frontend/`.
