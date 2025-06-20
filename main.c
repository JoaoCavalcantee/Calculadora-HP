#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expressao.h"

#define MAX_EXPR 512

void testar_expressao(char *expressao) {
    printf("\n====================================\n");
    printf("Expressao original: \"%s\"\n", expressao);
    
    // Obter forma pós-fixa
    char *posfixa = getFormaPosFixa(expressao);
    printf("Forma Pos-fixa: %s\n", posfixa);
    
    // Obter forma infixa
    char *infixa = getFormaInFixa(expressao);
    printf("Forma Infixa: %s\n", infixa);
    
    // Calcular o valor
    float valor = getValorInFixa(expressao);
    printf("Valor calculado: %.6f\n", valor);
    
    free(posfixa);
    free(infixa);
}

int main() {
    printf("====================================\n");
    printf("TESTADOR DA CALCULADORA DE EXPRESSOES\n");
    printf("====================================\n");
    
    // Testes automáticos
    char *testes[] = {
        // Expressões infixas
        "( 3 + 4 ) * 5",
        "7 * 2 + 4",
        "8 + ( 5 * ( 2 + 4 ) )",
        "( 6 / 2 + 3 ) * 4",
        "9 + ( 5 * ( 2 + 8 * 4 ) )",
        "log ( 2 + 3 ) / 5",
        "( log ( 10 ) ) ^ 3 + 2",
        "( 45 + 60 ) * cos ( 30 )",
        "sen ( 45 ) ^ 2 + 0.5"
        
        // Expressões pós-fixas
        "3 4 + 5 *",
        "7 2 * 4 +",
        "8 5 2 4 + * +",
        "6 2 / 3 + 4 *",
        "9 5 2 8 * 4 + * +",
        "2 3 + log 5 /",
        "10 log 3 ^ 2 +",
        "45 60 + 30 cos *",
        "0.5 45 sen 2 ^ +",
        
        NULL
    };
    
    // Executa testes automáticos
    printf("\n>>> EXECUTANDO TESTES AUTOMATICOS <<<\n");
    for(int i = 0; testes[i] != NULL; i++) {
        testar_expressao(testes[i]);
    }
    
    // Modo interativo
    printf("\n>>> MODO INTERATIVO (digite 'sair' para terminar) <<<\n");
    char entrada[MAX_EXPR];
    
    while(1) {
        printf("\nDigite a expressao: ");
        fgets(entrada, MAX_EXPR, stdin);
        
        // Remove newline
        entrada[strcspn(entrada, "\n")] = '\0';
        
        if(strcmp(entrada, "sair") == 0) break;
        
        testar_expressao(entrada);
    }
    
    printf("\nPrograma encerrado. Obrigado!\n");
    return 0;
}