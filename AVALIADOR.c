#include <stdio.h>     // Para entrada e sa�da padr�o (printf, fgets, fopen, etc.)
#include <stdlib.h>     // Para aloca��o din�mica de mem�ria (malloc, free, exit)
#include <ctype.h>      // Para fun��es de caractere (isdigit, isspace)
#include <stdbool.h>    // Para usar tipo bool, true, false
#include <string.h>     // Para manipula��o de strings (strlen, strcpy, etc.)
#include <limits.h>     // Para usar INT_MAX como valor especial
#include <math.h>       // Para fun��es matem�ticas (pow)

#define MAX 100         // Tamanho m�ximo de express�o e listas

// Enumera��o dos tipos de tokens poss�veis na express�o  
enum tokenType {  
    NUMERO,  
    PARENTESES,  
    OPERADOR,  
    NONE  
};  

// Estrutura que representa um token na express�o  
typedef struct {  
    char value;              // Caractere do token  
    enum tokenType type;     // Tipo do token  
    int peso;                // Prioridade do operador (se for operador)  
} Token;  

// N� de uma pilha de tokens  
typedef struct no{  
    Token tk;               // Token armazenado no n�  
    struct no *prox;        // Ponteiro para o pr�ximo n�  
} No;  

// Pilha de alta (ponteiros de n�)  
typedef struct {  
    No *cabeca;             // Ponteiro para o topo da pilha  
} Pilha;  

// Fun��o para inicializar a pilha de tokens  
void initPilha(Pilha *P) { P->cabeca = NULL; }  

// Cria um novo n� de pilha com o token fornecido  
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

// Verifica se a pilha de tokens est� vazia  
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

// Retorna o token no topo da pilha sem remov�-lo  
Token top(Pilha *P) {  
    Token tk = {'n', NONE, -1};  
    if(vaziaPilha(P)) return tk;  
    return P->cabeca->tk;  
}  

// N� de uma pilha de inteiros  
typedef struct node {  
    int valor;              // Valor do operando  
    struct node *prox;      // Ponteiro para o pr�ximo n�  
} Node;  

// Pilha de inteiros  
typedef struct {  
    Node *cabeca;           // Ponteiro para o topo da pilha  
} Pilha_int;  

// Fun��o para inicializar a pilha de inteiros  
void initPilhaInt(Pilha_int *P) { P->cabeca = NULL; }  

// Cria um novo n� de pilha de inteiros  
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

// Verifica se a pilha de inteiros est� vazia  
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

// Enumera��o dos tipos de erros poss�veis na valida��o  
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

// Fun��o para validar a express�o sintaticamente  
// Verifica caracteres, balanceamento de par�nteses, sequ�ncias v�lidas  
bool validar_expressao(const char *expressao, TipoErro *erro) {  
    int len = strlen(expressao);  
    int parenteses = 0; // Contador de par�nteses abertos  
    bool espera_numero = true; // Indica se espera um n�mero na posi��o atual  
    bool ultimo_foi_numero = false; // Flag se o �ltimo foi n�mero  
    bool ultimo_foi_parentese_fechado = false; // Flag se o �ltimo foi uma fechamento de par�nteses  

    *erro = ERRO_NENHUM; // Inicializa o erro  
    int i;  
    for (i= 0; i < len; i++) {  
        char c = expressao[i];  
        if (c == ' ') continue; // Ignora espa�os  

        // Verifica se o caractere � v�lido  
        if (!isdigit(c) && c != '+' && c != '-' && c != '*' && c != '/' && c != '^' && c != '(' && c != ')') {  
            *erro = ERRO_CARACTERE_INVALIDO;  
            return false;  
        }  

        // Gerenciamento de par�nteses  
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
        // Se for d�gito  
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

    // Verifica se parenteses est�o balanceados ao final  
    if (parenteses != 0) {  
        *erro = ERRO_PARENTESES_DESBALANCEADOS;  
        return false;  
    }  

    // Verifica se a express�o terminou corretamente (n�o com operador ou sinal)  
    if (espera_numero) {  
        *erro = ERRO_FINAL_INVALIDO;  
        return false;  
    }  

    return true;  
}  

// Fun��o para imprimir mensagem de erro com base no c�digo de erro  
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

// Verifica se a express�o cont�m divis�o por zero, antes de avaliar  
// Para evitar erros em tempo de execu��o  
bool contem_divisao_por_zero(Token* lista, int tamanho) {  
    int i;  
    for (i = 0; i < tamanho - 1; i++) {  
        if (lista[i].type == OPERADOR && lista[i].value == '/') {  
            if (lista[i + 1].type == NUMERO && lista[i + 1].value == '0') {  
                return true; // Encontrou divis�o por zero  
            }  
        }  
    }  
    return false; // Nenhuma divis�o por zero detectada  
}  

// Converte a express�o em uma lista de tokens  
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

        // Identifica n�meros  
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
                      3; // Prioridade m�xima para ^  
        }  
        // Identifica par�nteses  
        else if (expressao[i] == '(' || expressao[i] == ')') {  
            tk.type = PARENTESES;  
            tk.peso = 0;  
        } else {  
            tk.type = NONE; // Caractere inv�lido (n�o esperado)  
        }  
        lista[pos++] = tk; // Adiciona na lista de tokens  
        i++;  
    }  
    return pos; // Retorna quantidade de tokens obtidos  
}  

// Retorna a prioridade de um operador  
int precedencia(Token t) { return t.peso; }  

