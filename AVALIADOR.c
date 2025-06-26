#include <stdio.h>     // Para entrada e saída padrão (printf, fgets, fopen, etc.)
#include <stdlib.h>     // Para alocação dinâmica de memória (malloc, free, exit)
#include <ctype.h>      // Para funções de caractere (isdigit, isspace)
#include <stdbool.h>    // Para usar tipo bool, true, false
#include <string.h>     // Para manipulação de strings (strlen, strcpy, etc.)
#include <limits.h>     // Para usar INT_MAX como valor especial
#include <math.h>       // Para funções matemáticas (pow)

#define MAX 100         // Tamanho máximo de expressão e listas

// Enumeração dos tipos de tokens possíveis na expressão  
enum tokenType {  
    NUMERO,  
    PARENTESES,  
    OPERADOR,  
    NONE  
};  

// Estrutura que representa um token na expressão  
typedef struct {  
    char value;              // Caractere do token  
    enum tokenType type;     // Tipo do token  
    int peso;                // Prioridade do operador (se for operador)  
} Token;  

// Nó de uma pilha de tokens  
typedef struct no{  
    Token tk;               // Token armazenado no nó  
    struct no *prox;        // Ponteiro para o próximo nó  
} No;  

// Pilha de alta (ponteiros de nó)  
typedef struct {  
    No *cabeca;             // Ponteiro para o topo da pilha  
} Pilha;  

// Função para inicializar a pilha de tokens  
void initPilha(Pilha *P) { P->cabeca = NULL; }  

// Cria um novo nó de pilha com o token fornecido  
No *getNOPilha(Token tk) {  
    No *ptr = (No*)malloc(sizeof(No));  
    if(ptr == NULL) {  
        perror("Erro de alocacao de memoria.\n");  
        exit(-1);  
    }  
    ptr->prox = NULL;  
    ptr->tk = tk;  
    return ptr;  
}  

// Verifica se a pilha de tokens está vazia  
int vaziaPilha(Pilha *P) { return P->cabeca == NULL; }  

// Insere um token no topo da pilha  
int push(Pilha *P, Token tk) {  
    No *ptr = getNOPilha(tk);  
    ptr->prox = P->cabeca;  
    P->cabeca = ptr;  
    return 1;  
}  

// Remove o topo da pilha de tokens  
int pop(Pilha *P) {  
    if(vaziaPilha(P)) return 0;  
    No *auxi = P->cabeca;  
    P->cabeca = P->cabeca->prox;  
    free(auxi);  
    return 1;  
}  

// Retorna o token no topo da pilha sem removê-lo  
Token top(Pilha *P) {  
    Token tk = {'n', NONE, -1};  
    if(vaziaPilha(P)) return tk;  
    return P->cabeca->tk;  
}  

// Nó de uma pilha de inteiros  
typedef struct node {  
    int valor;              // Valor do operando  
    struct node *prox;      // Ponteiro para o próximo nó  
} Node;  

// Pilha de inteiros  
typedef struct {  
    Node *cabeca;           // Ponteiro para o topo da pilha  
} Pilha_int;  

// Função para inicializar a pilha de inteiros  
void initPilhaInt(Pilha_int *P) { P->cabeca = NULL; }  

// Cria um novo nó de pilha de inteiros  
Node *getNOPilhaInt(int n) {  
    Node *ptr = (Node*)malloc(sizeof(Node));  
    if(ptr == NULL) {  
        perror("Erro de alocacao de memoria.\n");  
        exit(-1);  
    }  
    ptr->valor = n;  
    ptr->prox = NULL;  
    return ptr;  
}  

// Verifica se a pilha de inteiros está vazia  
int vaziaPilhaInt(Pilha_int *P) { return P->cabeca == NULL; }  

// Insere um inteiro no topo da pilha  
int push_int(Pilha_int *P, int n) {  
    Node *ptr = getNOPilhaInt(n);  
    ptr->prox = P->cabeca;  
    P->cabeca = ptr;  
    return 1;  
}  

// Remove o topo da pilha de inteiros  
int pop_int(Pilha_int *P) {  
    if(vaziaPilhaInt(P)) return 0;  
    Node *ptr = P->cabeca;  
    P->cabeca = ptr->prox;  
    free(ptr);  
    return 1;  
}  

