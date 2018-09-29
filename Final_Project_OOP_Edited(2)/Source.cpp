#include<iostream>
#include<conio.h>
#include<fstream>
#include<string>

using namespace std;

////////////////////////////////////////
////////                     ///////////
////////     STRUCTURE       ///////////
////////                     ///////////
////////////////////////////////////////

struct meals
{
	string mealname;
	float mealcost;

	string returnname()
	{
		return mealname;
	}
	float returncost()
	{
		return mealcost;
	}
	void show()
	{
		cout << "Meal name : " << returnname() << endl;
		cout << "Cost : " << returncost() << endl;
	}
};

////////////////////////////////////////
////////                     ///////////
////////  CLASS DECLARATION  ///////////
////////                     ///////////
////////////////////////////////////////

class person
{
protected:
	char name[15];
	int age;
public:
	virtual void TakeData() = 0;
	virtual void ShowData() = 0;
};

class admin : public person
{
private:
	char username[15];
	int pin;
public:
	admin()
	{
		age = 0;
		pin = 0;
	}
	void TakeData();
	void ShowData();
	string GiveName();
	int GivePin();
	friend void operator <<(ofstream, admin);
};

class client : public person
{
public:
	void TakeData();
	void ShowData();
	void TakeData1();
	string GiveUsername();
	int GiveCardPin();
};

////////////////////////////////////////
////////                     ///////////
////////   AUTHENTICATION    ///////////
////////                     ///////////
////////////////////////////////////////

bool Authentication() // Authentication returns TRUE you can set the MENU,
{
	string name_temp;
	int pin_temp;

	cout << "\t\t\t\t\t\t\tEnter the username : ";
	cin >> name_temp;
	cout << "\t\t\t\t\t\t\tEnter the pin number : ";
	cin >> pin_temp;

	admin a;
	fstream filein("Admin", ios::in | ios::binary);
	while (filein.read((char*)&a, sizeof(a)))
	if (name_temp == a.GiveName() && pin_temp == a.GivePin())
	{
		cout << "\t\t\t\t\t\t\tAuthentication SUCCESSFUL!!!\n";
		return true;
	}
	else
		cout << "\t\t\t\t\t\t\tAuthentication FAILED!!!\n";

	filein.seekg(0, ios::beg);
	return false;
}

////////////////////////////////////////
////////                     ///////////
////////   ADMIN FUNCTION    ///////////
////////                     ///////////
////////////////////////////////////////

void admin::TakeData()
{
	if (Authentication())
	{
		cin.ignore();
		admin a1;
		cout << "\t\t\t\t\t\t\tEnter the name of new admin : ";
		cin.getline(name, 15);

		cout << "\t\t\t\t\t\t\tEnter age : ";
		while (!(cin >> age) || cin.get() != '\n')
		{
			cin.clear();
			cin.sync();
			cout << "\t\t\t\t\t\t\tINVALID";
			cout << "\t\t\t\t\t\t\tEnter age : ";
		}
	a:
		cin.ignore();
		cout << "\t\t\t\t\t\t\tEnter username : ";
		cin.getline(username, 15);

		cout << "\t\t\t\t\t\t\tEnter pin : ";
		while (!(cin >> pin) || cin.get() != '\n')
		{
			cin.clear();
			cin.sync();
			cout << "\t\t\t\t\t\t\tINVALID";
			cout << "\n\t\t\t\t\t\t\tEnter pin : ";
		}

		ifstream infile("Admin", ios::binary);
		while (infile.read((char*)&a1, sizeof(a1)))
		if (GiveName() == a1.GiveName() && GivePin() == a1.GivePin())
		{
			cout << "\t\t\t\t\t\t\tUSER BY THE GIVEN NAME AND PIN ALREADY EXISTS,PLEASE TRY AGAIN!!!\n";
			goto a;
		}
	}
}

void admin::ShowData()
{
	cout << "\t\t\t\t\t\t\tName : " << name << endl;
	cout << "\t\t\t\t\t\t\tAge : " << age << endl;
}
// WILL USE OPERATOR OVERLOADING HERE
string admin::GiveName()
{
	string temp = username; // username is a char[15]
	return temp;
}

int admin::GivePin()
{
	return pin;
}