// Converte a lista de tokens de infixo para postfix (RPN) usando Shunting Yard  
// Converte uma express�o infixa para a nota��o polonesa reversa (RPN)
void converter_para_RPN(Token* lista, Token* fila_saida, int tamanho, int* tamanho_saida) {
    Pilha operadores;              // Pilha que armazena operadores temporariamente
    initPilha(&operadores);        // Inicializa a pilha de operadores

    *tamanho_saida = 0;             // Inicializa o tamanho da fila de sa�da
    int i;

    // Itera por todos os tokens da express�o
    for (i = 0; i < tamanho; i++) {
        Token tk = lista[i];

        if (tk.type == NUMERO) {
            // Se for um n�mero, adiciona diretamente na fila de sa�da
            fila_saida[(*tamanho_saida)++] = tk;

        } else if (tk.type == OPERADOR) {
            // Se for um operador, desempilha operadores de maior ou igual preced�ncia
            while (!vaziaPilha(&operadores) && top(&operadores).type == OPERADOR &&
                   precedencia(top(&operadores)) >= precedencia(tk)) {
                fila_saida[(*tamanho_saida)++] = top(&operadores); // Adiciona na sa�da
                pop(&operadores);                                   // Remove da pilha
            }
            push(&operadores, tk);  // Empilha o operador atual
        } else if (tk.type == PARENTESES && tk.value == '(') {
            // Se for uma abre par�nteses, empilha
            push(&operadores, tk);
        } else if (tk.type == PARENTESES && tk.value == ')') {
            // Se for um fecha par�nteses, desempilha at� encontrar a abre
            while (!vaziaPilha(&operadores) && top(&operadores).value != '(') {
                fila_saida[(*tamanho_saida)++] = top(&operadores); // Adiciona na sa�da
                pop(&operadores);                                   // Remove da pilha
            }
            // Remove o par�nteses de abertura na pilha
            if (!vaziaPilha(&operadores) && top(&operadores).value == '(')
                pop(&operadores);
        }
    }

    // Ap�s processar toda a express�o, desempilha todos os operadores remanescentes
    while (!vaziaPilha(&operadores)) {
        fila_saida[(*tamanho_saida)++] = top(&operadores);
        pop(&operadores);
    }
}

// Fun��o para avaliar uma express�o em RPN
int avaliar_RPN(Token* fila_saida, int tamanho_saida) {
    Pilha_int operandos;            // Pilha de inteiros que armazena operandos
    initPilhaInt(&operandos);      // Inicializa a pilha de inteiros
    int i;

    // Percorre os tokens em RPN
    for (i = 0; i < tamanho_saida; i++) {
        Token tk = fila_saida[i];

        if (tk.type == NUMERO) {
            // Se for n�mero, empilha o valor (convertido de char para int)
            push_int(&operandos, tk.value - '0');
        } else if (tk.type == OPERADOR) {
            // Se for operador, desempilha dois operandos
            int b = top_int(&operandos); pop_int(&operandos);
            int a = top_int(&operandos); pop_int(&operandos);
            int r = 0;

            // Executa a opera��o correspondente
            switch (tk.value) {
                case '+': r = a + b; break;
                case '-': r = a - b; break;
                case '*': r = a * b; break;
                case '/': // Divide e verifica divis�o por zero
                    if (b == 0) {
                        printf("Erro: Divisao por zero!\n");
                        return 0; // Sai da fun��o em caso de divisao por zero
                    }
                    r = a / b; break;
                case '^': // Calcula pot�ncia
                    if (a == 0 && b == 0) {
                        printf("Erro: 0 elevado a 0 � indefinido!\n");
                        return 0; // Caso espec�fico indefinido
                    }
                    r = (int)pow(a, b); break;
            }
            push_int(&operandos, r); // Empilha o resultado
        }
    }

    // Retorna o resultado final (que ser� o �nico elemento na pilha)
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
    TipoErro erro;                // Vari�vel para erro de valida��o
    int linha_num = 1;            // Contador de linhas

    // Loop de leitura linha por linha do arquivo
    while (fgets(linha, sizeof(linha), fp)) {
        printf("\n--- EXPRESS�O %d ---\n", linha_num++);
        linha[strcspn(linha, "\n")] = '\0';  // Remove o '\n' do final da linha

        if (strlen(linha) == 0) {
            printf("Express�o vazia. Ignorando.\n");
            continue; // Pula express�es vazias
        }

        // Valida a express�o antes de processar
        if (validar_expressao(linha, &erro)) {
            // Tokeniza a express�o
            int total = tokenize(linha, lista);

            // Verifica se h� divis�o por zero na express�o
            if (contem_divisao_por_zero(lista, total)) {
                erro = ERRO_DIVISAO_POR_ZERO; // Seta erro de divis�o por zero
                imprimir_erro(erro); // Exibe mensagem de erro
                continue; // Continua para a pr�xima linha
            }

            printf("Express�o v�lida!\n");

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

            // Converte a express�o para RPN
            Token fila_saida[MAX];
            int tamanho_saida;
            converter_para_RPN(lista, fila_saida, total, &tamanho_saida);

            // Exibe a express�o em RPN
            printf("Express�o em RPN: ");
            for (i = 0; i < tamanho_saida; i++) {
                printf("%c ", fila_saida[i].value);
            }
            printf("\n");

            // Avalia a express�o em RPN
            int resultado = avaliar_RPN(fila_saida, tamanho_saida);
            printf("Resultado: %d\n", resultado);
        } else {
            // Caso a express�o seja inv�lida, imprime erro
            imprimir_erro(erro);
        }
    }

    // Fecha o arquivo ap�s processamento
    fclose(fp);
    return 0;
}
