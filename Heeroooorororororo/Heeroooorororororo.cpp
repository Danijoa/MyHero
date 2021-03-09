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
	std::string name[3] = { "숲", "늪", "땅" };
	int totalHP[3] = { 1, 2, 3 };
	int curHP;
	int monXP[3] = {10, 20, 30};
	int monMoney[3] = { 30, 50, 80};
	char monIcon[3] = { 'V', 'W', 'M' };
};

struct MazeData
{
	int ground[3] = {0, 1, 2};	//0(숲) 1(논) 2(땅)
	int width;
	int height;
	char icon[3] = { '+', '=', '-' };
};

struct Difficulty
{
	std::string diff[3] = { "초급[1]", "중급[2]", "고급[3]" };	
	int startHP[3] = { 150, 170, 200 };	//HeroData totalHP 결정
	int monsterNum;	//1~3마리
	int maze[3][2] = { {5,10}, {10,15},{15,20} };	//MazeData width, height 결정
};

struct HeroData
{
	std::string heroName;
	int hp;
	int totalHP;
	int money = 0;
	int curDifficulty;
	int playerX = 1;
	int playerY = 1;
	int playerMove = 0;
	int xp = 0;
};

struct potionData 
{
	std::string name[3] = { "전체 회복", "HP +10", "HP +20" };
	int cost[3] = { 60, 25, 45 };
	int recover[3] = { 0 , 10, 20 };
	int remain[3] = { 2, 5, 3 };
};

//몬스터 이동
void moveMonster(Difficulty &myDiff, MazeData &myMaze, MonsterData &myMonster, char **maze,char **rememMaze)
{
	int dir[4][2] = { {-1,0}, {1,0}, {0,-1}, {0,1} };	//상하좌우

	for (int i = 0; i < myDiff.monsterNum; i++)	//각 몬스터 위치 찾기
	{
		int curMonX = myMonster.pos[i][0];
		int curMonY = myMonster.pos[i][1];
		if (curMonX != -1 && curMonY != -1)		//이미 처리된 몬스터가 아니라면
		{
			while (true)
			{
				int moveRand = rand() % 4;
				int nextMonX = myMonster.pos[i][0] + dir[moveRand][0];
				int nextMonY = myMonster.pos[i][1] + dir[moveRand][1];
				if (nextMonX < 2 || nextMonX > myMaze.height - 2 || nextMonY < 2 || nextMonY > myMaze.width - 2
					|| maze[nextMonX][nextMonY] == myMonster.monIcon[0] || maze[nextMonX][nextMonY] == myMonster.monIcon[1] 
					|| maze[nextMonX][nextMonY] == myMonster.monIcon[2] || maze[nextMonX][nextMonY] == 'P')	//이동하지 못하는 방향
				{
					continue;
				}
				else	//상하좌우 랜덤 한칸씩 이동
				{
					maze[curMonX][curMonY] = rememMaze[curMonX][curMonY];	//이동 하기 전 지형 복구

					if (rememMaze[nextMonX][nextMonY] == myMaze.icon[0])	//이동할 지형에 따른 몬스터 아이콘(종류)지정
						maze[nextMonX][nextMonY] = myMonster.monIcon[0];
					if (rememMaze[nextMonX][nextMonY] == myMaze.icon[1])
						maze[nextMonX][nextMonY] = myMonster.monIcon[1];
					if (rememMaze[nextMonX][nextMonY] == myMaze.icon[2])
						maze[nextMonX][nextMonY] = myMonster.monIcon[2];

					myMonster.pos[i][0] += dir[moveRand][0];	//몬스터 위치 정보 수정 저장
					myMonster.pos[i][1] += dir[moveRand][1];
				}
				break;
			}
		}
	}
}

