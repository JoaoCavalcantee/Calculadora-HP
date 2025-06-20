#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>

#include "expressao.h"

#define TAM_MAX 512
#define TAM_PILHA 256
#define PI 3.1415926

// ==============================================================
// FUNÇÕES AUXILIARES
// ==============================================================

static bool ehOperador(char *token) {
    return (strcmp(token, "+") == 0 || strcmp(token, "-") == 0 ||
           strcmp(token, "*") == 0 || strcmp(token, "/") == 0 ||
           strcmp(token, "%") == 0 || strcmp(token, "^") == 0 ||
           strcmp(token, "log") == 0 || strcmp(token, "sen") == 0 ||
           strcmp(token, "cos") == 0 || strcmp(token, "tg") == 0);
}

static int precedencia(char *operador) {
    if(strcmp(operador, "log") == 0 || 
       strcmp(operador, "sen") == 0 || 
       strcmp(operador, "cos") == 0 || 
       strcmp(operador, "tg") == 0) {
        return 5;
    }
    
    if(strcmp(operador, "^") == 0) {
        return 4;
    }
    
    if(strcmp(operador, "*") == 0 || 
       strcmp(operador, "/") == 0 || 
       strcmp(operador, "%") == 0) {
        return 3;
    }
    
    if(strcmp(operador, "+") == 0 || 
       strcmp(operador, "-") == 0) {
        return 2;
    }
    
    if(strcmp(operador, "(") == 0) {
        return 1;
    }
    
    return 0;
}

static bool precisa_parenteses_esq(char *expr, char *op) {
    if(isdigit(expr[0]) || expr[0] == '.' || 
      (expr[0] == '-' && (isdigit(expr[1]) || expr[1] == '.'))) {
        return false;
    }
    
    if(strstr(expr, "log") == expr || 
       strstr(expr, "sen") == expr || 
       strstr(expr, "cos") == expr || 
       strstr(expr, "tg") == expr) {
        return false;
    }
    
    return true;
}

static bool precisa_parenteses_dir(char *expr, char *op) {
    if(isdigit(expr[0]) || expr[0] == '.' || 
      (expr[0] == '-' && (isdigit(expr[1]) || expr[1] == '.'))) {
        return false;
    }
    
    if(strstr(expr, "log") == expr || 
       strstr(expr, "sen") == expr || 
       strstr(expr, "cos") == expr || 
       strstr(expr, "tg") == expr) {
        return false;
    }
    
    if(strcmp(op, "^") == 0) return true;
    
    return true;
}

static int Verificar_tipo(char *expressao) {
    char Copia[TAM_MAX];
    strcpy(Copia, expressao);
    
    char *Caracteres_Separados[TAM_MAX];
    int contador = 0;

    char *token = strtok(Copia, " ");
    while (token != NULL) {
        Caracteres_Separados[contador++] = token;
        token = strtok(NULL, " ");
    }

    if(contador == 0) return 0;

    for (int i = 0; i < contador; i++) {
        if (strcmp(Caracteres_Separados[i], "(") == 0 || 
            strcmp(Caracteres_Separados[i], ")") == 0) {
            return 0;
        }
    }

    char *ultimo = Caracteres_Separados[contador - 1];
    
    if(strlen(ultimo) == 1 && strchr("+-*/%^", ultimo[0])) {
        return 1;
    }
    
    if(ehOperador(ultimo)) {
        return 1;
    }
    
    return 0;
}

// ==============================================================
// FUNÇÕES PÚBLICAS
// ==============================================================

char *getFormaPosFixa(char *Str) {

    if(Verificar_tipo(Str) == 1) {
        return strdup(Str);
    }

    char copia[TAM_MAX];
    strcpy(copia, Str);
    
    char *saida[TAM_PILHA];
    int contadorSaida = 0;
    
    char pilha[TAM_PILHA][10];
    int topo = -1;

    char *token = strtok(copia, " ");
    while(token != NULL) {
        if(isdigit(token[0]) || token[0] == '.' || 
          (token[0] == '-' && (isdigit(token[1]) || token[1] == '.'))) {
            saida[contadorSaida++] = strdup(token);
        }
        else if(strcmp(token, "(") == 0) {
            strcpy(pilha[++topo], "(");
        }
        else if(strcmp(token, ")") == 0) {
            while(topo >= 0 && strcmp(pilha[topo], "(") != 0) {
                saida[contadorSaida++] = strdup(pilha[topo--]);
            }
            
            if(topo >= 0) topo--;
            
            if(topo >= 0 && (strcmp(pilha[topo], "log") == 0 ||
                             strcmp(pilha[topo], "sen") == 0 ||
                             strcmp(pilha[topo], "cos") == 0 ||
                             strcmp(pilha[topo], "tg") == 0)) {
                saida[contadorSaida++] = strdup(pilha[topo--]);
            }
        }
        else if(ehOperador(token)) {
            while(topo >= 0 && 
                  strcmp(pilha[topo], "(") != 0 && 
                  precedencia(pilha[topo]) >= precedencia(token)) {
                saida[contadorSaida++] = strdup(pilha[topo--]);
            }
            
            strcpy(pilha[++topo], token);
        }
        
        token = strtok(NULL, " ");
    }

    while(topo >= 0) {
        saida[contadorSaida++] = strdup(pilha[topo--]);
    }

    char *resultado = (char *)malloc(TAM_MAX);
    resultado[0] = '\0';
    for(int i = 0; i < contadorSaida; i++) {
        strcat(resultado, saida[i]);
        if(i < contadorSaida - 1) strcat(resultado, " ");
        free(saida[i]);
    }
    
    return resultado;
}

