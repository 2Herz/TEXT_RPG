#include <iostream>
#include <windows.h>
using namespace std;

#define NAME_SIZE 64		// 이름의 크기
#define ITEM_DESC_LENGTH 1024		//	아이템 설명길이의 크기
#define INVENTORY_MAX 20		//	인벤토리 최대칸 수
#define STORE_WEAPON_MAX 3	//	무기상점에서 파는 무기의 최대 수
#define STORE_ARMOR_MAX 3	//	방어구상점에서 파는 방어구의 최대 수
#define LEVEL_MAX 15				//	레벨 최대 범위

/*
	메뉴구성과 변수설정으 편의성을 위해
	enum문과 구조체를 사용하였습니다.
*/
void	GotoXY(short PosX, short PosY)
{
	COORD	pos = { PosX, PosY };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

enum MAIN_MENU					//	로비화면
{
	MENU_NONE,
	MENU_MAP,
	MENU_STORE,
	MENU_INVENTORY,
	MENU_EXIT
};

enum MAP						//	맵(난이도별)
{
	MAP_NONE,
	MAP_EASY,
	MAP_NORMAL,
	MAP_HARD,
	MAP_BACK
};

enum JOB						//	직업
{
	JOB_NONE,
	JOB_KNIGHT,
	JOB_ARCHER,
	JOB_WIZARD,
	JOB_END
};

enum BATTLE			// 전투
{
	BATTLE_NONE,
	BATTLE_ATTACK,
	BATTLE_BACK
};

enum ITEM			// 아이템
{
	ITEM_WEAPON,
	ITEM_ARMOR,
	ITEM_BACK
};

enum STORE_MENU		// 상점
{
	S_MENU_NONE,
	S_MENU_WEAPON,
	S_MENU_ARMOR,
	S_MENU_BACK
};

enum EQUIP			// 장비착용
{
	EQ_NONE,
	EQ_WEAPON,
	EQ_ARMOR,
	EQ_MAX
};

struct _tagItem				// 아이템 정보를 담은 구조체
{
	char		strName[NAME_SIZE];
	char		strItem[NAME_SIZE];
	ITEM	eitem;
	int	Min;
	int	Max;
	int	Price;
	int	Sell;
	char strDesc[ITEM_DESC_LENGTH];
};

struct _tagInventory			// 인벤토리 정보를 담은 구조체
{
	_tagItem		item[INVENTORY_MAX];
	int		ItemCount;
	int		Gold;
};

struct _tagPlayer				//	플레이어의 정보를 담은 구조체
{
	char strName[NAME_SIZE];		//	캐릭터 이름
	char strJobName[NAME_SIZE];	//	캐릭터 직업이름
	JOB		ejob;								//	직업
	int		AttackMin;						//	공격력 최솟값
	int		AttackMax;						//	공격력 최댓값
	int		ArmorMin;						//	방어력 최솟값
	int		ArmorMax;						//	방어력 최댓값
	int		HP;									//	체력
	int		HPMax;							//	체력의 최댓값
	int		MP;									//	마나
	int		MPMax;							//	마나의 최댓값
	int		Exp;									//	경험치
	int		Level;								//	레벨
	_tagItem	Equip[EQ_MAX];
	bool		bEquip[EQ_MAX];
	_tagInventory		inventory;
};

struct _tagLevelUp				// 레벨업 정보를 담은 구조체
{
	int	AttackMin;
	int	AttackMax;
	int	ArmorMin;
	int	ArmorMax;
	int	HPMin;
	int	HPMax;
	int	MPMin;
	int	MPMax;
};

struct _tagMonster							// 몬스터의 정보를 담을 구조체
{
	char	strName[NAME_SIZE];		// 몬스터 이름
	int		AttackMin;						//	최소공격력
	int		AttackMax;						//	최대공격력
	int		ArmorMin;						//	최소방어력
	int		ArmorMax;						//	최대방어력
	int		HP;									//	체력
	int		HPMax;							//	최대체력
	int		MP;									//	마나
	int		MPMax;							//	최대마나
	int		Level;								//	몬스터 레벨
	int		GExp;								//	몬스터가 주는 획득 경험치
	int		GoldMin;							//	몬스터에게서 드랍되는 최소 골드
	int		GoldMax;							//	몬스터에게서 드랍되는 최대 골드
};

int main()
{
	srand(GetTickCount());

	const int	LevelUpExp[LEVEL_MAX] = { 4000, 10000, 20000, 35000, 50000, 70000, 100000, 150000, 200000, 400000, 800000, 1000000, 1500000, 2000000, 2500000 };

	_tagLevelUp	LvUpTable[JOB_END - 1] = {};

	LvUpTable[JOB_KNIGHT - 1].AttackMin = 500;
	LvUpTable[JOB_KNIGHT - 1].AttackMax = 900;
	LvUpTable[JOB_KNIGHT - 1].ArmorMin = 500;
	LvUpTable[JOB_KNIGHT - 1].ArmorMax = 1000;
	LvUpTable[JOB_KNIGHT - 1].HPMin = 500;
	LvUpTable[JOB_KNIGHT - 1].HPMax = 1000;
	LvUpTable[JOB_KNIGHT - 1].MPMin = 300;
	LvUpTable[JOB_KNIGHT - 1].MPMax = 600;

	LvUpTable[JOB_ARCHER - 1].AttackMin = 300;
	LvUpTable[JOB_ARCHER - 1].AttackMax = 600;
	LvUpTable[JOB_ARCHER - 1].ArmorMin = 250;
	LvUpTable[JOB_ARCHER - 1].ArmorMax = 500;
	LvUpTable[JOB_ARCHER - 1].HPMin = 300;
	LvUpTable[JOB_ARCHER - 1].HPMax = 600;
	LvUpTable[JOB_ARCHER - 1].MPMin = 300;
	LvUpTable[JOB_ARCHER - 1].MPMax = 500;

	LvUpTable[JOB_WIZARD - 1].AttackMin = 250;
	LvUpTable[JOB_WIZARD - 1].AttackMax = 500;
	LvUpTable[JOB_WIZARD - 1].ArmorMin = 300;
	LvUpTable[JOB_WIZARD - 1].ArmorMax = 700;
	LvUpTable[JOB_WIZARD - 1].HPMin = 400;
	LvUpTable[JOB_WIZARD - 1].HPMax = 800;
	LvUpTable[JOB_WIZARD - 1].MPMin = 500;
	LvUpTable[JOB_WIZARD - 1].MPMax = 1500;

	//	게임 시작 시 플레이어 정보를 설정
	_tagPlayer	player = {};

	//	플레이어의 이름을 입력받는다.
	GotoXY(25, 0);
	cout << "이름 : ";
	cin.getline(player.strName, NAME_SIZE - 1);

	int Job = JOB_NONE;
	while (Job == JOB_NONE)
	{
		system("cls");
		GotoXY(23, 0);
		cout << "1.전사" << endl;
		GotoXY(23, 1);
		cout << "2.궁수" << endl;
		GotoXY(23, 2);
		cout << "3.마법사" << endl;
		GotoXY(23, 3);
		cout << "SELECT YOUR JOB : ";
		cin >> Job;

		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1024, '\n');
			continue;
		}
		else if (Job <= JOB_NONE || Job >= JOB_END)
			Job = JOB_NONE;
	}
	player.Level = 1;
	player.Exp = 0;
	player.ejob = (JOB)Job;
	player.inventory.Gold = 100000;

	switch (player.ejob)
	{
	case JOB_KNIGHT:
		strcpy_s(player.strName, "전사");
		player.AttackMin = 250;
		player.AttackMax = 1250;
		player.ArmorMin = 500;
		player.ArmorMax = 1000;
		player.HPMax = 2500;
		player.MPMax = 1800;
		player.HP = 2500;
		player.MP = 2500;
		break;
	case JOB_ARCHER:
		strcpy_s(player.strName, "궁수");
		player.AttackMin = 200;
		player.AttackMax = 1200;
		player.ArmorMin = 300;
		player.ArmorMax = 900;
		player.HPMax = 2000;
		player.MPMax = 1500;
		player.HP = 2000;
		player.MP = 1500;
		break;
	case JOB_WIZARD:
		strcpy_s(player.strName, "마법사");
		player.AttackMin = 300;
		player.AttackMax = 1500;
		player.ArmorMin = 450;
		player.ArmorMax = 1000;
		player.HPMax = 2500;
		player.MPMax = 5000;
		player.HP = 2500;
		player.MP = 5000;
		break;
	}

	//	몬스터 생성
	_tagMonster		monsterArr[MAP_BACK - 1] = {};

	//	셰뷔르(Schevirr)
	strcpy_s(monsterArr[0].strName, "셰뷔르");
	monsterArr[0].AttackMin = 200;
	monsterArr[0].AttackMax = 600;
	monsterArr[0].ArmorMin = 50;
	monsterArr[0].ArmorMax = 100;
	monsterArr[0].HPMax = 3500;
	monsterArr[0].MPMax = 3500;
	monsterArr[0].HP = 3500;
	monsterArr[0].MP = 3500;
	monsterArr[0].Level = 1;
	monsterArr[0].GExp = 1000;
	monsterArr[0].GoldMin = 2000;
	monsterArr[0].GoldMax = 7000;

	//	파이뉘슈(Painuisch)
	strcpy_s(monsterArr[1].strName, "파이뉘슈");
	monsterArr[1].AttackMin = 400;
	monsterArr[1].AttackMax = 800;
	monsterArr[1].ArmorMin = 500;
	monsterArr[1].ArmorMax = 2500;
	monsterArr[1].HPMax = 7000;
	monsterArr[1].MPMax = 7000;
	monsterArr[1].HP = 7000;
	monsterArr[1].MP = 7000;
	monsterArr[1].Level = 30;
	monsterArr[1].GExp = 10000;
	monsterArr[1].GoldMin = 6000;
	monsterArr[1].GoldMax = 15000;

	//	보스 - 뷔르겐(Burgen)
	strcpy_s(monsterArr[2].strName, "뷔르겐");
	monsterArr[2].AttackMin = 4000;
	monsterArr[2].AttackMax = 8000;
	monsterArr[2].ArmorMin = 30000;
	monsterArr[2].ArmorMax = 100000;
	monsterArr[2].HPMax = 1500000;
	monsterArr[2].MPMax = 1500000;
	monsterArr[2].HP = 1500000;
	monsterArr[2].MP = 1500000;
	monsterArr[2].Level = 50;
	monsterArr[2].GExp = 300000;
	monsterArr[2].GoldMin = 20000;
	monsterArr[2].GoldMax = 300000;

	//	상점에서 판매할 아이템 목록들을 생성해준다.
	_tagItem		weapon[STORE_WEAPON_MAX] = {};
	_tagItem		armor[STORE_ARMOR_MAX] = {};

	//	무기정보
	strcpy_s(weapon[0].strName, "뚝배기");
	strcpy_s(weapon[0].strItem, "활");
	strcpy_s(weapon[0].strDesc, "이 무기는 뚝배기, 상대방의 뚝배기를 날려버린다..");
	weapon[0].Min = 250;
	weapon[0].Max = 500;
	weapon[0].Price = 3000;
	weapon[0].Sell = 2500;

	strcpy_s(weapon[1].strName, "몽키스페너");
	strcpy_s(weapon[1].strItem, "지팡이");
	strcpy_s(weapon[1].strDesc, "이 무기는 전문수리공들이 쓰는 몽키스페너, 상대방을 과감하게 조져버린다..");
	weapon[1].Min = 150;
	weapon[1].Max = 300;
	weapon[1].Price = 2000;
	weapon[1].Sell = 1000;

	strcpy_s(weapon[2].strName, "엑스칼리버");
	strcpy_s(weapon[2].strItem, "검");
	strcpy_s(weapon[2].strDesc, "전설상에 존재하는 성검, 다 찢어발긴다..");
	weapon[2].Min = 2000;
	weapon[2].Max = 4000;
	weapon[2].Price = 6000;
	weapon[2].Sell = 3000;
	//	방어구
	strcpy_s(armor[0].strName, "가마솥뚜껑");
	strcpy_s(armor[0].strItem, "방어구");
	strcpy_s(armor[0].strDesc, "예로부터 내려온 전통적인 방어구이다, 내 자신의 뚝배기정도는 지킬 수 있다.");
	armor[0].Min = 500;
	armor[0].Max = 1000;
	armor[0].Price = 4000;
	armor[0].Sell = 2500;

	strcpy_s(armor[1].strName, "철갑옷");
	strcpy_s(armor[1].strItem, "방어구");
	strcpy_s(armor[1].strDesc, "철로 만든 갑옷이다, 준수한 방어력을 갖췄다.");
	armor[1].Min = 500;
	armor[1].Max = 1000;
	armor[1].Price = 6000;
	armor[1].Sell = 3000;

	strcpy_s(armor[2].strName, "절대방패");
	strcpy_s(armor[2].strItem, "방어구");
	strcpy_s(armor[2].strDesc, "이름 그대로 극강의 방어력을 보여준다.");
	armor[2].Min = 2500;
	armor[2].Max = 5000;
	armor[2].Price = 10000;
	armor[2].Sell = 5000;

	while (true)
	{
		system("cls");
		GotoXY(25, 0);
		cout << "1.맵  2.상점  3.가방  4.종료" << endl;
		GotoXY(20, 2);
		cout << "SELECT MENU : ";
		int menu;
		cin >> menu;

		// 오입력 방지기능입니다.
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(1024, '\n');
			continue;
		}

		if (menu == MENU_EXIT)
			break;

		switch (menu)
		{
		case MENU_MAP:			// 맵
			while (true)
			{
				system("cls");
				GotoXY(35, 0);
				cout << " *** 맵 *** " << endl;
				GotoXY(23, 1);
				cout << "1.쉬움" << endl;
				GotoXY(23, 2);
				cout << "2.보통" << endl;
				GotoXY(23, 3);
				cout << "3.어려움" << endl;
				GotoXY(23, 4);
				cout << "4.뒤로가기" << endl;
				GotoXY(23, 5);
				cout << "SELECT MAP : ";
				cin >> menu;

				if (cin.fail())
				{
					cin.clear();
					cin.ignore(1024, '\n');
					continue;
				}

				if (menu == MAP_BACK)
					break;

				_tagMonster	monster = monsterArr[menu - 1];

				while (true)
				{
					system("cls");
					switch (menu)
					{
					case MAP_EASY:
						GotoXY(22, 0);
						cout << " ************* EASY : STAGE 1 ************ " << endl;
						break;
					case MAP_NORMAL:
						GotoXY(22, 0);
						cout << " ***** NORMAL  : STAGE - MIDDLE BOSS ***** " << endl;
						break;
					case MAP_HARD:
						GotoXY(22, 0);
						cout << " ******* HARD : STAGE - LAST BOSS ******* " << endl;
						break;
					}
					//	플레이어 정보 출력
					GotoXY(23, 1);
					cout << "=================PLAYER=================" << endl;
					GotoXY(23, 2);
					cout << "이름 : " << player.strName << "\t직업" <<
						player.strJobName << endl;
					GotoXY(23, 3);
					cout << "레벨 : " << player.Level << "\t 경험치 : " <<
						player.Exp << endl;

					if (player.bEquip[EQ_WEAPON] == true)
					{
						GotoXY(23, 4);
						cout << "공격력 : " << player.AttackMin << " + " <<
							player.Equip[EQ_WEAPON].Min << " - " <<
							player.AttackMax << " + " << player.Equip[EQ_WEAPON].Max;
					}
					else
					{
						GotoXY(23, 4);
						cout << "공격력 : " << player.AttackMin << " - " <<
							player.AttackMax;
					}
					if (player.bEquip[EQ_ARMOR] == true)
					{
						GotoXY(23, 5);
						cout << "JP : " << player.inventory.Gold << " Gold" << endl;
						cout << "\t방어력 : " << player.ArmorMin << " + " <<
							player.Equip[EQ_ARMOR].Min << " - " <<
							player.ArmorMax << " + " << player.Equip[EQ_ARMOR].Max << endl;
					}
					else
					{
						cout << "\t방어력 : " << player.ArmorMin << " - " <<
							player.ArmorMax << endl;
					}
					GotoXY(23, 6);
					cout << "체력 : " << player.HP << " / " << player.HPMax <<
						"\t마나 : " << player.MP << " / " << player.MPMax << endl;
					GotoXY(23, 7);
					cout << "JP : " << player.inventory.Gold << " Gold" << endl;

					if (player.bEquip[EQ_WEAPON])
					{
						GotoXY(23, 10);
						cout << "장착무기 : " << player.Equip[EQ_WEAPON].strName;
					}
					else
					{
						GotoXY(23, 11);
						cout << "장착무기 : 없음";
					}
					if (player.bEquip[EQ_ARMOR])
					{
						GotoXY(23, 12);
						cout << "\t장착방어구 : " << player.Equip[EQ_ARMOR].strName << endl;
					}
					else
					{
						GotoXY(23, 13);
						cout << "\t장착방어구 : 없음" << endl;
					}

					//	몬스터 정보 출력
					GotoXY(23, 9);
					cout << "=================MONSTER=================" << endl;
					GotoXY(23, 10);
					cout << "이름 : " << monster.strName << "\t 레벨 : " << monster.Level << endl;
					GotoXY(23, 11);
					cout << "공격력 : " << monster.AttackMin << " - " <<
						monster.AttackMax << "\t 방어력 : " << monster.ArmorMin << " - " <<
						monster.ArmorMax << endl;
					GotoXY(23, 12);
					cout << "체력 : " << monster.HP << " / " << monster.HPMax << "\t 마나 : " <<
						monster.MP << " / " << monster.MPMax << endl;
					GotoXY(23, 13);
					cout << "획득경험치 : " << monster.GExp << "\t 획득골드 : " << monster.GoldMin << " - "
						<< monster.GoldMax << endl;
					GotoXY(23, 14);
					cout << "========================================" << endl << endl;

					GotoXY(23, 17);
					cout << "메뉴를 선택하세요";
					GotoXY(23, 15);
					cout << "1. 공격" << endl;
					GotoXY(23, 16);
					cout << "2.도망가기" << endl;
					GotoXY(41, 17);
					cin >> menu;

					if (cin.fail())
					{
						cin.clear();
						cin.ignore(1024, '\n');
						continue;
					}

					else if (menu == BATTLE_BACK)
						break;

					else if (menu == BATTLE_BACK)
						break;

					switch (menu)
					{
					case BATTLE_ATTACK:		//	전투
					{
						int	AttackMin = player.AttackMin;
						int	AttackMax = player.AttackMax;

						// 무기를 장착하고 있을 경우 무기와 Min, Max를 더해준다.
						if (player.bEquip[EQ_WEAPON])
						{
							AttackMin += player.Equip[EQ_WEAPON].Min;
							AttackMax += player.Equip[EQ_WEAPON].Max;
						}

						int	Attack = rand() % (AttackMax - AttackMin + 1) +
							AttackMin;
						int	Armor = rand() % (monster.ArmorMax - monster.ArmorMin + 1) +
							monster.ArmorMin;

						int	Damage = Attack - Armor;

						Damage = Damage < 1 ? 1 : Damage;

						// 몬스터 HP를 감소시킨다.
						monster.HP -= Damage;
						GotoXY(23, 18);
						cout << player.strName << "가 " << monster.strName <<
							"에게 " << Damage << " 피해를 입혔습니다." << endl;

						// 몬스터가 죽었을 경우를 처리한다.
						if (monster.HP <= 0)
						{
							GotoXY(23, 19);
							cout << monster.strName << " 몬스터가 사망하였습니다." << endl;

							player.Exp += monster.GExp;
							int	Gold = (rand() % (monster.GoldMax - monster.GoldMin + 1) +
								monster.GoldMin);
							player.inventory.Gold += Gold;
							GotoXY(23, 20);
							cout << monster.GExp << " 경험치를 획득하였습니다." << endl;
							GotoXY(23, 21);
							cout << Gold << " Gold를 획득하였습니다." << endl;

							monster.HP = monster.HPMax;
							monster.MP = monster.MPMax;

							// 레벨업을 했는지 체크해본다.
							if (player.Exp >= LevelUpExp[player.Level - 1])
							{
								// 플레이어 경험치를 레벨업에 필요한 경험치만큼 차감한다.
								player.Exp -= LevelUpExp[player.Level - 1];

								// 레벨을 증가시킨다.
								++player.Level;
								GotoXY(23, 22);
								cout << "레벨업 하였습니다." << endl;

								// 능력치를 상승시킨다.
								// 직업 인덱스를 구한다.
								int	JobIndex = player.ejob - 1;
								int	HPUp = rand() % (LvUpTable[JobIndex].HPMax - LvUpTable[JobIndex].HPMin + 1) +
									LvUpTable[JobIndex].HPMin;
								int	MPUp = rand() % (LvUpTable[JobIndex].MPMax - LvUpTable[JobIndex].MPMin + 1) +
									LvUpTable[JobIndex].MPMin;

								player.AttackMin += LvUpTable[JobIndex].AttackMin;
								player.AttackMax += LvUpTable[JobIndex].AttackMax;
								player.ArmorMin += LvUpTable[JobIndex].ArmorMin;
								player.ArmorMax += LvUpTable[JobIndex].ArmorMax;

								player.HPMax += HPUp;
								player.MPMax += MPUp;

								// 체력과 마나를 회복시킨다.
								player.HP = player.HPMax;
								player.MP = player.MPMax;
							}
							system("pause");
							break;
						}

						// 몬스터가 살아있다면 플레이어를 공격한다.
						Attack = rand() % (monster.AttackMax - monster.AttackMin + 1) +
							monster.AttackMin;

						int	ArmorMin = player.ArmorMin;
						int	ArmorMax = player.ArmorMax;

						if (player.bEquip[EQ_ARMOR])
						{
							ArmorMin += player.Equip[EQ_ARMOR].Min;
							ArmorMax += player.Equip[EQ_ARMOR].Max;
						}

						Armor = rand() % (ArmorMax - ArmorMin + 1) +
							ArmorMin;

						Damage = Attack - Armor;

						Damage = Damage < 1 ? 1 : Damage;

						// 플레이어의 HP를 감소시킨다.
						player.HP -= Damage;
						GotoXY(23, 19);
						cout << monster.strName << "가 " << player.strName <<
							"에게 " << Damage << " 피해를 입혔습니다." << endl;

						// 플레이어가 죽었을 경우
						if (player.HP <= 0)
						{
							cout << player.strName << " 플레이어가 사망하였습니다." << endl;

							int	Exp = player.Exp * 0.1f;
							int	Gold = player.inventory.Gold * 0.1f;

							player.Exp -= Exp;
							player.inventory.Gold -= Gold;

							cout << Exp << " 경험치를 잃었습니다." << endl;
							cout << Gold << " Gold를 잃었습니다." << endl;

							// 플레이어의 HP와 MP를 회복한다.
							player.HP = player.HPMax;
							player.MP = player.MPMax;
						}
						GotoXY(23, 21);
						system("pause");
					}
					break;
					}
				}
			}
			break;
		case MENU_STORE:				//	상점
			while (true)
			{
				system("cls");
				GotoXY(25, 0);
				cout << "***************상점***************" << endl;
				GotoXY(25, 1);
				cout << "상점을 선택하세요 : ";
				GotoXY(25, 2);
				cout << "1.무기상점" << endl;
				GotoXY(25, 3);
				cout << "2.방어구상점" << endl;
				GotoXY(25, 4);
				cout << "3.뒤로가기" << endl;
				GotoXY(45, 1);
				cin >> menu;
				if (cin.fail())
				{
					cin.clear();
					cin.ignore(1024, '\n');
					continue;
				}

				else if (menu == S_MENU_BACK)
					break;

				switch (menu)
				{
				case S_MENU_WEAPON:
					while (true)
					{
						system("cls");
						cout << "***************무기상점***************" << endl;
						for (int i = 0; i < STORE_WEAPON_MAX; ++i)
						{
							cout << i + 1 << ". 이름 : " << weapon[i].strName <<
								"\t종류 : " << weapon[i].strItem << endl;
							cout << "공격력 : " << weapon[i].Min << " - " <<
								weapon[i].Max << endl;
							cout << "판매가격 : " << weapon[i].Price <<
								"\t구매가격 : " << weapon[i].Sell << endl;
							cout << "설명 : " << weapon[i].strDesc << endl << endl;
						}

						cout << STORE_WEAPON_MAX + 1 << ". 뒤로가기" << endl;
						cout << "보유금액 : " << player.inventory.Gold << " Gold" << endl;
						cout << "남은공간 : " << INVENTORY_MAX - player.inventory.ItemCount << endl;
						cout << "구입할 아이템을 선택하세요 : ";
						cin >> menu;

						if (cin.fail())
						{
							cin.clear();
							cin.ignore(1024, '\n');
							continue;
						}

						else if (menu == STORE_WEAPON_MAX + 1)
							break;

						else if (menu < 1 || menu > STORE_WEAPON_MAX + 1)
						{
							cout << "잘못 선택하였습니다." << endl;
							system("pause");
							continue;
						}

						// 상점판매목록 배열의 인덱스를 구해준다.
						int	weaponindex = menu - 1;

						// 인벤토리가 꽉 찼는지 검사한다.
						if (player.inventory.ItemCount == INVENTORY_MAX)
						{
							cout << "가방이 꽉 찼습니다." << endl;
							system("pause");
							continue;
						}

						// 돈이 부족할 경우
						else if (player.inventory.Gold < weapon[weaponindex].Price)
						{
							cout << "보유금액이 부족합니다." << endl;
							system("pause");
							continue;
						}

						// 처음에 iItemCount는 하나도 추가되어있지 않기 때문에 0으로 초기화
						// 되어 있으므로 0번 인덱스에 구매한 아이템을 추가하게 된다. 그리고
						// 카운트가 1이 된다. 다음번에 추가할때는 1번 인덱스에 추가하게된다.
						player.inventory.item[player.inventory.ItemCount] =
							weapon[weaponindex];
						++player.inventory.ItemCount;

						// 골드를 차감한다.
						player.inventory.Gold -= weapon[weaponindex].Price;

						cout << weapon[weaponindex].strName << " 아이템을 구매하였습니다." << endl;
						system("pause");
					}
					break;
				case S_MENU_ARMOR:
					while (true)
					{
						system("cls");
						cout << "***************방어구상점***************" << endl;
						for (int i = 0; i < STORE_ARMOR_MAX; ++i)
						{
							cout << i + 1 << "이름 : " << armor[i].strName <<
								"\t 종류 : " << armor[i].strItem << endl;
							cout << "공격력 : " << armor[i].Min << " - " <<
								armor[i].Max << endl;
							cout << "판매가격 : " << armor[i].Price <<
								"\t 구매가격 : " << armor[i].Sell << endl;
							cout << "설명 : " << armor[i].strDesc << endl;
						}
						cout << STORE_ARMOR_MAX + 1 << " : 뒤로가기" << endl;
						cout << "보유금액 : " << player.inventory.Gold << "Gold" << endl;
						cout << "남은공간 : " << INVENTORY_MAX - player.inventory.ItemCount << endl;
						cout << "구입 할 아이텝을 선택하세요 : ";
						cin >> menu;

						if (cin.fail())
						{
							cin.clear();
							cin.ignore(1024, '\n');
							continue;
						}

						else if (menu == STORE_ARMOR_MAX + 1)
							break;

						else if (menu < 1 || menu > STORE_ARMOR_MAX + 1)
						{
							cout << "잘못 선택하였습니다." << endl;
							system("pause");
							continue;
						}
						int armorindex = menu - 1;


						if (player.inventory.ItemCount = INVENTORY_MAX)
						{
							cout << "가방이 꽉 찼습니다." << endl;
							system("pause");
							continue;
						}
						else if (player.inventory.Gold < armor[armorindex].Price)
						{
							cout << "보유금액이 부족합니다." << endl;
							system("pause");
							continue;
						}
						player.inventory.item[player.inventory.ItemCount] =
							armor[armorindex];
						++player.inventory.ItemCount;

						player.inventory.Gold -= armor[armorindex].Price;

						cout << armor[armorindex].strName << "아이템을 구매하였습니다." << endl;
						system("pause");
					}
				}
				break;
			}
			break;
		case MENU_INVENTORY:			//	가방(인벤토리)
			while (true)
			{
				system("cls");
				cout << "*************************** 가방 ***************************" << endl;
				cout << "================== Player ==================" << endl;
				cout << "이름 : " << player.strName << "\t직업 : " <<
					player.strJobName << endl;
				cout << "레벨 : " << player.Level << "\t경험치 : " <<
					player.Exp << " / " << LevelUpExp[player.Level - 1] << endl;

				// 무기를 장착하고 있을 경우 공격력에 무기공격력을 추가하여 출력한다.
				if (player.bEquip[EQ_WEAPON] == true)
				{
					cout << "공격력 : " << player.AttackMin << " + " <<
						player.Equip[EQ_WEAPON].Min << " - " <<
						player.AttackMax << " + " << player.Equip[EQ_WEAPON].Max;
				}

				else
				{
					cout << "공격력 : " << player.AttackMin << " - " <<
						player.AttackMax;
				}

				// 방어구를 장착하고 있을 경우 방어력에 방어구 방어력을 추가하여 출력한다.
				if (player.bEquip[EQ_ARMOR] == true)
				{
					cout << "\t방어력 : " << player.ArmorMin << " + " <<
						player.Equip[EQ_ARMOR].Min << " - " <<
						player.ArmorMax << " + " << player.Equip[EQ_ARMOR].Max << endl;
				}

				else
				{
					cout << "\t방어력 : " << player.ArmorMin << " - " <<
						player.ArmorMax << endl;
				}

				cout << "체력 : " << player.HP << " / " << player.HPMax <<
					"\t마나 : " << player.MP << " / " << player.MPMax << endl;

				if (player.bEquip[EQ_WEAPON])
					cout << "장착무기 : " << player.Equip[EQ_WEAPON].strName;

				else
					cout << "장착무기 : 없음";

				if (player.bEquip[EQ_ARMOR])
					cout << "\t장착방어구 : " << player.Equip[EQ_ARMOR].strName << endl;

				else
					cout << "\t장착방어구 : 없음" << endl;

				cout << "보유골드 : " << player.inventory.Gold << " Gold" << endl << endl;

				for (int i = 0; i < player.inventory.ItemCount; ++i)
				{
					cout << i + 1 << ". 이름 : " << player.inventory.item[i].strName <<
						"\t종류 : " << player.inventory.item[i].strItem << endl;
					cout << "공격력 : " << player.inventory.item[i].Min << " - " <<
						player.inventory.item[i].Max << endl;
					cout << "판매가격 : " << player.inventory.item[i].Price <<
						"\t구매가격 : " << player.inventory.item[i].Sell << endl;
					cout << "설명 : " << player.inventory.item[i].strDesc << endl << endl;
				}

				cout << player.inventory.ItemCount + 1 << ". 뒤로가기" << endl;
				cout << "장착할 아이템을 선택하세요 : ";
				cin >> menu;

				if (cin.fail())
				{
					cin.clear();
					cin.ignore(1024, '\n');
					continue;
				}

				else if (menu == player.inventory.ItemCount + 1)
					break;

				else if (menu < 1 || menu > player.inventory.ItemCount + 1)
				{
					cout << "잘못 선택하였습니다." << endl;
					system("pause");
					continue;
				}

				// 아이템 인덱스를 구해준다.
				int	idx = menu - 1;

				// 제대로 선택했을 경우 해당 아이템의 종류에 따라 장착 부위를
				// 결정하게 한다.
				EQUIP	eq;

				switch (player.inventory.item[idx].eitem)
				{
				case ITEM_WEAPON:
					eq = EQ_WEAPON;
					break;
				case ITEM_ARMOR:
					eq = EQ_ARMOR;
					break;
				}


				// 아이템이 장착되어 있을 경우 장착되어있는 아이템과 장착할 아이템을
				// 교체해 주어야 한다. Swap 알고리즘을 활용한다.
				if (player.bEquip[eq] == true)
				{
					_tagItem	swap = player.Equip[eq];
					player.Equip[eq] = player.inventory.item[idx];
					player.inventory.item[idx] = swap;
				}

				// 장착되어있지 않을 경우 인벤토리 아이템을 장착창으로 옮기고
				// 인벤토리는 1칸 비워지게 된다.
				else
				{
					player.Equip[eq] = player.inventory.item[idx];

					for (int i = idx; i < player.inventory.ItemCount - 1; ++i)
					{
						player.inventory.item[i] = player.inventory.item[i + 1];
					}

					--player.inventory.ItemCount;

					// 장착을 했기 때문에 true로 만들어준다.
					player.bEquip[eq] = true;
				}

				cout << player.Equip[eq].strName << " 아이템을 장착하였습니다." << endl;

				system("pause");
			}
			break;
		default:
			cout << "잘못 선택하였습니다." << endl;
			break;
		}
	}
	GotoXY(20, 3);
	return 0;
}