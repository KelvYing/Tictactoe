#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;
struct Round{  //struct to avoid using global vars
	char a[3][3];	//board array
	string wTurn;	//string showing whose turn it is
	int turnNum;	//number of turns in the current game
	string s;	//string showing the state of the game
};
void printBoard(char a[3][3]){ //prints out the current state of the board
	for(int i =0; i <3; i++)
	{
		cout << '|' << a[i][0] << '|' << a[i][1] << '|' << a[i][2] << '|' << endl;
        } 
}
string checkWin(char a[3][3], int turn){ //check if a player have won yet
	string win = "won";
	for(int i = 0; i<3 ; i ++){//compaires the rows and columns
		if(a[i][0] == a[i][1]&& a[i][0] == a[i][2]&& a[i][1] == a[i][2] && a[i][0] != ' ') return win += a[i][0];
		if(a[0][i] == a[1][i] && a[0][i] == a[2][i]&& a[1][i] == a[2][i] && a[0][i] != ' ') return win += a[0][i];
	}
	if(a[0][0] == a[1][1] && a[0][0] == a[2][2] && a[1][1] == a[2][2] && a[2][2] != ' ') return win += a[0][0];//compaires the diagonals 
	if(a[0][2] == a[1][1] && a[0][2] == a[2][0] && a[1][1] == a[2][0] && a[2][0] != ' ') return win += a[0][2];
	if(turn == 9) return "tie"; //if its been 9 turns and there is no winner, it is a tie
	return "no";	//if passes through all this conditions, no winner and keep going
} 
bool checkErr(string p, char a[3][3], const string wTurn){ //checks if the input contains an error, takes in string to check input, array for the board and char for checking if it is the player's turn
	stringstream inp(p);	//turn the string back to a stream string
	string s;
	inp >> s;
	if(s != "x" && s != "o")return false; //if the first input is not x or o, error
	if(s == wTurn) return false;  //If the first input is the same as the last turn, error
	int c;
	if(!(inp >> c)) return false; //If the second input (column) is not an int, error
	if(c > 2 || c < 0) return false;//if it is an int but not in range, error
	int r;
	if(!(inp >> r)) return false; //same but for third input (row)
	if(r > 2 ||r < 0) return false; 
	if(a[c][r] == 'x' || a[c][r] == 'o') return false; //checks if the tile is already used
	if(inp.rdbuf()->in_avail() != 0) return false;//need to check if the stream is empty, makes sure there is nothing else in the line
	return true; //passes the error checking
}

