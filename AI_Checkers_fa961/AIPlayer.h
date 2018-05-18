//
//  AIPlayer.h
//  AI_Checkers_fa961
//
//  Created by Farhad Ahmed on 5/2/18.
//  Copyright © 2018 Farhad Ahmed. All rights reserved.
//

#ifndef AIPlayer_h
#define AIPlayer_h

#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <algorithm>

#include "Player.h"

int totalnumberofNodes = 0;

int numberofTimesPrunedMax = 0;
int numberofTimesPrunedMin = 0;

class Checkers;


using namespace std;


Checkers* maxofVector (vector<Checkers*> findMax){
    int max = -99999;
    Checkers* state = nullptr;
    for (int i = 0; i < findMax.size(); i++) {

        if (findMax[i]->getScore() > max) {
            max = findMax[i]->getScore();
            state =  findMax[i];

        }
    }
    return state;
}


int minofVector (vector<int> findMin){
    int min = 99999;
    
    for (int i = 0; i < findMin.size(); i++) {
        
        if (findMin[i] < min) {
            min = findMin[i];
            //cout << min << " -_- Min " ;
        }
        
    }
    //cout << endl;
    return min;
}


class AIplayer{
private:
    //map
    //key is current_pos, value is list of possible moves for that piece
    //possibleMoves
    map<pair<int, int>, vector<pair<int, int> > > possibleMoves;
    map<pair<int, int>, vector<pair<int, int> > > possiblejumpMoves;
    int piecesLeft;
    bool first = false;
    Checkers Game;
    friend class Player;
public:
    AIplayer(Checkers& G){
        Game = G;
        piecesLeft = G.getNumofPiecesAi(); // 6 pieces when initialized aka when game starts
        
        /* REPLACED BY updatePossibleMoves()
        for (int i = 0 ; i < 6 ; i++) {
            possibleMoves[Game.AiPieces[i]] = Game.getPossibleMoveAI(Game.AiPieces[i]);
            possiblejumpMoves[Game.AiPieces[i]] = Game.getPossibleJumpsAI(Game.AiPieces[i]);
        }
        */
        
        updatePossibleMoves();
        updatePossibleJumps();
        
    }
    
