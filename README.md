# 📄 DocEmbarque

**DocEmbarque** é um sistema de gerenciamento de excursões escolares. A aplicação foi escrita em **C** e se divide em dois módulos:

- **backend/**: concentra as regras de negócio e grava os dados em arquivos `.txt`;
- **frontend/**: interface gráfica feita em **GTK** que consome diretamente as funções do backend.

O objetivo é centralizar o cadastro de alunos, documentos e pagamentos, além de gerar planilhas de embarque e de seguro viagem.

## 📂 Estrutura do Repositório

- `doc-embarque/backend/`  – código fonte do backend
- `doc-embarque/frontend/` – código do frontend em GTK
- `doc-embarque.drawio`    – diagrama das entidades

---

## 🚀 Funcionalidades

- Cadastro completo de alunos por unidade e turma
- Geração automática de planilhas:
    - **Central**: dados consolidados (nome, RG, CPF, data de nascimento, pagamento, ficha, autorização)
    - **Embarque**: nome, RG e campo de assinatura
    - **Seguro Viagem**: nome completo, RG, CPF, data de nascimento
- Validação da autorização: sem assinatura, o aluno não embarca
- Controle individualizado por colégio e turma
- Integração com dados de pagamento e status de presença

---

## 🧩 Estrutura das Planilhas

- **Planilha Central**  
  Contém todos os dados do aluno (identificação, documentos, pagamento e autorização)

- **Planilha de Embarque**  
  Lista para uso no dia da viagem, com campo de assinatura confirmando a presença

- **Planilha de Seguro Viagem**  
  Informações exigidas pela seguradora para garantir a cobertura do aluno

---

## 🛠️ Tecnologias Utilizadas

- **C & CMake** para o desenvolvimento do backend e do frontend;
- **GTK 3** para a interface gráfica;
- **OpenPyXL / xlsxwriter** (Python) para geração de planilhas Excel (em versões anteriores);
- Git + GitHub para versionamento e colaboração.

---

## 📦 Instalação

```bash
git clone git@github.com:ViniciusInTech/doc-embarque.git
cd doc-embarque
```

### Compilando o backend

```bash
cd doc-embarque/backend
mkdir build && cd build
cmake .. && make
```

### Compilando o frontend

```bash
cd ../../frontend
mkdir build && cd build
cmake .. && make
```
