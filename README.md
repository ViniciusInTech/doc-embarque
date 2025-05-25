# 📄 DocEmbarque

**DocEmbarque** é um sistema de gerenciamento de excursões escolares, desenvolvido para centralizar o controle de cadastros de alunos, documentos obrigatórios e geração de planilhas de embarque e seguro viagem. O sistema busca garantir segurança, rastreabilidade e organização no processo de embarque de estudantes.

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

- [OpenPyXL / xlsxwriter] para geração de planilhas Excel (se aplicável)
- Git + GitHub para versionamento e colaboração

---

## 📦 Instalação

```bash
git clone git@github.com:ViniciusInTech/doc-embarque.git
cd doc-embarque
```
