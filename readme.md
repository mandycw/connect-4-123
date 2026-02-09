Windows

The base game is implemented in a similar fashion to TicTacToe.cpp, where the size of the grid and win conditions are most different. 

aiBoardEval uses calculateScore to check for sequences of 4 and based on the pieces in that sequence assigns it a score, favoring the center of the board. 
negamax finds the lowest empty cell in the column, and if its a valid move, checks it. Alpha beta pruning is then used to optimize negamax. 
updateAI uses similar logic to negamax, where the lowest empty cell in the column is found, then if a valid move is found, uses negamax to find the best move and stores it to play.