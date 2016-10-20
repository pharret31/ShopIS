#include <iostream>
#include <string>
#include <deque>
using namespace std;

FILE *file = NULL;
FILE *temp = NULL;

struct product{
	int count;
	int maxCount;
	char provider[20];
	char groupName[15];
} prod;

deque<product> buffer;
deque<product>::iterator iter;

void sellerMenu();
void buyerMenu();

int main()
{
	system("cls");
	file = fopen("base.grs","rb");
	if(file == NULL)
	{
		file = fopen("base.grs","wb");
		fclose(file);
		file = fopen("base.grs","rb");
	}
	while(fread(&prod,sizeof(prod),1,file) == 1)
	{
		buffer.push_back(prod);
	}

	fclose(file);

	if(buffer.empty())
	{
		cout << "Base was not found!";
		sellerMenu();
	}

	do
	{
		int answer = 0;
		string password;

		cout << "Welcome to the grocery shop!" << endl;
		cout << "Who are you?" << endl;
		cout << "1. Seller" << endl;
		cout << "2. Buyer" << endl;
		cout << "3. Quit" << endl;
		cin >> answer;

		if(answer == 1)
		{
			cout << "Enter the password: ";
			cin >> password;
			if(password == "password")
			{
				cout << "Wellcome Mr. Seller!" << endl;
				sellerMenu();
				system("cls");
			}
			cout << "Invalid password!" << endl;
		}
		else if(answer == 2)
		{
			cout << "Wellcome back Mr. Buyer!" << endl;
			buyerMenu();
			system("cls");
		}
		else if(answer == 3)
		{
			cout << "Good bye!" << endl;
			break;
		}

		cout << "Return again!" << endl;
	}
	while(1);

	file = fopen("base.grs","wb");
	for(iter = buffer.begin(); iter != buffer.end(); ++iter)
	{
		product prod = *iter;
		fwrite(&prod,sizeof(product),1,file);
	}
	fclose(file);

	cout << "Good day!" << endl;

	return 0;
}

void sellerMenu()
{
	system("cls");
	do
	{
		int choose = 0;
		char searchName[15];
		int tempCount;
		int answer;
		bool groupIsExist;

		cout << "Your action?" << endl;
		cout << "1. Add group" << endl;
		cout << "2. Delete group" << endl;
		cout << "3. Change group" << endl;
		cout << "4. Add product" << endl;
		cout << "5. Delete base" << endl;
		cout << "6. Quit" << endl;

		cin >> choose;
		switch (choose)
		{
		case 1:
			groupIsExist = false;
			cout << "Enter name of group: ";
			cin >> prod.groupName;
			for(iter = buffer.begin(); iter != buffer.end(); ++iter)
			{
				if(strcmp(iter->groupName,prod.groupName) == 0)
				{
					cout << "Name = " << iter->groupName << " | Count = " << iter->count << " | Max count = " << iter->maxCount << " | Provider = " << iter->provider << endl;
					groupIsExist = true;
				}
			}
			if(groupIsExist)
			{
				cout << "Group is exist!" << endl;
				break;
			}
			cout << "Enter max count: ";
			cin >> prod.maxCount;
			cout << "Enter provider: ";
			cin >> prod.provider;
			prod.count = 0;
			buffer.push_back(prod);
			break;
		case 2:
			cout << "Enter the group name: ";
			cin >> searchName;
			for(iter = buffer.begin(); iter != buffer.end(); ++iter)
			{
				if(strcmp(iter->groupName,searchName) == 0)
				{
					buffer.erase(iter);
				}
			}
			break;
		case 3:
			answer = 1;
			cout << "Enter the group name: ";
			cin >> searchName;
			for(iter = buffer.begin(); iter != buffer.end(); ++iter)
			{
				if(strcmp(iter->groupName,searchName) == 0)
				{
					cout << "Name = " << iter->groupName << " | Count = " << iter->count << " | Max count = " << iter->maxCount << " | Provider = " << iter->provider << endl;

					do
					{
						cout << "Enter max count: ";
						cin >> tempCount;

						if(tempCount < iter->count)
						{
							cout << "Some objects can't be placed. Throw them away? (1/0)";
							cin >> answer;
							if(answer == 1)
							{
								iter->maxCount = tempCount;
								iter->count = iter->maxCount;
								break;
							}
						}
					}
					while(answer != 1);

					cout << "Enter provider: ";
					cin >> iter->provider;
					cout << "Enter group name: ";
					cin >> iter->groupName;
					iter->count = tempCount;
					break;
				}
			}
			break;
		case 4:
			cout << "Enter the group name: ";
			cin >> searchName;
			for(iter = buffer.begin(); iter != buffer.end(); ++iter)
			{
				if(strcmp(iter->groupName,searchName) == 0)
				{
					cout << "Name = " << iter->groupName << " | Count = " << iter->count << " | Max count = " << iter->maxCount << " | Provider = " << iter->provider << endl;
					do
					{
						cout << "Enter count: ";
						cin >> tempCount;

						if(tempCount+iter->count > iter->maxCount)
						{
							cout << "Value out from border! Return Again." << endl;
							continue;
						}

						break;
					}
					while(1);
					iter->count += tempCount;
				}
			}
			break;
		case 5:
			if(buffer.empty())
			{
				cout << "Error! Base is not found" << endl;
				break;
			}
			buffer.clear();
			remove("base.grs");
			cout << "Base was succesful deleted!" << endl;
			break;
		case 6:
			return;
		default:
			cout << "Return again!" << endl;
			break;
		}
	}
	while(1);


}

void buyerMenu()
{
	system("cls");
	do
	{
		int count = 0;
		int choose = 0;
		int buyCount = 0;
		char searchName[15];

		cout << "Your action?" << endl;
		cout << "1. Show full information" << endl;
		cout << "2. Find group" << endl;
		cout << "3. Buy products" << endl;
		cout << "4. Quit" << endl;

		cin >> choose;
		switch (choose)
		{
		case 1:
			if(buffer.empty())
			{
				cout << "Base is empty!" << endl;
				break;
			}
			for(iter = buffer.begin(); iter != buffer.end(); ++iter)
			{
				cout << "Name = " << iter->groupName << " | Count = " << iter->count << " | Provider = " << iter->provider << endl;
			}
			break;
		case 2:
			cout << "Enter group name: ";
			cin >> searchName;

			for(iter = buffer.begin(); iter != buffer.end(); ++iter)
			{
				if(strcmp(iter->groupName,searchName) == 0)
				{
					cout << "Name = " << iter->groupName << " | Count = " << iter->count << " | Provider = " << iter->provider << endl;
					count++;
				}
			}
			if(count == 0)
			{
				cout << "Records was not found!" << endl;
			}
			break;
		case 3:
			cout << "Enter group name: ";
			cin >> searchName;

			for(iter = buffer.begin(); iter != buffer.end(); ++iter)
			{
				if(strcmp(iter->groupName,searchName) == 0)
				{
					cout << "Name = " << iter->groupName << " | Count = " << iter->count << " | Provider = " << iter->provider << endl;
					count++;
					break;
				}
			}
			if(count == 0)
			{
				cout << "Records was not found!" << endl;
			}
			else
			{
				do
				{
					cout << "Enter count of buying products: ";
					cin >> buyCount;
					if(buyCount > iter->count)
					{
						cout << "Error! Very much count";
					}
					else
					{
						iter->count -= buyCount;
						cout << "Thanks for buying!" << endl;
						break;
					}
				}while(buyCount != 0);
			}
			break;
		case 4:
			return;
		default:
			cout << "Return again!" << endl;
			break;
		}
	}
	while(1);
}