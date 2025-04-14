#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <ctype.h>

#ifdef WINDOWS
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

struct Board {
    char Board[3][3];
};

struct Player {
    uint8_t ID;
    char Name[20];
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
    bool EndGame;
};

void printBoard(struct Game *game);
void cleanBoard(struct Board *board); 
void getValidName(char *name, const char *prompt); 
void getData(struct Player *player1, struct Player *player2); 
void initRound(struct Game *game);
bool readPosition(uint8_t *x, uint8_t *y);
void insertPiece(struct Game *game);
struct Player checkIfSomeoneWon(struct Game *game); 
void printAndUpdateResult(struct Game *game, struct Player winner);
void showPlayersStats(struct Game game);
void showStatsAtEndGame(struct Game game);

int main() {
    
    struct Board board = {
        {
            {' ', ' ', ' '} ,
            {' ', ' ', ' '} ,
            {' ', ' ', ' '} 
        }
    };
    struct Player player1 = {1, "", ' ', 0, 0}; 
    struct Player player2 = {2, "", ' ', 0, 0};
    struct Position pos = {0, 0};

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
        .EndGame = false
    };

    getData(&game.Player1, &game.Player2);

    while (!game.EndGame) {

        struct Player winner = {0, "", ' ', 0, 0};

        initRound(&game);

        while (game.Moves < game.MAX_MOVES && !game.SomeoneWon) {
            printBoard(&game);
            insertPiece(&game);
            winner = checkIfSomeoneWon(&game);
        }

        printAndUpdateResult(&game, winner);

        char choice;
        printf("\nWanna play another round? (y/n): ");
        scanf(" %c", &choice);
        if (choice == 'n') {
            game.EndGame = true;
            printf("\nThanks for playing! Quitting game...\n\n");
            sleep(3);
            showStatsAtEndGame(game);
        }

    }

    return 0;
}

void showStatsAtEndGame(struct Game game) {
    system(CLEAR);

    printf("The game ends with:\n");
    printf("%d Rounds | %d Wins | %d Loss | %d Ties |\n\n", 
        game.Round, game.Wins, game.Loss, game.Ties);

    printf("Most Winner: ");

    if (game.Player1.Wins > game.Player2.Wins) {
        printf("%s.\n\n", game.Player1.Name);
    } else if (game.Player1.Wins < game.Player2.Wins) {
        printf("%s.\n\n", game.Player2.Name);
    } else {
        printf("None! Both players won the same amount of rounds.\n\n");
    }

    showPlayersStats(game);
}

void showPlayersStats(struct Game game) {
    struct Player players[2] = {game.Player1, game.Player2};

    int sizeArrayPlayers = sizeof(players) / sizeof(struct Player);

    printf("-------------------------\n");

    for (int i = 0; i < 2; i++) {
        printf("Player: %s\n", players[i].Name);
        printf("Wins: %d\n", players[i].Wins);
        printf("Loss: %d\n", players[i].Loss);
        printf("\n");
    }

    printf("-------------------------\n");

}

void printAndUpdateResult(struct Game *game, struct Player winner) {
    switch (winner.ID) {
        case 0:
            printf("\nRound %d ended in a tie!\n", game->Round);
            game->Ties++;
            break;
        case 1:
            printf("\n%s won Round %d!", winner.Name, game->Round);
            game->Player1.Wins++;
            game->Wins++;
            break;
        case 2:
            printf("\n%s won Round %d!", winner.Name, game->Round);
            game->Player2.Wins++;
            game->Wins++;
            break;
    }            
}

struct Player checkIfSomeoneWon(struct Game *game) {

    struct Player Player = {0, "", ' ', 0, 0};

    uint8_t winningSequences[8][3][2] = {
        { {0, 0}, {0, 1}, {0, 2} },
        { {1, 0}, {1, 1}, {1, 2} },
        { {2, 0}, {2, 1}, {2, 2} },
        { {0, 0}, {1, 0}, {2, 0} },
        { {0, 1}, {1, 1}, {2, 1} },
        { {0, 2}, {1, 2}, {2, 2} },
        { {0, 0}, {1, 1}, {2, 2} },
        { {0, 2}, {1, 1}, {2, 0} }
    };

    uint8_t winningSequencesNum = 8;
    uint8_t sequencePositions = 3;