char *getFormaInFixa(char *Str) {

    if(Verificar_tipo(Str) == 0) {
        return strdup(Str);
    }

    char copia[TAM_MAX];
    strcpy(copia, Str);

    char pilha[TAM_PILHA][TAM_MAX];
    int topo = -1;

    char *token = strtok(copia, " ");
    while(token != NULL) {
        if(isdigit(token[0]) || token[0] == '.' || 
          (token[0] == '-' && (isdigit(token[1]) || token[1] == '.'))) {
            strcpy(pilha[++topo], token);
        }
        else if(strcmp(token, "log") == 0 || 
                strcmp(token, "sen") == 0 || 
                strcmp(token, "cos") == 0 || 
                strcmp(token, "tg") == 0) {
            if(topo < 0) return strdup("ERRO: Falta operando para função");
            
            char operando[TAM_MAX];
            strcpy(operando, pilha[topo--]);
            
            char temp[TAM_MAX];
            sprintf(temp, "%s(%s)", token, operando);
            strcpy(pilha[++topo], temp);
        }
        else if(ehOperador(token)) {
            if(topo < 1) return strdup("ERRO: Operação requer dois operandos");
            
            char dir[TAM_MAX], esq[TAM_MAX];
            strcpy(dir, pilha[topo--]);
            strcpy(esq, pilha[topo--]);
            
            char esq_formatado[TAM_MAX];
            if(precisa_parenteses_esq(esq, token)) {
                sprintf(esq_formatado, "(%s)", esq);
            } else {
                strcpy(esq_formatado, esq);
            }
            
            char dir_formatado[TAM_MAX];
            if(precisa_parenteses_dir(dir, token)) {
                sprintf(dir_formatado, "(%s)", dir);
            } else {
                strcpy(dir_formatado, dir);
            }
            
            char temp[TAM_MAX];
            sprintf(temp, "%s %s %s", esq_formatado, token, dir_formatado);
            strcpy(pilha[++topo], temp);
        }
        else {
            return strdup("ERRO: Token desconhecido");
        }
        
        token = strtok(NULL, " ");
    }

    if(topo != 0) {
        return strdup("ERRO: Expressão incompleta");
    }
    
    return strdup(pilha[topo]);
}

float getValorPosFixa(char *StrPosFixa) {
    char copia[TAM_MAX];
    strcpy(copia, StrPosFixa);

    float pilha[TAM_PILHA];
    int topo = -1;
    
    char *token = strtok(copia, " ");
    char *endptr;

    while(token != NULL) {
        if(isdigit(token[0]) || token[0] == '.' || 
          (token[0] == '-' && (isdigit(token[1]) || token[1] == '.'))) {
            pilha[++topo] = strtof(token, &endptr);
        }
        else if(strcmp(token, "log") == 0) {
            if(topo < 0) {
                printf("Erro: Pilha vazia para 'log'\n");
                return 0.0f;
            }
            float num = pilha[topo--];
            if(num <= 0.0f) {
                printf("Erro: Logaritmo de número não-positivo (%f)\n", num);
                return 0.0f;
            }
            pilha[++topo] = log10f(num);
        }
        else if(strcmp(token, "sen") == 0) {
            if(topo < 0) {
                printf("Erro: Pilha vazia para 'sen'\n");
                return 0.0f;
            }
            float num = pilha[topo--];
            float radianos = num * PI / 180.0;
            pilha[++topo] = sinf(radianos);
        }
        else if(strcmp(token, "cos") == 0) {
            if(topo < 0) {
                printf("Erro: Pilha vazia para 'cos'\n");
                return 0.0f;
            }
            float num = pilha[topo--];
            float radianos = num * PI / 180.0;
            pilha[++topo] = cosf(radianos);
        }
        else if(strcmp(token, "tg") == 0) {
            if(topo < 0) {
                printf("Erro: Pilha vazia para 'tg'\n");
                return 0.0f;
            }
            float num = pilha[topo--];
            float radianos = num * PI / 180.0;
            
            if(fabsf(cosf(radianos)) < 1e-6) {
                printf("Erro: Tangente indefinida para %.1f graus\n", num);
                return 0.0f;
            }
            pilha[++topo] = tanf(radianos);
        }
        else if(ehOperador(token)) {
            if(topo < 1) {
                printf("Erro: Operação '%s' requer dois operandos\n", token);
                return 0.0f;
            }
             
            float b = pilha[topo--];
            float a = pilha[topo--];
            float resultado = 0.0f;

            if(strcmp(token, "+") == 0) {
                resultado = a + b;
            }
            else if(strcmp(token, "-") == 0) {
                resultado = a - b;
            }
            else if(strcmp(token, "*") == 0) {
                resultado = a * b;
            }
            else if(strcmp(token, "/") == 0) {
                if(fabsf(b) < 1e-6) {
                    printf("Erro: Divisão por zero\n");
                    return 0.0f;
                }
                resultado = a / b;
            }
            else if(strcmp(token, "%") == 0) {
                resultado = fmodf(a, b);
            }
            else if(strcmp(token, "^") == 0) {
                resultado = powf(a, b);
            }
            else {
                printf("Operador desconhecido: %s\n", token);
                return 0.0f;
            }
            
            pilha[++topo] = resultado;
        }
        else {
            printf("Token desconhecido: %s\n", token);
            return 0.0f;
        }
        
        token = strtok(NULL, " ");
    }

    if(topo != 0) {
        printf("Erro: %d valores não processados na pilha\n", topo + 1);
        return 0.0f;
    }
    
    return pilha[topo];
}

float getValorInFixa(char *StrInFixa) {
    char *posfixa = getFormaPosFixa(StrInFixa);
    float resultado = getValorPosFixa(posfixa);
    free(posfixa);
    return resultado;
}