// Retorna o valor do topo da pilha de inteiros  
int top_int(Pilha_int *P) {  
    if(vaziaPilhaInt(P)) return INT_MAX;  
    return P->cabeca->valor;  
}  

// Enumeração dos tipos de erros possíveis na validação  
typedef enum {  
    ERRO_NENHUM,  
    ERRO_CARACTERE_INVALIDO,  
    ERRO_PARENTESES_DESBALANCEADOS,  
    ERRO_OPERADOR_FALTANDO,  
    ERRO_OPERADOR_INVALIDO,  
    ERRO_NUMERO_APOS_NUMERO,  
    ERRO_FINAL_INVALIDO,  
    ERRO_DIVISAO_POR_ZERO  
} TipoErro;  

// Função para validar a expressão sintaticamente  
// Verifica caracteres, balanceamento de parênteses, sequências válidas  
bool validar_expressao(const char *expressao, TipoErro *erro) {  
    int len = strlen(expressao);  
    int parenteses = 0; // Contador de parênteses abertos  
    bool espera_numero = true; // Indica se espera um número na posição atual  
    bool ultimo_foi_numero = false; // Flag se o último foi número  
    bool ultimo_foi_parentese_fechado = false; // Flag se o último foi uma fechamento de parênteses  

    *erro = ERRO_NENHUM; // Inicializa o erro  
    int i;  
    for (i= 0; i < len; i++) {  
        char c = expressao[i];  
        if (c == ' ') continue; // Ignora espaços  

        // Verifica se o caractere é válido  
        if (!isdigit(c) && c != '+' && c != '-' && c != '*' && c != '/' && c != '^' && c != '(' && c != ')') {  
            *erro = ERRO_CARACTERE_INVALIDO;  
            return false;  
        }  

        // Gerenciamento de parênteses  
        if (c == '(') {  
            if (ultimo_foi_numero) {  
                *erro = ERRO_OPERADOR_FALTANDO;  
                return false;  
            }  
            parenteses++;  
            espera_numero = true;  
            ultimo_foi_parentese_fechado = false;  
        } else if (c == ')') {  
            parenteses--;  
            if (parenteses < 0) {  
                *erro = ERRO_PARENTESES_DESBALANCEADOS;  
                return false;  
            }  
            espera_numero = false;  
            ultimo_foi_parentese_fechado = true;  
        }  
        // Se for dígito  
        else if (isdigit(c)) {  
            if (!espera_numero) {  
                *erro = ERRO_NUMERO_APOS_NUMERO;  
                return false;  
            }  
            espera_numero = false;  
            ultimo_foi_numero = true;  
            ultimo_foi_parentese_fechado = false;  
        }  
        // Caso seja operador  
        else {  
            if (espera_numero && c != '-' && c != '+') {  
                *erro = ERRO_OPERADOR_INVALIDO;  
                return false;  
            }  
            espera_numero = true;  
            ultimo_foi_numero = false;  
            ultimo_foi_parentese_fechado = false;  
        }  
    }  

    // Verifica se parenteses estão balanceados ao final  
    if (parenteses != 0) {  
        *erro = ERRO_PARENTESES_DESBALANCEADOS;  
        return false;  
    }  

    // Verifica se a expressão terminou corretamente (não com operador ou sinal)  
    if (espera_numero) {  
        *erro = ERRO_FINAL_INVALIDO;  
        return false;  
    }  

    return true;  
}  

// Função para imprimir mensagem de erro com base no código de erro  
void imprimir_erro(TipoErro erro) {  
    switch (erro) {  
        case ERRO_CARACTERE_INVALIDO: printf("Erro: Caractere invalido.\n"); break;  
        case ERRO_PARENTESES_DESBALANCEADOS: printf("Erro: Parenteses desbalanceados.\n"); break;  
        case ERRO_OPERADOR_FALTANDO: printf("Erro: Operador faltando.\n"); break;  
        case ERRO_OPERADOR_INVALIDO: printf("Erro: Operador invalido.\n"); break;  
        case ERRO_NUMERO_APOS_NUMERO: printf("Erro: Numero apos numero.\n"); break;  
        case ERRO_FINAL_INVALIDO: printf("Erro: Expressao termina com operador.\n"); break;  
        case ERRO_DIVISAO_POR_ZERO: printf("Erro: Divisao por zero detectada na expressao.\n"); break;  
        default: printf("Erro desconhecido.\n"); break;  
    }  
}  