    int minmaxTree(Checkers* game, bool myturn, int depth, int min, int max, char difficulty){ // pass in game state with most recent move made
        totalnumberofNodes++; // increase for each one of root's children
        
        Checkers* gameCopy = new Checkers(*game); // make a copy to manipulate
        
        //gameCopy.updateAiLocations();
        //gameCopy.updatePlayerLocations();
        //updatePossibleMoves();
        //updatePossibleJumps();  // still need to do these
        
        
        //cout << "Board at depth: " << depth << '\n' << endl;
        //game.printBoard();
        
        //gameCopy.minMaxDecision();
        
        
        if (myturn) { // max player's turn
            int v = max;
            //cout << "MAX's Depth : " << depth << endl;
            AIplayer* a1 = new AIplayer(*gameCopy);
            vector<int> valuestoCompare;
            map<pair<int, int>, vector<pair<int, int> > > movesInthisState = a1->getPossibleMoves();
            map<pair<int, int>, vector<pair<int, int> > > jumpsInthisState = a1->getPossibleJumps();
            
            int numofJumps = a1->getnumofJumps();
            int numberofMoves = a1->getnumofMoves();
            //cout << numberofMoves << " moves possible!" << endl;
            
            if((numofJumps == 0 && numberofMoves == 0) || (depth == 0)){ // reached a point where a1 cant move
                // check if game has ended
                if (gameCopy->getNumofPiecesAi() > 0 && gameCopy ->getNumofPiecesPlayer() == 0) {
                    return 999; //win
                }
                //gameCopy.printBoard();
                //cout << gameCopy.evaluateAi() << endl;
                if (difficulty == 'E' || difficulty == 'e') {
                    return gameCopy->evaluateAiEasy();
                }
                else if (difficulty == 'M' || difficulty == 'm'){
                    return gameCopy->evaluateAiMedium();
                }
                else{
                    return gameCopy->evaluateAiHard();
                }
                
            }
            else if(numofJumps > 0){ // jumps possible
                // go through jumpsInthisState and do all jumps through recursion
                map<pair<int, int>, vector<pair<int, int> > >::iterator mapIterator1;
                for (mapIterator1 = jumpsInthisState.begin(); mapIterator1 != jumpsInthisState.end(); mapIterator1++) {
                    if (mapIterator1->second.size() != 0) { // possible jumps
                        for (int k = 0; k < mapIterator1->second.size(); k++) {
                            Checkers* nextState = new Checkers(*gameCopy);
                            nextState->makeJumpAI(mapIterator1->first, mapIterator1->second[k]);
                            //nextState->printBoard();
                            int value =minmaxTree(nextState, !myturn, depth-1, min, v, difficulty);
                            
                            if (value < v) {
                                v = value;
                                numberofTimesPrunedMax++;
                            }
                            if (v < min) {
                                
                                return min;
                            }
                            //valuestoCompare.push_back(minmaxTree(nextState, !myturn,depth-1));
                            
                            
                        }
                        
                        
                    }
                }
                
            }
            else{   // if only reg moves are possible
                // go through movesInthisState and do all moves through recursion
                map<pair<int, int>, vector<pair<int, int> > >::iterator mapIterator2;
                for (mapIterator2 = movesInthisState.begin(); mapIterator2 != movesInthisState.end(); mapIterator2++) {
                    if (mapIterator2->second.size() != 0) { // possible moves
                        for (int k = 0; k < mapIterator2->second.size(); k++) {
                            Checkers* nextState = new Checkers(*gameCopy);
                            nextState->makeMoveAI(mapIterator2->first, mapIterator2->second[k]);
                            //nextState->printBoard();
                            int value =minmaxTree(nextState, !myturn, depth-1, min, v, difficulty);
                            //cout << "Score: " << value << '\n' << endl;
                            //nextState->printBoard();
                            if (value < v) {
                                v = value;
                                numberofTimesPrunedMax++;
                            }
                            if (v < min) {
                                
                                return min;
                            }

                            //valuestoCompare.push_back(minmaxTree(nextState, !myturn,depth-1));
                            
                        }
                        
                        
                    }
                }
                
                

            }
            //cout<< "MAX HERE: " << maxofVector(valuestoCompare) <<endl;
            // after getting all values on this level return max;
            //return maxofVector(valuestoCompare);
            //cout << "SOME V VAL: " << v << endl;
            return v;
            

        }
        
        else{       // min player's turn
            int v = min;
            //cout << "MIN's Depth : " << depth << endl;
            
            Player* p1 = new Player(*gameCopy);
            vector<int> valuestoCompare;
            map<pair<int, int>, vector<pair<int, int> > > movesInthisState = p1->getPossibleMoves();
            map<pair<int, int>, vector<pair<int, int> > > jumpsInthisState = p1->getPossibleJumps();
            
            int numofJumps = p1->getnumofJumps();
            int numberofMoves = p1->getnumofMoves();
            
            if((numofJumps == 0 && numberofMoves == 0) || (depth == 0)){ // reached a point where a1 cant move
                // check if game has ended
                if (gameCopy->getNumofPiecesAi() > 0 && gameCopy ->getNumofPiecesPlayer() == 0) {
                    return 999; //win for ai
                }
                if (difficulty == 'E' || difficulty == 'e') {
                    return gameCopy->evaluateAiEasy();
                }
                else if (difficulty == 'M' || difficulty == 'm'){
                    return gameCopy->evaluateAiMedium();
                }
                else{
                    return gameCopy->evaluateAiHard();
                }
            }
            
            else if(numofJumps > 0){ // jumps possible
                // go through jumpsInthisState and do all jumps through recursion
                map<pair<int, int>, vector<pair<int, int> > >::iterator mapIterator1;
                for (mapIterator1 = jumpsInthisState.begin(); mapIterator1 != jumpsInthisState.end(); mapIterator1++) {
                    if (mapIterator1->second.size() != 0) { // possible jumps
                        for (int k = 0; k < mapIterator1->second.size(); k++) {
                            Checkers* nextState = new Checkers(*gameCopy);
                            nextState->makeJumpPlayer(mapIterator1->first, mapIterator1->second[k]);
                            //nextState->printBoard();
                            int value = minmaxTree(nextState, !myturn, depth-1, v, max, difficulty);
                            if (value > v) {
                                v = value;
                                numberofTimesPrunedMin++;
                            }
                            if (v > max) {
                                
                                return max;
                            }
                            
                            
                        }
                        
                        
                    }
                }
                
            }
            else{   // if only reg moves are possible
                // go through movesInthisState and do all moves through recursion
                map<pair<int, int>, vector<pair<int, int> > >::iterator mapIterator2;
                for (mapIterator2 = movesInthisState.begin(); mapIterator2 != movesInthisState.end(); mapIterator2++) {
                    if (mapIterator2->second.size() != 0) { // possible moves
                        for (int k = 0; k < mapIterator2->second.size(); k++) {
                            Checkers* nextState = new Checkers(*gameCopy);
                            nextState->makeMovePlayer(mapIterator2->first, mapIterator2->second[k]);
                            //nextState->printBoard();
                            int value = minmaxTree(nextState, !myturn, depth-1, v, max, difficulty);
                            
                            if (value > v) {
                                v = value;
                                numberofTimesPrunedMin++;
                            }
                            if (v > max) {
                                
                                return max;
                            }
                            //valuestoCompare.push_back(minmaxTree(nextState, !myturn,depth-1));
                        }
                        
                        
                    }
                }
                
                
            }
            //cout<< "MIN HERE: " << minofVector(valuestoCompare) <<endl;
            //return minofVector(valuestoCompare);
            return v;
            
        }
        
        
     
        
        
    }
    
