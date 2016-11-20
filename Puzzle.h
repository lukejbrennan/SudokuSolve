#ifndef Puzzle_H
#define Puzzle_H

#include <iostream>
using namespace std;

#include <vector>
#include <cstdlib>
#include <fstream>
#define VICTORY 1
#define VALID_MOVE 1

template <typename T>
class Puzzle{
										
	public:
		//default constructor (reads in the file sets values for puzzle
		Puzzle(); 
	
		//play the game (display board, ask for a move, call moveCheck(), call winCheck() )
		void playSudoku();

		//Determine if an entry is valid or not on a board		
		//VALID_MOVE is returned if move is acceptable and valid
		int moveCheck(int row_in, int col_in, int numChoice);

		//Determine if the player has won the game yet. 
		//VICTORY is output if the player has won
		int winCheck(); 

		//display puzzle
		void printPuzzle();

		//update board after number was placed
		void updateBoard(int row, int col); 

		//Solve simple 9x9 Sudoku board of integers
		int simpleSolve(); // returns 1 if it is adding numbers, 0 otherwise

		//Prints possible numbers in possibles (third dim of possibles)
		void printPossibles();
		void printPossible(int row, int col); //not overloaded, but should be... pressed for time

		//solves medium 9x9 Sudoku board of integers
		void singleton();

		//synergizes simpleSolve and singleton
		void synSolve();

	private:
		vector<vector <T> > puzzle; // stores numbers in puzzle
		vector<	vector<vector <T> > > possibles; // stores possible values to go in a particular spot on board (3D)
		int size;		

}; //end class Puzzle

template<typename T>
Puzzle<T>::Puzzle(){
	size=9;
	string fileName;
	vector<T> tempVec;
	T value; //char or int (for this program)
	vector<T> d1;
	vector<vector <T> > d2; 

	//Readin file to fileName
	cout << "Please give a file name: ";
	cin >> fileName;
	ifstream myReadFile;
	myReadFile.open(fileName.c_str());

	//obtain values of fileName until EOF
	while(!myReadFile.eof()){
		for(int row=0; row<size; row++){ 
			for(int col=0; col<size; col++){ 
				myReadFile >> value; //readin one value at a time
				if(value == ' ') continue; //if there's a space in the file, don't put it into the puzzle
				else tempVec.push_back(value); //readin values to the rowVector, tempVec				
			}
			puzzle.push_back(tempVec); //pushes each column into the puzzle
			tempVec.clear(); // erases the row in preparation for the new row
		}
	}
	
	//initialize possibility vector
	for(int possNumPosition=0; possNumPosition<9; possNumPosition++){ 	//position in third dimension of possibles (goes from [0-8] ) 
		for(int row=0; row<size; row++){	
			for(int col=0; col<size; col++){
				d1.push_back(col); // initialize a column
			}	
			d2.push_back(d1); // pushes the column into the board
		}
		possibles.push_back(d2);
	}

	// Store 1-9 in all positions of possibles (make every number a possible
	//number to place at every positon on the board)
	for(int possNum=1; possNum<=9; possNum++){ 	//value in third dimension of possibles (goes from [1-9] )
		for(int row=0; row<size; row++){	
			for(int col=0; col<size; col++){
				possibles[row][col][possNum-1]=possNum; //stores 1 as a possible number in possibles[row][col][0]
			}
		}
	}
	
	//Elimate numbers that are not possible (based on unaltered board) from possibles
	for(int possNum=1; possNum<=9; possNum++){ 	//value in third dimension of possibles (goes from [1-9] )	
		for(int row=0; row<size; row++){	
			for(int col=0; col<size; col++){
				if( moveCheck(row, col, possNum)!= VALID_MOVE){ //check if a possNum can be placed at row, col
					possibles[row][col][possNum-1]=0; //replace non-placable number with a 0 
				}
				if ( puzzle[row][col] != 0 ){ //if a number already exists on the puzzle
					possibles[row][col][possNum-1]=0; //replace all numbers to 0	
				}
			}
		}
	}
}//end default constructor 

template<typename T>
void Puzzle<T>::printPossibles(){

	for(int row=0; row<size; row++){	
		for(int col=0; col<size; col++){
				cout << row+1 << " and " << col+1 << endl;
			for(int i=0; i<9; i++){
				cout << possibles[row][col][i] << ", "; 
			}
			cout << endl; 
		}
	}
}// end printPossibles