////////////////////////////////////////
////////                     ///////////
////////    MENU FUNCTION    ///////////
////////                     ///////////
////////////////////////////////////////

void cText(char*);

void SetMenu()
{
	if (Authentication())
	{
		meals m1;
		fstream file("Menu", ios::in | ios::binary | ios::app);
		cText("SETTING NEW MENU");
		while (true)
		{
			cin.ignore();
			cout << "\t\t\t\t\t\t\tMeal name : ";
			cin.clear();

			getline(cin, m1.mealname);
			while (true)
			{
				cout << "\t\t\t\t\t\t\tIts cost[in sums] : ";
				cin >> m1.mealcost;
				if (!cin) // or if(cin.fail())
				{
					// user didn't input a number
					cin.clear(); // reset failbit
					cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip bad input
					cout << "\t\t\t\t\t\t\tPlease Re-Enter the values" << endl;
					continue;
					// next, request user reinput
				}
				else break;
			}

			file.write((char*)&m1, sizeof(m1));

			cout << "\t\t\t\t\t\t\tPress Enter to continue adding\n";
			cin.ignore();

			string temp;
			getline(cin, temp);
			if (temp.empty())
				continue;
			else break;
		}
		file.seekp(0, ios::beg);
	}
}

void GetMenu()
{
	meals m2;
	fstream file("Menu", ios::in | ios::binary);
	while (file.read((char*)&m2, sizeof(m2)))
		m2.show();
	file.close();
}

float CalculateBill(string mealName, int portion)
{
	meals m1;
	float bill = 0;
	ifstream readMenu("Menu", ios::binary);

	if (readMenu.is_open())
	{
		while (readMenu.read((char*)&m1, sizeof(m1)))
		if (mealName == m1.returnname())
			bill += m1.returncost()*portion;
		cout << bill;
	}
	return bill;
}

void Order(string mName, int portion)
{
	string address;
	string time;
	float Bill = 0;
	int choice;

	cout << "\t\t\t\t\t\t\tDo you want us to deliver?\n\t\t\t\t\t\t\t1. Yes\t2. No" << endl;
	cin >> choice;
	if (choice == 1)
	{
		cout << "\t\t\t\t\t\t\tEnter your valid address: ";
		cin >> address;
		cout << "\t\t\t\t\t\t\tAt What time do you want to reciece ?";
		cin >> time;
		cout << "\t\t\t\t\t\t\tOkay! Your purchase is saved! We will deliver it by: " << time;
		cout << endl;
		Bill += CalculateBill(mName, portion);
	}
	else
	{
		Bill += CalculateBill(mName, portion);

	}
	cout << "\t\t\t\t\t\t\tYou Have to pay: " << Bill << endl;
}

void cText(char* s)
{
	int l = strlen(s);
	int pos = (int)((110 - l) / 2);
	for (int i = 0; i<pos; i++)
		cout << " ";
	cout << s << endl;
}

////////////////////////////////////////
////////                     ///////////
////////   CLIENT FUNCTIONS  ///////////
////////                     ///////////
////////////////////////////////////////

void client::ShowData()
{
	cout << "\t\t\t\t\t\t\tName : " << name << endl;
	cout << "\t\t\t\t\t\t\tAge : " << age << endl;
	//	cout << "Address : " << address << endl;
	//cout << "Card's Pin : " << cardpin << endl;
}

void client::TakeData()
{
	cin.ignore();
	cout << "\t\t\t\t\t\t\tEnter name : ";
	cin.getline(name, 15);
	cout << "\t\t\t\t\t\t\tEnter your age : ";
	while (!(cin >> age) || cin.get() != '\n')
	{
		cin.clear();
		cin.sync();
		cout << "\t\t\t\t\t\t\tINVALID";
		cout << "\t\t\t\t\t\t\tEnter your age : ";
	}
}

////////////////////////////////////////
////////                     ///////////
////////   FILE HANDLING     ///////////
////////                     ///////////
////////////////////////////////////////

void WriteFile(admin a)
{
	fstream fileout("Admin", ios::out | ios::binary | ios::app);
	fileout.write((char*)&a, sizeof(a));
	//fileout.seekp(0, ios::beg);
	fileout.close();
}

