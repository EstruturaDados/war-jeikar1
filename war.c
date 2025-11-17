#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

/*
    WAR Estruturado com:
    - Territorios alocados dinamicamente
    - Ataque entre territórios (simulação com dados)
    - Missões estratégicas (sorteadas e armazenadas dinamicamente)
    - Verificação de missão após cada ataque
*/

/* ---------- TIPO ---------- */
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

/* ---------- PROTÓTIPOS ---------- */
Territorio* cadastrarTerritorios(int total);
void exibirTerritorios(Territorio* mapa, int total);

void atacar(Territorio* atacante, Territorio* defensor);

void liberarMemoria(Territorio* mapa);

void atribuirMissao(char** destino, char* missoes[], int totalMissoes);
void exibirMissao(const char* missao);
int verificarMissao(const char* missao, Territorio* mapa, int total, const char* minhaCor, const char* corAdversaria);

/* Funções utilitárias */
static void toLowerCase(char* dst, const char* src);
static int strEqualsIgnoreCase(const char* a, const char* b);


/* ========================= MAIN ========================= */
int main() {
    srand((unsigned)time(NULL));  // Inicializa gerador de números aleatórios

    int total;
    printf("Digite o numero de territorios que deseja cadastrar: ");
    if (scanf("%d", &total) != 1 || total <= 0) {
        printf("Entrada invalida. Encerrando.\n");
        return 1;
    }

    /* Cadastro dos territórios (dinâmico) */
    Territorio* mapa = cadastrarTerritorios(total);
    if (!mapa) {
        printf("Falha na alocacao de memoria. Encerrando.\n");
        return 1;
    }

    /* --- Configuracao dos jogadores --- */
    char corJogador1[10], corJogador2[10];

    printf("\nJogador 1, escolha a cor do seu exercito (ex: vermelho): ");
    scanf("%9s", corJogador1);

    printf("Jogador 2, escolha a cor do seu exercito (ex: azul): ");
    scanf("%9s", corJogador2);

    /* --- Lista de missoes possiveis (pelo menos 5) --- */
    char* missoesDisponiveis[] = {
        "Conquistar 3 territorios",
        "Conquistar 5 territorios",
        "Eliminar todas as tropas da cor vermelha",
        "Ter mais tropas totais que o adversario",
        "Conquistar o primeiro territorio"
    };
    const int totalMissoes = sizeof(missoesDisponiveis) / sizeof(missoesDisponiveis[0]);

    /* Atribuir missao dinamicamente a cada jogador */
    char* missaoJog1 = NULL;
    char* missaoJog2 = NULL;

    atribuirMissao(&missaoJog1, missoesDisponiveis, totalMissoes);
    atribuirMissao(&missaoJog2, missoesDisponiveis, totalMissoes);

    /* Mostrar missões ao início (cada jogador vê a sua) */
    printf("\n--- Missao do Jogador 1 (%s) ---\n", corJogador1);
    exibirMissao(missaoJog1);

    printf("\n--- Missao do Jogador 2 (%s) ---\n", corJogador2);
    exibirMissao(missaoJog2);

    /* Menu principal: exibir, atacar, sair */
    int opcao;
    int vencedor = 0; // 0 = nenhum, 1 = jogador1, 2 = jogador2

    do {
        printf("\n=========== MENU ===========\n");
        printf("1 - Exibir territorios\n");
        printf("2 - Realizar ataque\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida. Encerrando.\n");
            break;
        }

        if (opcao == 1) {
            exibirTerritorios(mapa, total);
        }
        else if (opcao == 2) {
            /* Mostra mapa e pede atacante/defensor (numeracao p/ usuario 1..N) */
            exibirTerritorios(mapa, total);

            int at, df;
            printf("\nDigite o indice do territorio atacante (1 a %d): ", total);
            scanf("%d", &at);
            printf("Digite o indice do territorio defensor (1 a %d): ", total);
            scanf("%d", &df);

            at--; df--; // ajusta para indice 0-based

            /* Validacao dos indices */
            if (at < 0 || at >= total || df < 0 || df >= total) {
                printf("Indices invalidos!\n");
                continue;
            }
            /* Validar que nao atacou territorio da mesma cor */
            if (strEqualsIgnoreCase(mapa[at].cor, mapa[df].cor)) {
                printf("Um territorio nao pode atacar outro da mesma cor!\n");
                continue;
            }
            /* Verificar se atacante tem tropas suficientes */
            if (mapa[at].tropas <= 1) {
                printf("O territorio atacante nao tem tropas suficientes para atacar (precisa > 1).\n");
                continue;
            }

            /* Realiza o ataque */
            atacar(&mapa[at], &mapa[df]);

            /* Após o ataque, verificar missões silenciosamente */
            if (verificarMissao(missaoJog1, mapa, total, corJogador1, corJogador2)) {
                printf("\n--- Jogador 1 (%s) cumpriu a missao! ---\n", corJogador1);
                vencedor = 1;
                break;
            }
            if (verificarMissao(missaoJog2, mapa, total, corJogador2, corJogador1)) {
                printf("\n--- Jogador 2 (%s) cumpriu a missao! ---\n", corJogador2);
                vencedor = 2;
                break;
            }
        }

    } while (opcao != 0);

    if (opcao == 0 && vencedor == 0) {
        printf("\nJogo encerrado sem vencedor por missao.\n");
    } else if (vencedor != 0) {
        printf("\nParabens! Jogador %d venceu por cumprir sua missao.\n", vencedor);
    }

    /* Liberar memorias dinamicas */
    liberarMemoria(mapa);

    if (missaoJog1) free(missaoJog1);
    if (missaoJog2) free(missaoJog2);

    return 0;
}

