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
        //place piece
        if(square && !square->bit()){
            Bit *bit = PieceForPlayer(getCurrentPlayer()->playerNumber() == 0 ? HUMAN_PLAYER : AI_PLAYER);
            if (bit){
                bit->setPosition(square->getPosition());
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
    //check horizontal
    for (int y = 0; y < 6 ; ++y){
        for (int x = 0; x < 4; ++x){
            Player *winner = ownerAt(y * 7 + x);
            //add to x
            if (winner && winner == ownerAt(y * 7 + x + 1) && winner == ownerAt(y * 7 + x + 2) && winner == ownerAt(y * 7 + x + 3)){
                return winner;
            }
        }
    }
    //check vertical
    for (int x = 0; x < 7 ; ++x){
        for (int y = 0; y < 3; ++y){
            Player *winner = ownerAt(y * 7 + x);
            //add to y
            if (winner && winner == ownerAt((y + 1)* 7 + x) && winner == ownerAt((y + 2)* 7 + x) && winner == ownerAt((y + 3) * 7 + x)){
                return winner;
            }
        }
    }

    //check diagonal
    //top left to bot right
    //y < 3 and x < 4 to not go out of bounds
    for (int y = 0; y < 3; ++y){
        for (int x = 0; x < 4; ++x){
            Player *winner = ownerAt(y * 7 + x);
            if (winner && winner == ownerAt((y + 1) * 7 + (x + 1)) && winner == ownerAt((y + 2) * 7 + (x + 2)) && winner == ownerAt((y + 3) * 7 + (x + 3))){
                return winner;
            }
        }
    }

    //top right to bot left
    for (int y = 0; y < 3; ++y){
        for(int x = 3; x < 7; ++x){
            Player *winner = ownerAt(y * 7 + x);
            if (winner && winner == ownerAt((y + 1) * 7 + (x-1)) && winner == ownerAt((y + 2) * 7 + (x-2)) && winner == ownerAt((y + 3) * 7 + (x-3))){
                return winner;
            }
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
    _grid->forEachSquare([&](ChessSquare* square, int x, int y) {
        int index = y * 7 + x;
        int playerNumber = s[index] - '0';
        if (playerNumber) {
            square->setBit(PieceForPlayer(playerNumber - 1) );
        } else {
            square->setBit(nullptr);
        }
    });
    
}

void ConnectFour::updateAI(){
    
}