//상점 open
void store(HeroData &myHero, potionData &myPosion)
{
	std::cout << "\n--상점에 오신것을 환영합니다.--\n";
	std::cout << "판매목록→	[1] 전체 회복 (60골드/"<< myPosion.remain[0] <<"개 남음)	[2] HP +10 (25골드/"<< myPosion.remain[1] <<"개 남음)	[3] HP +20 (45골드/"<< myPosion.remain[2] <<"개 남음)	[4] 안사요.\n";
	int choice;
	std::cout << "무엇을 구매하겠습니까: ";
	std::cin >> choice;
	
	if (choice == 4)
		return;

	if (myHero.money - myPosion.cost[choice - 1] < 0)	//돈이 부족하면
	{
		std::cout << "\n!! 잔액 부족, 돌아가쎄요\n";
		Sleep(1500);
	}
	else	//구매 가능 하면
	{
		if (myPosion.remain[choice - 1]<=0)	//물약 개수 부족하면
		{
			std::cout << "물약 개수 부족.. 나가.....\n";
			Sleep(1500);
			return;
		}
		myPosion.remain[choice - 1]--;
		if (choice - 1 == 0)	// 전체 체력 회복
			myHero.hp += myHero.totalHP - myHero.hp;
		else	//그외
		{
			myHero.hp += myPosion.recover[choice - 1];
			if (myHero.hp > myHero.totalHP)
				myHero.hp = myHero.totalHP;
		}
		myHero.money -= myPosion.cost[choice - 1];	//돈 지불
		std::cout << "\n[ HP: " << myHero.hp << "  Money: " << myHero.money << "  XP: " << myHero.xp << " ]\n\n";
		std::cout << "감사합니다^-^";
		Sleep(1500);
	}
}

