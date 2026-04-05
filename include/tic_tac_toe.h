//
// Created by asgyu on 2026. ápr. 5..
//

#ifndef SNAILSPACE_TIC_TAC_TOE_H
#define SNAILSPACE_TIC_TAC_TOE_H
#include <SDL_stdinc.h>

typedef struct {
    int board[3][3];
    int round;
    int cursorX, cursorY;
    int currentPlayer;
    bool active;
    int winner;
    bool isGameOver;
} TicTacToe;

int check_tic_tac_toe_winner(TicTacToe* ttt);

void update_mita_ai(TicTacToe* ttt, Uint32 currentTime);

void clear_tic_tac_toe(TicTacToe* ttt);

#endif //SNAILSPACE_TIC_TAC_TOE_H