// Verifica se a expressão contém divisão por zero, antes de avaliar  
// Para evitar erros em tempo de execução  
bool contem_divisao_por_zero(Token* lista, int tamanho) {  
    int i;  
    for (i = 0; i < tamanho - 1; i++) {  
        if (lista[i].type == OPERADOR && lista[i].value == '/') {  
            if (lista[i + 1].type == NUMERO && lista[i + 1].value == '0') {  
                return true; // Encontrou divisão por zero  
            }  
        }  
    }  
    return false; // Nenhuma divisão por zero detectada  
}  

// Converte a expressão em uma lista de tokens  
int tokenize(char* expressao, Token* lista) {  
    int i = 0, pos = 0;  
    while (expressao[i] != '\0') {  
        if (isspace(expressao[i])) {  
            i++;  
            continue;  
        }  
        Token tk;  
        tk.value = expressao[i];  
        tk.peso = -1;  

        // Identifica números  
        if (isdigit(expressao[i])) {  
            tk.type = NUMERO;  
            tk.peso = 0;  
        }  
        // Identifica operadores  
        else if (expressao[i] == '+' || expressao[i] == '-' || expressao[i] == '*' || expressao[i] == '/' || expressao[i] == '^') {  
            tk.type = OPERADOR;  
            // Define prioridade  
            tk.peso = (expressao[i] == '+' || expressao[i] == '-') ? 1 :   
                      (expressao[i] == '*' || expressao[i] == '/') ? 2 :   
                      3; // Prioridade máxima para ^  
        }  
        // Identifica parênteses  
        else if (expressao[i] == '(' || expressao[i] == ')') {  
            tk.type = PARENTESES;  
            tk.peso = 0;  
        } else {  
            tk.type = NONE; // Caractere inválido (não esperado)  
        }  
        lista[pos++] = tk; // Adiciona na lista de tokens  
        i++;  
    }  
    return pos; // Retorna quantidade de tokens obtidos  
}  

// Retorna a prioridade de um operador  
int precedencia(Token t) { return t.peso; }  

// Converte a lista de tokens de infixo para postfix (RPN) usando Shunting Yard  
// Converte uma expressão infixa para a notação polonesa reversa (RPN)
void converter_para_RPN(Token* lista, Token* fila_saida, int tamanho, int* tamanho_saida) {
    Pilha operadores;              // Pilha que armazena operadores temporariamente
    initPilha(&operadores);        // Inicializa a pilha de operadores

    *tamanho_saida = 0;             // Inicializa o tamanho da fila de saída
    int i;

    // Itera por todos os tokens da expressão
    for (i = 0; i < tamanho; i++) {
        Token tk = lista[i];

        if (tk.type == NUMERO) {
            // Se for um número, adiciona diretamente na fila de saída
            fila_saida[(*tamanho_saida)++] = tk;

        } else if (tk.type == OPERADOR) {
            // Se for um operador, desempilha operadores de maior ou igual precedência
            while (!vaziaPilha(&operadores) && top(&operadores).type == OPERADOR &&
                   precedencia(top(&operadores)) >= precedencia(tk)) {
                fila_saida[(*tamanho_saida)++] = top(&operadores); // Adiciona na saída
                pop(&operadores);                                   // Remove da pilha
            }
            push(&operadores, tk);  // Empilha o operador atual
        } else if (tk.type == PARENTESES && tk.value == '(') {
            // Se for uma abre parênteses, empilha
            push(&operadores, tk);
        } else if (tk.type == PARENTESES && tk.value == ')') {
            // Se for um fecha parênteses, desempilha até encontrar a abre
            while (!vaziaPilha(&operadores) && top(&operadores).value != '(') {
                fila_saida[(*tamanho_saida)++] = top(&operadores); // Adiciona na saída
                pop(&operadores);                                   // Remove da pilha
            }
            // Remove o parênteses de abertura na pilha
            if (!vaziaPilha(&operadores) && top(&operadores).value == '(')
                pop(&operadores);
        }
    }

    // Após processar toda a expressão, desempilha todos os operadores remanescentes
    while (!vaziaPilha(&operadores)) {
        fila_saida[(*tamanho_saida)++] = top(&operadores);
        pop(&operadores);
    }
}

