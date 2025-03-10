#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512
#define TILE_SIZE (SCREEN_WIDTH / 8)

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *boardTexture = NULL;
SDL_Texture *piecesTexture = NULL;

// Matriz de peças representada por caracteres (simplificado)
char board[8][8] = {
    {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
};

// Coordenadas da peça selecionada (-1 = nenhuma)
int selectedX = -1, selectedY = -1;

// Inicializa SDL2
int initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erro ao inicializar SDL: %s\n", SDL_GetError());
        return 0;
    }
    window = SDL_CreateWindow("Jogo de Xadrez", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Erro ao criar janela: %s\n", SDL_GetError());
        return 0;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erro ao criar renderer: %s\n", SDL_GetError());
        return 0;
    }
    return 1;
}

// Carrega imagens do tabuleiro e das peças
int loadTextures() {
    SDL_Surface *boardSurface = IMG_Load("board.png");
    if (!boardSurface) {
        printf("Erro ao carregar board.png: %s\n", IMG_GetError());
        return 0;
    }
    boardTexture = SDL_CreateTextureFromSurface(renderer, boardSurface);
    SDL_FreeSurface(boardSurface);

    SDL_Surface *piecesSurface = IMG_Load("pieces.png");
    if (!piecesSurface) {
        printf("Erro ao carregar pieces.png: %s\n", IMG_GetError());
        return 0;
    }
    piecesTexture = SDL_CreateTextureFromSurface(renderer, piecesSurface);
    SDL_FreeSurface(piecesSurface);

    return 1;
}

// Desenha o tabuleiro e as peças na tela
void drawBoard() {
    SDL_RenderCopy(renderer, boardTexture, NULL, NULL);

    // Tamanho de cada peça na imagem (assumindo uma spritesheet 8x2)
    int pieceSize = 64;

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            char piece = board[y][x];
            if (piece != ' ') {
                SDL_Rect srcRect = { 0, 0, pieceSize, pieceSize };
                SDL_Rect destRect = { x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE };

                switch (piece) {
                    case 'P': srcRect.x = 0; srcRect.y = 0; break;
                    case 'p': srcRect.x = 0; srcRect.y = pieceSize; break;
                    case 'R': srcRect.x = pieceSize; srcRect.y = 0; break;
                    case 'r': srcRect.x = pieceSize; srcRect.y = pieceSize; break;
                    case 'N': srcRect.x = 2 * pieceSize; srcRect.y = 0; break;
                    case 'n': srcRect.x = 2 * pieceSize; srcRect.y = pieceSize; break;
                    case 'B': srcRect.x = 3 * pieceSize; srcRect.y = 0; break;
                    case 'b': srcRect.x = 3 * pieceSize; srcRect.y = pieceSize; break;
                    case 'Q': srcRect.x = 4 * pieceSize; srcRect.y = 0; break;
                    case 'q': srcRect.x = 4 * pieceSize; srcRect.y = pieceSize; break;
                    case 'K': srcRect.x = 5 * pieceSize; srcRect.y = 0; break;
                    case 'k': srcRect.x = 5 * pieceSize; srcRect.y = pieceSize; break;
                }
                SDL_RenderCopy(renderer, piecesTexture, &srcRect, &destRect);
            }
        }
    }
    SDL_RenderPresent(renderer);
}

// Manipula entrada do mouse
void handleMouseClick(int mouseX, int mouseY) {
    int col = mouseX / TILE_SIZE;
    int row = mouseY / TILE_SIZE;

    if (selectedX == -1) {
        // Selecionar peça
        if (board[row][col] != ' ') {
            selectedX = col;
            selectedY = row;
        }
    } else {
        // Mover peça para novo local
        board[row][col] = board[selectedY][selectedX];
        board[selectedY][selectedX] = ' ';
        selectedX = -1;
        selectedY = -1;
    }
}

// Loop principal do jogo
void gameLoop() {
    int running = 1;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = 0;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                handleMouseClick(e.button.x, e.button.y);
            }
        }
        drawBoard();
        SDL_Delay(100);
    }
}

// Libera recursos
void closeSDL() {
    SDL_DestroyTexture(boardTexture);
    SDL_DestroyTexture(piecesTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Função principal
int main() {
    if (!initSDL()) return 1;
    if (!loadTextures()) return 1;

    gameLoop();
    closeSDL();

    return 0;
}
