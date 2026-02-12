#include "ConnectFour.h"
using namespace std;

ConnectFour::ConnectFour(){
    _grid = new Grid(7, 6);
}

ConnectFour::~ConnectFour(){
    delete _grid;
}

Bit* ConnectFour::PieceForPlayer(const int playerNumber){
    Bit *bit = new Bit();
    bit->LoadTextureFromFile(playerNumber ==  AI_PLAYER ? "yellow.png" : "red.png");
    bit->setOwner(getPlayerAt(playerNumber == AI_PLAYER ? 1 : 0));
    return bit;
}

void ConnectFour::setUpBoard()
{
    setNumberOfPlayers(2);
    _gameOptions.rowX = 7;
    _gameOptions.rowY = 6;
    _grid->initializeSquares(80, "square.png");

    if (_gameOptions.AIPlaying) {
        setAIPlayer(AI_PLAYER);
    }

    startGame();
}

bool ConnectFour::actionForEmptyHolder(BitHolder &holder){
    
    int x = holder.getPosition().x / 80;
    //for each row, bottom up
    for(int y = 5; y >=0; y--){
        //if empty
        ChessSquare *square = _grid->getSquare(x, y);
        ChessSquare *topsquare = _grid->getSquare(x, 0);
        //place piece
        if(square && !square->bit()){
            Bit *bit = PieceForPlayer(getCurrentPlayer()->playerNumber() == 0 ? HUMAN_PLAYER : AI_PLAYER);
            if (bit){
                bit->setPosition(topsquare->getPosition());
                bit->moveTo(square->getPosition());
                square->setBit(bit);
                endTurn();
                return true;
            }   
            return false;
        }
    }
    
    
    return false;
}

    

bool ConnectFour::canBitMoveFrom(Bit &bit, BitHolder &src)
{
    // you can't move anything in connect four
    return false;
}

bool ConnectFour::canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst)
{
    // you can't move anything in connect four
    return false;
}

void ConnectFour::stopGame()
{
    _grid->forEachSquare([](ChessSquare* square, int x, int y) {
        square->destroyBit();
    });
}

Player *ConnectFour::ownerAt(int index) const{
    ChessSquare *square = _grid->getSquare(index %7, index / 7);
    if (!square || !square->bit()) {
        return nullptr;
    }
    return square->bit()->getOwner();
}

Player *ConnectFour::checkForWinner(){
    
    for (int y = 0; y < 6 ; ++y){
        for (int x = 0; x < 4; ++x){
            Player *winner = ownerAt(y * 7 + x);
            //horizontal
            if (winner && winner == ownerAt(y * 7 + x + 1) && 
                          winner == ownerAt(y * 7 + x + 2) && 
                          winner == ownerAt(y * 7 + x + 3)) return winner;
            
        }
    }

    //vertical
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 7; ++x) {
            Player *winner = ownerAt(y * 7 + x);
            if (winner && winner == ownerAt((y + 1) * 7 + x) && 
                          winner == ownerAt((y + 2) * 7 + x) && 
                          winner == ownerAt((y + 3) * 7 + x)) return winner;
        }
    }
    //tl to bl
    for (int y = 0; y < 3; ++y) {
        for (int x = 0; x < 4; ++x) {
            Player *winner = ownerAt(y * 7 + x);
            if (winner && winner == ownerAt((y + 1) * 7 + (x + 1)) && 
                          winner == ownerAt((y + 2) * 7 + (x + 2)) && 
                          winner == ownerAt((y + 3) * 7 + (x + 3))) return winner;
        }
    }
    //tr to bl
    for (int y = 0; y < 3; ++y) {
        for (int x = 3; x < 7; ++x) {
            Player *winner = ownerAt(y * 7 + x);
            if (winner && winner == ownerAt((y + 1) * 7 + (x - 1)) && 
                          winner == ownerAt((y + 2) * 7 + (x - 2)) && 
                          winner == ownerAt((y + 3) * 7 + (x - 3))) return winner;
        }
    }
    return nullptr;
}

bool ConnectFour::checkForDraw(){
    bool isDraw = true;
    // check to see if the board is full
    _grid->forEachSquare([&isDraw](ChessSquare* square, int x, int y) {
        if (!square->bit()) {
            isDraw = false;
        }
    });
    return isDraw;
}

std::string ConnectFour::initialStateString(){
    return "000000000000000000000000000000000000000000";
}

