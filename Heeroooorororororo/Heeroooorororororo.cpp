// Heeroooorororororo.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <Windows.h>    //sleep
#include <cstdio>       //rand
#include <ctime>        //time
#include <conio.h>
#include <string>
//using namespace std;

struct HeroData
{
	std::string heroName;
	int hp;
	int totalHP;
	int money = 0;
	int playerX = 1;
	int playerY = 1;
};

struct Difficulty
{
	std::string diff[3] = { "초급[1]", "중급[2]", "고급[3]" };
	int startHP[3] = { 50, 70, 100 };
	int monsterNum;	//1~3마리
};

int main()
{
	//--------------정보 입력 화면--------------
	std::cout << "\n ---- 영웅은 절차적 ----\n\n";

	//영웅 이름 입력
	HeroData myHero;
	std::cout << "영웅 이름을 입력하세요: ";
	std::cin >> myHero.heroName;

	//난이도 선택, 기타 시작 정보 설정
	Difficulty myDiff;
	int d;
	std::cout << "\n난이도를 선택해주세요: ";
	std::cout << "[ 초급(1) 중급(2) 고급(3) ]\n→ ";
	d = _getch();
	std::cout << myDiff.diff[((int)d - 48) - 1] << " 선택하였습니다\n\n";
	myHero.hp = myDiff.startHP[((int)d - 48) - 1];
	myHero.totalHP = myDiff.startHP[((int)d - 48) - 1];
	myDiff.monsterNum = ((int)d - 48);	

	//다음 화면으로 넘어가기
	std::cout << "<<     '" << myHero.heroName << "'님 게임을 시작합니다!     >>\n\n";
	Sleep(1000);
	std::system("cls");

	//--------------몬스터 배틀 화면--------------
	srand(time(NULL));

	//maze판 만들기
	char maze[10][15];
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (i == 0 || i == 9 || j == 0 || j == 14)
				maze[i][j] = '#';
			else
				maze[i][j] = ' ';
		}
	}

	//플레이어 시작점 설정
	maze[myHero.playerX][myHero.playerY] = 'P';

	//몬스터 위치 설정
	int monNumCnt = 0;
	int monX, monY;
	while (monNumCnt < myDiff.monsterNum)	//maze[1][1]~maze[8][13] 실사용 범위
	{
		monX = (rand() % 8) + 1;
		monY = (rand() % 13) + 1;
		if (maze[monX][monY] != 'P' && maze[monX][monY] != 'M')
		{
			monNumCnt++;
			maze[monX][monY] = 'M';
		}
	}

	while (true)
	{
		//maze판 출력
		std::cout << "\n";
		for (int i = 0; i < 10; i++)
		{
			std::cout << "    ";
			for (int j = 0; j < 15; j++)
			{
				std::cout << maze[i][j] << " ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";

		//플레이어 이동
		int userInput = _getch();
		//std::cout << "userInput: " << userInput << "\n";
		//std::cout << "(char)userInput: " << (char)userInput << "\n";
		int nextX = myHero.playerX;
		int nextY = myHero.playerY;
		//std::cout << "(myHero.playerX, myHero.playerY): " << myHero.playerX << "," << myHero.playerY << "\n";
		if ((char)userInput == 'w' && (myHero.playerX - 1) >= 1)  //키 조작
			nextX = myHero.playerX - 1;
		if ((char)userInput == 's' && (myHero.playerX + 1) <= 8)
			nextX = myHero.playerX + 1;
		if ((char)userInput == 'a' && (myHero.playerY - 1) >= 1)
			nextY = myHero.playerY - 1;
		if ((char)userInput == 'd' && (myHero.playerY + 1) <= 13)
			nextY = myHero.playerY + 1;
		//std::cout << "(nextX, next): " << nextX << "," << nextY << "\n";
		//Sleep(1000);

		//몬스터 만난 경우
		bool shopEnter = false;
		if (maze[nextX][nextY] == 'M')
		{
			std::cout << "\n \"몬스터를 만났다! 가위바위보를 하자!! \" \n\n";

			while (true)
			{
				//유저 가위 바위 보 입력 받기
				std::string fightString;
				std::cout << "(가위 / 바위 / 보) 중 하나를 선택해 입력해주세요:  ";
				std::cin >> fightString;
				int playerFight = 0;
				if (fightString.compare("가위") == 0) //compare는 결과가 같으면 0
					playerFight = 1;
				if (fightString.compare("바위") == 0)
					playerFight = 2;
				if (fightString.compare("보") == 0)
					playerFight = 3;

				//컴퓨터 가위 바위 보 입력 받기
				int comFight = (rand() % 3 + 1);
				std::cout << "컴퓨터의 선택:   ";
				if (comFight == 1)
					std::cout << "가위" << "\n";
				else if (comFight == 2)
					std::cout << "바위" << "\n";
				else if (comFight == 3)
					std::cout << "보" << "\n";

				//배틀시작
				if ((comFight == 1 && playerFight == 1) || (comFight == 2 && playerFight == 2) || (comFight == 3 && playerFight == 3))	//무승부
				{
					std::cout << "→ 비겼습니다\n";
					std::cout << "[ HP: " << myHero.hp << "  Money: " << myHero.money << " ]" << "\n\n";
					continue;
				}
				else if ((comFight == 1 && playerFight == 2) || (comFight == 2 && playerFight == 3) || (comFight == 3 && playerFight == 1))	//승리
				{
					myDiff.monsterNum--;
					if (myDiff.monsterNum == 0)
					{
						std::cout << "→ 모든 몬스터 처치! 이겼습니다\n\n";
						std::cout << "축하합니다 WIN!!!!\n";
						return 0;
					}
					std::cout << "→ 몬스터 처치! 이겼습니다\n";

					int mon = (rand() % 9) + 40;
					myHero.money += mon;
					std::cout << "[ HP: " << myHero.hp << "  Money: " << myHero.money << " ]" << "\n\n";

					shopEnter = true;
					break;
				}
				else //패배
				{
					std::cout << "→ 졌습니다\n";
					myHero.hp -= 20;
					if (myHero.hp <= 0)
					{
						std::cout << "[ HP: " << 0 << "  Money: " << myHero.money << " ]" << "\n\n";
						std::cout << "\n	---용사의 죽음---\n";
						return 0;
					}
					std::cout << "[ HP: " << myHero.hp << "  Money: " << myHero.money << " ]" << "\n\n";
					continue;
				}
			}
		}

		//상점 띄우기
		if (shopEnter == true)
		{
			std::cout << "\n--상점에 오신것을 환영합니다.--\n";
			std::cout << "판매목록→	[1] 전체 회복 (60골드)	[2] HP +10 (25골드)	[3] HP +20 (45골드)\n";
			int store;
			std::cout << "무엇을 구매하겠습니까: ";
			std::cin >> store;
			if (store == 1)	//1번 구매
			{
				if (myHero.money - 60 < 0)
				{
					std::cout << "잔액 부족, 돌아가쎄오\n";
					std::cout << "[ HP: " << myHero.hp << "  Money: " << myHero.money << " ]" << "\n\n";
					Sleep(1500);
				}
				else
				{
					myHero.hp += (myHero.totalHP - myHero.hp);
					myHero.money -= 60;
					std::cout << "\n 감사합니다	[ HP: " << myHero.hp << "  Money: " << myHero.money << " ]" << "\n\n";
					Sleep(1500);
				}
			}
			else if (store == 2)	//2번 구매
			{
				if (myHero.money - 25 < 0)
				{
					std::cout << "잔액 부족, 돌아가쎄오\n";
					std::cout << "[ HP: " << myHero.hp << "  Money: " << myHero.money << " ]" << "\n\n";
					Sleep(1500);
					break;
				}
				else 
				{
					if (myHero.hp + 10 > myHero.totalHP)
						myHero.hp += (myHero.totalHP - myHero.hp);
					else
						myHero.hp += 10;
					myHero.money -= 25;
					std::cout << "\n 감사합니다	[ HP: " << myHero.hp << "  Money: " << myHero.money << " ]" << "\n\n";
					Sleep(1500);
				}
			}
			else if (store == 3)	//3번 구매
			{
				if (myHero.money - 45 < 0)
				{
					std::cout << "잔액 부족, 돌아가쎄오\n";
					std::cout << "[ HP: " << myHero.hp << "  Money: " << myHero.money << " ]" << "\n\n";
					Sleep(1500);
					break;
				}
				else
				{
					if (myHero.hp + 20 > myHero.totalHP)
						myHero.hp += (myHero.totalHP - myHero.hp);
					else
						myHero.hp += 20;
					myHero.money -= 45;
					std::cout << "\n 감사합니다	[ HP: " << myHero.hp << "  Money: " << myHero.money << " ]" << "\n\n";
					Sleep(1500);
				}
			}
		}

		//maze 정보 변경
		maze[myHero.playerX][myHero.playerY] = ' ';	
		maze[nextX][nextY] = 'P';
		myHero.playerX = nextX, myHero.playerY = nextY;

		//화면 지우기
		std::system("cls");
	}

	return 0;
}