/* ========================= FUNCOES ========================= */

/* Cadastro dinamico dos territorios */
Territorio* cadastrarTerritorios(int total) {
    Territorio* mapa = (Territorio*) calloc(total, sizeof(Territorio));
    if (!mapa) return NULL;

    printf("\n=== Cadastro de Territorios ===\n");
    for (int i = 0; i < total; i++) {
        printf("\nTerritorio %d:\n", i + 1);

        printf("Nome: ");
        scanf("%29s", mapa[i].nome);

        printf("Cor do exercito: ");
        scanf("%9s", mapa[i].cor);

        printf("Tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
    return mapa;
}

/* Exibe todos os territorios cadastrados (numeracao 1..N) */
void exibirTerritorios(Territorio* mapa, int total) {
    printf("\n======= Territorios Cadastrados =======\n");
    for (int i = 0; i < total; i++) {
        printf("\nID %d:\n", i + 1);
        printf("Nome: %s\n", mapa[i].nome);
        printf("Cor: %s\n", mapa[i].cor);
        printf("Tropas: %d\n", mapa[i].tropas);
    }
}

/* Simulação simples de ataque: rolagem de 1 dado para cada lado */
void atacar(Territorio* atacante, Territorio* defensor) {
    printf("\n=== Ataque iniciado ===\n");
    printf("%s (%s) ATACANDO %s (%s)\n",
           atacante->nome, atacante->cor,
           defensor->nome, defensor->cor);

    if (atacante->tropas <= 1) {
        printf("O atacante nao tem tropas suficientes para atacar!\n");
        return;
    }

    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\nDado do atacante: %d\n", dadoAtacante);
    printf("Dado do defensor: %d\n\n", dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Resultado: O atacante venceu!\n");

        int tropasOcupantes = atacante->tropas / 2;
        if (tropasOcupantes < 1) tropasOcupantes = 1; // garante movimento mínimo

        defensor->tropas = tropasOcupantes;
        /* copia cor do atacante para o defensor (conquista) */
        strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor) - 1);
        defensor->cor[sizeof(defensor->cor)-1] = '\0';

        atacante->tropas -= tropasOcupantes;
        if (atacante->tropas < 0) atacante->tropas = 0;

        printf("O territorio defensor foi conquistado! %d tropas ocupam o novo territorio.\n", tropasOcupantes);
    } else {
        printf("Resultado: O defensor resistiu!\n");
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
        printf("O atacante perdeu 1 tropa.\n");
    }
}

/* Libera memoria do mapa */
void liberarMemoria(Territorio* mapa) {
    free(mapa);
    printf("\nMemoria liberada com sucesso!\n");
}

