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
int minimax(Tile board[9], Tile turn, unsigned *totalDepthSearched)
{
  // evaluate position for player (turn)
  Tile winningPlayer = winner(board);
  if (winningPlayer != EMPTY)
    return winningPlayer * turn; // flips loss on opponent to win for player and vice versa

  unsigned minDepthSearched = 10;
  int move = -1;
  int score = -2; // makes algo prefer losing move over no move
  for (size_t i = 0; i < 9; i++)
  {
    if (board[i] == EMPTY)
    {
      searched++;
      board[i] = turn;

      unsigned depthSearched = *totalDepthSearched + 1;
      int currScore = -minimax(board, getNextTurn(turn), &depthSearched);
      if (currScore > score || (currScore == score && depthSearched < minDepthSearched))
      {
        score = currScore;
        move = i;
        minDepthSearched = depthSearched;
      }

      board[i] = EMPTY;
    }
  }

  if (move == -1)
    return 0;

  *totalDepthSearched += minDepthSearched;
  return score;
}

void botPlay(Tile board[9], Tile turn)
{
  Tile copy[9];
  memcpy(copy, board, 9 * sizeof(Tile));

  unsigned totalDepthSearched = 0;
  unsigned minDepthSearched = 10;
  int move = -1;
  int score = -2; // makes algo prefer losing move over no move
  for (size_t i = 0; i < 9; i++)
  {
    if (copy[i] == EMPTY)
    {
      searched++;
      copy[i] = turn;

      unsigned depthSearched = totalDepthSearched + 1;
      int currScore = -minimax(copy, getNextTurn(turn), &depthSearched);
      if (currScore > score || (currScore == score && depthSearched < minDepthSearched))
      {
        score = currScore;
        move = i;
        minDepthSearched = depthSearched;
      }

      copy[i] = EMPTY;
    }
  }

  if (move != -1)
  {
    board[move] = turn;
    nextTurn();

    totalDepthSearched += minDepthSearched;
    printf(" score: %i, move: %i, searched: %i, depthOfSeries: %i ", score, move, searched, minDepthSearched);
  }
  else
  {
    // draw
  }
}
