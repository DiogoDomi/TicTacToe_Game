#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define LINES 3
#define COLS 3

typedef struct Board {
    char _Board[LINES][COLS];

    void (*printBoard)(struct Board *table);
    void (*cleanBoard)(struct Board *table); 
} Board;

typedef struct Player {
    char *Name;
    char Piece;
    int Win, Loss, Tie;

} Player;

typedef struct Position {
    int X, Y;
} Position;

typedef struct Match {
    Player Player1, Player2;
    Board board;
    int Round, Wins, Ties, Loss;
    bool isFinished;

    void (*insertPiece)(struct Board *board, struct Player *player, struct Position pos);
} Match;

void printBoard(Board *board);
void cleanBoard(Board *board); 
void insertPiece(Board *board, Player *player, Position pos);

int main() {
    return 0;
}

/*
bool checkIfSomeoneWon(Table *table) {
    int rowWin[] = {{(0,0), (0,1), (0,2)}, {(1,0), (1,1), (1,2)}, {(2,0), (2,1), (2,2)}};
    int columnWin[] = {{(0,0), (1,0), (2,0)}, {(0,1), (1,1), (2,1)}, {(0,2), (1,2), (2,2)}};
    int diagonalWin[] = {{(0,0), (1,1), (2,2)}, {(0,2), (1,1), (2,0)}};

    for (int i = 0; i < LINES; i++) {
        for (int j = 0; j < COLS; j++) {

        }
    }
}
*/
void getData(Player *player1, Player *player2) {
    printf("Player 1 name: ");
    scanf("%s", player1->Name);
    printf("\n");

    printf("Player 2 name: ");
    scanf("%s", player2->Name);
    printf("\n");

    printf("Sort the pieces or you wanna choose?\n");
    printf("S for Sort\nC for Choose: ");
    char option;
    scanf("%c", &option);

}

void insertPiece(Board *board, Player *player, Position pos) {
    if ( (pos.X < 1 && pos.X > LINES) || (pos.Y < 1 && pos.Y > COLS) ) {
        printf("Position not allowed. Try again.\n");
        return;
    }
    if (board->_Board[pos.X - 1][pos.Y - 1] != '-') {
        printf("This Position already has a piece. Choose another place.\n");
        return;
    }
    board->_Board[pos.X - 1][pos.Y - 1] = player->Piece;
}

void cleanBoard(Board *board) {
    for (int i = 0; i < LINES; i++) {
        for (int j = 0; j < COLS; j++) {
            board->_Board[i][j] = '-';
        }
    }
}

void printTable(Board *board) {
    printf("|----------------------------|\n");
    printf("|----- TIC-TAC-TOE GAME -----|\n");
    printf("|----------------------------|\n");

    for (int i=0; i<LINES; i++){
        if (i > 0) {
            printf("|         ---|---|---        |\n");
        }
        printf("|         ");
        for (int j=0; j<COLS; j++) {
            if (j == COLS - 1) {
                printf(" %c", board->_Board[i][j]);
            } else{
                printf(" %c |", board->_Board[i][j]);
            }
        }
        printf("         |\n");
    };
    printf("|----------------------------|\n");
    printf("|----- TIC-TAC-TOE GAME -----|\n");
    printf("|----------------------------|\n\n");
}