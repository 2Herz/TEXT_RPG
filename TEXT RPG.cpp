#include <iostream>
#include <windows.h>
using namespace std;

#define NAME_SIZE 64		// �̸��� ũ��
#define ITEM_DESC_LENGTH 1024		//	������ ��������� ũ��
#define INVENTORY_MAX 20		//	�κ��丮 �ִ�ĭ ��
#define STORE_WEAPON_MAX 3	//	����������� �Ĵ� ������ �ִ� ��
#define STORE_ARMOR_MAX 3	//	���������� �Ĵ� ���� �ִ� ��
#define LEVEL_MAX 15				//	���� �ִ� ����

/*
	�޴������� ���������� ���Ǽ��� ����
	enum���� ����ü�� ����Ͽ����ϴ�.
*/
void	GotoXY(short PosX, short PosY)
{
	COORD	pos = { PosX, PosY };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

enum MAIN_MENU					//	�κ�ȭ��
{
	MENU_NONE,
	MENU_MAP,
	MENU_STORE,
	MENU_INVENTORY,
	MENU_EXIT
};

enum MAP						//	��(���̵���)
{
	MAP_NONE,
	MAP_EASY,
	MAP_NORMAL,
	MAP_HARD,
	MAP_BACK
};

enum JOB						//	����
{
	JOB_NONE,
	JOB_KNIGHT,
	JOB_ARCHER,
	JOB_WIZARD,
	JOB_END
};

enum BATTLE			// ����
{
	BATTLE_NONE,
	BATTLE_ATTACK,
	BATTLE_BACK
};

enum ITEM			// ������
{
	ITEM_WEAPON,
	ITEM_ARMOR,
	ITEM_BACK
};

enum STORE_MENU		// ����
{
	S_MENU_NONE,
	S_MENU_WEAPON,
	S_MENU_ARMOR,
	S_MENU_BACK
};

enum EQUIP			// �������
{
	EQ_NONE,
	EQ_WEAPON,
	EQ_ARMOR,
	EQ_MAX
};

struct _tagItem				// ������ ������ ���� ����ü
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

struct _tagInventory			// �κ��丮 ������ ���� ����ü
{
	_tagItem		item[INVENTORY_MAX];
	int		ItemCount;
	int		Gold;
};

struct _tagPlayer				//	�÷��̾��� ������ ���� ����ü
{
	char strName[NAME_SIZE];		//	ĳ���� �̸�
	char strJobName[NAME_SIZE];	//	ĳ���� �����̸�
	JOB		ejob;								//	����
	int		AttackMin;						//	���ݷ� �ּڰ�
	int		AttackMax;						//	���ݷ� �ִ�
	int		ArmorMin;						//	���� �ּڰ�
	int		ArmorMax;						//	���� �ִ�
	int		HP;									//	ü��
	int		HPMax;							//	ü���� �ִ�
	int		MP;									//	����
	int		MPMax;							//	������ �ִ�
	int		Exp;									//	����ġ
	int		Level;								//	����
	_tagItem	Equip[EQ_MAX];
	bool		bEquip[EQ_MAX];
	_tagInventory		inventory;
};

struct _tagLevelUp				// ������ ������ ���� ����ü
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

struct _tagMonster							// ������ ������ ���� ����ü
{
	char	strName[NAME_SIZE];		// ���� �̸�
	int		AttackMin;						//	�ּҰ��ݷ�
	int		AttackMax;						//	�ִ���ݷ�
	int		ArmorMin;						//	�ּҹ���
	int		ArmorMax;						//	�ִ����
	int		HP;									//	ü��
	int		HPMax;							//	�ִ�ü��
	int		MP;									//	����
	int		MPMax;							//	�ִ븶��
	int		Level;								//	���� ����
	int		GExp;								//	���Ͱ� �ִ� ȹ�� ����ġ
	int		GoldMin;							//	���Ϳ��Լ� ����Ǵ� �ּ� ���
	int		GoldMax;							//	���Ϳ��Լ� ����Ǵ� �ִ� ���
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

	//	���� ���� �� �÷��̾� ������ ����
	_tagPlayer	player = {};

	//	�÷��̾��� �̸��� �Է¹޴´�.
	GotoXY(25, 0);
	cout << "�̸� : ";
	cin.getline(player.strName, NAME_SIZE - 1);

	int Job = JOB_NONE;
	while (Job == JOB_NONE)
	{
		system("cls");
		GotoXY(23, 0);
		cout << "1.����" << endl;
		GotoXY(23, 1);
		cout << "2.�ü�" << endl;
		GotoXY(23, 2);
		cout << "3.������" << endl;
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
		strcpy_s(player.strName, "����");
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
		strcpy_s(player.strName, "�ü�");
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
		strcpy_s(player.strName, "������");
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

	//	���� ����
	_tagMonster		monsterArr[MAP_BACK - 1] = {};

	//	�κ߸�(Schevirr)
	strcpy_s(monsterArr[0].strName, "�κ߸�");
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

	//	���̴���(Painuisch)
	strcpy_s(monsterArr[1].strName, "���̴���");
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

	//	���� - �߸���(Burgen)
	strcpy_s(monsterArr[2].strName, "�߸���");
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

	//	�������� �Ǹ��� ������ ��ϵ��� �������ش�.
	_tagItem		weapon[STORE_WEAPON_MAX] = {};
	_tagItem		armor[STORE_ARMOR_MAX] = {};

	//	��������
	strcpy_s(weapon[0].strName, "�ҹ��");
	strcpy_s(weapon[0].strItem, "Ȱ");
	strcpy_s(weapon[0].strDesc, "�� ����� �ҹ��, ������ �ҹ�⸦ ����������..");
	weapon[0].Min = 250;
	weapon[0].Max = 500;
	weapon[0].Price = 3000;
	weapon[0].Sell = 2500;

	strcpy_s(weapon[1].strName, "��Ű�����");
	strcpy_s(weapon[1].strItem, "������");
	strcpy_s(weapon[1].strDesc, "�� ����� �������������� ���� ��Ű�����, ������ �����ϰ� ����������..");
	weapon[1].Min = 150;
	weapon[1].Max = 300;
	weapon[1].Price = 2000;
	weapon[1].Sell = 1000;

	strcpy_s(weapon[2].strName, "����Į����");
	strcpy_s(weapon[2].strItem, "��");
	strcpy_s(weapon[2].strDesc, "������ �����ϴ� ����, �� ����߱��..");
	weapon[2].Min = 2000;
	weapon[2].Max = 4000;
	weapon[2].Price = 6000;
	weapon[2].Sell = 3000;
	//	��
	strcpy_s(armor[0].strName, "�����ܶѲ�");
	strcpy_s(armor[0].strItem, "��");
	strcpy_s(armor[0].strDesc, "���κ��� ������ �������� ���̴�, �� �ڽ��� �ҹ�������� ��ų �� �ִ�.");
	armor[0].Min = 500;
	armor[0].Max = 1000;
	armor[0].Price = 4000;
	armor[0].Sell = 2500;

	strcpy_s(armor[1].strName, "ö����");
	strcpy_s(armor[1].strItem, "��");
	strcpy_s(armor[1].strDesc, "ö�� ���� �����̴�, �ؼ��� ������ �����.");
	armor[1].Min = 500;
	armor[1].Max = 1000;
	armor[1].Price = 6000;
	armor[1].Sell = 3000;

	strcpy_s(armor[2].strName, "�������");
	strcpy_s(armor[2].strItem, "��");
	strcpy_s(armor[2].strDesc, "�̸� �״�� �ذ��� ������ �����ش�.");
	armor[2].Min = 2500;
	armor[2].Max = 5000;
	armor[2].Price = 10000;
	armor[2].Sell = 5000;

	while (true)
	{
		system("cls");
		GotoXY(25, 0);
		cout << "1.��  2.����  3.����  4.����" << endl;
		GotoXY(20, 2);
		cout << "SELECT MENU : ";
		int menu;
		cin >> menu;

		// ���Է� ��������Դϴ�.
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
		case MENU_MAP:			// ��
			while (true)
			{
				system("cls");
				GotoXY(35, 0);
				cout << " *** �� *** " << endl;
				GotoXY(23, 1);
				cout << "1.����" << endl;
				GotoXY(23, 2);
				cout << "2.����" << endl;
				GotoXY(23, 3);
				cout << "3.�����" << endl;
				GotoXY(23, 4);
				cout << "4.�ڷΰ���" << endl;
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
					//	�÷��̾� ���� ���
					GotoXY(23, 1);
					cout << "=================PLAYER=================" << endl;
					GotoXY(23, 2);
					cout << "�̸� : " << player.strName << "\t����" <<
						player.strJobName << endl;
					GotoXY(23, 3);
					cout << "���� : " << player.Level << "\t ����ġ : " <<
						player.Exp << endl;

					if (player.bEquip[EQ_WEAPON] == true)
					{
						GotoXY(23, 4);
						cout << "���ݷ� : " << player.AttackMin << " + " <<
							player.Equip[EQ_WEAPON].Min << " - " <<
							player.AttackMax << " + " << player.Equip[EQ_WEAPON].Max;
					}
					else
					{
						GotoXY(23, 4);
						cout << "���ݷ� : " << player.AttackMin << " - " <<
							player.AttackMax;
					}
					if (player.bEquip[EQ_ARMOR] == true)
					{
						GotoXY(23, 5);
						cout << "JP : " << player.inventory.Gold << " Gold" << endl;
						cout << "\t���� : " << player.ArmorMin << " + " <<
							player.Equip[EQ_ARMOR].Min << " - " <<
							player.ArmorMax << " + " << player.Equip[EQ_ARMOR].Max << endl;
					}
					else
					{
						cout << "\t���� : " << player.ArmorMin << " - " <<
							player.ArmorMax << endl;
					}
					GotoXY(23, 6);
					cout << "ü�� : " << player.HP << " / " << player.HPMax <<
						"\t���� : " << player.MP << " / " << player.MPMax << endl;
					GotoXY(23, 7);
					cout << "JP : " << player.inventory.Gold << " Gold" << endl;

					if (player.bEquip[EQ_WEAPON])
					{
						GotoXY(23, 10);
						cout << "�������� : " << player.Equip[EQ_WEAPON].strName;
					}
					else
					{
						GotoXY(23, 11);
						cout << "�������� : ����";
					}
					if (player.bEquip[EQ_ARMOR])
					{
						GotoXY(23, 12);
						cout << "\t������ : " << player.Equip[EQ_ARMOR].strName << endl;
					}
					else
					{
						GotoXY(23, 13);
						cout << "\t������ : ����" << endl;
					}

					//	���� ���� ���
					GotoXY(23, 9);
					cout << "=================MONSTER=================" << endl;
					GotoXY(23, 10);
					cout << "�̸� : " << monster.strName << "\t ���� : " << monster.Level << endl;
					GotoXY(23, 11);
					cout << "���ݷ� : " << monster.AttackMin << " - " <<
						monster.AttackMax << "\t ���� : " << monster.ArmorMin << " - " <<
						monster.ArmorMax << endl;
					GotoXY(23, 12);
					cout << "ü�� : " << monster.HP << " / " << monster.HPMax << "\t ���� : " <<
						monster.MP << " / " << monster.MPMax << endl;
					GotoXY(23, 13);
					cout << "ȹ�����ġ : " << monster.GExp << "\t ȹ���� : " << monster.GoldMin << " - "
						<< monster.GoldMax << endl;
					GotoXY(23, 14);
					cout << "========================================" << endl << endl;

					GotoXY(23, 17);
					cout << "�޴��� �����ϼ���";
					GotoXY(23, 15);
					cout << "1. ����" << endl;
					GotoXY(23, 16);
					cout << "2.��������" << endl;
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
					case BATTLE_ATTACK:		//	����
					{
						int	AttackMin = player.AttackMin;
						int	AttackMax = player.AttackMax;

						// ���⸦ �����ϰ� ���� ��� ����� Min, Max�� �����ش�.
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

						// ���� HP�� ���ҽ�Ų��.
						monster.HP -= Damage;
						GotoXY(23, 18);
						cout << player.strName << "�� " << monster.strName <<
							"���� " << Damage << " ���ظ� �������ϴ�." << endl;

						// ���Ͱ� �׾��� ��츦 ó���Ѵ�.
						if (monster.HP <= 0)
						{
							GotoXY(23, 19);
							cout << monster.strName << " ���Ͱ� ����Ͽ����ϴ�." << endl;

							player.Exp += monster.GExp;
							int	Gold = (rand() % (monster.GoldMax - monster.GoldMin + 1) +
								monster.GoldMin);
							player.inventory.Gold += Gold;
							GotoXY(23, 20);
							cout << monster.GExp << " ����ġ�� ȹ���Ͽ����ϴ�." << endl;
							GotoXY(23, 21);
							cout << Gold << " Gold�� ȹ���Ͽ����ϴ�." << endl;

							monster.HP = monster.HPMax;
							monster.MP = monster.MPMax;

							// �������� �ߴ��� üũ�غ���.
							if (player.Exp >= LevelUpExp[player.Level - 1])
							{
								// �÷��̾� ����ġ�� �������� �ʿ��� ����ġ��ŭ �����Ѵ�.
								player.Exp -= LevelUpExp[player.Level - 1];

								// ������ ������Ų��.
								++player.Level;
								GotoXY(23, 22);
								cout << "������ �Ͽ����ϴ�." << endl;

								// �ɷ�ġ�� ��½�Ų��.
								// ���� �ε����� ���Ѵ�.
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

								// ü�°� ������ ȸ����Ų��.
								player.HP = player.HPMax;
								player.MP = player.MPMax;
							}
							system("pause");
							break;
						}

						// ���Ͱ� ����ִٸ� �÷��̾ �����Ѵ�.
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

						// �÷��̾��� HP�� ���ҽ�Ų��.
						player.HP -= Damage;
						GotoXY(23, 19);
						cout << monster.strName << "�� " << player.strName <<
							"���� " << Damage << " ���ظ� �������ϴ�." << endl;

						// �÷��̾ �׾��� ���
						if (player.HP <= 0)
						{
							cout << player.strName << " �÷��̾ ����Ͽ����ϴ�." << endl;

							int	Exp = player.Exp * 0.1f;
							int	Gold = player.inventory.Gold * 0.1f;

							player.Exp -= Exp;
							player.inventory.Gold -= Gold;

							cout << Exp << " ����ġ�� �Ҿ����ϴ�." << endl;
							cout << Gold << " Gold�� �Ҿ����ϴ�." << endl;

							// �÷��̾��� HP�� MP�� ȸ���Ѵ�.
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
		case MENU_STORE:				//	����
			while (true)
			{
				system("cls");
				GotoXY(25, 0);
				cout << "***************����***************" << endl;
				GotoXY(25, 1);
				cout << "������ �����ϼ��� : ";
				GotoXY(25, 2);
				cout << "1.�������" << endl;
				GotoXY(25, 3);
				cout << "2.������" << endl;
				GotoXY(25, 4);
				cout << "3.�ڷΰ���" << endl;
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
						cout << "***************�������***************" << endl;
						for (int i = 0; i < STORE_WEAPON_MAX; ++i)
						{
							cout << i + 1 << ". �̸� : " << weapon[i].strName <<
								"\t���� : " << weapon[i].strItem << endl;
							cout << "���ݷ� : " << weapon[i].Min << " - " <<
								weapon[i].Max << endl;
							cout << "�ǸŰ��� : " << weapon[i].Price <<
								"\t���Ű��� : " << weapon[i].Sell << endl;
							cout << "���� : " << weapon[i].strDesc << endl << endl;
						}

						cout << STORE_WEAPON_MAX + 1 << ". �ڷΰ���" << endl;
						cout << "�����ݾ� : " << player.inventory.Gold << " Gold" << endl;
						cout << "�������� : " << INVENTORY_MAX - player.inventory.ItemCount << endl;
						cout << "������ �������� �����ϼ��� : ";
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
							cout << "�߸� �����Ͽ����ϴ�." << endl;
							system("pause");
							continue;
						}

						// �����ǸŸ�� �迭�� �ε����� �����ش�.
						int	weaponindex = menu - 1;

						// �κ��丮�� �� á���� �˻��Ѵ�.
						if (player.inventory.ItemCount == INVENTORY_MAX)
						{
							cout << "������ �� á���ϴ�." << endl;
							system("pause");
							continue;
						}

						// ���� ������ ���
						else if (player.inventory.Gold < weapon[weaponindex].Price)
						{
							cout << "�����ݾ��� �����մϴ�." << endl;
							system("pause");
							continue;
						}

						// ó���� iItemCount�� �ϳ��� �߰��Ǿ����� �ʱ� ������ 0���� �ʱ�ȭ
						// �Ǿ� �����Ƿ� 0�� �ε����� ������ �������� �߰��ϰ� �ȴ�. �׸���
						// ī��Ʈ�� 1�� �ȴ�. �������� �߰��Ҷ��� 1�� �ε����� �߰��ϰԵȴ�.
						player.inventory.item[player.inventory.ItemCount] =
							weapon[weaponindex];
						++player.inventory.ItemCount;

						// ��带 �����Ѵ�.
						player.inventory.Gold -= weapon[weaponindex].Price;

						cout << weapon[weaponindex].strName << " �������� �����Ͽ����ϴ�." << endl;
						system("pause");
					}
					break;
				case S_MENU_ARMOR:
					while (true)
					{
						system("cls");
						cout << "***************������***************" << endl;
						for (int i = 0; i < STORE_ARMOR_MAX; ++i)
						{
							cout << i + 1 << "�̸� : " << armor[i].strName <<
								"\t ���� : " << armor[i].strItem << endl;
							cout << "���ݷ� : " << armor[i].Min << " - " <<
								armor[i].Max << endl;
							cout << "�ǸŰ��� : " << armor[i].Price <<
								"\t ���Ű��� : " << armor[i].Sell << endl;
							cout << "���� : " << armor[i].strDesc << endl;
						}
						cout << STORE_ARMOR_MAX + 1 << " : �ڷΰ���" << endl;
						cout << "�����ݾ� : " << player.inventory.Gold << "Gold" << endl;
						cout << "�������� : " << INVENTORY_MAX - player.inventory.ItemCount << endl;
						cout << "���� �� �������� �����ϼ��� : ";
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
							cout << "�߸� �����Ͽ����ϴ�." << endl;
							system("pause");
							continue;
						}
						int armorindex = menu - 1;


						if (player.inventory.ItemCount = INVENTORY_MAX)
						{
							cout << "������ �� á���ϴ�." << endl;
							system("pause");
							continue;
						}
						else if (player.inventory.Gold < armor[armorindex].Price)
						{
							cout << "�����ݾ��� �����մϴ�." << endl;
							system("pause");
							continue;
						}
						player.inventory.item[player.inventory.ItemCount] =
							armor[armorindex];
						++player.inventory.ItemCount;

						player.inventory.Gold -= armor[armorindex].Price;

						cout << armor[armorindex].strName << "�������� �����Ͽ����ϴ�." << endl;
						system("pause");
					}
				}
				break;
			}
			break;
		case MENU_INVENTORY:			//	����(�κ��丮)
			while (true)
			{
				system("cls");
				cout << "*************************** ���� ***************************" << endl;
				cout << "================== Player ==================" << endl;
				cout << "�̸� : " << player.strName << "\t���� : " <<
					player.strJobName << endl;
				cout << "���� : " << player.Level << "\t����ġ : " <<
					player.Exp << " / " << LevelUpExp[player.Level - 1] << endl;

				// ���⸦ �����ϰ� ���� ��� ���ݷ¿� ������ݷ��� �߰��Ͽ� ����Ѵ�.
				if (player.bEquip[EQ_WEAPON] == true)
				{
					cout << "���ݷ� : " << player.AttackMin << " + " <<
						player.Equip[EQ_WEAPON].Min << " - " <<
						player.AttackMax << " + " << player.Equip[EQ_WEAPON].Max;
				}

				else
				{
					cout << "���ݷ� : " << player.AttackMin << " - " <<
						player.AttackMax;
				}

				// ���� �����ϰ� ���� ��� ���¿� �� ������ �߰��Ͽ� ����Ѵ�.
				if (player.bEquip[EQ_ARMOR] == true)
				{
					cout << "\t���� : " << player.ArmorMin << " + " <<
						player.Equip[EQ_ARMOR].Min << " - " <<
						player.ArmorMax << " + " << player.Equip[EQ_ARMOR].Max << endl;
				}

				else
				{
					cout << "\t���� : " << player.ArmorMin << " - " <<
						player.ArmorMax << endl;
				}

				cout << "ü�� : " << player.HP << " / " << player.HPMax <<
					"\t���� : " << player.MP << " / " << player.MPMax << endl;

				if (player.bEquip[EQ_WEAPON])
					cout << "�������� : " << player.Equip[EQ_WEAPON].strName;

				else
					cout << "�������� : ����";

				if (player.bEquip[EQ_ARMOR])
					cout << "\t������ : " << player.Equip[EQ_ARMOR].strName << endl;

				else
					cout << "\t������ : ����" << endl;

				cout << "������� : " << player.inventory.Gold << " Gold" << endl << endl;

				for (int i = 0; i < player.inventory.ItemCount; ++i)
				{
					cout << i + 1 << ". �̸� : " << player.inventory.item[i].strName <<
						"\t���� : " << player.inventory.item[i].strItem << endl;
					cout << "���ݷ� : " << player.inventory.item[i].Min << " - " <<
						player.inventory.item[i].Max << endl;
					cout << "�ǸŰ��� : " << player.inventory.item[i].Price <<
						"\t���Ű��� : " << player.inventory.item[i].Sell << endl;
					cout << "���� : " << player.inventory.item[i].strDesc << endl << endl;
				}

				cout << player.inventory.ItemCount + 1 << ". �ڷΰ���" << endl;
				cout << "������ �������� �����ϼ��� : ";
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
					cout << "�߸� �����Ͽ����ϴ�." << endl;
					system("pause");
					continue;
				}

				// ������ �ε����� �����ش�.
				int	idx = menu - 1;

				// ����� �������� ��� �ش� �������� ������ ���� ���� ������
				// �����ϰ� �Ѵ�.
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


				// �������� �����Ǿ� ���� ��� �����Ǿ��ִ� �����۰� ������ ��������
				// ��ü�� �־�� �Ѵ�. Swap �˰����� Ȱ���Ѵ�.
				if (player.bEquip[eq] == true)
				{
					_tagItem	swap = player.Equip[eq];
					player.Equip[eq] = player.inventory.item[idx];
					player.inventory.item[idx] = swap;
				}

				// �����Ǿ����� ���� ��� �κ��丮 �������� ����â���� �ű��
				// �κ��丮�� 1ĭ ������� �ȴ�.
				else
				{
					player.Equip[eq] = player.inventory.item[idx];

					for (int i = idx; i < player.inventory.ItemCount - 1; ++i)
					{
						player.inventory.item[i] = player.inventory.item[i + 1];
					}

					--player.inventory.ItemCount;

					// ������ �߱� ������ true�� ������ش�.
					player.bEquip[eq] = true;
				}

				cout << player.Equip[eq].strName << " �������� �����Ͽ����ϴ�." << endl;

				system("pause");
			}
			break;
		default:
			cout << "�߸� �����Ͽ����ϴ�." << endl;
			break;
		}
	}
	GotoXY(20, 3);
	return 0;
}