    for (uint8_t winningsequence = 0; winningsequence < winningSequencesNum; winningsequence++) {
        uint8_t player1Count = 0, player2Count= 0;

        for (uint8_t position = 0; position < sequencePositions; position++) {
            uint8_t posX = winningSequences[winningsequence][position][0];
            uint8_t posY = winningSequences[winningsequence][position][1];

            char piece = game->Board.Board[posX][posY];

            if (piece == game->Player1.Piece) {
                player1Count++;
            } else if (piece == game->Player2.Piece) {
                player2Count++;
            }
        }
        if (player1Count == 3) {
            game->SomeoneWon = true;
            game->IsPlayer1Turn = true;
            game->IsPlayer2Turn = false;
            return game->Player1;
        } else if (player2Count == 3) {
            game->SomeoneWon = true;
            game->IsPlayer1Turn = false;
            game->IsPlayer2Turn = true;
            return game->Player2;
        }
    }

    return Player;
}

void insertPiece(struct Game *game) {
    if (game->IsPlayer1Turn) {
        printf("Your turn, Player 1. -> %c\n", game->Player1.Piece);
    } else {
        printf("Your turn, Player 2. -> %c\n", game->Player2.Piece);
    }

    if (!readPosition(&game->Position.X, &game->Position.Y)) {
        printf("Invalid input. Please enter two numbers between 1 and 3.\n");
    }

    if (game->Position.X < 1 || game->Position.X > 3 || game->Position.Y < 1 || game->Position.Y > 3) {
        printf("Position not allowed. Try again.\n");
        return;
    }
    if (game->Board.Board[game->Position.X - 1][game->Position.Y - 1] != ' ') {
        printf("This Position already has a piece. Choose another place.\n");
        return;
    }

    if (game->IsPlayer1Turn) {
        game->Board.Board[game->Position.X - 1][game->Position.Y - 1] = game->Player1.Piece;
        game->IsPlayer1Turn = false;
        game->IsPlayer2Turn = true;
    } else {
        game->Board.Board[game->Position.X - 1][game->Position.Y - 1] = game->Player2.Piece;
        game->IsPlayer1Turn = true;
        game->IsPlayer2Turn = false;
    }

    game->Moves++;
}

bool readPosition(uint8_t *x, uint8_t *y) {
    printf("Put a piece on the table. Write in this form -> (Row Column): ");

    if(scanf("%hhu %hhu", x, y) != 2) {
        while (getchar() != '\n');
        return false;
    }
    return true;
}

void initRound(struct Game *game) {
    game->SomeoneWon = false;
    game->Round++;
    game->Moves = 0;
    cleanBoard(&game->Board);
    printf("Loading game...\n");
    sleep(5);
}

void getData(struct Player *player1, struct Player *player2) {
    system(CLEAR);
    
    getValidName(player1->Name, "Player 1 name: ");
    getValidName(player2->Name, "Player 2 name: ");

    printf("\n");

    uint8_t option;

    while (true) {
        printf("Choose the pieces to play.\n");
        printf("1 for 'X'\n2 for 'O'\n");
        printf("Player 1 piece: ");

        if(scanf("%hhu", &option) != 1) {
            while(getchar() != '\n');
            printf("Invalid input. Please enter 1 or 2.\n");
            continue;
        }

        if (option == 1 || option == 2) {
            break;
        }
        printf("Invalid option. Choose 1 or 2 only.\n\n");
    }

    player1->Piece = (option == 1) ? 'X' : 'O';
    player2->Piece = (option == 1) ? 'O' : 'X';

    printf("\n");
    printf("The data is saved.\n");
}

void getValidName(char *name, const char *prompt) {
    while (true) {
        printf("%s", prompt);
        scanf("%19s", name);

        bool isValid = true;
        for (int i = 0; name[i] != '\0'; i++) {
            if (!isalnum(name[i])) {
                isValid = false;
                break;
            }
        }

        if (isValid) break;

        printf("Invalid name. Use only letters and numbers (max 19 characters).\n\n");
    }
}

void cleanBoard(struct Board *board) {
    for (uint8_t i = 0; i < 3; i++) {
        for (uint8_t j = 0; j < 3; j++) {
            board->Board[i][j] = ' ';
        }
    }
}

void printBoard(struct Game *game) {
    system(CLEAR);
    printf("|----------------------------|\n");
    printf("|---------- Round %d ---------|\n", game->Round);
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
                printf(" %c", game->Board.Board[i][j]);
            } else{
                printf(" %c |", game->Board.Board[i][j]);
            }
        }
        printf("         |\n");
    };
    printf("|----------------------------|\n");
    printf("|----- TIC-TAC-TOE GAME -----|\n");
    printf("|----------------------------|\n\n");
}
