// Heeroooorororororo.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <Windows.h>    //sleep
#include <cstdio>       //rand
#include <ctime>        //time
#include <conio.h>
#include <string>
//using namespace std;

struct MonsterData
{
	int pos[3][2];	//최대3마리
};

struct MazeData
{
	int width;
	int height;
};

struct Difficulty
{
	std::string diff[3] = { "초급[1]", "중급[2]", "고급[3]" };
	int startHP[3] = { 50, 70, 100 };
	int monsterNum;	//1~3마리
	int maze[3][2] = { {5,10}, {10,15},{15,20} };
};

struct HeroData
{
	std::string heroName;
	int hp;
	int totalHP; 
	int money = 0;
	int playerX = 1;
	int playerY = 1;
	int playerMove = 0;
};

int main()
{
	//--------------정보 입력 화면--------------
	std::cout << "\n ---- 영웅은 절차적 ----\n\n";

	//영웅 이름 입력
	HeroData myHero;
	std::cout << "영웅 이름을 입력하세요: ";
	std::cin >> myHero.heroName;

	//난이도 선택
	Difficulty myDiff;
	int d;
	std::cout << "\n난이도를 선택해주세요: ";
	std::cout << "[ 초급(1) 중급(2) 고급(3) ]\n→ ";
	d = _getch();
	std::cout << myDiff.diff[((int)d - 48) - 1] << " 선택하였습니다\n\n";

	//난이도에 따른 시작 정보 설정
	myHero.hp = myDiff.startHP[((int)d - 48) - 1];
	myHero.totalHP = myDiff.startHP[((int)d - 48) - 1];
	myDiff.monsterNum = ((int)d - 48);	
	MazeData myMaze;
	myMaze.height = myDiff.maze[((int)d - 48) - 1][0];
	myMaze.width = myDiff.maze[((int)d - 48) - 1][1];
	
	//다음 화면으로 넘어가기
	std::cout << "<<     '" << myHero.heroName << "'님 게임을 시작합니다!     >>\n\n";
	Sleep(1000);
	std::system("cls");

	//--------------몬스터 배틀 화면--------------
	srand(time(NULL));

	//maze판 만들기
	char** maze = new char* [myMaze.height];
	for (int i = 0; i < myMaze.height; i++)
	{
		maze[i] = new char[myMaze.width];
	}
	for (int i = 0; i < myMaze.height; i++)
	{
		for (int j = 0; j < myMaze.width; j++)
		{
			if (i == 0 || i == (myMaze.height - 1) || j == 0 || j == (myMaze.width - 1))
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
	MonsterData myMonster;
	while (monNumCnt < myDiff.monsterNum)	//maze[1][1]~maze[8][13] 실사용 범위
	{
		monX = (rand() % (myMaze.height - 2)) + 1;
		monY = (rand() % (myMaze.width - 2)) + 1;
		if (maze[monX][monY] != 'P' && maze[monX][monY] != 'M')
		{
			myMonster.pos[monNumCnt][0] = monX;
			myMonster.pos[monNumCnt][1] = monY;
			monNumCnt++;
			maze[monX][monY] = 'M';
		}
	}

	while (true)
	{
		//maze판 출력
		std::cout << "\n";
		for (int i = 0; i < myMaze.height; i++)
		{
			std::cout << "    ";
			for (int j = 0; j < myMaze.width; j++)
			{
				std::cout << maze[i][j] << " ";
			}
			std::cout << "\n";
		}
		std::cout << "\n";

		//플레이어 이동
		int userInput = _getch();
		myHero.playerMove++;
		int nextX = myHero.playerX;
		int nextY = myHero.playerY;
		if ((char)userInput == 'w' && (myHero.playerX - 1) >= 1)  //키 조작
			nextX = myHero.playerX - 1;
		if ((char)userInput == 's' && (myHero.playerX + 1) <= myMaze.height - 2)
			nextX = myHero.playerX + 1;
		if ((char)userInput == 'a' && (myHero.playerY - 1) >= 1)
			nextY = myHero.playerY - 1;
		if ((char)userInput == 'd' && (myHero.playerY + 1) <= myMaze.width - 2)
			nextY = myHero.playerY + 1;

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
		for (int i = 0; i < myDiff.monsterNum; i++)
		{
			if (nextX == myMonster.pos[i][0] && nextY == myMonster.pos[i][1])
			{
				myMonster.pos[i][0] = -1;
				myMonster.pos[i][1] = -1;
				break;
			}
		}
		myHero.playerX = nextX, myHero.playerY = nextY;

		//화면 지우기
		std::system("cls");

		//플레이어 두번 이동 시 몬스터 한칸 이동
		int dir[4][2] = { {-1,0}, {1,0}, {0,-1}, {0,1} };	//상하좌우
		if (myHero.playerMove % 2 == 0)
		{
			for (int i = 0; i < myDiff.monsterNum; i++)
			{				
				int curMonX = myMonster.pos[i][0];
				int curMonY = myMonster.pos[i][1];
				if (curMonX != -1 && curMonY != -1)
				{
					while (true)
					{
						int moveRand = rand() % 4;
						int nextMonX = myMonster.pos[i][0] + dir[moveRand][0];
						int nextMonY = myMonster.pos[i][1] + dir[moveRand][1];
						if (nextMonX < 2 || nextMonX > myMaze.height - 2 || nextMonY < 2 || nextMonY > myMaze.width - 2 
							|| maze[nextMonX][nextMonY] == 'M' || maze[nextMonX][nextMonY] == 'P')	//이동하지 못하는 경우
						{
							continue;
						}
						else
						{
							maze[curMonX][curMonY] = ' ';
							maze[nextMonX][nextMonY] = 'M';
							myMonster.pos[i][0] += dir[moveRand][0];
							myMonster.pos[i][1] += dir[moveRand][1];
						}
						break;
					}
				}
			}
		}
	}

	return 0;
}
