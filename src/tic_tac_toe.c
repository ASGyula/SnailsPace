//
// Created by asgyu on 2026. ápr. 5..
//

#include "tic_tac_toe.h"

#include <SDL_stdinc.h>
#include <stdlib.h>
#include <string.h>

int check_tic_tac_toe_winner(TicTacToe* ttt){
    for(int i = 0; i < 3; i++){
        if(ttt->board[i][0] != 0 && ttt->board[i][0] == ttt->board[i][1] && ttt->board[i][0] == ttt->board[i][2])
            return ttt->board[i][0];
        if(ttt->board[0][i] != 0 && ttt->board[0][i] == ttt->board[1][i] && ttt->board[0][i] == ttt->board[2][i])
            return ttt->board[0][i];
    }

    if(ttt->board[0][0] != 0 && ttt->board[0][0] == ttt->board[1][1] && ttt->board[0][0] == ttt->board[2][2])
        return ttt->board[0][0];
    if(ttt->board[0][2] != 0 && ttt->board[0][2] == ttt->board[1][1] && ttt->board[0][2] == ttt->board[2][0])
        return ttt->board[0][2];

    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
            if(ttt->board[i][j] == 0) return 0;

    return 3;
}

void update_mita_ai(TicTacToe* ttt, Uint32 currentTime){
    if (ttt->currentPlayer != 2 || ttt->isGameOver) return;

    static Uint32 mitaDecisionTime = 0;
    if(mitaDecisionTime == 0){
        mitaDecisionTime = currentTime + 1200;
        return;
    }

    if(currentTime < mitaDecisionTime) return;

    int moveX = -1, moveY = -1;

    int emptyCells[9][2];
    int count = 0;
    for(int i=0; i<3; i++) {
        for(int j=0; j<3; j++) {
            if(ttt->board[i][j] == 0) {
                emptyCells[count][0] = i;
                emptyCells[count][1] = j;
                count++;
            }
        }
    }

    if(count > 0) {
        int r = rand() % count;
        ttt->board[emptyCells[r][0]][emptyCells[r][1]] = 2;
    }

    ttt->currentPlayer = 1;
    mitaDecisionTime = 0;

    int res = check_tic_tac_toe_winner(ttt);
    if(res != 0){
        ttt->winner = res;
        ttt->isGameOver = true;
    }
}

void clear_tic_tac_toe(TicTacToe* ttt){
    int currentRound = ttt->round;
    memset(ttt->board, 0, sizeof(ttt->board));

    ttt->round = currentRound + 1;
    ttt->round++;
    ttt->cursorX = 1;
    ttt->cursorY = 1;
    ttt->currentPlayer = 1;
    ttt->winner = 0;
    ttt->isGameOver = false;
}