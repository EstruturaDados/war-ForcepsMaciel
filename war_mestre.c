// WAR — No nível Mestre
// Missões estratégicas com ponteiros, alocação dinâmica e verificação de vitória.

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

typedef struct {
    char nome[20];
    char cor[MAX_COR + 1];
    char* missao;   // alocada dinamicamente
} Jogador;

/*  utilidades  */

static void trim_nl(char* s) {
    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n') s[n - 1] = '\0';
}

static void ler_str(const char* msg, char* buf, size_t tam) {
    printf("%s", msg);
    if (fgets(buf, (int)tam, stdin) == NULL) { if (tam) buf[0] = '\0'; return; }
    trim_nl(buf);
}

static void limpar_buffer(void) {
    int c; while ((c = getchar()) != '\n' && c != EOF) {}
}

static int dado(void) { return (rand() % 6) + 1; }

/*  cadastro / exibição  */

void cadastrar_territorios(Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\n-- Cadastro do território %d/%d --\n", i + 1, n);
        ler_str("Nome: ", mapa[i].nome, sizeof(mapa[i].nome));
        ler_str("Cor do exército: ", mapa[i].cor, sizeof(mapa[i].cor));
        printf("Número de tropas (int): ");
        while (scanf("%d", &mapa[i].tropas) != 1 || mapa[i].tropas < 0) {
            limpar_buffer(); printf("Valor inválido. Digite um inteiro >= 0: ");
        }
        limpar_buffer();
    }
}

void exibir_mapa(const Territorio* mapa, int n) {
    printf("\n==== MAPA ATUAL (%d territórios) ====\n", n);
    printf("%-3s | %-30s | %-10s | %-6s\n", "#", "Nome", "Cor", "Tropas");
    printf("-----+--------------------------------+------------+--------\n");
    for (int i = 0; i < n; i++) {
        printf("%-3d | %-30s | %-10s | %-6d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

/*  ataque*/

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
        printf("Atacante VENCEU! %s agora pertence a %s (movidas %d tropas).\n",
               defensor->nome, defensor->cor, mover);
    } else if (ra < rd) {
        if (atacante->tropas > 0) atacante->tropas -= 1;
        printf("Defensor VENCEU! %s perdeu 1 tropa.\n", atacante->nome);
    } else {
        printf("Empate — ninguém perdeu tropas.\n");
    }
}

/* pra missões */
/* Vetor de strings com pelo menos 5 missões diferentes */
static const char* MISSOES[] = {
    "Controlar ao menos 3 territorios",
    "Somar ao menos 18 tropas no total",
    "Nao existirem territorios da cor vermelha",
    "Controlar ao menos 2 territorios",
    "Ter mais territorios que o adversario"
};
enum { QTD_MISSOES = sizeof(MISSOES)/sizeof(MISSOES[0]) };

/* sorteia índice válido em [0, totalMissoes) */
static int sorteia_indice(int totalMissoes) {
    if (totalMissoes <= 0) return 0;
    return rand() % totalMissoes;
}

/* Função pedida: copia a missão sorteada para destino (buffer já alocado) */
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int idx = sorteia_indice(totalMissoes);
    strcpy(destino, missoes[idx]);
}

/* Para cumprir o requisito de "armazenar dinamicamente":
   aloca o buffer e chama atribuirMissao() pra copiar */
char* atribuirMissaoDinamica(char* missoes[], int totalMissoes) {
    int idx = sorteia_indice(totalMissoes);
    size_t len = strlen(missoes[idx]) + 1;
    char* p = (char*)malloc(len);
    if (!p) { fprintf(stderr, "Falha ao alocar memoria para missao.\n"); exit(1); }
    memcpy(p, missoes[idx], len);
    return p;
}

/* helpers para verificar condições com base na cor do jogador */
static int total_territorios_da_cor(const Territorio* mapa, int n, const char* cor) {
    int c = 0;
    for (int i = 0; i < n; i++) if (strcmp(mapa[i].cor, cor) == 0) c++;
    return c;
}
static int total_tropas_da_cor(const Territorio* mapa, int n, const char* cor) {
    int s = 0;
    for (int i = 0; i < n; i++) if (strcmp(mapa[i].cor, cor) == 0) s += mapa[i].tropas;
    return s;
}
static int existe_cor(const Territorio* mapa, int n, const char* cor) {
    for (int i = 0; i < n; i++) if (strcmp(mapa[i].cor, cor) == 0) return 1;
    return 0;
}

// Implementação pedida: recebe a string da missão.
  