    Checkers& makeMinMaxMove(char difficulty){
        totalnumberofNodes = 1;
         numberofTimesPrunedMax = 0;
         numberofTimesPrunedMin = 0;
        Game.updatePlayerLocations();
        Game.updateAiLocations();
        updatePossibleJumps();
        updatePossibleMoves();

        int max = 999999;
        int min = -999999;
        int depth = 9;
        
        
        // find the state reachable from current state that mactches this minmax value and return it
        // first check jumps then moves
        int numberofJumps = getnumofJumps(); // 0  return means that there are no possible jumps in current state
        int numberofMoves = getnumofMoves();
        vector<Checkers*> valuestoCompare;
        
        if (numberofMoves == 0 && numberofJumps == 0) {
            cout << "No possible moves!" << endl;
            return Game;
        }
        
        if (numberofJumps != 0) {
            // go through possible jumps
            
            map<pair<int, int>, vector<pair<int, int> > >::iterator mapIterator1;
            for (mapIterator1 = possiblejumpMoves.begin(); mapIterator1 != possiblejumpMoves.end(); mapIterator1++) {
                if (mapIterator1->second.size() != 0) { // possible jumps
                    for (int k = 0; k < mapIterator1->second.size(); k++) {
                        Checkers* nextState = new Checkers(Game);
                        
                        
                        nextState->makeJumpAI(mapIterator1->first, mapIterator1->second[k]);
                        
                        
                        int score = minmaxTree(nextState, true, depth, min, max, difficulty);
                        nextState->updateScore(score);
                        //cout << "Score: " << score << endl;
                        
                        valuestoCompare.push_back(nextState);
                    
                        
                    }
                    
                    
                }
            }
        }
        else{ // regular moves
            map<pair<int, int>, vector<pair<int, int> > >::iterator mapIterator1;
            for (mapIterator1 = possibleMoves.begin(); mapIterator1 != possibleMoves.end(); mapIterator1++) {
                if (mapIterator1->second.size() != 0) { // possible jumps
                    for (int k = 0; k < mapIterator1->second.size(); k++) {
                        Checkers* nextState = new Checkers(Game);

                        nextState->makeMoveAI(mapIterator1->first, mapIterator1->second[k]);
                        
                        int score = minmaxTree(nextState, true, depth, min, max, difficulty);
                        nextState->updateScore(score);
       
                        
                        valuestoCompare.push_back(nextState);
             
                    }
                    
                    
                }
            }
        }
        
        Checkers* state = maxofVector(valuestoCompare); // returns max of root's direct children

        state->printBoard();
        cout << "\nMaximum depth reached: " << depth << "\n" << endl;
        cout << "\nTotal Number of Nodes Generated: " << totalnumberofNodes << "\n" << endl;
        cout << "\nTotal Number of Times Pruned Max: " << numberofTimesPrunedMax << "\n" << endl;
        cout << "\nTotal Number of Times Pruned Min: " << numberofTimesPrunedMin << "\n" << endl;
    
        state->printAILocations();
        return *state;
        
        
    }
    
   
    
    
    
