//
//  Player.h
//  AI_Checkers_fa961
//
//  Created by Farhad Ahmed on 5/2/18.
//  Copyright © 2018 Farhad Ahmed. All rights reserved.
//

#ifndef Player_h
#define Player_h

#include <iostream>
#include <vector>
#include <utility>
#include <map>


#include "Checkers.h"


using namespace std;

class Player{
private:
    //map
    //key is current_pos, value is list of possible moves for that piece
    //possibleMoves
    map<pair<int, int>, vector<pair<int, int> > > possibleMoves;
    map<pair<int, int>, vector<pair<int, int> > > possiblejumpMoves;
    int piecesLeft;
    Checkers Game;
    friend class AIplayer;
    
public:
    Player(Checkers& G){
        Game = G;
        piecesLeft = G.getNumofPiecesPlayer(); // 6 pieces when initialized aka when game starts
        
        updatePossibleMoves();
        updatePossibleJumps();
    }
    
    Checkers& choosePiece(){  // takes in coordinates of piece to move
        updatePossibleMoves();
        updatePossibleJumps();
        
       
        
        piecesLeft = Game.PlayerPieces.size();
        cout<< "PLAYER PIECES LEFT: " << piecesLeft << endl;
        
        int numberofJumps = getnumofJumps(); // 0  return means that there are no possible jumps in current state
        int numberofMoves = getnumofMoves();
        cout << numberofMoves << " moves possible!" << endl;
        
        if (Game.getNumofPiecesAi() == 0) {
            return Game;
        }
        
        //Game.printPlayerLocations();
        //printALLMoves();
        
        printPlayerLocations();
        
        
        if (numberofJumps != 0) {
            //force some jumping
            cout << numberofJumps << " jump(s) possible!\n" << endl;
            
            cout<< "gotta do sum jumpin' now huh?\n" << endl;
            
            cout<< "Which jump would you like to make?\n" <<endl;
            printALLJumps();
            
            int jumptoMake;
            pair<int, int> pieceToMakeJump (-1,-1);
            cin >> jumptoMake;
            bool stillchoosingjump = true;
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
            Checkers& updatedGame = makeJumpPlayer(pieceToMakeJump);
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
        
        Checkers& updatedGame = Game.makeMovePlayer(pieceCoords, locationCoords);
        
        Game = updatedGame;
        
        
        map<pair<int, int>, vector<pair<int, int> > >::iterator mapIterator;
        pair<int, int> oldpieceLocation (pieceCoords.first, pieceCoords.second);
        mapIterator = possibleMoves.find(oldpieceLocation);                         // removing old piece location from map
        possibleMoves.erase(mapIterator);
        
        
        //Game.printBoard();

        //Game.printPlayerLocations();
     
        return updatedGame;
        
    }
    
    Checkers& makeJumpPlayer(pair<int, int> pieceToMakeJump){
        if (pieceToMakeJump.first == -1 && pieceToMakeJump.second == -1) { //some error, leave immediately
            return Game;
        }
        cout << "Making jump from ";
        map<pair<int, int>, vector<pair<int, int> > >::iterator mapIterator;
        int i = 0;
        for (mapIterator = possiblejumpMoves.begin(); mapIterator != possiblejumpMoves.end(); mapIterator++, i++)
        { // advancing iterator to find element
            if (mapIterator->first.first == pieceToMakeJump.first && mapIterator->first.second == pieceToMakeJump.second) {
                break;
            }
            
        }
        // after for loop, we have reached the jump to make, display possible moves
        
        cout << "(" << (mapIterator->first).first << "," << (mapIterator->first).second << ") over ";
        
        cout << "(" << (mapIterator->second)[0].first << "," << (mapIterator->second)[0].second << ")\n";
        
        pair<int, int> playerPiece ((mapIterator->first).first, (mapIterator->first).second);
        pair<int, int> aiPiece ((mapIterator->second)[0].first, (mapIterator->second)[0].second);
        

        Checkers& updatedGame = Game.makeJumpPlayer(playerPiece, aiPiece);
        
        Game = updatedGame;
        
        map<pair<int, int>, vector<pair<int, int> > >::iterator mapIterator2;
        pair<int, int> oldpieceLocation (playerPiece.first, playerPiece.second);
        mapIterator2 = possiblejumpMoves.find(oldpieceLocation);                         // removing jump after jump is made
        possiblejumpMoves.erase(mapIterator2);
        
        mapIterator2 = possibleMoves.find(oldpieceLocation);                             // updating possible moves
        possibleMoves.erase(mapIterator2);
        
        updatePossibleMoves();
        updatePossibleJumps();
        
        
        //Game.printBoard();
        //Game.printPlayerLocations();
        //Game.printAILocations();
        
        
        return updatedGame;
        
    }
    
    void printPlayerLocations(){
        cout<< "\nPlayer Locations: " << endl;
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
        map<pair<int, int>, vector<pair<int, int> > > newpossibleMoves;
        for (int i =0; i<piecesLeft; i++) {
            pair<int, int>  piece (Game.PlayerPieces[i].first, Game.PlayerPieces[i].second);
            newpossibleMoves[piece] = Game.getPossibleMovePlayer(piece);
        }
        possibleMoves = newpossibleMoves;
    }
    
    void updatePossibleJumps(){
        map<pair<int, int>, vector<pair<int, int> > > newpossibleJumpMoves;
        for (int i = 0 ; i < piecesLeft ; i++) {
            //cout<< "("<<Game.PlayerPieces[i].first << ", " <<Game.PlayerPieces[i].second <<") --> ";
            pair<int, int>  piece (Game.PlayerPieces[i].first, Game.PlayerPieces[i].second);
            newpossibleJumpMoves[piece] = Game.getPossibleJumpsPlayer(piece); // update possible jumps
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
    
    
    bool updateBoard(Checkers& updatedGame){
        Game = updatedGame;
        Game.updateAiLocations();
        Game.updatePlayerLocations();
        if (!getnumofMoves()) {
            return false;
        }
        //Game.printBoard();
        return true;
    }
    
    map<pair<int, int>, vector<pair<int, int> > > getPossibleMoves(){return possibleMoves;}
    map<pair<int, int>, vector<pair<int, int> > > getPossibleJumps(){return possiblejumpMoves;}
    
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
            printPlayerLocations();
            //Game.printPlayerLocations();
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
    
    void printJumpPieceCoords(int index){
        map<pair<int, int>, vector<pair<int, int> > >::iterator mapIterator;
        int i = 0;
        for (mapIterator = possibleMoves.begin(); i<index-1; mapIterator++, i++)   // advance iterator till we reach index ex) if option chosen was 1, i will only go to 0th element in map
        { //dont need to do anything, just advancing iterator here
        }
        cout << "(" << (mapIterator->first).first << ", " << (mapIterator->first).second << ")" << endl;
    }
    
    int getnumofJumps(){
        updatePossibleMoves();
        updatePossibleJumps();
        
        int numberofOfJumps = 0;
        map<pair<int, int>, vector<pair<int, int> > >::iterator mapIterator;
        for (mapIterator = possiblejumpMoves.begin(); mapIterator != possiblejumpMoves.end(); mapIterator++)
        {
            if ((mapIterator->second).size() != 0) {
                numberofOfJumps++;
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
    
    void getPossibleJumpMoves(){
        map<pair<int, int>, vector<pair<int, int> > >::iterator mapIterator;
        for (mapIterator = possiblejumpMoves.begin(); mapIterator != possiblejumpMoves.end(); mapIterator++)
        {
            mapIterator->second = Game.getPossibleJumpsPlayer(mapIterator->first);
            cout<< mapIterator->first.first <<", " << mapIterator->first.second << ", --> " << mapIterator->second.size() <<endl;
            
        }
    }
    
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



#endif /* Player_h */
