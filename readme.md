Windows

The base game is implemented in a similar fashion to TicTacToe.cpp, where the size of the grid and win conditions are most different. The size of the grid is 7 by 6 allowing 42 spaces to be played. To get the piece to be placed at the lowest empty column, a for loop is used that checks from bottom up for the next availble slot. Clicking on an empty square in the that column places a piece in the lowest available slot. checkForWinner utilizes different loop bounds for each win condition(horizontal, vertical, diagonal(top left to bottom right, top right to bottom left). 

aiBoardEval uses calculateScore which uses the state string to check for sequences of 4 and based on the pieces in that sequence assigns it a score, favoring the center of the board. 
negamax finds the lowest empty cell in the column, and if its a valid move, checks it. Alpha beta pruning is then used to optimize negamax. 
updateAI uses similar logic to negamax, where the lowest empty cell in the column is found, then if a valid move is found, uses negamax to find the best move and stores it to play.

Added buttons to Application.cpp with options to start the game as 2 player, player 1st vs. AI or AI 1st vs. player.
