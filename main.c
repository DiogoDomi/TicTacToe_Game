#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

typedef struct Board {
    char _Board[3][3];
} Board;

typedef struct Player {
    char Name[15];
    char Piece;
    uint8_t Win;
    uint8_t Loss;
    uint8_t Tie;

} Player;

typedef struct Position {
    uint8_t X;
    uint8_t Y;
} Position;

typedef struct Game {
    Board Board;
    Player Player1, Player2;
    Position Position;

    uint8_t Round;
    uint8_t Moves;
    uint8_t Wins;
    uint8_t Ties;
    uint8_t Loss;
    uint8_t MAX_MOVES;
    bool isPlayer1Turn;
    bool isPlayer2Turn;
    bool someoneWon;
    bool endRound;
    bool endGame;

} Game;

void getData(Player *player1, Player *player2); 
void printBoard(Board board);
void cleanBoard(Board *board); 
void insertPiece(Game *game);
void initRound(Game *game);

int main() {
    
    Board board;
    Player player1, player2;
    Position pos = {
        .X = 0,
        .Y = 0
    };
    Game game = {
        .Board = board,
        .Player1 = player1,
        .Player2 = player2,
        .Position = pos,
        .Round = 0,
        .Moves = 0,
        .Wins = 0,
        .Ties = 0,
        .Loss = 0,
        .MAX_MOVES = 9,
        .isPlayer1Turn = true,
        .isPlayer2Turn = false,
        .someoneWon = false,
        .endRound = false,
        .endGame = false
    };

    getData(&game.Player1, &game.Player2);
    initRound(&game);

    while (game.endGame) {
        insertPiece(&game);
        printBoard(game.Board);

    };
    return 0;
}

bool checkIfSomeoneWon(Game *game) {
    uint8_t rowWin[] = {{(0,0), (0,1), (0,2)}, {(1,0), (1,1), (1,2)}, {(2,0), (2,1), (2,2)}};
    uint8_t columnWin[] = {{(0,0), (1,0), (2,0)}, {(0,1), (1,1), (2,1)}, {(0,2), (1,2), (2,2)}};
    uint8_t diagonalWin[] = {{(0,0), (1,1), (2,2)}, {(0,2), (1,1), (2,0)}};

    uint8_t sizeOfRow = size_of(rowWin) / sizeof(uint8_t);
    uint8_t sizeOfCol = size_of(columnWin) / sizeof(uint8_t);
    uint8_t sizeOfDiag = size_of(diagonalWin) / sizeof(uint8_t);

    uint8_t player1Moves;
    uint8_t player2Moves;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (game->Board._Board[i][j] == game->Player1.Piece) {
                player1Moves++;
            } else if (game->Board._Board[i][j] == game->Player2.Piece) {
                player2Moves++;
            }
        }
    }


    // TERMINAR ESSA FUNÇÃO DO CARAIO







}

void initRound(Game *game) {
    game->endRound = false;
    cleanBoard(&game->Board);
    
    printf("Loading game...\n");
    sleep(5);

    printBoard(game->Board);

    game->Round++;
}

void insertPiece(Game *game) {
    if (game->isPlayer1Turn) {
        printf("Your turn, Player 1.\n");
    } else {
        printf("Your turn, Player 2.\n");
    }

    printf("Put a piece on the table. Choose (X Y): ");
    scanf("%hhu %hhu", &game->Position.X, &game->Position.Y);

    if ( (game->Position.X < 1 && game->Position.X > 3) || (game->Position.Y < 1 && game->Position.Y > 3) ) {
        printf("Position not allowed. Try again.\n");
        return;
    }
    if (game->Board._Board[game->Position.X - 1][game->Position.Y - 1] != ' ') {
        printf("This Position already has a piece. Choose another place.\n");
        return;
    }

    if (game->isPlayer1Turn) {
        game->Board._Board[game->Position.X - 1][game->Position.Y - 1] = game->Player1.Piece;
        game->isPlayer1Turn = false;
        game->isPlayer2Turn = true;
    } else {
        game->Board._Board[game->Position.X - 1][game->Position.Y - 1] = game->Player2.Piece;
        game->isPlayer1Turn = true;
        game->isPlayer2Turn = false;
    }

    game->Moves++;

}

void cleanBoard(Board *board) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board->_Board[i][j] = ' ';
        }
    }
}

void printBoard(Board board) {
    system("clear");
    printf("|----------------------------|\n");
    printf("|----- TIC-TAC-TOE GAME -----|\n");
    printf("|----------------------------|\n");

    for (int i=0; i<3; i++){
        if (i > 0) {
            printf("|         ---|---|---        |\n");
        }
        printf("|         ");
        for (int j=0; j<3; j++) {
            if (j == 3 - 1) {
                printf(" %c", board._Board[i][j]);
            } else{
                printf(" %c |", board._Board[i][j]);
            }
        }
        printf("         |\n");
    };
    printf("|----------------------------|\n");
    printf("|----- TIC-TAC-TOE GAME -----|\n");
    printf("|----------------------------|\n\n");
}

void getData(Player *player1, Player *player2) {
    printf("Choose the players names.\n");
    printf("Player 1 name: ");
    scanf("%19s", player1->Name);

    printf("Player 2 name: ");
    scanf("%19s", player2->Name);
    printf("\n");

    printf("Choose the pieces to play.\n");
    printf("1 for 'X'\n2 for 'O'\n");
    printf("Player 1 piece: ");

    uint8_t option;
    scanf("%hhu", &option);

    if (option == 1) {
        player1->Piece = 'X';
        player2->Piece = 'O';
    } else {
        player1->Piece = 'O';
        player2->Piece = 'X';
    }
    printf("\n");
    printf("The data is saved.\n");
}