/* Sorteia uma missão dentre as disponiveis e aloca dinamicamente a string destino */
void atribuirMissao(char** destino, char* missoes[], int totalMissoes) {
    if (!destino || totalMissoes <= 0) return;

    int idx = rand() % totalMissoes;
    const char* selecionada = missoes[idx];
    size_t len = strlen(selecionada);

    *destino = (char*) malloc(len + 1);
    if (!*destino) return;

    strcpy(*destino, selecionada);
}

/* Exibe a missao (passada por valor - somente leitura) */
void exibirMissao(const char* missao) {
    if (!missao) {
        printf("Nenhuma missao.\n");
        return;
    }
    printf("Sua missao: %s\n", missao);
}

/* Verifica se a missao foi cumprida.
   Implementacao simples que reconhece as missoes pre-definidas.
   Retorna 1 se cumprida, 0 caso contrario.
*/
int verificarMissao(const char* missao, Territorio* mapa, int total, const char* minhaCor, const char* corAdversaria) {
    if (!missao || !mapa || total <= 0 || !minhaCor) return 0;

    /* Normaliza em lower-case para comparacoes seguras */
    char missaoLow[256];
    toLowerCase(missaoLow, missao);

    /* 1) Conquistar 3 territorios */
    if (strstr(missaoLow, "conquistar 3")) {
        int count = 0;
        for (int i = 0; i < total; i++)
            if (strEqualsIgnoreCase(mapa[i].cor, minhaCor)) count++;
        return (count >= 3) ? 1 : 0;
    }

    /* 2) Conquistar 5 territorios */
    if (strstr(missaoLow, "conquistar 5")) {
        int count = 0;
        for (int i = 0; i < total; i++)
            if (strEqualsIgnoreCase(mapa[i].cor, minhaCor)) count++;
        return (count >= 5) ? 1 : 0;
    }

    /* 3) Eliminar todas as tropas da cor vermelha
       Aqui assumimos que a missao menciona 'vermelha' explicitamente.
       A verificacao checa se existe qualquer territorio com cor 'vermelha' que tenha tropas > 0.
    */
    if (strstr(missaoLow, "eliminar") && strstr(missaoLow, "vermelha")) {
        for (int i = 0; i < total; i++) {
            if (strEqualsIgnoreCase(mapa[i].cor, "vermelha") && mapa[i].tropas > 0)
                return 0; // ainda existe tropas vermelhas
        }
        return 1; // nao existem tropas vermelhas
    }

    /* 4) Ter mais tropas totais que o adversario */
    if (strstr(missaoLow, "mais tropas")) {
        int somaMinha = 0, somaAdv = 0;
        for (int i = 0; i < total; i++) {
            if (strEqualsIgnoreCase(mapa[i].cor, minhaCor)) somaMinha += mapa[i].tropas;
            else if (corAdversaria && strEqualsIgnoreCase(mapa[i].cor, corAdversaria)) somaAdv += mapa[i].tropas;
        }
        return (somaMinha > somaAdv) ? 1 : 0;
    }

    /* 5) Conquistar o primeiro territorio */
    if (strstr(missaoLow, "primeiro")) {
        if (total > 0 && strEqualsIgnoreCase(mapa[0].cor, minhaCor)) return 1;
        return 0;
    }

    /* Missao nao reconhecida (por seguranca) */
    return 0;
}

/* -------------------- UTILITÁRIAS -------------------- */

/* copia src para dst em minusculas (dst deve suportar tamanho) */
static void toLowerCase(char* dst, const char* src) {
    if (!dst || !src) return;
    while (*src) {
        *dst++ = (char) tolower((unsigned char)*src++);
    }
    *dst = '\0';
}

/* compara duas strings ignorando case (NULL-safe) */
static int strEqualsIgnoreCase(const char* a, const char* b) {
    if (!a || !b) return 0;
    while (*a && *b) {
        char ca = (char) tolower((unsigned char)*a);
        char cb = (char) tolower((unsigned char)*b);
        if (ca != cb) return 0;
        a++; b++;
    }
    return (*a == '\0' && *b == '\0');
}