template<typename T>
void Puzzle<T>::printPossible(int row, int col){
	for(int i=0; i<9; i++){
				cout << possibles[row][col][i] << ", " ; 
	}
}//end printPossible

template<typename T>
void Puzzle<T>::updateBoard(int rowIN, int colIN){ 
	// check and update the column
	for(int row=0; row<9; row++){
		for(int possNum=1; possNum<=9; possNum++){
			if( moveCheck(row, colIN, possNum)!=VALID_MOVE ){ //check if a possNum can be placed at row, col
					possibles[row][colIN][possNum-1]=0; //replace non-placable number with a 0 
			}
			if ( puzzle[row][colIN] != 0 ){ //if a number already exists on the puzzle
				possibles[row][colIN][possNum-1]=0; //replace all numbers to 0	
			}
		}
	}
	// check and update row
	for(int col=0; col<9; col++){
		for(int possNum=1; possNum<=9; possNum++){
			if( moveCheck(rowIN, col, possNum)!=VALID_MOVE ){ //check if a possNum can be placed at row, col
					possibles[rowIN][col][possNum-1]=0; //replace non-placable number with a 0 
			}
			if ( puzzle[rowIN][col] != 0 ){ //if a number already exists on the puzzle
				possibles[rowIN][col][possNum-1]=0; //replace all numbers to 0	
			}
		}
	}
	//check and update minigrid
		//move rowIN and colIN to the top left hand corner of the current box
		while(rowIN % 3 != 0) rowIN--; 
		while(colIN % 3 !=0) colIN--; 
		//iterate through mini-grid revelant to selected row and col 
		for(int col=colIN; col<colIN+3; col++){
			for(int row=rowIN; row<rowIN+3; row++){
				for( int possNum=1; possNum<=9; possNum++){
					if( moveCheck(rowIN, colIN, possNum)!=VALID_MOVE ){ //check if a possNum can be placed at row, col
						possibles[rowIN][colIN][possNum-1]=0; //replace non-placable number with a 0 
					}
					if ( puzzle[rowIN][colIN] != 0 ){ //if a number already exists on the puzzle
						possibles[rowIN][colIN][possNum-1]=0; //replace all numbers to 0	
					}
				}
			}
		}
} //end updateBoard function

template<typename T>
int Puzzle<T>::simpleSolve(){
	//Initialize Variables
	int count_zeros=0;
	int currentNum; // stores the current possible number to be placed in possibles.
					// only placed into the board when there is only one placable number (eight zeros)
	int	count=0;	
	int addingNums=0; //addingNums=1 if the simpleSolve is adding numbers to the board

	//Iterate through the board, 
	//Look for a vector with only one-non zero value and Place number at that location
	for(int row=0; row<size; row++){	
		for(int col=0; col<size; col++){
			//count number of zeros in the possibles[row][col] 
			for(int possNum=1; possNum<=9; possNum++){ 	//value in third dimension of possibles (goes from [1-9] )	
				if( possibles[row][col][possNum-1] == 0 ){
					count_zeros++; //keeps track of how many zeros are in the third dimension of the possibles vectors
									//if only 1 number were placable, then there'd be 8 zeros and the placable number 							
				}
				else {
					currentNum= possibles[row][col][possNum-1]; //store non-zero entry at [row][col]
				}				
			}
			//cout << "cout_zeros: " << count_zeros << endl;
			if (count_zeros == 8){ //places currentNum in the board
				puzzle[row][col]=currentNum;
				updateBoard(row, col); // updates the row, col and minigrid after a number was added at [row][col]
				addingNums=1;
				return addingNums;
			}	
			count_zeros=0; //reset for iteration through next position on the board	
		}
	}

	return 0; // if no numbers were added, then addingNums wasn't returned, and simpleSolve has stopped solving

}//end of simpleSolve function