int main()
{
	HeroData myHero;
	Difficulty myDiff;
	MazeData myMaze;
	MonsterData myMonster;
	potionData myPosion;

	//--------------정보 입력 화면--------------
	std::cout << "\n ---- 영웅은 절차적 ----\n\n";

	//영웅 이름 입력
	std::cout << "영웅 이름을 입력하세요: ";
	std::cin >> myHero.heroName;

	//난이도 선택
	int d;
	std::cout << "\n난이도를 선택해주세요: ";
	std::cout << "[ 초급(1) 중급(2) 고급(3) ]\n→ ";
	d = _getch();
	std::cout << myDiff.diff[((int)d - 48) - 1] << " 선택하였습니다\n\n";

	//난이도에 따른 시작 정보 설정
	myHero.hp = myDiff.startHP[((int)d - 48) - 1];
	myHero.totalHP = myDiff.startHP[((int)d - 48) - 1];
	myHero.curDifficulty = (int)d - 48;
	myDiff.monsterNum = ((int)d - 48);
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
	char** rememMaze = new char* [myMaze.height];
	for (int i = 0; i < myMaze.height; i++)
	{
		maze[i] = new char[myMaze.width];
		rememMaze[i] = new char[myMaze.width];
	}
	for (int i = 0; i < myMaze.height; i++)
	{
		for (int j = 0; j < myMaze.width; j++)
		{
			if (i == 0 || i == (myMaze.height - 1) || j == 0 || j == (myMaze.width - 1))
			{
				maze[i][j] = 'O';
				rememMaze[i][j] = 'O';
			}
			else
			{
				int iconRand = rand() % 3;
				maze[i][j] = myMaze.icon[iconRand];
				rememMaze[i][j] = myMaze.icon[iconRand];
			}
		}
	}

	//플레이어 시작점 설정
	maze[myHero.playerX][myHero.playerY] = 'P';

	//몬스터 위치 설정
	int monX = 0, monY = 0;
	for(int i=0; i<myDiff.monsterNum; i++)
	{
		monX = (rand() % (myMaze.height - 2)) + 1;//maze[1][1]~maze[8][13] 실사용 범위
		monY = (rand() % (myMaze.width - 2)) + 1;
		if (maze[monX][monY] != 'P' && maze[monX][monY] != myMonster.monIcon[0] 
			&& maze[monX][monY] != myMonster.monIcon[1] && maze[monX][monY] != myMonster.monIcon[2])	//이미ground 가 아닌 곳
		{
			myMonster.pos[i][0] = monX;	//몬스터 포지션 지정
			myMonster.pos[i][1] = monY;
			if (maze[monX][monY] == myMaze.icon[0])	//지형에 따른 몬스터 아이콘(종류)지정
				maze[monX][monY] = myMonster.monIcon[0];
			if (maze[monX][monY] == myMaze.icon[1])	
				maze[monX][monY] = myMonster.monIcon[1];
			if (maze[monX][monY] == myMaze.icon[2])	
				maze[monX][monY] = myMonster.monIcon[2];
		}
	}

	while (true)
	{

		std::cout << "\n    [남은 몬스터 수: " << myDiff.monsterNum << "마리]\n";
		std::cout << "\n    [ HP: " << myHero.hp << "  Money: " << myHero.money << "  XP: "<< myHero.xp << " ]\n\n";
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
		bool fightEnter = false;
		int curMon = -1;
		if (maze[nextX][nextY] == myMonster.monIcon[0])
		{
			curMon = 0;
			fightEnter = true;
		}
		else if (maze[nextX][nextY] == myMonster.monIcon[1])
		{
			curMon = 1;
			fightEnter = true;
		}
		else if (maze[nextX][nextY] == myMonster.monIcon[2])
		{
			curMon = 2;
			fightEnter = true;
		}

		//베틀 시작
		bool storeEnter = false;
		if (fightEnter == true)
		{
			myDiff.monsterNum--;
			std::cout << "\n \""<< myMonster.name[curMon]<< " 몬스터를 만났다! 가위바위보를 하자!! \"" << "(총 " << myMonster.totalHP[curMon] << "번 이겨야함)\n\n";

			myMonster.curHP = myMonster.totalHP[curMon];
			while (myMonster.curHP > 0)	//몬스터 HP 0일 때 까지
			{
				//컴퓨터 가위 바위 보 입력 받기
				int comFight = (rand() % 3 + 1);
				std::cout << "컴퓨터의 선택:   ";
				if (comFight == 1)
					std::cout << "가위" << "\n";
				else if (comFight == 2)
					std::cout << "바위" << "\n";
				else if (comFight == 3)
					std::cout << "보" << "\n";

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

				//가위 바위 보 결과
				if ((comFight == 1 && playerFight == 1) || (comFight == 2 && playerFight == 2) || (comFight == 3 && playerFight == 3))	//무승부
				{
					std::cout << "→ 비겼습니다\n";
					std::cout << "[ HP: " << myHero.hp << "  Money: " << myHero.money << "  XP: " << myHero.xp << " ]\n\n";
					continue;
				}
				else if ((comFight == 1 && playerFight == 2) || (comFight == 2 && playerFight == 3) || (comFight == 3 && playerFight == 1))	//승리
				{
					if (myDiff.monsterNum == 0)
					{
						std::cout << "→ 모든 몬스터 처치!~!~~!!!!\n\n";
						std::cout << "축하합니다 WIN!!!!\n";
						return 0;
					}

					myMonster.curHP--;
					std::cout << "→ 몬스터 처치! " << myMonster.curHP << "번 남았습니다\n";

					for (int i = 0; i < myDiff.monsterNum; i++)	//이긴 몬스터 표시
					{
						if (myMonster.pos[i][0] == nextX && myMonster.pos[i][1] == nextY)
						{
							myMonster.pos[i][0] = -1;
							myMonster.pos[i][1] = -1;
							break;
						}
					}
					myHero.xp += myMonster.monXP[curMon];	//용사 xp 올려주기
					myHero.money += myMonster.monMoney[curMon];	//용사 동 올려주기
					std::cout << "[ HP: " << myHero.hp << "  Money: " << myHero.money << "  XP: " << myHero.xp << " ]\n\n";

					storeEnter = true;
					continue;
				}
				else //패배
				{
					std::cout << "→ 졌습니다\n";
					myHero.hp -= 20;
					if (myHero.hp <= 0)
					{
						std::cout << "[ HP: " << myHero.hp << "  Money: " << myHero.money << "  XP: " << myHero.xp << " ]\n\n";
						std::cout << "\n	---용사의 죽음---\n";
						return 0;
					}
					std::cout << "[ HP: " << myHero.hp << "  Money: " << myHero.money << "  XP: " << myHero.xp << " ]\n\n";
					continue;
				}
			}
		}

		//상점 띄우기
		if (storeEnter == true)
			store(myHero, myPosion);

		//maze 정보 변경
		maze[myHero.playerX][myHero.playerY] = rememMaze[myHero.playerX][myHero.playerY];
		maze[nextX][nextY] = 'P';
		myHero.playerX = nextX, myHero.playerY = nextY;

		//화면 지우기
		std::system("cls");

		//플레이어 두번 이동 시 몬스터 한칸 이동
		if (myHero.playerMove % 2 == 0)
			moveMonster(myDiff, myMaze, myMonster, maze, rememMaze);
	}


	return 0;
}