Round passTurn(const Round stat, string p){
	Round tmpstat = stat;
	if(checkErr(p,tmpstat.a,tmpstat.wTurn)){	//no invalid input if it passes checkErr function
	string s; 
	int c;
	int r;
	stringstream inp(p);
	inp >> s;	//first part of stream is player x or o
	inp >> r;	//second part of stream is row num
	inp >> c;	//third part of stream is column num
	tmpstat.a[r][c] = s[0];	//set specified row/column of array to x or o
	tmpstat.wTurn = s;	//changes turn to x or o, checks if next turn is going to be the same char
	printBoard(tmpstat.a);	//print out the board
	s = checkWin(tmpstat.a,tmpstat.turnNum);	//check if there is a winner
	tmpstat.turnNum += 1;	//increment turn num
	if(s == "wonx"){	//change tmpstat.s (status) to x won! if x won
		tmpstat.s = "x won!";
		return tmpstat;
	}
	if(s == "wono"){	//change to o won!
		tmpstat.s = "o won!";
		return tmpstat;
	}
	if(s == "tie"){		//change to tie
		tmpstat.s = "Tie";
		return tmpstat;
	}
	tmpstat.s = "no";	//if no one won status is changed to no
	return tmpstat;
	}else{		//If input didn't pass the checkErr function
		tmpstat.s = "Invalid move: ";	//change status to invalid move
		return tmpstat;
	}
}
void print(int xwin, int owin, int tie, int invalid){ //Prints the stats of game when program terminate
	cout << "x Wins:         " << xwin <<endl;
	cout << "o Wins:         " << owin <<endl;
	cout << "Ties:           " << tie <<endl;
	cout << "Aborted Games:  " << invalid <<endl;
}
int main(int argc, char **argv){
	string wTurn = " ";					//Shows whose turn it is
	int turnNum = 0; 					//number of turns, if hit nine without a winner, considered a tie	
	Round stat {{' ',' ',' ',' ',' ',' ',' ', ' ', ' '},wTurn,turnNum," "}; //initalize the struct with empty board and status
	int xwin = 0;//vars for the total number of games and wins
	int owin = 0;
	int tie = 0;
	int invalid = 0;
	if(argc == 1){ //if only one arg, read from stdin
		string p;
		while(getline(cin, p)){ //while not eof
			stat = passTurn(stat, p);	//start by passing a turn
			if(stat.s == "x won!"){	//if the status of the struct != no then we increment valid counter and break
				cout << stat.s << endl;
				xwin++;
				stat= {{' ',' ',' ',' ',' ',' ',' ', ' ', ' '},wTurn,turnNum," "};
				break;
			}
			if(stat.s == "o won!"){
				cout << stat.s << endl;
				owin++;
				stat= {{' ',' ',' ',' ',' ',' ',' ', ' ', ' '},wTurn,turnNum," "};
				break;
			}
			if(stat.s == "Tie"){
				cout << stat.s << endl;
				tie++;
				stat= {{' ',' ',' ',' ',' ',' ',' ', ' ', ' '},wTurn,turnNum," "};
				break;
			}
			if(stat.s == "Invalid move: "){
				cerr << stat.s << p << endl;
				invalid++;
				stat= {{' ',' ',' ',' ',' ',' ',' ', ' ', ' '},wTurn,turnNum," "};
				break;
			}
		}
		if(stat.s != " "){//if reached eof and status isn't empty (aka mid game), error unfinished game
			invalid++;
			cerr << "Unfinished game"<<endl;
		}
		print(xwin, owin, tie,invalid); //print the board
		if(invalid > 0) return 1;
		return 0;
	}
	else{
		for(int i = 1; i < argc ; i++){	//loops through all the args
			ifstream file{argv[i]}; 
			if(!file){//checks if file name is bad
				cerr << "Bad file: " << argv[i] << endl;
				invalid++;
				continue;
			}
			string v;
			wTurn = " ";
			turnNum = 0;
			stat = {{' ',' ',' ',' ',' ',' ',' ', ' ', ' '},wTurn,turnNum," "};	
			while(getline(file,v)){
				stat = passTurn(stat,v);
				if(stat.s == "x won!"){//similar to no args but have to reset status,turnnum and whoseturn between each file
					cout << stat.s <<endl;
					xwin++;
					wTurn = " ";
					turnNum = 0;
					stat = {{' ',' ',' ',' ',' ',' ',' ', ' ', ' '},wTurn,turnNum," "};	
					break;
				}
				if(stat.s == "o won!"){
					cout << stat.s <<endl;
					owin++;
					wTurn = " ";
					turnNum = 0;
					stat = {{' ',' ',' ',' ',' ',' ',' ', ' ', ' '},wTurn,turnNum," "};	
					break;
				}
				if(stat.s == "Tie"){
					cout << stat.s <<endl;
					tie++;
					wTurn = " ";
					turnNum = 0;
					stat = {{' ',' ',' ',' ',' ',' ',' ', ' ', ' '},wTurn,turnNum," "};	
					break;
				}
				if(stat.s == "Invalid move: "){
					cerr << stat.s << v << endl;
					invalid++;
					wTurn = " ";
					turnNum = 0;
					stat = {{' ',' ',' ',' ',' ',' ',' ', ' ', ' '},wTurn,turnNum," "};	
					break;
				}
			}	//similar to prev part
			if(stat.s != " "){
				invalid++;
				cerr << "Unfinished game" << endl;
			}
		}
			print(xwin, owin, tie,invalid);
			if(invalid > 0) return 1;
			return 0;
	}
}
