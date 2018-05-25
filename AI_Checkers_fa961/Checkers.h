//
//  Checkers.h
//  AI_Checkers_fa961
//
//  Created by Farhad Ahmed on 5/2/18.
//  Copyright © 2018 Farhad Ahmed. All rights reserved.
//

#ifndef Checkers_h
#define Checkers_h

#include <iostream>
#include <vector>
#include <utility>
#include <map>



using namespace std;





class Checkers{
private:
    vector<vector<string> > checkerBoard;
    vector<pair<int, int> > PlayerPieces;
    vector<pair<int, int> > AiPieces;
    friend class Player;
    friend class AIplayer;
    int score;
    int height = 6;
    int width = 6;
public:

    Checkers(){
        vector<vector<string> > Board(6, vector<string>(6, "[E]"));
        checkerBoard = Board;
        //Initializing board
        for (int i =0; i<height; i++) {
            for (int j =0; j<width; j++) {
                if (j%2 == 0 && i%2 ==0) {
                    checkerBoard[i][j] = "[L]";     //Light
                }
                else if (j%2 == 0 && i%2 ==1){
                    checkerBoard[i][j] = "[D]";
                }
                else if (j%2 == 1 && i%2 == 0){
                    checkerBoard[i][j] = "[D]";     //Dark
                }
                else{
                    checkerBoard[i][j] = "[L]";
                }
            }
        }
        
        // initialize player and AI pieces
        for (int i =0; i<height; i++) {
            for (int j =0; j<width; j++) {
                
                if (j%2 == 0 && i%2 ==1 && i < 2){
                    checkerBoard[i][j] = "[A]";                     // AI pieces
                    pair<int,int> aiPiece = make_pair(i, j);
                    AiPieces.push_back(aiPiece);
                    
                }
                if (j%2 == 1 && i%2 == 0 && i < 2){
                    checkerBoard[i][j] = "[A]";
                    pair<int,int> aiPiece = make_pair(i, j);
                    AiPieces.push_back(aiPiece);
                }
                if (j%2 == 0 && i%2 ==1 && i > 3){
                    checkerBoard[i][j] = "[P]";                     // Player Pieces
                    pair<int,int> playerPiece = make_pair(i, j);
                    PlayerPieces.push_back(playerPiece);
                }
                if (j%2 == 1 && i%2 == 0 && i > 3){
                    checkerBoard[i][j] = "[P]";
                    pair<int,int> playerPiece = make_pair(i, j);
                    PlayerPieces.push_back(playerPiece);
                }
            }
        }
    }
    
