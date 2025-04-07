#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

struct Board {
    char _Board[3][3];
};

struct Player {
    char Name[15];
    char Piece;
    uint8_t Wins;
    uint8_t Loss;
};

struct Position {
    uint8_t X;
    uint8_t Y;
};

struct Game {
    struct Board Board;
    struct Player Player1, Player2;
    struct Position Position;

    uint8_t Round;
    uint8_t Moves;
    uint8_t Wins;
    uint8_t Ties;
    uint8_t Loss;
    uint8_t MAX_MOVES;

    bool IsPlayer1Turn;
    bool IsPlayer2Turn;
    bool SomeoneWon;
    bool EndRound;
    bool EndGame;

};

void printBoard(struct Board board);
void cleanBoard(struct Board *board); 
void getData(struct Player *player1, struct Player *player2); 
void initRound(struct Game *game);
void insertPiece(struct Game *game);
struct Player checkIfSomeoneWon(struct Game *game); 

int main() {
    
    struct Board board;
    struct Player player1, player2;
    struct Position pos;

    struct Game game = {
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
        .IsPlayer1Turn = true,
        .IsPlayer2Turn = false,
        .SomeoneWon = false,
        .EndRound = false,
        .EndGame = false
    };

    /*
    getData(&game.Player1, &game.Player2);
    initRound(&game);

    while (!game.endGame) {
        insertPiece(&game);
        printBoard(game.Board);

    };*/

    return 0;
}

struct Player checkIfSomeoneWon(struct Game *game) {

    struct Player Player;

    uint8_t winningSequences[8][3][2] = {
        { {0, 0}, {0, 1}, {0, 2} },
        { {1, 0}, {1, 1}, {1, 2} },
        { {2, 0}, {2, 1}, {2, 2} },
        { {0, 0}, {1, 0}, {2, 0} },
        { {0, 1}, {1, 1}, {1, 2} },
        { {0, 2}, {1, 2}, {2, 2} },
        { {0, 0}, {1, 1}, {2, 2} },
        { {0, 2}, {1, 1}, {2, 0} }
    };

    uint8_t winningSequencesNum = 8;
    uint8_t sequencePositions = 3;

    uint8_t player1CountPieces;
    uint8_t player2CountPieces;

    uint8_t posX, posY;

    for (uint8_t winningsequence = 0; winningsequence < winningSequencesNum; winningsequence++) {
        player1CountPieces = 0;
        player2CountPieces = 0;
        for (uint8_t position = 0; position < sequencePositions; position++) {
            posX = winningSequences[winningsequence][position][0];
            posY = winningSequences[winningsequence][position][1];
            if (game->Board._Board[posX][posY] == game->Player1.Piece) {
                player1CountPieces++;
            } else {
                player2CountPieces++;
            }
        }
        if (player1CountPieces == 3) {
            game->Player1.Wins++;
            game->Wins++;
            game->SomeoneWon = true;
            Player = game->Player1;
        }
        if (player2CountPieces == 3) {
            game->Player2.Wins++;
            game->Wins++;
            game->SomeoneWon = true;
            Player = game->Player2;
        }
    }

    return Player;
}

void insertPiece(struct Game *game) {
    if (game->IsPlayer1Turn) {
        printf("Your turn, Player 1.\n");
    } else {
        printf("Your turn, Player 2.\n");
    }

    printf("Put a piece on the table. Choose (X Y): ");
    scanf("%hhu %hhu", &game->Position.X, &game->Position.Y);

    if ( game->Position.X < 1 || game->Position.X > 3 || game->Position.Y < 1 || game->Position.Y > 3 ) {
        printf("Position not allowed. Try again.\n");
        return;
    }
    if (game->Board._Board[game->Position.X - 1][game->Position.Y - 1] != ' ') {
        printf("This Position already has a piece. Choose another place.\n");
        return;
    }

    if (game->IsPlayer1Turn) {
        game->Board._Board[game->Position.X - 1][game->Position.Y - 1] = game->Player1.Piece;
        game->IsPlayer1Turn = false;
        game->IsPlayer2Turn = true;
    } else {
        game->Board._Board[game->Position.X - 1][game->Position.Y - 1] = game->Player2.Piece;
        game->IsPlayer1Turn = true;
        game->IsPlayer2Turn = false;
    }

    game->Moves++;
}

void initRound(struct Game *game) {
    game->EndRound = false;
    cleanBoard(&game->Board);
    
    printf("Loading game...\n");
    sleep(5);

    printBoard(game->Board);

    game->Round++;
}

void getData(struct Player *player1, struct Player *player2) {
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

void cleanBoard(struct Board *board) {
    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            board->_Board[i][j] = ' ';
        }
    }
}

void printBoard(struct Board board) {
    system("clear");
    printf("|----------------------------|\n");
    printf("|----- TIC-TAC-TOE GAME -----|\n");
    printf("|----------------------------|\n");

    for (uint8_t i=0; i<3; i++){
        if (i > 0) {
            printf("|         ---|---|---        |\n");
        }
        printf("|         ");
        for (int8_t j=0; j<3; j++) {
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