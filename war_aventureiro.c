// WAR — pro Nível Aventureiro
// Implementa ataque entre territórios com alocação dinâmica e ponteiros.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 30
#define MAX_COR  10

typedef struct {
    char nome[MAX_NOME + 1];
    char cor[MAX_COR + 1];
    int tropas;
} Territorio;

// Remove o \n final de uma string, se tiver
static void trim_nl(char *s) {
    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n') s[n - 1] = '\0';
}

// leitura de string com mensagem
static void ler_str(const char* msg, char* buf, size_t tam) {
    printf("%s", msg);
    if (fgets(buf, (int)tam, stdin) == NULL) {
        if (tam) buf[0] = '\0';
        return;
    }
    trim_nl(buf);
}

static void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// cadastro de territórios
void cadastrar(Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\n-- Cadastro do território %d/%d --\n", i + 1, n);
        ler_str("Nome: ", mapa[i].nome, sizeof(mapa[i].nome));
        ler_str("Cor do exército: ", mapa[i].cor, sizeof(mapa[i].cor));

        printf("Número de tropas (int): ");
        while (scanf("%d", &mapa[i].tropas) != 1 || mapa[i].tropas < 0) {
            limpar_buffer();
            printf("Valor inválido. Digite um inteiro >= 0: ");
        }
        limpar_buffer();
    }
}

// exibir os territórios
void exibir(const Territorio* mapa, int n) {
    printf("\n==== MAPA ATUAL (%d territórios) ====\n", n);
    printf("%-3s | %-30s | %-10s | %-6s\n", "#", "Nome", "Cor", "Tropas");
    printf("-----+--------------------------------+------------+--------\n");
    for (int i = 0; i < n; i++) {
        printf("%-3d | %-30s | %-10s | %-6d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

static int dado(void) { return (rand() % 6) + 1; }

// função de ataque
void atacar(Territorio* atacante, Territorio* defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\nNão é permitido atacar um território da mesma cor.\n");
        return;
    }

    if (atacante->tropas <= 0) {
        printf("\nO atacante não tem tropas suficientes.\n");
        return;
    }

    int ra = dado();
    int rd = dado();
    printf("\n=== BATALHA ===\n");
    printf("Atacante: %s (cor=%s, tropas=%d) | dado=%d\n",
           atacante->nome, atacante->cor, atacante->tropas, ra);
    printf("Defensor:  %s (cor=%s, tropas=%d) | dado=%d\n",
           defensor->nome, defensor->cor, defensor->tropas, rd);

    if (ra > rd) {
        int mover = atacante->tropas / 2;
        if (mover < 1) mover = 1;
        if (mover > atacante->tropas) mover = atacante->tropas;

        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = mover;
        atacante->tropas -= mover;

        printf("Atacante VENCEU! %s agora pertence a %s.\n",
               defensor->nome, defensor->cor);
    } else if (ra < rd) {
        if (atacante->tropas > 0) atacante->tropas -= 1;
        printf("Defensor VENCEU! %s perdeu 1 tropa.\n", atacante->nome);
    } else {
        printf("Empate — ninguém perdeu tropas.\n");
    }
}

// liberação da memória
void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

//  índic de território
int escolher_indice(const char* rotulo, const Territorio* mapa, int n) {
    int idx;
    printf("%s (digite o índice): ", rotulo);
    while (scanf("%d", &idx) != 1 || idx < 0 || idx >= n) {
        limpar_buffer();
        printf("Índice inválido. Tente novamente: ");
    }
    limpar_buffer();
    return idx;
}

int main(void) {
    srand((unsigned)time(NULL));

    int n;
    printf("==== WAR Estruturado — Nível Aventureiro ====\n");
    printf("Quantos territórios deseja cadastrar? ");
    while (scanf("%d", &n) != 1 || n <= 0) {
        limpar_buffer();
        printf("Valor inválido. Digite um inteiro > 0: ");
    }
    limpar_buffer();

    Territorio* mapa = (Territorio*)calloc((size_t)n, sizeof(Territorio));
    if (!mapa) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        return 1;
    }

    cadastrar(mapa, n);
    exibir(mapa, n);

    while (1) {
        char op[8];
        printf("\nDeseja realizar um ataque? (s/n): ");
        if (fgets(op, sizeof(op), stdin) == NULL) break;
        if (op[0] == 'n' || op[0] == 'N') break;

        exibir(mapa, n);
        int iAtk = escolher_indice("Índice do ATACANTE", mapa, n);
        int iDef = escolher_indice("Índice do DEFENSOR", mapa, n);

        atacar(&mapa[iAtk], &mapa[iDef]);
        exibir(mapa, n);
    }

    liberarMemoria(mapa);
    printf("\nPrograma finalizado.\n");
    return 0;
}
