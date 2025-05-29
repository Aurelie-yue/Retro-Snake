#define _CRT_SECURE_NO_WARNINGS
//reference graphics library file
#include <iostream>
#include <graphics.h>
#include <stdio.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include<Mmsystem.h>
#pragma comment(lib,"winmm.lib")
using namespace std;

//define macros:block width,reference coordinates,window width and height
#define SIZE 20
#define x_origin 0
#define y_origin 0
#define WIDTH 600
#define HEIGHT 600
#define random(a,b) (rand()%(b-a-1)+a+1)

//initialize the food
int food[2] = { 18,15 };

//initialize the score
int score = 0;

//initialize the snake's body
//use a two-dimensional array to store the coordinates of each section of the snake
int snakeLength = 12;
int snake[900][2] =
{
	{14,21},
	{13,21},
	{13,20},
	{12,20},
	{11,20},
	{11,19},
	{11,18},
	{11,17},
	{10,17},
	{10,16},
	{10,15},
	{9,15},
};

//draw the background of the game
void drawBK() {
	IMAGE img;
	loadimage(&img, "pic/snake1.jpg", 600, 600);
	putimage(0, 0, &img);
}


//add a bgm
void playMusic() {
	mciSendString("open music/d.mp3 ", NULL, 0, NULL);
	mciSendString("play music/d.mp3 ", NULL, 0, NULL);
	mciSendString("repeat music/d.mp3 ", NULL, 0, NULL);//loop music
}


//draw the body of the snake
void drawSnake()
{
	for (int i = 0; i < snakeLength; i++) {
		//use gradient to make the snake more beautiful~
		setfillcolor(RGB(240-i*1.58, 128-i*2.67, 128-i*7.75));
		fillcircle(snake[i][0] * SIZE, snake[i][1] * SIZE, SIZE / 2);
	};
}


//to check whether the snake can move
int canMove(char input) {
	int headX = snake[0][0];
	int headY = snake[0][1];

	//use 'wasd' to control the direction
	switch (input) {
	case'A':headX -= 1; break;
	case'D':headX += 1; break;
	case'W':headY -= 1; break;
	case'S':headY += 1; break;
	}

	//whether the head will be out of border
	if (headX <= 0 || headX >= WIDTH / SIZE || headY <= 0 || headY >= HEIGHT / SIZE)return 0;

	//whether the snake go across itself
	for (int i = 1; i < snakeLength; i++) {
		if (snake[i][0] == headX && snake[i][1] == headY)return 0;
	}
	return 1;
}


//how the snake eat food
int canFood(int x, int y) {

	for (int i = 0; i < snakeLength; i++) {
		//can not have the same coordinates as the snake
		if (x == snake[i][0] && y == snake[i][1])
			return 0;
	}

	//food can not appear in border
	if (x <= 0 || x >= 30 || y <= 0 || y >= 30)
		return 0;
	return 1;
}

int createFood() {
	srand(time(NULL));
	//range of random numbers
	food[0] = rand() % (WIDTH / SIZE);
	food[1] = rand() % (HEIGHT / SIZE);
	if (canFood(food[0], food[1]))
		return 1;
	else
		return createFood();
}

void drawFood() {
	setfillcolor(RGB(245, 223, 77));
	solidrectangle(food[0] * SIZE - SIZE / 2, food[1] * SIZE - SIZE / 2,
		food[0] * SIZE + SIZE / 2, food[1] * SIZE + SIZE / 2);
}


//how to move the snake by 'wasd'
char direction = 'D';/*start by default to the right*/
void moveSnake() {
	int headX = snake[0][0];
	int headY = snake[0][1];
	int tailX = snake[snakeLength - 1][0];
	int tailY = snake[snakeLength - 1][1];
	
	switch (direction) {
	case'A':headX -= 1; break;
	case'D':headX += 1; break;
	case'W':headY -= 1; break;
	case'S':headY += 1; break;
	}
	
	//if the snake can not move?
	if (!canMove(direction))
	{
		settextstyle(60, 0, _T("Bauhaus 93"));
		settextcolor(BLACK);
		//cout 'you died' and pause game
		outtextxy(180, 230, "YOU DIED");
		return;
	}
	
	//otherwise the game go on
	for (int i = snakeLength - 1; i > 0; i--) {
		snake[i][0] = snake[i - 1][0];
		snake[i][1] = snake[i - 1][1];
	}
	snake[0][0] = headX;
	snake[0][1] = headY;

	//whether the snake has eaten the food
	if (food[0] == headX && food[1] == headY) {
		createFood();
		score += 10;
		snakeLength++;
		snake[snakeLength - 1][0] = tailX;
		snake[snakeLength - 1][1] = tailY;
	}
}

//make the snake move by itself
void autoMove() {
	cleardevice();
	drawBK();

	moveSnake();
	drawSnake();
	drawFood();
	
	//score increase
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	settextstyle(30, 0, _T("Arial"));
	outtextxy(0, 0, "SCORE:");
	char Score[10];
	sprintf(Score, "%d", score);
	outtextxy(5 * SIZE, 0, Score);

	//speed
	Sleep(350);
}


//print the snake array on the consol to check if moved correctly
void printArray() {
	for (int i = 0; i < snakeLength; i++) {
		cout << "(" << snake[i][0] << "," << snake[i][1] << ")";
	}
	cout << endl;
}



//main function section
int main() {
	playMusic();
	initgraph(WIDTH, HEIGHT, EX_SHOWCONSOLE); 

	drawBK(); 
	drawSnake(); 
	drawFood(); 

	BeginBatchDraw(); //start batch drawing
	while (true) {
		DWORD startTime = GetTickCount(); //get the current time
		if (_kbhit()) {
			char input = _getch();
			if ((input == 'A' || input == 'D' || input == 'S' || input == 'W') && canMove(input)) {
				direction = input;
			}
		}
		autoMove(); 
		printArray(); 
		DWORD endTime = GetTickCount(); //get the end time
		DWORD elapsedTime = endTime - startTime; //calculate the elapsed time
		if (elapsedTime < 1000/60) { //if the elapsed time is less than 350ms
			Sleep(1000/60 - elapsedTime); //sleep for the remaining time to maintain the speed
		}

		FlushBatchDraw(); //flush batch drawing
	}
	EndBatchDraw(); //end batch drawing

	_getch();
	closegraph(); 
	return 0;
}