std::string ConnectFour::stateString(){
    std::string s = "000000000000000000000000000000000000000000";
    _grid->forEachSquare([&](ChessSquare* square, int x, int y) {
        Bit *bit = square->bit();
        if (bit) {
            s[y * 7 + x] = std::to_string(bit->getOwner()->playerNumber()+1)[0];
        }
    });
    return s;
}

void ConnectFour::setStateString(const std::string &s){
    return;
}


void ConnectFour::updateAI(){
    BitHolder* bestMove = nullptr;
    std::string state = stateString();
    int bestVal = -1000000;

    //for each column, check best move
    for (int i = 0; i < 7; ++i){
        int moveIndex = -1;
        //for each row, bottom up
        for (int y = 5; y >= 0; --y) {
            if (state[y * 7 + i] == '0') {
                moveIndex = y * 7 + i;
                break;
            }
        }
        //if we found a valid move, check it
        if(moveIndex != -1 && state[moveIndex] == '0'){
            //try the move
            state[moveIndex] = '2';
            int newVal = -negamax(state, 6, -1000000, 1000000, HUMAN_PLAYER);
            //backtrack
            state[moveIndex] = '0';
            if (newVal > bestVal){
                bestVal = newVal;
                //store best move
                bestMove = _grid->getSquare(i, moveIndex/ 7);
            }
        }
    }
    if(bestMove) {
        if (actionForEmptyHolder(*bestMove)) {
        }
    }
}

int calculateScore(int row, int col, std::string& state, int rowStep, int colStep){
    int good = 0; //for
    int bad = 0; //against
    int empty = 0; //netural

    //check 4 in a row
    for (int i = 0; i < 4; ++i){
        //calculate the current row and column
        int currRow = row + (i * rowStep);
        int currCol = col + (i * colStep);
        int index = currRow * 7 + currCol;
        char cell = state[index];

        if (cell == '2') good++;
        else if (cell == '1') bad++;
        else empty++;
    }

    int score = 0;
    if(good == 4) {score += 10000;} 
    if (good == 3 && empty == 1){score += 50;}
    if (good == 2 && empty == 2){score += 20;}

    if (bad == 2 && empty == 2){score -= 21;}
    if (bad == 3 && empty == 1){score -= 501;}
    if (bad == 4){score -= 10000;}

    return score;
}

int aiBoardEval(std::string& state){
    int score = 0;
    //bias for center
    for (int y = 0; y < 6; ++y) {
        if (state[y * 7 + 3] == '2') score += 30; 
        else if (state[y * 7 + 3] == '1') score -= 30; 
    }

    //horizontal
    for (int y = 0; y < 6; ++y) {
        for (int x = 0; x < 4; ++x) {
            score += calculateScore(y, x, state, 0, 1); 
        }
    }

    //vertical 
    for (int x = 0; x < 7; ++x) {
        for (int y = 0; y < 3; ++y) {
            score += calculateScore(y, x, state, 1, 0);
        }
    }

    //diagonal
    for (int y = 0; y < 3; ++y) {
        //tl to br
        for (int x = 0; x < 4; ++x) {
            score += calculateScore(y, x, state, 1, 1); 
        }
        //tr to bl
        for (int x = 3; x < 7; ++x) {
            score += calculateScore(y, x, state, 1, -1); 
        }
    }
    
    return score;
}

int ConnectFour::negamax(std::string& state, int depth, int alpha, int beta, int playerColor){
    //check for terminal state or depth limit
    int eval = aiBoardEval(state);
    if(std::abs(eval) >= 100000 || depth == 0){
        return playerColor * eval;
    }
    //draw
    if (state.find('0') == std::string::npos) return 0;

    int bestVal = -1000000;

    for (int i = 0; i < 7; ++i){
        int moveIndex = -1;
        for (int y = 5; y >= 0; --y) {
            if (state[y * 7 + i] == '0') {
                moveIndex = y * 7 + i;
                break;
                }
        }
        
            if(moveIndex != -1 &&state[moveIndex] == '0'){
                state[moveIndex] = playerColor == HUMAN_PLAYER ? '1' : '2';
                //flip playercolor and negate score for opps perspective
                int newVal = -negamax(state, depth - 1, -beta, -alpha, -playerColor);
                //backtrack
                state[moveIndex] = '0';
                if (newVal > bestVal){
                    bestVal = newVal;
                }     
                //alpha beta pruning
                alpha = std::max(alpha, bestVal);
                if (alpha >=beta){
                    break;
                }
            }
    }
    return bestVal;
}