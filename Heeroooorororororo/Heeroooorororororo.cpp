#include <iostream>
#include <Windows.h>    //sleep
#include <cstdio>       //rand
#include <ctime>        //time
#include <conio.h>
#include <string>
//using namespace std;

//몬스터 공통 사용 정보
struct MonsterCommonData
{
	char mon_Icon[3] = { 'V','W','M' };
	int mon_Money[3] = { 30,50,80 };
	int mon_XP[3] = { 10,20,30 };
	int mon_totalHP[3] = { 1,2,3 };
	std::string mon_name[3] = {"숲", "늪", "땅"};
};

//몬스터 정보
struct MonsterData
{
	std::string name;
	int totalHP;
	int curHP;
	int monXP;
	int monMoney;
	char monIcon;
	int monsterX;
	int monsterY;
	bool isAlive= true;
}; 

//맵 정보
struct MazeData
{
	int ground[3] = { 0, 1, 2 };	//0(숲) 1(논) 2(땅)
	int width;
	int height;
	char icon[3] = { '+', '=', '-' };
	int totalMonster;
	int monsterNum;	//지금 맵에 총 몇마리?
};

//난이도
struct Difficulty
{
	std::string diff[3] = { "초급[1]", "중급[2]", "고급[3]" };
	int startHP[3] = { 150, 170, 200 };	//HeroData totalHP 결정
	int monsterNumLevel[3] = { 1, 2, 3 };	//1~3마리
	int maze[3][2] = { {5,10}, {10,15},{15,20} };	//MazeData width, height 결정
};

//영웅 정보
struct HeroData
{
	std::string heroName;
	int hp;
	int totalHP;
	int money = 0;
	int curDifficulty;
	int playerX = 1;
	int playerY = 1;
	int nextX;
	int nextY;
	int playerMove = 0;
	int xp = 0;
};

//상점 정보
struct StoreData
{
	std::string name[4] = { "HP +10", "HP +20" , "HP +30" , "전체 회복"};
	int recover[3] = { 10 , 20, 30 };	//+전체회복
	int cost[4] = { 50, 75, 100, 150 };
	int remain[4] = { 5, 3, 2, 1 };
};

//몬스터 이동
void moveMonster(Difficulty& myDiff, MazeData& myMaze, MonsterData* myMonster, char** maze, char** rememMaze, MonsterCommonData& myCommon)
{
	int dir[4][2] = { {-1,0}, {1,0}, {0,-1}, {0,1} };	//상하좌우

	for (int i = 0; i < myMaze.totalMonster; i++)	//각 몬스터 위치 찾기
	{
		if (myMonster[i].isAlive == true)		//아직 살아있는 몬스터라면
		{
			int curMonX = myMonster[i].monsterX;
			int curMonY = myMonster[i].monsterY;

			while (true)
			{
				int moveRand = rand() % 4;

				int nextMonX = curMonX + dir[moveRand][0];
				int nextMonY = curMonY + dir[moveRand][1];

				if (nextMonX < 2 || nextMonX > myMaze.height - 2 || nextMonY < 2 || nextMonY > myMaze.width - 2
					|| maze[nextMonX][nextMonY] == 'P' || maze[nextMonX][nextMonY] == myCommon.mon_Icon[0] 
					|| maze[nextMonX][nextMonY] == myCommon.mon_Icon[1] || maze[nextMonX][nextMonY] == myCommon.mon_Icon[2])	//이동하지 못하는 방향
				{
					continue;
				}
				else	//상하좌우 랜덤 한칸씩 이동
				{
					maze[curMonX][curMonY] = rememMaze[curMonX][curMonY];	//이동 하기 전 지형 복구

					if (rememMaze[nextMonX][nextMonY] == myMaze.icon[0])	//이동할 지형에 따른 몬스터 아이콘(종류)지정
						maze[nextMonX][nextMonY] = myCommon.mon_Icon[0];

					if (rememMaze[nextMonX][nextMonY] == myMaze.icon[1])
						maze[nextMonX][nextMonY] = myCommon.mon_Icon[1];

					if (rememMaze[nextMonX][nextMonY] == myMaze.icon[2])
						maze[nextMonX][nextMonY] = myCommon.mon_Icon[2];

					for (int j = 0; j < sizeof(myMaze.icon) / sizeof(char); j++)	//이동할 지형에 따라
					{
						if (rememMaze[nextMonX][nextMonY] == myMaze.icon[j])
						{
							maze[nextMonX][nextMonY] = myCommon.mon_Icon[j];	//몬스터 아이콘(종류)지정
							//변경된 몬스터 데이터
							myMonster[i].name = myCommon.mon_name[j];
							myMonster[i].totalHP = myCommon.mon_totalHP[j];
							myMonster[i].monXP = myCommon.mon_XP[j];
							myMonster[i].monMoney = myCommon.mon_Money[j];
							myMonster[i].monIcon = myCommon.mon_Icon[j];
						}
					}

					myMonster[i].monsterX += dir[moveRand][0];	//몬스터 위치 정보 수정 저장
					myMonster[i].monsterY += dir[moveRand][1];
				}
				break;	//이동했으면 탈출
			}
		}
	}
}

