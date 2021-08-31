#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "win.h"
#include "tile.h"
#include "board.h"

Tile getNextTurn(Tile turn)
{
  if (turn == X)
    return O;
  else
    return X;
}

int searched = 0;
int minimax(Tile board[9], Tile turn)
{
  // evaluate position for player (turn)
  Tile winningPlayer = winner(board);
  if (winningPlayer != EMPTY)
    return winningPlayer * turn; // flips loss on opponent to win for player and vice versa

  int move = -1;
  int score = -2; // makes algo prefer losing move over no move
  for (size_t i = 0; i < 9; i++)
  {
    if (board[i] == EMPTY)
    {
      searched++;
      board[i] = turn;

      int currScore = -minimax(board, getNextTurn(turn));
      if (currScore > score)
      {
        score = currScore;
        move = i;
      }

      board[i] = EMPTY;
    }
  }

  if (move == -1)
    return 0;
  return score;
}

void botPlay(Tile board[9], Tile turn)
{
  Tile copy[9];
  memcpy(copy, board, 9 * sizeof(Tile));

  int move = -1;
  int score = -2; // makes algo prefer losing move over no move
  for (size_t i = 0; i < 9; i++)
  {
    if (copy[i] == EMPTY)
    {
      searched++;
      copy[i] = turn;

      int currScore = -minimax(copy, getNextTurn(turn));
      if (currScore > score)
      {
        score = currScore;
        move = i;
      }

      copy[i] = EMPTY;
    }
  }

  if (move != -1)
  {
    board[move] = turn;
    nextTurn();
  }
  else
  {
    // draw
  }

  printf(" score: %i, move: %i, searched: %i ", score, move, searched);
}