    Checkers& choosePiece(){  // takes in coordinates of piece to move
        
        updatePossibleMoves();
        updatePossibleJumps();
        
        //Game.printAILocations();
        piecesLeft = Game.AiPieces.size();
        cout << "AI PIECES LEFT: " << piecesLeft << endl;
        
        int numberofJumps = getnumofJumps(); // 0  return means that there are no possible jumps in current state
        int numberofMoves = getnumofMoves();
        cout << numberofMoves << " moves possible!" << endl;
        
        //cout<< "AI POSSIBLE MOVES:" << endl;
        //printALLMoves();
        //Game.printAILocations();
        
        printAILocations();
        
        if (numberofMoves == 0 && numberofJumps == 0) {
            cout << "No possible moves!" << endl;
            return Game;
        }
        
        
        if (numberofJumps != 0) {
            cout << numberofJumps << " jump(s) possible!\n" << endl;
            
            cout<< "gotta do sum jumpin' now huh?\n" << endl;
            
            cout<< "Which jump would you like to make?\n" <<endl;
            printALLJumps();
            
            int jumptoMake;
            pair<int, int> pieceToMakeJump (-1,-1);
            cin >> jumptoMake;                                          // was taking input for testing purposes
            bool stillchoosingjump = true;                              // instead of asking for input here generate random number in range of number of move choices and do that move
            while (stillchoosingjump) {
                if (jumptoMake > numberofJumps || jumptoMake < 1) {
                    cout << "Please choose an actual jump from the list!" <<endl;
                    cin >> jumptoMake;
                }
                else{
                    cout << "\nMaking jump from: ";
                    
                    getcoordsofJumpChosen(jumptoMake, pieceToMakeJump);
                    cout << '(' <<pieceToMakeJump.first << ", " << pieceToMakeJump.second << ")" <<endl;
                    stillchoosingjump = false;
                }
            }       // got valid choice from input
            // now get to making jump
            Checkers& updatedGame = makeJumpAI(pieceToMakeJump);
            return updatedGame;
            
        }
        
        else{
            cout << "Which piece would you like to move?\n" << endl;
            int piecetoMove;
            cin >> piecetoMove;
            bool stillchoosingPiece = true;
            while (stillchoosingPiece) {
                if (piecetoMove > piecesLeft || piecetoMove < 1) {
                    cout << "Please choose an actual piece from the list!" <<endl;
                    cin >> piecetoMove;
                }
                else{
                    cout << "\nMoving piece at: ";
                    printPieceCoords(piecetoMove);
                    stillchoosingPiece = false;
                }
            }
            //piece to move has been chosen
            //now need to show available legal moves for that piece

            int numberofPossibleMoves = printMovesFromIndex(piecetoMove-1);
            if (numberofPossibleMoves < 0) {    // error was returned
                Checkers& updatedGame = choosePiece();
                return updatedGame;
            }
            else{
                Checkers& updatedGame = chooseMove(numberofPossibleMoves, piecetoMove-1);
                return updatedGame;
            }

        }
        
    }
    
