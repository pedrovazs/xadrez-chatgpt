#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BOARD_SIZE 8

// Declaração do tabuleiro global
char board[BOARD_SIZE][BOARD_SIZE];

// Função para inicializar o tabuleiro com a posição inicial padrão do xadrez
void initBoard() {
    // Preenche todas as casas com espaço
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = ' ';
        }
    }
    
    // Peças brancas (letras maiúsculas) e pretas (letras minúsculas)
    // Rook (Torre), Knight (Cavalo), Bishop (Bispo), Queen (Rainha), King (Rei)
    board[7][0] = 'R'; board[7][7] = 'R';
    board[7][1] = 'N'; board[7][6] = 'N';
    board[7][2] = 'B'; board[7][5] = 'B';
    board[7][3] = 'Q'; board[7][4] = 'K';
    
    board[0][0] = 'r'; board[0][7] = 'r';
    board[0][1] = 'n'; board[0][6] = 'n';
    board[0][2] = 'b'; board[0][5] = 'b';
    board[0][3] = 'q'; board[0][4] = 'k';
    
    // Peões
    for (int j = 0; j < BOARD_SIZE; j++) {
        board[6][j] = 'P';
        board[1][j] = 'p';
    }
}

// Função para exibir o tabuleiro no terminal
void printBoard() {
    printf("  a b c d e f g h\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%d ", BOARD_SIZE - i);
        for (int j = 0; j < BOARD_SIZE; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

// Converte uma posição no formato de notação algébrica (ex: "e2") para coordenadas (linha, coluna)
int parsePosition(const char *pos, int *row, int *col) {
    if (pos == NULL || strlen(pos) < 2)
        return 0;
    char file = pos[0];
    char rank = pos[1];
    if (file < 'a' || file > 'h' || rank < '1' || rank > '8')
        return 0;
    *col = file - 'a';
    *row = BOARD_SIZE - (rank - '0'); // Linha 0 corresponde à 8ª fileira
    return 1;
}

// Função para mover uma peça da posição de origem para a posição de destino
// Esta função realiza apenas verificações básicas (não valida movimentos específicos de cada peça)
int movePiece(int srcRow, int srcCol, int destRow, int destCol, int isWhiteTurn) {
    char piece = board[srcRow][srcCol];
    if (piece == ' ') {
        printf("Nenhuma peça na posição de origem.\n");
        return 0;
    }
    
    // Verifica se a peça pertence ao jogador da vez
    if (isWhiteTurn && !isupper(piece)) {
        printf("Peça inválida para o jogador branco.\n");
        return 0;
    }
    if (!isWhiteTurn && !islower(piece)) {
        printf("Peça inválida para o jogador preto.\n");
        return 0;
    }
    
    // Verifica se a posição de destino não contém uma peça do mesmo jogador
    char destPiece = board[destRow][destCol];
    if (destPiece != ' ') {
        if (isWhiteTurn && isupper(destPiece)) {
            printf("Posição de destino ocupada por sua própria peça.\n");
            return 0;
        }
        if (!isWhiteTurn && islower(destPiece)) {
            printf("Posição de destino ocupada por sua própria peça.\n");
            return 0;
        }
    }
    
    // Executa o movimento
    board[destRow][destCol] = piece;
    board[srcRow][srcCol] = ' ';
    return 1;
}

int main() {
    initBoard();
    int isWhiteTurn = 1; // Variável para controlar a vez (1: branco, 0: preto)
    char move[10];
    
    while (1) {
        printBoard();
        printf("Vez do %s. Digite o movimento (ex: e2 e4) ou 'sair' para terminar: ", isWhiteTurn ? "branco" : "preto");
        fgets(move, sizeof(move), stdin);
        move[strcspn(move, "\n")] = '\0'; // Remove a quebra de linha
        
        if (strcmp(move, "sair") == 0) {
            break;
        }
        
        // Espera o movimento no formato "origem destino"
        char src[3], dest[3];
        if (sscanf(move, "%2s %2s", src, dest) != 2) {
            printf("Movimento inválido. Tente novamente.\n");
            continue;
        }
        
        int srcRow, srcCol, destRow, destCol;
        if (!parsePosition(src, &srcRow, &srcCol) || !parsePosition(dest, &destRow, &destCol)) {
            printf("Posição inválida. Utilize notação como e2 e e4.\n");
            continue;
        }
        
        if (movePiece(srcRow, srcCol, destRow, destCol, isWhiteTurn)) {
            // Alterna a vez do jogador
            isWhiteTurn = !isWhiteTurn;
        } else {
            printf("Movimento não efetuado. Tente novamente.\n");
        }
    }
    
    printf("Jogo encerrado.\n");
    return 0;
}
