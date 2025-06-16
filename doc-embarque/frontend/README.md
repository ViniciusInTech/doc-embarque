# 🌐 Frontend

Esta pasta contém a interface gráfica em GTK para o projeto **DocEmbarque**.
O frontend está organizado em módulos pequenos para cada tela/dialogo.
Atualmente há uma janela de login e um diálogo de cadastro de usuários que
consomem diretamente as funções do backend.

## Compilação

```bash
    cd doc-embarque/frontend
    mkdir build && cd build
    cmake .. && make
```

O executável será gerado em `build/bin/doc_embarque_frontend`.

Ao executar, o aplicativo carrega o arquivo `style.css` para aplicar uma
aparência mais moderna à interface.