template<typename T>
void Puzzle<T>::singleton(){
	vector<int> temp;
	int tempCol;
	// Place singletons in each row (comparing possibles of all columns of one row) 
	for(int row=0; row<9; row++){	
		for(int col=0; col<9; col++){ //only used to construct temp vector
			if(puzzle[row][col] == 0 ){ // only look at entries that need to be filled in
				for(int possNum=1; possNum<=9; possNum++){ ///create temporary vector with only the non-zero entries of possibles		 
					if(possibles[row][col][possNum-1] != 0){
						temp.push_back(possibles[row][col][possNum-1]);
					}
				}
				tempCol=col; // stores the column for which temp stores the possible values
				for(int COL=0; COL<9; COL++){ // determine if we can eliminate values from temp
					if( (puzzle[row][COL]==0)  && (tempCol!=COL)  ){ //comparing temp to the other columns that have to be filled 
																	//make sure we aren't comparing temp to itself
						for( int itemp=0; itemp< temp.size(); itemp++ ){ //loop through all values in temp
							for(int possNum=1; possNum<=9; possNum++){ 
								if(temp[itemp] == possibles[row][COL][possNum-1]){
									temp.erase(temp.begin()+itemp);
								}
							}
						}
					}								
				}
				if (temp.size() == 1){ // after we've deleted what we can from temp, check its size. if it's size=1...
					puzzle[row][col]= temp[0]; //place only value in temp on board
					updateBoard(row, col);
					printPuzzle(); 
					cout << row+1 << "and" << col+1 << ". Added: "<< temp[0] << endl;
				}
				temp.clear(); // clear temp after we have (or have not added the appropriate value to the board
			}
		}
	}
} //end of singleton function

template<typename T>
void Puzzle<T>::synSolve(){
	int winStatus= !VICTORY;
	while(winStatus !=VICTORY){ //while the player has not won
		while( simpleSolve()== 1 ); //continue to rule simpleSolve until it stops adding numbers to the boar
		if(winCheck() == 1) break; 
		singleton(); 		
		winStatus= winCheck(); // check if the game has been won		
	}
}// end of synSolve()

template<typename T>
int Puzzle<T>::moveCheck(int row_in, int col_in, int numChoice){
	vector<T> numHistory; //vector to keep track of duplicated numbers
	
	
	//ENSURE COLUMNS contain no repeat numbers [1-9]	
	numHistory.push_back(numChoice); // add numChoice into history of numbers 
	for(int row=0; row<size; row++){ //iterate through rows
		for(int i=0; i<numHistory.size(); i++){ //iterate through numHistory
			//if there is a match (other than zero) in the current column to numHistory, selection is invalid
			if(puzzle[row][col_in]==numHistory[i] && puzzle[row][col_in]!=0){
				//cout << numChoice << " already exists in the column " << (col_in+1);
				//cout << ". No changes made to the puzzle board" << endl;				
				return !VALID_MOVE;
			} 
			//if there is no match between numHistory and the numbers in current column and if the current position is
			//not zero, then add the value at the position on the board where there is not a match
			else if (puzzle[row][col_in]!=numHistory[i] && puzzle[row][col_in]!=0){  
				numHistory.push_back(puzzle[row][col_in]); 
				break;	//once we've recorded the number that did not match our numHistory, we can move to the next row		
			}
			else break; //this is the case where the current position of puzzle is zero
		}			
	}
	numHistory.clear();

	//ENSURE ROWS contain no repeat numbers [1-9]
	numHistory.push_back(numChoice); // add numChoice into history of numbers 
	for(int col=0; col<size; col++){
		for(int i=0; i<numHistory.size(); i++){
			if(puzzle[row_in][col]==numHistory[i] && puzzle[row_in][col]!=0){
				//cout << numChoice << " already exists in the row " << (row_in+1);
				//cout << ". No changes made to the puzzle board" << endl;				
				return !VALID_MOVE;
			} 		
			//add non-documented numbers to numHistory (if the numbers are not zero)	
			else if (puzzle[row_in][col]!=numHistory[i] && puzzle[row_in][col]!=0){
				numHistory.push_back(puzzle[row_in][col]); 
				break;	//once we've recorded the number that didn't match numHistory, we can move to the next column		
			}
			else break; //this is the case where the current position of puzzle is zero
		}			
	}
	numHistory.clear();

	//ENSURE every mini grid contains no repeat numbers [1-9]
	numHistory.push_back(numChoice); // add numChoice into history of numbers
 
	//move row_in and col_in to the top left hand corner of the current box
	while(row_in % 3 != 0) row_in--; 
	while(col_in % 3 !=0) col_in--; 

	//iterate through mini-grid revelant to selected row and col 
	for(int col=col_in; col<col_in+3; col++){
		for(int row=row_in; row<row_in+3; row++){
			for(int i=0; i<numHistory.size(); i++){ //iterate through numHistory to check for a match
				//if there's a match between numHistory and the value at the current row, col position
				// of the puzzle (and if the match isn't zero!), then output error (can't do that) 
				if(puzzle[row][col]==numHistory[i] && puzzle[row][col]!=0){ 
					//cout << numChoice << " already exists in the current mini-grid. ";
					//cout << "No changes made to the puzzle board" << endl;				
					return !VALID_MOVE;
				}
				//add non-documented numbers to numHistory (if the numbers are not zero)
				else if (puzzle[row][col]!=numHistory[i] && puzzle[row][col]!=0){
					numHistory.push_back(puzzle[row][col]);
					break;			
				}
				else break; //this is the case where the current position of puzzle is zero
			}
		}
	}
	numHistory.clear();
	return VALID_MOVE;
}//end moveCheck function 

