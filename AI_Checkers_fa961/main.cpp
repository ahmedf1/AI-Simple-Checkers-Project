//
//  main.cpp
//  AI_Checkers_fa961
//
//  Created by Farhad Ahmed on 4/14/18.
//  Copyright © 2018 Farhad Ahmed. All rights reserved.
//

#include <iostream>
#include <vector>
#include <utility>
#include <map>


#include "Checkers.h"
#include "AIPlayer.h"
#include "Player.h"


using namespace std;



struct BoardSquare{

    char color;         // D for Dark, L for Light
    bool pieceOnSquare = false; // True for piece is on current square
    char player;        // N for None, P for Player, A for AI
};

//class Player;
//class AIplayer;





int main(int argc, const char * argv[]) {
    cout << "Wanna play some checkers?" <<endl;
    cout << "(Y/N)" <<endl;
    
    char response;
    cin >> response;
    
    if (response == 'Y' || response == 'y') {
        bool keepPlaying = true;
        
        
        
    
        
        
        
        while (keepPlaying) {
            
            bool currentGame = true;
            
            cout << "\nChoose a difficulty: \n";
            cout << "Easy (E)\n";
            cout << "Medium (M)\n";
            cout << "Hard (H)\n";
            
            cin >> response;
            cout << "\nLet's Play!\n" <<endl;
       
            
            Checkers* newGame = new Checkers();
            Player p1(*newGame);
            AIplayer a1(*newGame);
            
            Checkers& updatedGame = *newGame;
            
            newGame->printBoard();
            
            cout<< "Would you like to go first or second?\n";
            cout << "1 for First" << endl;
            cout << "2 for Second\n" << endl;
            
            int isSecond;
            cin >> isSecond;
            
            if (isSecond == 2) {
                cout << "\nSecond Chosen! CPU will go First!\n"<<endl;
                a1.setOrder(true);
                updatedGame = a1.makeMinMaxMove(response);
                updatedGame.printBoard();
            }
            
            //a1.printALLMoves();
            
            while (currentGame) {
                
                p1.updateBoard(updatedGame);
                updatedGame = p1.choosePiece();
                updatedGame.printBoard();
                a1.updateBoard(updatedGame);
                
                updatedGame = a1.makeMinMaxMove(response);
                updatedGame.printBoard();
                updatedGame.printAILocations();
                currentGame = p1.updateBoard(updatedGame);
                
                
            }
            
            int numofAiPieces = updatedGame.getNumofPiecesAi();
            int numofPlayerPieces = updatedGame.getNumofPiecesPlayer();
            
            if (numofAiPieces == numofPlayerPieces) { // Draw
                cout << "\nITS A DRAW!\n" << endl;
            }
            else if (numofPlayerPieces > numofAiPieces){
                cout << "\nYOU WIN!\n" << endl;
            }
            else{
                cout << "\nCPU WINS!\n" << endl;
            }
            
            
            
            
            
            
            
            
            
            
            //after game is finished
            cout<< "\nGame Over!" <<endl;
            cout<< "Play Again? " <<endl;
            cout<< "(Y/N)" <<endl;
            cin >> response;
            if (response == 'N' || response == 'n') {
                keepPlaying = false;
            }
            
        }
        

    }
    
    cout << "Maybe another time then!" <<endl;


    
    
    //cout<< "Player Locations: ";
    //newGame.printPlayerLocations();
    //cout<< "Opponent Locations: ";
    //newGame.printAILocations();
    

}
