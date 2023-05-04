#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <vector>

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPos = { 0,0 };

void gotoxy(int x, int y) {
	COORD c = { x,y };
	SetConsoleCursorPosition(h, c);
}

void gotoxy(COORD c) {
	SetConsoleCursorPosition(h, c);
}

struct snake {
	COORD pos;
	char shape;
	snake(int x, int y) {
		pos.X = x;
		pos.Y = y;
	}
	snake(int x, int y, int s) {
		pos.X = x;
		pos.Y = y;
		shape = char(s);
	}
};

bool operator== (const COORD a, const COORD b) {
	if (a.Y == b.Y && a.X == b.X) return true;
	else return false;
}

std::vector<snake> s;
int pointX = 1;
int pointY = 1;
char previousDirection = 'd';
char getKey;
bool gameOver = false;

int appleX = 0;
int appleY = 0;

int score = 0;

void movement() {
	for (int i = s.size() - 1; i > 0; i--) {
		s[i] = s[i - 1];
	}
	switch (previousDirection) {
	case 'w':
		s[0].pos.Y--;
		if (s[0].pos.Y == 0) s[0].pos.Y = 20;
		break;
	case 'a':
		s[0].pos.X--;
		if (s[0].pos.X == 0) s[0].pos.X = 20;
		break;
	case 'd':
		s[0].pos.X++;
		if (s[0].pos.X == 21) s[0].pos.X = 1;
		break;
	case 's':
		s[0].pos.Y++;
		if (s[0].pos.Y == 21) s[0].pos.Y = 1;
		break;
	default:
		break;
	}
}

bool changeDirection() {
	switch (previousDirection) {
	case 'w':
		if (getKey == 'w' || getKey == 's') {
			return false;
		}
		else if (getKey == 'a') {
			previousDirection = getKey;
			s[0].shape = 187;
			return true;
		}
		else if (getKey == 'd') {
			previousDirection = getKey;
			s[0].shape = 201;
			return true;
		}
		break;

	case 's':
		if (getKey == 'w' || getKey == 's') {
			return false;
		}
		else if (getKey == 'a') {
			previousDirection = getKey;
			s[0].shape = 188;
			return true;
		}
		else if (getKey == 'd') {
			previousDirection = getKey;
			s[0].shape = 200;
			return true;
		}
		break;

	case 'a':
		if (getKey == 'a' || getKey == 'd') {
			return false;
		}
		else if (getKey == 'w') {
			previousDirection = getKey;
			s[0].shape = 200;
			return true;
		}
		else if (getKey == 's') {
			previousDirection = getKey;
			s[0].shape = 201;
			return true;
		}
		break;

	case 'd':
		if (getKey == 'a' || getKey == 'd') {
			return false;
		}
		else if (getKey == 'w') {
			previousDirection = getKey;
			s[0].shape = 188;
			return true;
		}
		else if (getKey == 's') {
			previousDirection = getKey;
			s[0].shape = 187;
			return true;
		}
		break;
	}
}

void genApple() {
	appleX = 1 + rand() % 19;
	appleY = 1 + rand() % 19;
}

void drawApple() {
	gotoxy(appleX, appleY);
	std::cout << 'o';
}

void eraseApple() {
	gotoxy(appleX, appleY);
	std::cout << ' ';
}

void drawBoard() {
	std::cout << (char)218;
	for (int i = 0; i < 20; i++) {
		std::cout << (char)196;
	}
	std::cout << (char)191;
	gotoxy(0, 21);
	std::cout << (char)192;
	for (int i = 0; i < 20; i++) {
		std::cout << char(196);
	}
	std::cout << (char)217;
	for (int i = 1; i <= 20; i++) {
		gotoxy(0, i);
		std::cout << (char)179;
		gotoxy(21, i);
		std::cout << (char)179;
	}
	gotoxy(0, 22);
}

void drawSnake() {
	gotoxy(s[s.size() - 1].pos);
	std::cout << ' ';
	movement();
	gotoxy(s[1].pos);
	std::cout << s[1].shape;
	gotoxy(s[0].pos);
	std::cout << 'O';
}

void checking() {
	for (int i = 1; i < s.size(); i++) {
		if (s[i].pos == s[0].pos) {
			gameOver = true;
		}
		if (s[i].pos.X == appleX && s[i].pos.Y == appleY) {
			appleX = s[s.size() - 1].pos.X;
			appleY = s[s.size() - 1].pos.Y;
		}
	}
}

void viewScore() {
	gotoxy(22, 0);
	std::cout << "Score: " << score;
}

void updateScore() {
	gotoxy(30, 0);
	std::cout << score;
}

int main() {
	srand(time(NULL));
	drawBoard();
	viewScore();
	s.push_back(snake(2, 1));
	s.push_back(snake(1, 1, 205));
	genApple();
	checking();
	drawSnake();
	drawApple();
	while (!gameOver) {
		if (s[0].pos.X == appleX && s[0].pos.Y == appleY) {
			score++;
			viewScore();
			s.push_back(snake(appleX, appleY));
			eraseApple();
			genApple();
		}
		if (_kbhit()) {
			getKey = _getch();
		}
		if (!changeDirection()) {
			if (previousDirection == 'a' || previousDirection == 'd') s[0].shape = (char)205;
			else s[0].shape = (char)186;
		}
		drawSnake();
		checking();
		drawApple();
		gotoxy(22, 21);
		Sleep(100);
	}
	gotoxy(0, 22);
	std::cout << "Game over";
	return 0;
}