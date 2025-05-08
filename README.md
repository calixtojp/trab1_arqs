# Projeto Organização de Arquivos - SCC0215

Este repositório contém a implementação de um sistema em C para gerenciar arquivos de dados binários, executando operações análogas aos comandos SQL _CREATE INDEX_, _SELECT (WHERE)_, _DELETE_, _INSERT_ e _UPDATE_. O código segue rigorosamente as especificações didáticas da disciplina de Organização de Arquivos (SCC0215), garantindo consistência, performance e manutenibilidade.

## Problema Abordado

Gerenciar eficientemente operações de consulta, inserção, remoção e atualização em arquivos de dados binários, com e sem o suporte de índices, assegurando:

- **Consistência transacional**: mecanismos de status para evitar corrupção de arquivos em caso de falhas (travamento, queda de energia).
- **Modularidade**: separação clara entre lógica de entrada/saída, manipulação de arquivos de dados e índices, e implementação das funcionalidades.
- **Tratamento de valores nulos** e reutilização de espaço de registros logicamente removidos.
- **Melhorias de desempenho** por meio de índices lineares quando disponíveis, e busca sequencial quando não.

## Solução Implementada

A solução é organizada em módulos bem definidos:

1. **manipulacao**: Biblioteca genérica para abstrair operações em arquivos de dados (`ArqDados`) e índices (`ArqIndex`), incluindo alocação, leitura de cabeçalhos, manipulação de vetores de índices, ordenação e escrita em disco.
2. **auxiliares**: Funções utilitárias para leitura de parâmetros, tratamento de strings com aspas, exibição de erros e interface com a função `binarioNaTela()` para saída binária automatizada.
3. **funcionalidades**: Implementação de cada operação (funções `create_index()`, `where()`, `delete_from()`, `insert_into()`, `update()`) seguindo padrões:
   - Leitura e validação de parâmetros de entrada.
   - Abertura de arquivos em modos apropriados (leitura, escrita, atualização).
   - Verificação de consistência (`status` nos cabeçalhos).
   - Uso de callbacks (`FncAcao`, `FncFinaliza`) para aplicar ações genéricas sobre registros.
   - Escrita de alterações e reescrita de índices conforme necessário.
4. **main.c**: Ponto de entrada que seleciona a funcionalidade com base em código lido da entrada padrão.

### Fluxo Geral de uma Operação

1. **Leitura** dos parâmetros obrigatórios (arquivos, campo indexado, tipo de dado, número de operações).
2. **Inicialização** das estruturas via `alocar_arq_dados()` e `alocar_arq_index()`.
3. **Abertura** dos arquivos de dados e índice nos modos corretos.
4. **Validação** de consistência: aborta com mensagem de erro se algum arquivo estiver inconsistente.
5. **Processamento**:
   - Para consultas (`where`): busca binária via índice ou busca sequencial.
   - Para modificações (insert/delete/update): atualização de registros e do vetor de índices em memória.
6. **Reescrita** dos arquivos de índice e cabeçalhos, ajustando campos de status e contadores.
7. **Saída** binária usando `binarioNaTela()` para validação automática.

## Boas Práticas e Estrutura

- **Modularização em camadas**: cada responsabilidade (I/O, lógica de negócios, estrutura de dados) em arquivos separados.
- **Abstração**: tipos opacos (`ArqDados_t`, `ArqIndex_t`) e funções de alto nível escondem detalhes de implementação.
- **Flexibilidade**: uso de _callback functions_ para definir ações genéricas sobre registros, facilitando extensões futuras.
- **Documentação**: comentários em nível de funções, parâmetros e blocos, explicando fluxos internos.
- **Tratamento robusto de erros**: verificações após cada operação crítica, com saída padronizada.
- **Reutilização de espaço**: abordagem estática para reaproveitar registros logicamente removidos, reduzindo fragmentação.

## Compilação e Execução

1. **Compilar**:
   ```bash
   make all
````

2. **Executar** (exemplos):

   ```bash
   ./programaTrab 3 dados.bin id inteiro indice.bin       # Criar índice
   ./programaTrab 4 dados.bin id inteiro indice.bin 2     # Consultar registros
   ./programaTrab 5 dados.bin id inteiro indice.bin 3     # Remover registros
   ./programaTrab 6 dados.bin id inteiro indice.bin 2     # Inserir registros
   ./programaTrab 7 dados.bin id inteiro indice.bin 1     # Atualizar registros
   ```

## Licença

Este projeto é parte das atividades da disciplina SCC0215 e está disponível para fins educacionais.

```
```