void WriteFile(client c)
{
	fstream fileout("Client", ios::out | ios::binary | ios::app);
	fileout.write((char*)&c, sizeof(c));
	//fileout.seekp(0, ios::beg);
	fileout.close();
}
////////////////////////////////////////
void ReadFile(admin b)
{
	fstream filein("Admin", ios::in | ios::binary);
	while (filein.read((char*)&b, sizeof(b)))
		b.ShowData();
	//filein.seekg(0, ios::beg);
	filein.close();
}

void ReadFile(client d)
{
	fstream filein("Client", ios::in | ios::binary);
	while (filein.read((char*)&d, sizeof(d)))
		d.ShowData();
	//filein.seekg(0, ios::beg);
	filein.close();
}

////////////////////////////////////////
////////                     ///////////
////////OPERATOR OVERLOADING ///////////
////////                     ///////////
////////////////////////////////////////

void GoodBye()
{
	cout << "\n\n\t\t\t\t\t\t\tTHANK YOU PURCHASE, GOOD BYE. COME ALONG AGAIN !!!";
}

void TestFunc()
{
	int choice;
	string fName;
	int portions = 0;
	cout << "\t\t\t\t\t\t\tType Food Name: ";
	cin >> fName;

	cout << "\t\t\t\t\t\t\tEnter Number of portions: ";
	cin >> portions;

	Order(fName, portions);

	cout << "\t\t\t\t\t\t\tDo want to order again ?" << endl;
	cout << "\t\t\t\t\t\t\t1. Yes\t 2. No" << endl;
	cin >> choice;

	if (choice == 1)
		TestFunc();
	else
		GoodBye(); // GoodBye Function Call
}

int main()
{
	int choice = 0;

	while (choice != 3)
	{
		cout << "\t\t\t\t\t  ---=== ONLINE FOOD-BOOKING SYSTEM ===---\n";
		cout << "\n\n\t\t\t\t\t\t ---===   OPTIONS   ===---\n";
		cout << "\n\n\t\t\t\t\t\t\t1. Admin\n\t\t\t\t\t\t\t2. Client\n\t\t\t\t\t\t\t3. Exit\n";

		cout << "\n\n\t\t\t\t\t\tYour choice : ";
		cin >> choice;

		if (choice != 1 && (choice != 3 && choice != 2))
		{
			cout << "\n\t\t\t\t\t\t --== INVALID OPTION ==--";
			_getch();
			system("cls");
		}

		if (choice == 1)
		{
			int ch;
			admin a1;
			system("cls");

			cout << "\n\n\t\t\t\t\t\t ---===   OPTIONS   ===---\n";
			cout << "\n\n\t\t\t\t\t\t\t1. Add a new Admin\n\t\t\t\t\t\t\t2. See all data of Admins\n\t\t\t\t\t\t\t3. Set Menu\n\t\t\t\t\t\t\t4. Exit\n";

			cout << "\n\n\t\t\t\t\t\tYour choice : ";
			cin >> ch;

			switch (ch)
			{
			case 1:
				a1.TakeData();
				//a1.ShowData();
				WriteFile(a1);
				break;
			case 2:
				ReadFile(a1);
				break;
			case 3:
				SetMenu();
				GetMenu();
				break;
			case 4:
				break;
			default:
				cout << "\n\n\t\t\t\t\t\t --== INVALID OPTION ==--\n";
				break;
			}
			system("pause");
			system("cls");
		}
		else
		if (choice == 2)
		{
			int ch1;
			client c1;
			system("cls");

			cout << "\n\n\t\t\t\t\t\t ---===   OPTIONS   ===---\n";
			cout << "\n\n\t\t\t\t\t\t\t1. Registration\n\t\t\t\t\t\t\t2. See the Menu\n\t\t\t\t\t\t\t3. Exit\n";

			cout << "\n\n\t\t\t\t\t\tYour choice : ";
			cin >> ch1;

			switch (ch1)
			{
			case 1:
				c1.TakeData();
				break;
			case 2:
				GetMenu();
				TestFunc();
				break;
			case 3:
				break;
			default:
				cout << "\n\n\t\t\t\t\t\t --== INVALID OPTION ==--\n";
				break;
			}
			system("pause");
			system("cls");
		}
	}
	_getch();
	return 0;
}