    Checkers& chooseMove(int numberofPossibleMoves, int indexofPiece){
        cout<< "numberofPossibleMoves is: "<<numberofPossibleMoves <<endl;
        cout << "Where would you like to move it?\n" << endl;
        
        int locationNumber;
        cin >> locationNumber;
        
        bool stillchoosingMove = true;
        
        while (stillchoosingMove) {
            if (locationNumber > (numberofPossibleMoves) || locationNumber < 1) {
                cout << "Please choose an actual move from the list!\n" <<endl;
                cin >> locationNumber;
            }
            else{
                stillchoosingMove = false;
            }
        }
        
        cout << "\nMoving ";
        map<pair<int, int>, vector<pair<int, int> > >::iterator mapIterator;
        int i = 0;
        
        for (mapIterator = possibleMoves.begin(); i<indexofPiece; mapIterator++, i++)   // advance iterator till we reach index ex) if option chosen was 1, i will only go to 0th element in map
        { //dont need to do anything, just advancing iterator here
        }
        // after for loop, we have reached the piece to move, display possible moves
        
        cout << "(" << (mapIterator->first).first << "," << (mapIterator->first).second << ") to ";
        
        cout << "(" << (mapIterator->second)[locationNumber-1].first << "," << (mapIterator->second)[locationNumber-1].second << ")\n";
        
        pair<int, int> pieceCoords ((mapIterator->first).first, (mapIterator->first).second);
        pair<int, int> locationCoords ((mapIterator->second)[locationNumber-1].first, (mapIterator->second)[locationNumber-1].second);
        
        //call makeMove Function
        Checkers& updatedGame = makeMove(pieceCoords, locationCoords);
        return updatedGame;
    }
    
    Checkers& makeMove(pair<int, int> pieceCoords, pair<int, int> locationCoords){
        // needs to call some makeMove function inside the CheckerBoard Class
        
        Checkers& updatedGame = Game.makeMoveAI(pieceCoords, locationCoords);
        Game = updatedGame;
        
        /* commented out because it was missing from the player counterpart // will add back if any errors arise
         
        for (int i = 0 ; i < piecesLeft ; i++) {
            possibleMoves[Game.AiPieces[i]] = Game.getPossibleMoveAI(Game.AiPieces[i]);
        }
        */
        
        map<pair<int, int>, vector<pair<int, int> > >::iterator mapIterator;
        pair<int, int> oldpieceLocation (pieceCoords.first, pieceCoords.second);
        mapIterator = possibleMoves.find(oldpieceLocation);                         // removing old piece location from map
        possibleMoves.erase(mapIterator);
        
        Game.printBoard();
        
        Game.printAILocations();
        return updatedGame;
    }
    
    //////////////////////////////////
    // put in makeJumpAI here       //
    //                              //
    //   !!!!!!!!!!!!!!!!!!!!!!     //
    //                              //
    //                              //
    //                              //
    //////////////////////////////////
    