template<typename T>
int Puzzle<T>::winCheck(){
	for (int row=0; row<size; row++){
		for (int col=0; col<size; col++){
			if( puzzle[row][col] == 0 ){
				//cout << "Keep trying!" << endl; //shows that the game has not yet been won				
				return !VICTORY; //if board is empty
			}		
		}		 
	}
	// if we didn't hit any 0's as we swept through the board,  
	// then the board is full with valid inputs and therefore the
	// game is won. 
	cout << "Congratulations! You won! Here is your completed puzzle: " << endl; //victory message
	printPuzzle(); 
	return VICTORY;
}//end winCheck function

template<typename T>
void Puzzle<T>::printPuzzle(){
	cout << endl << "New Board:" << endl << endl;
	for(int row=0; row<size; row++){
		for(int col=0; col<size; col++){
			cout << puzzle[row][col] << " ";
		}
		cout << endl; //print new line for next row
	}

}//end printPuzzle function

//play the game (display board, ask for a move, call moveCheck(), call winCheck() )
template<typename T>
void Puzzle<T>::playSudoku(){
	int rowChoice, colChoice, numChoice; //inputs from the user
	int row, col; //row=rowChoice-1 so that the first row=0 (similar with col)	
	int moveValidity= !VALID_MOVE; // These are equal when a valid move is chosen.
	int winStatus= !VICTORY; 	
	while(winStatus !=VICTORY){ //while the player has not won	
		//display board
		printPuzzle();
	
		// ask for a move and keep asking until a valid move has been selected
		while(!moveValidity){ //while move choice is invalid
			//ask for a move
			cout << "Select a row" << endl;
			cin >> rowChoice;
			//Check row input
			if( rowChoice>size || rowChoice <1 ){
				cout << "That's not a row..." << endl;
				moveValidity= !VALID_MOVE;
				break;
			}
			//Check Column input
			cout << "Select a column" << endl;
			cin >> colChoice;
			if( colChoice>size || colChoice <1 ){
				cout << "That's not a column..." << endl;
				moveValidity= !VALID_MOVE;
				break;
			}
			//shift rowChoice and colChoice over by one
			row= rowChoice-1;
			col= colChoice-1;

			//Check position to avoid overwriting (doesn't allow player to erase any past mistakes...)
			if( puzzle[row][col]!=0 ){
				cout << "There's already a number there, and you can't overwrite it!" << endl;
				break;
			}
			//Check number input
			cout << "Select a number [1-9]" << endl;
			cin >> numChoice;	
			if( numChoice>9 || numChoice <1 ){
				cout << "That's not a number between 1 and 9..." << endl;
				moveValidity= !VALID_MOVE;
				break;
			}
			
			
			//call "moveCheck()" which returns VALID_MOVE if the move is valid
			moveValidity=moveCheck(row, col, numChoice); 
		}
		if(moveValidity) puzzle[row][col]= numChoice; // if the move is valid, add it to the board
		updateBoard(row, col);
		moveValidity=!VALID_MOVE; //reset option for the next iteration
		//call "winCheck()"
		winStatus=winCheck();
	}

}//end of playSudoku function


#endif