int verificarMissao(char* missao, Territorio* mapa, int tamanho,
                    const char* corJogador, const char* corAdversario) {
    if (strcmp(missao, MISSOES[0]) == 0) {
        // "Controlar ao menos 3 territorios"
        return total_territorios_da_cor(mapa, tamanho, corJogador) >= 3;
    } else if (strcmp(missao, MISSOES[1]) == 0) {
        // "Somar ao menos 18 tropas no total"
        return total_tropas_da_cor(mapa, tamanho, corJogador) >= 18;
    } else if (strcmp(missao, MISSOES[2]) == 0) {
        // "Mao existirem territorios da cor vermelha"
        return !existe_cor(mapa, tamanho, "vermelha");
    } else if (strcmp(missao, MISSOES[3]) == 0) {
        // "Controlar ao menos 2 territorios"
        return total_territorios_da_cor(mapa, tamanho, corJogador) >= 2;
    } else if (strcmp(missao, MISSOES[4]) == 0) {
        // "Ter mais territorios que o adversario"
        int meus = total_territorios_da_cor(mapa, tamanho, corJogador);
        int deles = total_territorios_da_cor(mapa, tamanho, corAdversario);
        return meus > deles;
    }
    return 0; 
}

void exibirMissao(const char* nomeJog, const char* missao) {
    printf("\nMissao de %s: %s\n", nomeJog, missao);
}


void liberarMemoria(Territorio* mapa, Jogador* js, int qtdJog) {
    free(mapa);
    if (js) {
        for (int i = 0; i < qtdJog; i++) free(js[i].missao);
        free(js);
    }
}

int escolher_indice(const char* rotulo, int n) {
    int idx;
    printf("%s (indice): ", rotulo);
    while (scanf("%d", &idx) != 1 || idx < 0 || idx >= n) {
        limpar_buffer(); printf("Indice invalido. Tente novamente: ");
    }
    limpar_buffer();
    return idx;
}

int main(void) {
    srand((unsigned)time(NULL));

    int n;
    printf("==== WAR Estruturado — Nivel Mestre ====\n");
    printf("Quantos territorios deseja cadastrar? ");
    while (scanf("%d", &n) != 1 || n <= 0) {
        limpar_buffer(); printf("Valor invalido. Digite um inteiro > 0: ");
    }
    limpar_buffer();

    Territorio* mapa = (Territorio*)calloc((size_t)n, sizeof(Territorio));
    if (!mapa) { fprintf(stderr, "Falha ao alocar memoria para mapa.\n"); return 1; }

    cadastrar_territorios(mapa, n);
    exibir_mapa(mapa, n);

    // dois jogadores
    int qtdJog = 2;
    Jogador* J = (Jogador*)calloc(qtdJog, sizeof(Jogador));
    if (!J) { fprintf(stderr, "Falha ao alocar memoria para jogadores.\n"); free(mapa); return 1; }

    // cadastro simples dos jogadores + cor e missão 
    for (int i = 0; i < qtdJog; i++) {
        printf("\n-- Jogador %d/%d --\n", i + 1, qtdJog);
        ler_str("Nome do jogador: ", J[i].nome, sizeof(J[i].nome));
        ler_str("Sua cor (ex.: azul, vermelho, verde...): ", J[i].cor, sizeof(J[i].cor));

        // alocada dinamicamente
        // vetor de atribuirMissao()
        char* missoesDyn[QTD_MISSOES];
        for (int k = 0; k < QTD_MISSOES; k++) missoesDyn[k] = (char*)MISSOES[k];

        J[i].missao = atribuirMissaoDinamica(missoesDyn, QTD_MISSOES);
        exibirMissao(J[i].nome, J[i].missao);
    }

    // loop de turnos, então após cada ataque, verifica silenciosamente missões
    while (1) {
        char op[8];
        printf("\nDeseja realizar um ataque? (s/n): ");
        if (fgets(op, sizeof(op), stdin) == NULL) break;
        if (op[0] == 'n' || op[0] == 'N') break;

        exibir_mapa(mapa, n);

        // escolher atacante ou defensor por indice
        int iAtk = escolher_indice("Indice do ATACANTE", n);
        int iDef = escolher_indice("Indice do DEFENSOR", n);

        atacar(&mapa[iAtk], &mapa[iDef]);
        exibir_mapa(mapa, n);

        // verificação silenciosa de missões
        // J[0] vs J[1]
        int venceu0 = verificarMissao(J[0].missao, mapa, n, J[0].cor, J[1].cor);
        int venceu1 = verificarMissao(J[1].missao, mapa, n, J[1].cor, J[0].cor);

        if (venceu0 && venceu1) {
            printf("\nAmbos cumpriram suas missoes no mesmo turno! Empate tecnico.\n");
            break;
        } else if (venceu0) {
            printf("\nVENCEDOR: %s — Missao cumprida: %s\n", J[0].nome, J[0].missao);
            break;
        } else if (venceu1) {
            printf("\nVENCEDOR: %s — Missao cumprida: %s\n", J[1].nome, J[1].missao);
            break;
        }
    }

    liberarMemoria(mapa, J, qtdJog);
    printf("\nPrograma finalizado.\n");
    return 0;
}