    Checkers& makeJumpAI(pair<int, int> pieceToMakeJump){
        if (pieceToMakeJump.first == -1 && pieceToMakeJump.second == -1) { //some error, leave immediately
            return Game;
        }
        
        cout << "Making jump from ";
        map<pair<int, int>, vector<pair<int, int> > >::iterator mapIterator;
        int i = 0;
        for (mapIterator = possiblejumpMoves.begin(); mapIterator != possiblejumpMoves.end(); mapIterator++, i++)   // advance iterator till we reach index ex) if option chosen was 1, i will only go to 0th element in map
        { //dont need to do anything, just advancing iterator here
            if (mapIterator->first.first == pieceToMakeJump.first && mapIterator->first.second == pieceToMakeJump.second) {
                break;
            }
            
        }
        // after for loop, we have reached the jump to make, display possible moves
        
        cout << "(" << (mapIterator->first).first << "," << (mapIterator->first).second << ") over ";
        
        cout << "(" << (mapIterator->second)[0].first << "," << (mapIterator->second)[0].second << ")\n";
        
        pair<int, int> aiPiece ((mapIterator->first).first, (mapIterator->first).second);
        pair<int, int> playerPiece ((mapIterator->second)[0].first, (mapIterator->second)[0].second);
        

        Checkers& updatedGame = Game.makeJumpAI(aiPiece, playerPiece);
        
        Game = updatedGame;
        
        map<pair<int, int>, vector<pair<int, int> > >::iterator mapIterator2;
        pair<int, int> oldpieceLocation (aiPiece.first, aiPiece.second);
        mapIterator2 = possiblejumpMoves.find(oldpieceLocation);                         // removing jump after jump is made
        possiblejumpMoves.erase(mapIterator2);
        
        mapIterator2 = possibleMoves.find(oldpieceLocation);                             // updating possible moves
        possibleMoves.erase(mapIterator2);
        
        updatePossibleMoves();
        updatePossibleJumps();
        
        
        Game.printBoard();
        Game.printAILocations();
        
        
        return updatedGame;
        
    }

    
    
    void printAILocations(){
        cout<< "\nAI Locations: " << endl;
        int i = 1;
        map<pair<int, int>, vector<pair<int, int> > >::iterator mapIterator;
        for (mapIterator = possibleMoves.begin(); mapIterator != possibleMoves.end(); mapIterator++)
        {
            
            cout << i++ << ". (" <<(mapIterator->first).first << ", " << (mapIterator->first).second << ")"<<endl;
            
        }
        cout<<endl;
        
    }
    
 
    
    
    void updatePossibleMoves(){
        // get new moves from new piece locations
        map<pair<int, int>, vector<pair<int, int> > >  newpossibleMoves;
        for (int i =0; i<Game.AiPieces.size(); i++) {
            pair<int, int>  piece (Game.AiPieces[i].first, Game.AiPieces[i].second);
            newpossibleMoves[piece] = Game.getPossibleMoveAI(piece);
        }
        possibleMoves = newpossibleMoves;
    }
    
    
    /////////////////////////////////////
    // put in updatePossibleJumps here //
    //                                 //
    //  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!  //
    //                                 //
    //                                 //
    //                                 //
    /////////////////////////////////////
    
    
    void updatePossibleJumps(){
        map<pair<int, int>, vector<pair<int, int> > > newpossibleJumpMoves;
        for (int i = 0 ; i < Game.AiPieces.size() ; i++) {
            //cout<< "("<<Game.AiPieces[i].first << ", " <<Game.AiPieces[i].second <<") --> ";
            pair<int, int>  piece (Game.AiPieces[i].first, Game.AiPieces[i].second);
            newpossibleJumpMoves[piece] = Game.getPossibleJumpsAI(piece);         // update possible jumps
            /*
            for (int j =0; j < newpossibleJumpMoves[piece].size(); j++) {
                cout << "(" << newpossibleJumpMoves[piece][j].first << ", " << newpossibleJumpMoves[piece][j].second << ") ";
                
                Game.printLocationContents(newpossibleJumpMoves[piece][j]);
            }
            cout <<endl;
             */
        }
        possiblejumpMoves = newpossibleJumpMoves;
    }
    
    
    
    
    void updateBoard(Checkers& updatedGame){
        Game = updatedGame;
        Game.updateAiLocations();
        Game.updatePlayerLocations();
        //Game.printBoard();
    }
    