// Função para avaliar uma expressão em RPN
int avaliar_RPN(Token* fila_saida, int tamanho_saida) {
    Pilha_int operandos;            // Pilha de inteiros que armazena operandos
    initPilhaInt(&operandos);      // Inicializa a pilha de inteiros
    int i;

    // Percorre os tokens em RPN
    for (i = 0; i < tamanho_saida; i++) {
        Token tk = fila_saida[i];

        if (tk.type == NUMERO) {
            // Se for número, empilha o valor (convertido de char para int)
            push_int(&operandos, tk.value - '0');
        } else if (tk.type == OPERADOR) {
            // Se for operador, desempilha dois operandos
            int b = top_int(&operandos); pop_int(&operandos);
            int a = top_int(&operandos); pop_int(&operandos);
            int r = 0;

            // Executa a operação correspondente
            switch (tk.value) {
                case '+': r = a + b; break;
                case '-': r = a - b; break;
                case '*': r = a * b; break;
                case '/': // Divide e verifica divisão por zero
                    if (b == 0) {
                        printf("Erro: Divisao por zero!\n");
                        return 0; // Sai da função em caso de divisao por zero
                    }
                    r = a / b; break;
                case '^': // Calcula potência
                    if (a == 0 && b == 0) {
                        printf("Erro: 0 elevado a 0 é indefinido!\n");
                        return 0; // Caso específico indefinido
                    }
                    r = (int)pow(a, b); break;
            }
            push_int(&operandos, r); // Empilha o resultado
        }
    }

    // Retorna o resultado final (que será o único elemento na pilha)
    return top_int(&operandos);
}

int main() {
    // Abre o arquivo de entrada
    FILE *fp = fopen("in.txt", "r");
    if (!fp) {
        perror("Erro ao abrir o arquivo in.txt");
        return 1;
    }

    char linha[MAX];              // Buffer para leitura de linhas
    Token lista[MAX];             // Lista de tokens
    TipoErro erro;                // Variável para erro de validação
    int linha_num = 1;            // Contador de linhas

    // Loop de leitura linha por linha do arquivo
    while (fgets(linha, sizeof(linha), fp)) {
        printf("\n--- EXPRESSÃO %d ---\n", linha_num++);
        linha[strcspn(linha, "\n")] = '\0';  // Remove o '\n' do final da linha

        if (strlen(linha) == 0) {
            printf("Expressão vazia. Ignorando.\n");
            continue; // Pula expressões vazias
        }

        // Valida a expressão antes de processar
        if (validar_expressao(linha, &erro)) {
            // Tokeniza a expressão
            int total = tokenize(linha, lista);

            // Verifica se há divisão por zero na expressão
            if (contem_divisao_por_zero(lista, total)) {
                erro = ERRO_DIVISAO_POR_ZERO; // Seta erro de divisão por zero
                imprimir_erro(erro); // Exibe mensagem de erro
                continue; // Continua para a próxima linha
            }

            printf("Expressão válida!\n");

            // Mostra os tokens gerados
            printf("Tokens gerados:\n");
            int i;
            for (i = 0; i < total; i++) {
                printf("Valor: %c | Tipo: ", lista[i].value);
                // Identifica o tipo do token
                switch (lista[i].type) {
                    case NUMERO: printf("NUMERO"); break;
                    case OPERADOR: printf("OPERADOR"); break;
                    case PARENTESES: printf("PARENTESES"); break;
                    default: printf("INVALIDO"); break;
                }
                printf(" | Peso: %d\n", lista[i].peso);
            }

            // Converte a expressão para RPN
            Token fila_saida[MAX];
            int tamanho_saida;
            converter_para_RPN(lista, fila_saida, total, &tamanho_saida);

            // Exibe a expressão em RPN
            printf("Expressão em RPN: ");
            for (i = 0; i < tamanho_saida; i++) {
                printf("%c ", fila_saida[i].value);
            }
            printf("\n");

            // Avalia a expressão em RPN
            int resultado = avaliar_RPN(fila_saida, tamanho_saida);
            printf("Resultado: %d\n", resultado);
        } else {
            // Caso a expressão seja inválida, imprime erro
            imprimir_erro(erro);
        }
    }

    // Fecha o arquivo após processamento
    fclose(fp);
    return 0;
}