    Checkers(Checkers& G){                                              // copy constructor
        vector<vector<string> > Board(6, vector<string>(6, "[E]"));
        checkerBoard = Board;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                checkerBoard[i][j] = G.checkerBoard[i][j];
            }
        }
        
        
        updateAiLocations();
        updatePlayerLocations();
        
        
    }
    
    void updateScore(int newScore){ // changes score
        this->score = newScore;
    }
    
    int getScore(){                 // returns score
        return this->score;
    }
    
    vector<pair<int, int> > getPossibleMoveAI(pair<int,int> aiPiece){
        
        vector<pair<int, int> > moves;// possible moves for the piece passed in
        // when moving forward there are only two possible directions
        // left diagonal or right diagonal
        
        pair<int, int> moveLD; // left diagonal
        pair<int, int> moveRD; // right diagonal
        
        moveLD.first = aiPiece.first + 1;
        moveLD.second = aiPiece.second - 1;
        
        moveRD.first = aiPiece.first + 1;
        moveRD.second = aiPiece.second + 1;
        
        if ((0 <= moveLD.first) && (0 <= moveLD.second)) {      // bounds checking
            if (moveLD.first < 6 && moveLD.second < 6) {
                //now need to check what currently occupies this space
                //if dark --> that means empty and possible move
                //if light --> major error, light squares shouldnt even be a possible move
                // if Player Piece --> add to list of possible jumps ***** JUMP MOVES MUST BE TAKEN ***
                
                string whatsinLD = checkerBoard[moveLD.first][moveLD.second];
                
                if(whatsinLD == "[D]"){             // Empty Dark square so we can move there
                    moves.push_back(moveLD);
                }
                
                
            }
            
        }
        
        
        
        if ((0 <= moveRD.first) && (0 <= moveRD.second)) {      // bounds checking
            if (moveRD.first < 6 && moveRD.second < 6 ) {
                string whatsinRD = checkerBoard[moveRD.first][moveRD.second];
                
                if(whatsinRD == "[D]"){                         // Empty Dark square so we can move there
                    moves.push_back(moveRD);
                }
                
            }
        }
        
        return moves;
    }
    
    
    vector<pair<int, int> > getPossibleMovePlayer(pair<int,int> playerPiece){    // ex. (4,1) --> [(3,0), (3,2)]
        
        
        vector<pair<int, int> > moves; // possible moves for the piece passed in
        // when moving forward there are only two possible directions
        // left diagonal or right diagonal
        
        pair<int, int> moveLD; // left diagonal
        pair<int, int> moveRD; // right diagonal
        
        moveLD.first = playerPiece.first - 1;
        moveLD.second = playerPiece.second - 1;
        
        moveRD.first = playerPiece.first - 1;
        moveRD.second = playerPiece.second + 1;
        
        if ((0 <= moveLD.first) && (0 <= moveLD.second)) {      // bounds checking
            if (moveLD.first < 6 && moveLD.second < 6) {
                //now need to check what currently occupies this space
                //if dark --> that means empty and possible move
                //if light --> major error, light squares shouldnt even be a possible move
                // if AI Piece --> add to list of possible jumps ***** JUMP MOVES MUST BE TAKEN ***

                
                string whatsinLD = checkerBoard[moveLD.first][moveLD.second];
                
                if(whatsinLD == "[D]"){
                    moves.push_back(moveLD);
                }
                
                
            }
            
        }
        
        
        
        if ((0 <= moveRD.first) && (0 <= moveRD.second)) {      // bounds checking
            if (moveRD.first < 6 && moveRD.second < 6 ) {
                string whatsinRD = checkerBoard[moveRD.first][moveRD.second];
                
                if(whatsinRD == "[D]"){
                    moves.push_back(moveRD);
                }
                
            }
        }
        
        
        
        return moves;
    }
    
    map<pair<int, int>, vector<pair<int, int> > > getAllMoves(bool AiTurn){   // returns all possible moves for each piece
        map<pair<int, int>, vector<pair<int, int> > > moves;
        if (!AiTurn) {
            // get moves for Player
            for (int i = 0; i < PlayerPieces.size(); i++) {
                moves[PlayerPieces[i]] = getPossibleMovePlayer(PlayerPieces[i]);
            }
        }
        else{
            for (int i = 0; i < AiPieces.size(); i++) {
                moves[AiPieces[i]] = getPossibleMoveAI(AiPieces[i]);
            }
        }
        return moves;
    }
    
    map<pair<int, int>, vector<pair<int, int> > > getAllJumps(bool AiTurn){    // returns all possible jumps for each piece
        map<pair<int, int>, vector<pair<int, int> > > moves;
        if (!AiTurn) {
            // get moves for Player
            for (int i = 0; i < PlayerPieces.size(); i++) {
                moves[PlayerPieces[i]] = getPossibleJumpsPlayer(PlayerPieces[i]);
            }
        }
        else{
            for (int i = 0; i < AiPieces.size(); i++) {
                moves[AiPieces[i]] = getPossibleJumpsAI(AiPieces[i]);
            }
        }
        return moves;
    }
    

    
    
  
    
    vector<pair<int, int> > getPossibleJumpsPlayer(pair<int,int> playerPiece){
        
        
        vector<pair<int, int> > jumps; // possible jumps for the piece passed in
        // when moving forward there are only two possible directions
        // left diagonal or right diagonal
        
        pair<int, int> moveLD; // left diagonal
        pair<int, int> moveRD; // right diagonal
        
        moveLD.first = playerPiece.first - 1;
        moveLD.second = playerPiece.second - 1;
        
        moveRD.first = playerPiece.first - 1;
        moveRD.second = playerPiece.second + 1;
        
        if ((0 <= moveLD.first) && (0 <= moveLD.second)) {      // bounds checking
            if (moveLD.first < 6 && moveLD.second < 6) {
                
                //now need to check what currently occupies this space
                // if AI Piece --> add to list of possible jumps ***** JUMP MOVES MUST BE TAKEN ***
               
                
                string whatsinLD = checkerBoard[moveLD.first][moveLD.second];
                
                if (whatsinLD == "[A]") {
                    if (0 <= (moveLD.first-1) && (0 <= (moveLD.second-1))) {  // checking space of where we are landing after jump
                        if ((moveLD.first-1) < 6 && (moveLD.second-1) < 6) {
                            string whatsinLDLD = checkerBoard[moveLD.first-1][moveLD.second-1]; // left diaganol's LD
                            if(whatsinLDLD == "[D]"){     // LD is an AI piece, check that the LD of that is empty
                                jumps.push_back(moveLD);
                            }
                            
                        }
                    }
                }
                
                
            }
            
        }
        
        if ((0 <= moveRD.first) && (0 <= moveRD.second)) {      // bounds checking
            if (moveRD.first < 6 && moveRD.second < 6 ) {
                string whatsinRD = checkerBoard[moveRD.first][moveRD.second];
                
                if (whatsinRD == "[A]") {
                    if (0 <= (moveRD.first-1) && (0 <= moveRD.second+1)) {  // checking space of where we are landing after jump
                        if ((moveRD.first-1) < 6 && (moveRD.second+1) < 6) {
                            
                            string whatsinRDRD = checkerBoard[moveRD.first-1][moveRD.second+1]; // right diaganol's RD
                            
                            if(whatsinRDRD == "[D]"){     // RD is an AI piece, check that the RD of that is empty
                                jumps.push_back(moveRD);
                            }
                            
                        }
                    }
                }
                
                
                
            }
        }
        
        
        return jumps;
        
        
    }
    
    vector<pair<int, int> > getPossibleJumpsAI(pair<int,int> aiPiece){
        
        
        vector<pair<int, int> > jumps; // possible jumps for the piece passed in
        // when moving forward there are only two possible directions
        // left diagonal or right diagonal
        
        pair<int, int> moveLD; // left diagonal
        pair<int, int> moveRD; // right diagonal
        
        moveLD.first = aiPiece.first + 1;
        moveLD.second = aiPiece.second - 1;
        
        moveRD.first = aiPiece.first + 1;
        moveRD.second = aiPiece.second + 1;
        
        
        if ((0 <= moveLD.first) && (0 <= moveLD.second)) {      // bounds checking
            if (moveLD.first < 6 && moveLD.second < 6) {
                
                //now need to check what currently occupies this space
                // if Player Piece --> add to list of possible jumps ***** JUMP MOVES MUST BE TAKEN ***

                
                string whatsinLD = checkerBoard[moveLD.first][moveLD.second];
           
                if(whatsinLD == "[P]"){
                    if (0 <= (moveLD.first+1) && (0 <= (moveLD.second-1))) { // checking space of where we are landing after jump
                        if ((moveLD.first+1) < 6 && (moveLD.second-1) < 6) {
                            string whatsinLDLD = checkerBoard[moveLD.first+1][moveLD.second-1];
                            if (whatsinLDLD == "[D]") {
                                jumps.push_back(moveLD);
                            }
                        }
                    }
                   
                }
                
                
            }
            
        }
        
        if ((0 <= moveRD.first) && (0 <= moveRD.second)) {      // bounds checking
            if (moveRD.first < 6 && moveRD.second < 6 ) {
                string whatsinRD = checkerBoard[moveRD.first][moveRD.second];
            
                if(whatsinRD == "[P]"){
                    if (0 <= (moveRD.first+1) && (0 <= moveRD.second+1)) {
                        if ((moveRD.first+1) < 6 && (moveRD.second+1) < 6) {
                            string whatsinRDRD = checkerBoard[moveRD.first+1][moveRD.second+1];
                            if (whatsinRDRD == "[D]") {
                                jumps.push_back(moveRD);
                            }
                        }
                    }
                    
                }
                
            }
        }
        
        
        return jumps;
        
        
    }
    
    
    
    Checkers& makeMovePlayer(pair<int, int> PieceCoords, pair<int, int> LocationCoords){   // given coords of piece and legal location to move to, board is adjusted
        //need to update PlayerPieces Vector to reflect the change
        for (int i = 0; i < PlayerPieces.size(); i++) {
            if (PlayerPieces[i].first == PieceCoords.first && PlayerPieces[i].second == PieceCoords.second) {   // found matching vector element
                PlayerPieces[i].first = LocationCoords.first;
                PlayerPieces[i].second = LocationCoords.second;
            }
            
        }
        checkerBoard[PieceCoords.first][PieceCoords.second] = "[D]";
        checkerBoard[LocationCoords.first][LocationCoords.second] = "[P]";
        return *this;
        
        
    }
    
    Checkers& makeMoveAI(pair<int, int> PieceCoords, pair<int, int> LocationCoords){   // given coords of piece and legal location to move to, board is adjusted
        //need to update AiPieces Vector to reflect the change
        for (int i = 0; i < AiPieces.size(); i++) {
            if (AiPieces[i].first == PieceCoords.first && AiPieces[i].second == PieceCoords.second) {   // found matching vector element
                AiPieces[i].first = LocationCoords.first;
                AiPieces[i].second = LocationCoords.second;
            }
            
        }
        
        checkerBoard[PieceCoords.first][PieceCoords.second] = "[D]";
        checkerBoard[LocationCoords.first][LocationCoords.second] = "[A]";
        return *this;
        
    }
    
    Checkers& makeJumpPlayer(pair<int, int> playerPiece, pair<int, int> aiPiece){  // aiPiece is coords of piece to be eaten
        bool LDmove = true; // Left diaganol move or not
        if (aiPiece.second > playerPiece.second) {  // if j(second of pair) of aiPiece is greater, then it's a RD move
            LDmove = false;
        }
        
        if (LDmove) {   // make jump
            for (int i = 0; i < PlayerPieces.size(); i++) {
                if (PlayerPieces[i].first == playerPiece.first && PlayerPieces[i].second == playerPiece.second) {   // found matching vector element
                    PlayerPieces[i].first = aiPiece.first-1;
                    PlayerPieces[i].second = aiPiece.second-1;
                    
                }
            }
            checkerBoard[playerPiece.first][playerPiece.second] = "[D]";        // adjust board
            checkerBoard[aiPiece.first][aiPiece.second] = "[D]";
            checkerBoard[aiPiece.first-1][aiPiece.second-1] = "[P]";
        }
        else{   // RDmove
            for (int i = 0; i < PlayerPieces.size(); i++) {
                if (PlayerPieces[i].first == playerPiece.first && PlayerPieces[i].second == playerPiece.second) {   // found matching vector element
                    PlayerPieces[i].first = aiPiece.first-1;
                    PlayerPieces[i].second = aiPiece.second+1;
                    
                }
                
            }
            checkerBoard[playerPiece.first][playerPiece.second] = "[D]";        // adjust board
            checkerBoard[aiPiece.first][aiPiece.second] = "[D]";
            checkerBoard[aiPiece.first-1][aiPiece.second+1] = "[P]";
            
            
            
        }
        // now go through aiPieces vector and remove piece that was eaten
        // adjust Aiplayer's data members
       
        
        int indexofAipiecetoRemove = 0;
        for (; indexofAipiecetoRemove < AiPieces.size(); indexofAipiecetoRemove++) {
            if (AiPieces[indexofAipiecetoRemove].first == aiPiece.first && AiPieces[indexofAipiecetoRemove].second == aiPiece.second) {
                AiPieces.erase(AiPieces.begin() + indexofAipiecetoRemove);
            }// find index of element that was eaten and erase that element from vector
        }
        
        
        this->updatePlayerLocations();
        this->updateAiLocations();
        
        return *this;
    }
    
    
    Checkers& makeJumpAI(pair<int, int> aiPiece, pair<int, int> playerPiece){
        bool LDmove = true; // Left diaganol move or not
        if (aiPiece.second < playerPiece.second) {  // if j(second of pair) of aiPiece is greater, then it's a RD move
            LDmove = false;
        }
        if (LDmove) {
            for (int i = 0; i < AiPieces.size(); i++) {
                if (AiPieces[i].first == aiPiece.first && AiPieces[i].second == aiPiece.second) {   // found matching element
                    AiPieces[i].first = playerPiece.first+1;
                    AiPieces[i].first = playerPiece.second-1;
                }
            }
            checkerBoard[aiPiece.first][aiPiece.second] = "[D]";  // location we are jumping from
            checkerBoard[playerPiece.first][playerPiece.second] = "[D]"; // location of player piece we are eating
            checkerBoard[playerPiece.first+1][playerPiece.second-1] = "[A]";    // location of landing space
        }
        
        else{   //RDmove
            for (int i = 0; i < AiPieces.size(); i++) {
                if (AiPieces[i].first == aiPiece.first && AiPieces[i].second == aiPiece.second) {
                    AiPieces[i].first = playerPiece.first+1;
                    AiPieces[i].second = playerPiece.second+1;
                }
            }
            checkerBoard[aiPiece.first][aiPiece.second] = "[D]";
            checkerBoard[playerPiece.first][playerPiece.second] = "[D]";
            checkerBoard[playerPiece.first+1][playerPiece.second+1] = "[A]";
        }
        
        // now go through player pieces vector and remove piece that was eaten
        // adjust Player's data members
        
        int indexofPlayerpiecetoRemove = 0;
        for (; indexofPlayerpiecetoRemove < PlayerPieces.size(); indexofPlayerpiecetoRemove++) {
            if (PlayerPieces[indexofPlayerpiecetoRemove].first == playerPiece.first && PlayerPieces[indexofPlayerpiecetoRemove].second == playerPiece.second) {
                PlayerPieces.erase(PlayerPieces.begin() + indexofPlayerpiecetoRemove);
            }
        }
        
        return *this;
    }
    
    int getNumofPiecesAi(){return AiPieces.size();}
    int getNumofPiecesPlayer(){return PlayerPieces.size();}
    
    void printBoard(){
        cout<<"\n    0  1  2  3  4  5 " <<endl;
        for (int i =0; i<height; i++) {
            cout << ' ' << i << ' ';
            for (int j =0; j<width; j++) {
                
                
                
                cout << checkerBoard[i][j];
            }
            cout<<endl;
            
        }
        cout<<'\n'<<endl;
    }
    
    void updatePlayerLocations(){
        vector<pair<int, int> > newLocations;
        for (int i = 0; i<height; i++) {
            for (int j = 0; j < width; j++) {
                if (checkerBoard[i][j] == "[P]") {  // found player piece
                    pair<int, int> piece = make_pair(i, j);
                    newLocations.push_back(piece);
                }
            }
        }
        PlayerPieces = newLocations;
    }
    
    void updateAiLocations(){
        vector<pair<int, int> > newLocations;
        for (int i = 0; i<height; i++) {
            for (int j = 0; j < width; j++) {
                if (checkerBoard[i][j] == "[A]") {  // found Ai piece
                    pair<int, int> piece = make_pair(i, j);
                    newLocations.push_back(piece);
                }
            }
        }
        AiPieces = newLocations;
    }
    
    void printPlayerLocations(){
        
        cout<< "Player Locations: " << endl;
        for (int i = 0; i < PlayerPieces.size(); i++) {
            cout << i+1 << ". (" << PlayerPieces[i].first << ", "<< PlayerPieces[i].second << ") " << endl;
        }
        cout<<endl;
        
    }
    
    void printAILocations(){
        cout<< "AI Locations: " << endl;
        for (int i = 0; i < AiPieces.size(); i++) {
            cout << i+1 << ". (" << AiPieces[i].first << ", "<< AiPieces[i].second << ") " <<endl;
        }
        cout<<endl;
    }
    
    
    void printLocationContents(pair<int, int> coordinates){
        cout << checkerBoard[coordinates.first][coordinates.second]<<endl;
    }
    
    
    int evaluateAiEasy(){
        return (AiPieces.size() - PlayerPieces.size());
    }
    
    int evaluateAiMedium(){
        // 7 score for an Ai Piece that is in the opponent's half
        // 5 score for an Ai Piece that is in the Ai's half
        int total = 0;
        for (int k = 0; k < AiPieces.size(); k++) {
            if (AiPieces[k].first <= 2) {
                total += 5;
            }
            else{
                total += 7;
            }
        }
        
        return total+= evaluateAiEasy();
    }
    
    int evaluateAiHard(){
        // same as medium but add row
        // give extra value to corner pieces and pieces along the edges since they cant be jumped
        int total = 0;
        for (int k = 0; k < AiPieces.size(); k++) {
            if (AiPieces[k].first <= 2) {
                total += 3;
            }
            if (AiPieces[k].first > 2) {
                total += 5;
            }
            
            
          
            if (AiPieces[k].second == 0 || AiPieces[k].second == 5){
                total += 9;
            }
        }
        
        return total+= evaluateAiEasy();
        
    }
};



#endif /* Checkers_h */