    void printState(){
        Game.printBoard();
    }
    
    int printMovesFromIndex(int index){
        
        cout<< "Possible Moves for ";
        map<pair<int, int>, vector<pair<int, int> > >::iterator mapIterator;
        int i = 0;
        for (mapIterator = possibleMoves.begin(); i<index; mapIterator++, i++)   // advance iterator till we reach index ex) if option chosen was 1, i will only go to 0th element in map
        { //dont need to do anything, just advancing iterator here
        }
        // after for loop, we have reached the piece to move, display possible moves
        
        cout << (mapIterator->first).first << ", " << (mapIterator->first).second << " -->\n";
        if ((mapIterator->second).size() == 0) {
            cout << "No moves available for this piece!\n"<<endl;
            cout << "Choose a different piece to move!\n"<<endl;
            Game.printBoard();
            printAILocations();
            //Game.printAILocations(); replaced with above function due to indexing errors
            //choosePiece();  //error looping
            return -1;;
        }
        int j = 0;
        for ( ;j < (mapIterator->second).size(); j++) {
            
            cout << j+1 << ". (" << (mapIterator->second)[j].first << "," << (mapIterator->second)[j].second << ")\n";
        }
        cout <<endl;
        return j;
        
    }
    
    
    void printPieceCoords(int index){
        map<pair<int, int>, vector<pair<int, int> > >::iterator mapIterator;
        int i = 0;
        for (mapIterator = possibleMoves.begin(); i<index-1; mapIterator++, i++)   // advance iterator till we reach index ex) if option chosen was 1, i will only go to 0th element in map
        { //dont need to do anything, just advancing iterator here
        }
        cout << "(" << (mapIterator->first).first << ", " << (mapIterator->first).second << ")" << endl;
    }
    
    
    //////////////////////////////////////////////////////////////
    // put in printJumpPieceCoords here                         //
    //                                                          //
    //  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!                           //
    //                                                          //
    //                                                          //
    //                                                          //
    //////////////////////////////////////////////////////////////
    
    void printJumpPieceCoords(int index){
        map<pair<int, int>, vector<pair<int, int> > >::iterator mapIterator;
        int i = 0;
        for (mapIterator = possibleMoves.begin(); i<index-1; mapIterator++, i++)   // advance iterator till we reach index ex) if option chosen was 1, i will only go to 0th element in map
        { //dont need to do anything, just advancing iterator here
        }
        cout << "(" << (mapIterator->first).first << ", " << (mapIterator->first).second << ")" << endl;
    }
    
    
    //////////////////////////////////////////////////////////////
    // put in getnumofJumps here                                //
    //                                                          //
    //  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!                           //
    //                                                          //
    //                                                          //
    //                                                          //
    //////////////////////////////////////////////////////////////
    
    int getnumofJumps(){
        updatePossibleMoves();
        updatePossibleJumps();
        
        int numberofOfJumps = 0;
        map<pair<int, int>, vector<pair<int, int> > >::iterator mapIterator;
        for (mapIterator = possiblejumpMoves.begin(); mapIterator != possiblejumpMoves.end(); mapIterator++)
        {
            if ((mapIterator->second).size() != 0) {
                numberofOfJumps+= mapIterator->second.size();
            }
        }
        return numberofOfJumps;         // if numberofJumps is 0 at this point, then no jumps possible
        
    }
    
