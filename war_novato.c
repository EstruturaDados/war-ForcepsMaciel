// WAR — pro nível Novato 
// Cadastro simples de 5 territórios

#include <stdio.h>
#include <string.h>

#define MAX_NOME 30
#define MAX_COR  15
#define N_TERR   5

// a parte do território
typedef struct {
    char nome[MAX_NOME + 1];
    char cor[MAX_COR + 1];
    int tropas;
} Territorio;

// remove o '\n' 
void mm_trim_nl(char *s) {
    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n') s[n - 1] = '\0';
}

// limpa buffer 
void mm_limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int main(void) {
    Territorio mapa[N_TERR];

    printf(" Desafio WAR (Novato) — Cadastro de Territórios \n");

    for (int i = 0; i < N_TERR; i++) {
        printf("\nTerritório %d/%d\n", i + 1, N_TERR);

        printf("Nome: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        mm_trim_nl(mapa[i].nome);

        printf("Cor do exército: ");
        fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin);
        mm_trim_nl(mapa[i].cor);

        printf("Número de tropas (int): ");
        while (scanf("%d", &mapa[i].tropas) != 1) {
            mm_limpar_buffer();
            printf("Valor inválido. Digite um inteiro: ");
        }
        mm_limpar_buffer();
    }

    printf("\n==== Mapa — %d territórios ====\n", N_TERR);
    printf("%-3s | %-30s | %-15s | %-6s\n", "#", "Nome", "Cor", "Tropas");
    printf("-----+--------------------------------+-----------------+--------\n");
    for (int i = 0; i < N_TERR; i++) {
        printf("%-3d | %-30s | %-15s | %-6d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }

    printf("\n[MM] Cadastro concluído. 👍\n");
    return 0;
}
