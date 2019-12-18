#include<iostream>
#include<string>
#include<conio.h>
#include<time.h>
#include<windows.h>
#include<fstream>
#include<iomanip>
#include"gotoxy.h"
#include"Hide.h"
using namespace std;

struct coord {
	int x;
	int y;
};
coord CurTet;
coord shadowpos;
const int width = 10;
const int height = 20;
const string ScorePath = "C:\\Users\\Shane\\Desktop\\Coding\\C++\\Tetris\\scores.txt";
const string ConfigPath = "C:\\Users\\Shane\\Desktop\\Coding\\C++\\Tetris\\config.txt";
int score;
int ticknum = 0;
int SolidifyCounter = 0;
int SolidifyRate = 1;
bool SwapReady = true;
bool paused = false;
bool ShadowOn = true;
bool running;
string CurrentTetrimino[4] = {
	"xxxx",
	"xxxx",
	"xxxx",
	"xxxx",
};
string hold[4] = {
	"....",
	"....",
	"....",
	"....",
};
string shadow[] = {
	"....",
	"....",
	"....",
	"....",
};
const string Tetrimino[][4] = {
	{
		".I..",
		".I..",
		".I..",
		".I..",
	},
	{
		".S..",
		".SS.",
		"..S.",
		"....",
	},
	{
		"..Z.",
		".ZZ.",
		".Z..",
		"....",
	},
	{
		"..T.",
		".TT.",
		"..T.",
		"....",
	},
	{
		"....",
		".OO.",
		".OO.",
		"....",
	},
	{
		".L..",
		".L..",
		".LL.",
		"....",
	},
	{
		"..J.",
		"..J.",
		".JJ.",
		"....",
	},
};
char grid[height][width] = {
	{'.','.','.','.','.','.','.','.','.','.'},
	{'.','.','.','.','.','.','.','.','.','.'},
	{'.','.','.','.','.','.','.','.','.','.'},
	{'.','.','.','.','.','.','.','.','.','.'},
	{'.','.','.','.','.','.','.','.','.','.'},
	{'.','.','.','.','.','.','.','.','.','.'},
	{'.','.','.','.','.','.','.','.','.','.'},
	{'.','.','.','.','.','.','.','.','.','.'},
	{'.','.','.','.','.','.','.','.','.','.'},
	{'.','.','.','.','.','.','.','.','.','.'},
	{'.','.','.','.','.','.','.','.','.','.'},
	{'.','.','.','.','.','.','.','.','.','.'},
	{'.','.','.','.','.','.','.','.','.','.'},
	{'.','.','.','.','.','.','.','.','.','.'},
	{'.','.','.','.','.','.','.','.','.','.'},
	{'.','.','.','.','.','.','.','.','.','.'},
	{'.','.','.','.','.','.','.','.','.','.'},
	{'.','.','.','.','.','.','.','.','.','.'},
	{'.','.','.','.','.','.','.','.','.','.'},
	{'.','.','.','.','.','.','.','.','.','.'},
};
void color(int c){
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void ColorSel(char ch){
	switch (ch) {
		case 'I':
			color(3);
			break;
		case 'S':
			color(4);
			break;
		case 'Z':
			color(2);
			break;
		case 'T':
			color(13);
			break;
		case 'O':
			color(14);
			break;
		case 'L':
			color(6);
			break;
		case 'J':
			color(12);
			break;
		default: 
			color(7);
			break;
	}
}
void FillBoard(){

	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			gotoxy(1 + 2 * j, 6 + i);
			if(grid[i][j] == '.'){
				cout << "  ";
			}
			else{
				ColorSel(grid[i][j]);
				cout << char(219) << char(219);
				color(7);
			}
		}
	}
}
void tick(){
	Sleep(50);
	ticknum++;
	if(ticknum >= 3000){
		ticknum = 0;
	}
}
void PrintSpeedNum(int speed){
	gotoxy(1,4);
	cout << 16 - speed;	
}
void PrintGrid(){
	int g[27][18] = {
		{ 201, 205, 205, 205, 205, 205, 205, 203, 205, 205, 205, 205, 203, 205, 205, 205, 205, 187},
		{ 186, 32, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186},
		{ 186, 32, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186},
		{ 186, 32, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186},
		{ 186, 32, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186},
		{ 204, 205, 205, 205, 205, 205, 205, 202, 205, 205, 205, 205, 185, 32, 32, 32, 32, 186},
		{ 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186},
		{ 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186},
		{ 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186},
		{ 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186},
		{ 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186},
		{ 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186},
		{ 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186},
		{ 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186},
		{ 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186},
		{ 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186},
		{ 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186},
		{ 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186},
		{ 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186},
		{ 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186},
		{ 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186},
		{ 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186},
		{ 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186},
		{ 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186},
		{ 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186},
		{ 186, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 186, 32, 32, 32, 32, 186},
		{ 200, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 205, 202, 205, 205, 205, 205, 188},
	};
	for(int i = 0; i < 27; i++) {
		for(int j = 0; j < 18; j++) {
			if( j == 0 || j == 12 || j == 6 || j == 7 || j == 17)
				cout << char(g[i][j]);
			else cout << char(g[i][j]) << char(g[i][j]);
		}
		cout << endl;
	}
	gotoxy(1,3);
	cout << "Speed:";
	gotoxy(1, 1);
	cout << "Score:";
	gotoxy(22, 1);
	cout <<"Up Next:";
}
void PrintHold(){
	coord h = {13, 1};
	for(int i = 0; i < 4; i++){
		for(int  j = 0; j < 4; j++){
			gotoxy(h.x + 2*j, h.y +  i);
			if(hold[i][j] == '.'){
				cout << "  ";
			}
			else {
				ColorSel(hold[i][j]);
				cout << char(219) << char(219);
				color(7);
			}
		}
	}
}
void PrintScoreNum(){
	gotoxy(1, 2);
	cout << score;
}
void PrintTetrimino(){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			gotoxy(CurTet.x + 2 * j, CurTet.y + i);
			if(CurrentTetrimino[i][j] != '.'){
				ColorSel(CurrentTetrimino[i][j]);
				cout << char(219) << char(219);
				color(7);
			}
		}
	}
}
void DeleteTetrimino(){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			gotoxy(CurTet.x + 2 * j, CurTet.y + i);
			if(CurrentTetrimino[i][j] != '.'){
				cout << "  ";
			}
		}
	}
}
bool collide(int dx = 0, int dy = 0){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			if(CurrentTetrimino[i][j] != '.' && (grid[CurTet.y - 6 + i + dy][((CurTet.x -1) / 2) + j + dx] != '.' || ((CurTet.x -1) / 2) + j + dx < 0 || ((CurTet.x -1) / 2) + j + dx > 9)){
				return true;
			}
		}
	}
	return false;
}
void PrintShadow(){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			gotoxy(shadowpos.x + 2*j, shadowpos.y + i);
			if(shadow[i][j] != '.'){
				ColorSel(CurrentTetrimino[i][j]);
				cout << char(176) << char(176);
				color(7);
			}
		}
	}
}
void DeleteShadow(){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			gotoxy(shadowpos.x + 2*j, shadowpos.y + i);
			if(shadow[i][j] != '.'){
				cout << "  ";
			}
		}
	}
}
void UpdateShadow(){
	if(ShadowOn){
		DeleteShadow();
		shadowpos = {CurTet.x, CurTet.y};
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				shadow[i][j] = CurrentTetrimino[i][j];
			}
		}
		while(1){
			bool coll = false;
			for(int i = 0; i < 4; i++){
				for(int j = 0; j < 4; j++){
					if(shadow[i][j] != '.' && (grid[shadowpos.y - 6 + i + 1][((shadowpos.x -1) / 2) + j] != '.' || ((shadowpos.x -1) / 2) + j < 0 || ((shadowpos.x -1) / 2) + j > 9)){
						coll = true;
					}
				}
			}
			if(!coll){
				shadowpos.y++;						
			}
			else break;
		}
		PrintShadow();
	}
}
void rotate(char c){
	string temp[4];
	DeleteTetrimino();
	if(c == 'r'){
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4;j++){
				temp[i][j] = CurrentTetrimino[3 - j][i];
			}
		}
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				CurrentTetrimino[i][j] = temp[i][j];
			}
		}
		if(collide()){
			for(int i = 0; i < 4; i++){
				for(int j = 0; j < 4;j++){
					temp[i][j] = CurrentTetrimino[j][3 - i];
				}
			}
			for(int i = 0; i < 4; i++){
				for(int j = 0; j < 4; j++){
					CurrentTetrimino[i][j] = temp[i][j];
				}
			}
		}
	}
	else if(c == 'l'){
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4;j++){
				temp[i][j] = CurrentTetrimino[j][3 - i];
			}
		}
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				CurrentTetrimino[i][j] = temp[i][j];
			}
		}
		if(collide()){
			for(int i = 0; i < 4; i++){
				for(int j = 0; j < 4;j++){
					temp[i][j] = CurrentTetrimino[3 - j][i];
				}
			}
			for(int i = 0; i < 4; i++){
				for(int j = 0; j < 4; j++){
					CurrentTetrimino[i][j] = temp[i][j];
				}
			}
		}

	}
	UpdateShadow();
	PrintTetrimino();
}
void RandPiece(){
	int random = rand() % 7;
	for(int i = 0; i < 4; i++){
		CurrentTetrimino[i] = Tetrimino[random][i];
	}
}
void swap(){
	if(SwapReady){
		DeleteTetrimino();
		CurTet = {7, 6};
		bool empty = true;
		string temp[4];
		for(int i = 0; i < 4; i++){
			temp[i] = hold[i];
			hold[i] = CurrentTetrimino[i];
			CurrentTetrimino[i] = temp[i];
			if(CurrentTetrimino[i] != "...."){
				empty = false;
			}
		}
		if (empty){
			RandPiece();
		}
		UpdateShadow();
		PrintHold();
		PrintTetrimino();
		SwapReady = false;
	}
}
bool DetectClear(int i){
	for(int j = 0; j < width; j++){
		if(grid[i][j] == '.'){
			return false;
		}
	}
	return true;
}
void ClearLine(){
	int counter = 0;
	for(int i = 0; i < height; i++){
		if(DetectClear(i)){
			counter++;
			for(int j = 0; j < width; j++){
				grid[i][j] = '.';
			}
			for(int j = i; j >= 0; j--){
				for(int k = 0; k < width; k++){
					if(j == 0){
						grid[j][k] = '.';
					}
					else{
						grid[j][k] = grid[j - 1][k];
					}
				}
			}
		}
	}
	switch(counter) {
		case 0:
			score += 0;
			break;
		case 1:
			score += 100;
			break;
		case 2:
			score += 400;
			break;
		case 3:
			score += 800;
			break;
		case 4:
			score += 1600;
			break;
	}
	FillBoard();
	PrintScoreNum();
}
void solidify(){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			if(CurrentTetrimino[i][j] != '.'){ 
				grid[CurTet.y - 7 + i + 1][((CurTet.x -1) / 2) + j] = CurrentTetrimino[i][j];
			}
		}
	}
	for(int i = 0; i < 4; i++){
		shadow[i] = "....";
	}
	CurTet = { 7, 6};
	RandPiece();
	ClearLine();
	UpdateShadow();
	PrintTetrimino();
	SwapReady = true;
	if(collide()) running = false;
}
void MoveLeft(){
	if(!collide(-1,0)){
		DeleteTetrimino();
		CurTet.x -= 2;
		UpdateShadow();
		PrintTetrimino();
	}
}
void MoveRight(){
	if(!collide(1,0)){
		DeleteTetrimino();
		CurTet.x += 2;
		UpdateShadow();
		PrintTetrimino();
	}
}
void MoveDown(){
	if(!collide(0,1)){
		DeleteTetrimino();
		CurTet.y++;
		PrintTetrimino();
	}
	else if(SolidifyCounter == SolidifyRate){
		solidify();
		SolidifyCounter = 0;
	}
	else SolidifyCounter++;
}
void InstaDrop(){
	DeleteTetrimino();
	while(1){
		score += 5;
		PrintScoreNum();
		if(!collide(0,1)){
			CurTet.y++;
		}
		else{
			PrintTetrimino();
			solidify();
			break;
		}
	}
}
bool cont(){
	char ch;
	do{
		cout << "Do you wish to play again? y/n" << endl;
		ch = getch();
	}while(ch != 'y' && ch != 'Y' && ch != 'n' && ch != 'N');
	if(ch == 'y' || ch == 'Y') return true;
	else return false;
}
void pause(){
	if(!paused){
		paused = true;
		gotoxy(6, 14);
		cout << "--PAUSED--";

	}
	else{
		paused = false;
		FillBoard();
		PrintTetrimino();
	}
}
void kbin(){
	if(kbhit()){
		char ch;
		ch = getch();
		if(ch == 'q'){
			if(!paused) rotate('l');
		}
		else if(ch == 'e'){
			if(!paused) rotate('r');
		}
		else if(ch == 'w'){
			if(!paused) InstaDrop();
		}
		else if(ch == 'a'){
			if(!paused) MoveLeft();
		}
		else if(ch == 's'){
			if(!paused){
				MoveDown();
				score += 5;
				PrintScoreNum();
			}
		}
		else if(ch == 'd'){
			if(!paused) MoveRight();
		}
		else if(ch == 'p'){
			pause();
		}
		else if(ch == ' '){
			if(!paused) swap();
		}
		else if (ch == 27){
			gotoxy(0, 26);
			ShowConsoleCursor(true);
			exit(0);
		}
	}
}
void DisplayScores(){
	ifstream scrs(ScorePath);
	string str;
	while (scrs >> str){
		cout << setw(15) << str << "  ";
		scrs >> str;
		cout << str << endl;
	}
	scrs.close();
	cout << endl;
}
void RecordScore(){
	string name;
	string names[100];
	int scores[100];
	int i = 1;
	while(1){
		cout << "Please enter your name: ";
		cin >> name;
		if(name.length() > 15){
			cout << "Your name must be under 15 characters" << endl << endl;
		}
		else break;
	}
	names[0] = name;
	scores[0] = score;
	ifstream scrs(ScorePath);
	for(i = 1;scrs >> names[i]; i++){
		scrs >> scores[i];
	}
      	scrs.close();
	for(int j = 0; j < i; j++){
		for(int k = 1; k < i - j; k++){
			if(scores[k - 1] < scores[k]){
				int temp = scores[k - 1];
				scores[k - 1] = scores[k];
				scores[k] = temp;
				string tempstr = names[k - 1];
				names[k - 1] = names[k];
				names[k] = tempstr;
			}
		}
	}
	ofstream s(ScorePath);
	for(int j = 0; j < i; j++){
		s << names[j] << " " << scores[j] << endl;
	}	
	s.close();
	DisplayScores();
}
void config(){
	while(1){
		char choice;
		bool sh;
		ifstream icfg(ConfigPath);
		icfg >> sh;		
		icfg.close();



		cout << "1) Toggle Shadow (Currently ";
		if(sh){
			color(10);
			cout << "on";
			color(7);
		}
		else{
			color(12);
			cout << "off";
			color(7);
		}
		cout << ")" << endl;

		cout << endl << "2) Back" << endl;

		choice = getch();
		if(choice == '1'){
			ofstream ocfg(ConfigPath);
			if(sh) ocfg << false;
			else ocfg << true;
			ocfg.close();
		}
		else if(choice == '2')break;
		system("cls");
	}
}
void menu(){
	while(1){
		char choice;
		color(13);
		cout << " _______  _______  _______  ______    ___   _______ " << endl;
		cout << "|       ||       ||       ||    _ |  |   | |       |" << endl;
		cout << "|_     _||    ___||_     _||   | ||  |   | |  _____|" << endl;
		cout << "  |   |  |   |___   |   |  |   |_||_ |   | | |_____ " << endl;
		cout << "  |   |  |    ___|  |   |  |    __  ||   | |_____  |" << endl;
		cout << "  |   |  |   |___   |   |  |   |  | ||   |  _____| |" << endl;
		cout << "  |___|  |_______|  |___|  |___|  |_||___| |_______|" << endl;
		color(10);
		cout << "     ____ ____      ____ ____ ____ ____ ____ " << endl;
		cout << "    ||b |||y ||    ||S |||h |||a |||n |||e ||" << endl;
		cout << "    ||__|||__||    ||__|||__|||__|||__|||__||" << endl;
		cout << "    |/__\\|/__\\|    |/__\\|/__\\|/__\\|/__\\|/__\\|" << endl << endl;
		color(7);
		cout << "1) Play" << endl << endl;
		cout << "2) High Scores" << endl << endl;
		cout << "3) Config" << endl << endl;
		cout << "4) Exit" << endl << endl;
		choice = getch();
		if(choice == '1') break;
		else if (choice == '2'){
			system("cls");
			DisplayScores();
			system("pause");
		}
		else if(choice == '3') {
			system("cls");
			config();
		}
		else if(choice == '4') {
			ShowConsoleCursor(true);
			exit(0);
		}
		system("cls");
	}
	system("cls");
}
int main(){
	while(1){
		ShowConsoleCursor(false);
		menu();
		ifstream icfg(ConfigPath);
		icfg >> ShadowOn;
		icfg.close();
		srand(time(NULL));
		int speed = 15;
		CurTet = {7, 6};
		shadowpos = {7, 6};
		score = 0;
		running = true;
		RandPiece();
		PrintGrid();
		PrintTetrimino();
		PrintScoreNum();
		PrintSpeedNum(speed);
		UpdateShadow();
		while(running){
			kbin();
			if(!paused){
				if(ticknum % speed == 0){
					MoveDown();
				}
				tick();
				if(ticknum % 500 == 0){
					speed--;
					if(speed <= 1) speed = 1;
					else if(speed <= 8) SolidifyRate += 2;
					PrintSpeedNum(speed);
				}
			}
		}
		gotoxy(0, 27);
		ShowConsoleCursor(true);
		cout << "You lost!\nYour Score Was " << score << endl;
		RecordScore();
		if(!cont()) break;
		for(int i = 0; i < height; i++){
			for(int j = 0; j < width; j++){
				grid[i][j] = '.';
			}
		}
	}
	return 0;
}