    int getnumofMoves(){
        updatePossibleMoves();
        updatePossibleJumps();
        
        int numberofOfMoves = 0;
        map<pair<int, int>, vector<pair<int, int> > >::iterator mapIterator;
        for (mapIterator = possibleMoves.begin(); mapIterator != possibleMoves.end(); mapIterator++)
        {
            if ((mapIterator->second).size() != 0) {
                numberofOfMoves += mapIterator->second.size();
            }
        }
        return numberofOfMoves;         // if numberofJumps is 0 at this point, then no jumps possible
        
    }
    
    
    //////////////////////////////////////////////////////////////
    // put in getPossibleJumpMoves here                         //
    //                                                          //
    //  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!                           //
    //                                                          //
    //                                                          //
    //                                                          //
    //////////////////////////////////////////////////////////////
    
    void getPossibleJumpMoves(){
        map<pair<int, int>, vector<pair<int, int> > >::iterator mapIterator;
        for (mapIterator = possiblejumpMoves.begin(); mapIterator != possiblejumpMoves.end(); mapIterator++)
        {
            mapIterator->second = Game.getPossibleJumpsAI(mapIterator->first);
            cout<< mapIterator->first.first <<", " << mapIterator->first.second << ", --> " << mapIterator->second.size() <<endl;
            
        }
    }
    
    bool setOrder(bool order){
        first = order;
        return first;
    }
    
    map<pair<int, int>, vector<pair<int, int> > > getPossibleMoves(){return possibleMoves;}
    map<pair<int, int>, vector<pair<int, int> > > getPossibleJumps(){return possiblejumpMoves;}
    
    //////////////////////////////////////////////////////////////
    // put in printALLJumps here                                //
    //                                                          //
    //  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!                           //
    //                                                          //
    //                                                          //
    //                                                          //
    //////////////////////////////////////////////////////////////
    
    void printALLJumps(){
        int numberofOfJumps = 0;
        map<pair<int, int>, vector<pair<int, int> > >::iterator mapIterator;
        for (mapIterator = possiblejumpMoves.begin(); mapIterator != possiblejumpMoves.end(); mapIterator++)
        {
            if ((mapIterator->second).size() != 0) {
                numberofOfJumps++;
                cout << numberofOfJumps << ". ("<< (mapIterator->first).first << ", " << (mapIterator->first).second << ") --> ";
                
                for (int i = 0; i < (mapIterator->second).size(); i++) {
                    cout << "(" << (mapIterator->second)[i].first << "," << (mapIterator->second)[i].second << ") ";
                }
                cout<< endl;
            }
            
            
        }
        cout << endl;
        
    }
    
    //////////////////////////////////////////////////////////////
    // put in getcoordsofJumpChosen here                        //
    //                                                          //
    //  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!                           //
    //                                                          //
    //                                                          //
    //                                                          //
    //////////////////////////////////////////////////////////////
    
    pair<int, int> getcoordsofJumpChosen(int jumpoptionChosen, pair<int, int>& pairtoreturn){
        int counter = 0;
        pair<int, int> coordsofPiece(-1,-1);
        map<pair<int, int>, vector<pair<int, int> > >::iterator mapIterator;
        for (mapIterator = possiblejumpMoves.begin(); mapIterator != possiblejumpMoves.end(); mapIterator++) {      // find ith element with move possible
            if (mapIterator->second.size() != 0) {
                counter++;
            }
            if (counter == jumpoptionChosen) {
                pairtoreturn.first = mapIterator->first.first;
                pairtoreturn.second = mapIterator->first.second;
                return pairtoreturn;
            }
            
        }
        return coordsofPiece;
        
        
    }
    
    void printALLMoves(){
        map<pair<int, int>, vector<pair<int, int> > >::iterator mapIterator;
        for (mapIterator = possibleMoves.begin(); mapIterator != possibleMoves.end(); mapIterator++)
        {
            
            cout << (mapIterator->first).first << ", " << (mapIterator->first).second << " --> ";
            
            for (int i = 0; i < (mapIterator->second).size(); i++) {
                cout << "(" << (mapIterator->second)[i].first << "," << (mapIterator->second)[i].second << ") ";
            }
            cout <<endl;
            
        }
    }
};


#endif /* AIPlayer_h */
