#include <stdlib.h>
#include <stdbool.h>
#include "tile.h"

const bool winningPatterns[8][9] = {
    {[0] = true, [1] = true, [2] = true},
    {[3] = true, [4] = true, [5] = true},
    {[6] = true, [7] = true, [8] = true},
    {[0] = true, [3] = true, [6] = true},
    {[1] = true, [4] = true, [7] = true},
    {[2] = true, [5] = true, [8] = true},
    {[0] = true, [4] = true, [8] = true},
    {[2] = true, [4] = true, [6] = true},
};

bool isWin(Tile board[9], Tile player)
{
  for (size_t i = 0; i < 8; i++)
  {
    int matches = 0;

    for (size_t j = 0; j < 9; j++)
    {
      if (board[j] == player && winningPatterns[j])
      {
        matches++;
      }
    }

    if (matches == 3)
      return true;
  }

  return false;
}