//상점 open
void store(HeroData& myHero, StoreData& myPosion)
{
	std::cout << "\n--상점에 오신것을 환영합니다.--\n";
	std::cout << "판매목록→	[1] HP +10 (50골드/" << myPosion.remain[0] << "개 남음)	[2] HP +20 (75골드/" << myPosion.remain[1] << "개 남음)	[3] HP +30 (100골드/" << myPosion.remain[2] << "개 남음)	[4] 전체 회복 (150골드/" << myPosion.remain[3] << "개 남음)\n";
	int choice;
	std::cout << "무엇을 구매하겠습니까: ";
	std::cin >> choice;

	if (myHero.money - myPosion.cost[choice - 1] < 0)	//돈이 부족하면
	{
		std::cout << "\n!! 잔액 부족, 돌아가쎄요\n";
		Sleep(1500);
	}
	else	//구매 가능 하면
	{
		if (myPosion.remain[choice - 1] <= 0)	//물약 개수 부족하면
		{
			std::cout << "물약 개수 부족.. 나가.....\n";
			Sleep(1500);
			return;
		}
		myPosion.remain[choice - 1]--;
		if (choice - 1 == 4)	// 전체 체력 회복
			myHero.hp = myHero.totalHP;
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

//난이도 + 시작 정보
void setDifficulty(Difficulty& myDiff, MazeData& myMaze, HeroData& myHero)
{
	int d;
	std::cout << "\n난이도를 선택해주세요: ";
	std::cout << "[ 초급(1) 중급(2) 고급(3) ]\n→ ";
	d = _getch();
	std::cout << myDiff.diff[((int)d - 48) - 1] << " 선택하였습니다\n\n";

	//난이도에 따른 시작 정보 설정
	myHero.hp = myDiff.startHP[((int)d - 48) - 1];
	myHero.totalHP = myDiff.startHP[((int)d - 48) - 1];
	myHero.curDifficulty = (int)d - 48;
	myMaze.monsterNum = myDiff.monsterNumLevel[((int)d - 48) - 1];
	myMaze.totalMonster = myDiff.monsterNumLevel[((int)d - 48) - 1];
	myMaze.height = myDiff.maze[((int)d - 48) - 1][0];
	myMaze.width = myDiff.maze[((int)d - 48) - 1][1];
}

//maze 지형 만들기
void makeMaze(MazeData& myMaze, char** maze, char** rememMaze)
{
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
}

//몬스터 위치 설정
void setMonster(MazeData &myMaze, char **maze, MonsterCommonData &myCommon, MonsterData* myMonster)
{
	int monX = 0, monY = 0;
	for (int i = 0; i < myMaze.totalMonster; i++)
	{
		monX = (rand() % (myMaze.height - 2)) + 1;//maze[1][1]~maze[8][13] 실사용 범위
		monY = (rand() % (myMaze.width - 2)) + 1;
		if (maze[monX][monY] != 'P' && maze[monX][monY] != myCommon.mon_Icon[0] && maze[monX][monY] != myCommon.mon_Icon[1] && maze[monX][monY] != myCommon.mon_Icon[2])	//이미ground 가 아닌 곳
		{
			//몬스터 포지션 지정
			myMonster[i].monsterX = monX;
			myMonster[i].monsterY = monY;

			//지형에 따른 몬스터 아이콘(종류)지정
			for (int j = 0; j < 3; j++)
			{
				if (maze[monX][monY] == myMaze.icon[j])
				{
					//몬스터 정보 저장
					myMonster[i].name = myCommon.mon_name[j];
					myMonster[i].totalHP = myCommon.mon_totalHP[j];
					myMonster[i].monXP = myCommon.mon_XP[j];
					myMonster[i].monMoney = myCommon.mon_Money[j];
					myMonster[i].monIcon = myCommon.mon_Icon[j];
					//맵에 몬스처 찍기
					maze[monX][monY] = myCommon.mon_Icon[j];
					break;
				}
			}
		}
	}
}

//가위바위보 시작
int fightStart(MonsterData* myMonster, HeroData& myHero, MazeData& myMaze, int curMon)
{
	std::cout << "\n \"" << myMonster[curMon].name << " 몬스터를 만났다! 가위바위보를 하자!! \"" << "(총 " << myMonster[curMon].totalHP << "번 이겨야함)\n\n";

	myMonster[curMon].curHP = myMonster[curMon].totalHP;
	while (myMonster[curMon].curHP > 0)	//몬스터 HP 0일 때 까지
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
			myMonster[curMon].curHP--;
			if (myMaze.monsterNum == 1 && myMonster[curMon].curHP == 0)	//승리, 게임 종료
			{
				std::cout << "→ 모든 몬스터 처치!~!~~!!!!\n\n";
				std::cout << "축하합니다 WIN!!!!\n";
				return 2;
			}
			std::cout << "→ 몬스터 처치! " << myMonster[curMon].curHP << "번 남았습니다\n";
			myMonster[curMon].isAlive = false; //이긴 몬스터 표시
			myHero.xp += myMonster[curMon].monXP;	//용사 xp 올려주기
			myHero.money += myMonster[curMon].monMoney;	//용사 동 올려주기
			std::cout << "[ HP: " << myHero.hp << "  Money: " << myHero.money << "  XP: " << myHero.xp << " ]\n\n";

			//storeEnter = true;
			if (myMonster[curMon].curHP == 0)
			{
				myMaze.monsterNum--;	//몬스터 총 처치 횟수 줄여주기
				return 1;
			}
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

//플레이어 이동
void playerMove(HeroData& myHero, MazeData& myMaze)
{
	int userInput = _getch();
	myHero.playerMove++;
	myHero.nextX = myHero.playerX;
	myHero.nextY = myHero.playerY;
	if ((char)userInput == 'w' && (myHero.playerX - 1) >= 1)  //키 조작
		myHero.nextX--;
	if ((char)userInput == 's' && (myHero.playerX + 1) <= myMaze.height - 2)
		myHero.nextX++;
	if ((char)userInput == 'a' && (myHero.playerY - 1) >= 1)
		myHero.nextY--;
	if ((char)userInput == 'd' && (myHero.playerY + 1) <= myMaze.width - 2)
		myHero.nextY++;
}

//maze 출력
void mazePrint(MazeData& myMaze, char** maze)
{
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
}

int main()
{
	HeroData myHero;
	Difficulty myDiff;
	MazeData myMaze;
	MonsterData* myMonster;
	MonsterCommonData myCommon;
	StoreData myPosion;

	//--------------정보 입력 화면--------------
	std::cout << "\n ---- 영웅은 절차적 ----\n\n";

	//영웅 이름 입력
	std::cout << "영웅 이름을 입력하세요: ";
	std::cin >> myHero.heroName;

	//난이도 선택
	setDifficulty(myDiff, myMaze, myHero);
	myMonster = new MonsterData[myMaze.monsterNum];	//몬스터 동적 배열 만들기
	
	//다음 화면으로 넘어가기
	std::cout << "<<     '" << myHero.heroName << "'님 게임을 시작합니다!     >>\n\n";
	Sleep(1000);
	std::system("cls");

	//--------------몬스터 배틀 화면--------------
	srand(time(NULL));

	//maze판 만들기(동적)
	char** maze = new char* [myMaze.height];
	char** rememMaze = new char* [myMaze.height];
	for (int i = 0; i < myMaze.height; i++)
	{
		maze[i] = new char[myMaze.width];
		rememMaze[i] = new char[myMaze.width];
	}
	makeMaze(myMaze, maze, rememMaze);	//maze 지형 만들기

	//플레이어 시작점 설정
	maze[myHero.playerX][myHero.playerY] = 'P';

	//몬스터 위치 설정
	setMonster(myMaze, maze, myCommon, myMonster);

	while (true)
	{
		std::cout << "\n    [남은 몬스터 수: " << myMaze.monsterNum << "마리]\n";
		std::cout << "\n    [ HP: " << myHero.hp << "  Money: " << myHero.money << "  XP: " << myHero.xp << " ]\n\n";
		
		//maze판 출력
		mazePrint(myMaze, maze);
		
		//플레이어 이동
		playerMove(myHero, myMaze);

		//몬스터 만난 경우
		bool fightEnter = false;
		int curMon = -1;
		for (int i = 0; i < myMaze.totalMonster; i++)
		{
			if (myMonster[i].monsterX == myHero.nextX && myMonster[i].monsterY == myHero.nextY && myMonster[i].isAlive == true)
			{
				fightEnter = true;
				curMon = i;	//몬스터 배열에서 몇번째 있는 몬스터 맞났는가?
				break;
			}
		}

		//베틀 시작
		int storeEnter = 0;
		if (fightEnter == true)
			storeEnter = fightStart(myMonster, myHero,myMaze, curMon);

		//상점 띄우기
		if (storeEnter == 1)
		{
			int checkStore;
			std::cout << "상점에 들어가시겠습니까?	YES(1) / No(0)	:";
			std::cin >> checkStore;
			if (checkStore == 1)
				store(myHero, myPosion);
		}
		else if (storeEnter == 2)	//몬스터 다 죽임
			return 0;

		//maze 정보 변경
		maze[myHero.playerX][myHero.playerY] = rememMaze[myHero.playerX][myHero.playerY];
		maze[myHero.nextX][myHero.nextY] = 'P';
		myHero.playerX = myHero.nextX, myHero.playerY = myHero.nextY;

		//화면 지우기
		std::system("cls");

		//플레이어 두번 이동 시 몬스터 한칸 이동
		if (myHero.playerMove % 2 == 0)
			moveMonster(myDiff, myMaze, myMonster, maze, rememMaze, myCommon);
	}

	return 0;
}
