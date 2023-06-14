#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
using namespace std;

string FileName = "Client.txt";

struct sClient {
	string AccountNumber, PinCode, Name, Phone;
	double AccountBalance;
	bool MarkForDelete = false;
};

void showMainMenu();

vector <string> SpiltString(string s1, string delim) {
	vector <string> vstring;
	string sWord;
	short pos = 0;
	while ((pos = s1.find(delim)) != std::string::npos) {
		sWord = s1.substr(0, pos);
		if (sWord != " ") {
			vstring.push_back(sWord);
		}
		s1.erase(0, pos + delim.length());
	}
	if (s1 != " ") {
		vstring.push_back(s1);
	}

	return vstring;
}

sClient ConvertLineToRecord(string line, string sperater = "#//#") {
	sClient Client;
	vector < string > vstring;
	vstring = SpiltString(line, sperater);

	Client.AccountNumber = vstring[0];
	Client.PinCode = vstring[1];
	Client.Name = vstring[2];
	Client.Phone = vstring[3];
	Client.AccountBalance = stod(vstring[4]);

	return Client;
}

string ConvertRecordToLine(sClient Client, string sperater = "#//#") {
	string stringReocrd = "";

	stringReocrd = Client.AccountNumber + sperater;
	stringReocrd += Client.PinCode + sperater;
	stringReocrd += Client.Name + sperater;
	stringReocrd += Client.Phone + sperater;
	stringReocrd += to_string(Client.AccountBalance);

	return stringReocrd;

}

vector <sClient> LoadCleintsDataFromFile(string FileName) {
	vector <sClient> vClients;
	fstream MyFile;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string line;
		sClient Client;

		while (getline(MyFile, line))
		{
			Client = ConvertLineToRecord(line);

			vClients.push_back(Client);
		}

		MyFile.close();
	}
	return vClients;
}

void PrintClientCard(sClient Client) {
	cout << "\nThe Following are the Clinets details\n\n";
	cout << "\n Account Number  = " << Client.AccountNumber;
	cout << "\n PinCode         = " << Client.PinCode;
	cout << "\n Client Name     = " << Client.Name;
	cout << "\n Phone           = " << Client.Phone;
	cout << "\n Account Balance = " << Client.AccountBalance;
	cout << endl;
}

bool FindClientByAccountNumber(string Number, vector <sClient> vClients, sClient& Client) {
	for (sClient c : vClients) {
		if (c.AccountNumber == Number) {
			Client = c;
			return true;
		}
	}
	return false;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients) {

	for (sClient& Client : vClients) {
		if (Client.AccountNumber == AccountNumber) {
			Client.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

vector <sClient> SavClientDataToFile(string FileName, vector <sClient> vClients) {
	fstream MyFile;

	MyFile.open(FileName, ios::out);

	string line;

	if (MyFile.is_open())
	{

		for (sClient c : vClients) {
			if (c.MarkForDelete == false) {
				line = ConvertRecordToLine(c);
				MyFile << line << endl;
			}
		}

		MyFile.close();
	}
	return vClients;
}

sClient ChangeClientRecord(string AccountNumber) {
	sClient Client;

	Client.AccountNumber = AccountNumber;

	cout << "Enter PinCode? ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Client Name? ";
	getline(cin, Client.Name);

	cout << "Enter Phone? ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance? ";
	cin >> Client.AccountBalance;

	return Client;
}

bool ClientIsExits(string AccountNumber, string FileName) {
	vector <sClient> vClients;
	fstream MyFile;

	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string line;
		sClient Client;

		while (getline(MyFile, line))
		{
			Client = ConvertLineToRecord(line);

			if (Client.AccountNumber == AccountNumber) {
				MyFile.close();
				return true;
			}
			vClients.push_back(Client);
		}

		MyFile.close();
	}
	return false;
}

sClient ReadClient() {
	sClient Client;

	cout << "Enter AccountNumber? ";
	getline(cin >> ws, Client.AccountNumber);

	while (ClientIsExits(Client.AccountNumber, FileName)) {
		cout << "CLient [" << Client.AccountNumber << "] is Exist, Enter another Account Number? ";
		getline(cin >> ws, Client.AccountNumber);
	}

	cout << "Enter PinCode? ";
	getline(cin, Client.PinCode);

	cout << "Enter Client Name? ";
	getline(cin, Client.Name);

	cout << "Enter Phone? ";
	getline(cin, Client.Phone);

	cout << "Enter Account Balance? ";
	cin >> Client.AccountBalance;

	return Client;

}

void AddNewClientToFile(string FileName, string line) {
	fstream MyFile;

	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open()) {
		MyFile << line << endl;
		MyFile.close();
	}
}

void AddNewClient() {
	sClient Client;
	Client = ReadClient();
	AddNewClientToFile(FileName, ConvertRecordToLine(Client));
}

void AddClient() {
	char Add = 'y';
	do {
		cout << "\nAdd New CLient\n\n";

		AddNewClient();

		cout << "Client Added successfully, do you want to add more? ";
		cin >> Add;
	} while (toupper(Add) == 'Y');
}

string ReadAccountNumber() {
	string AccountNumber = "";
	cout << "Enter AccountNumber? ";
	cin >> AccountNumber;
	return AccountNumber;
}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients) {
	sClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
		PrintClientCard(Client);

		cout << "\n\nAre you sure you want to Update this Client?Y/N? ";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y') {

			for (sClient& c : vClients) {
				if (c.AccountNumber == AccountNumber) {
					c = ChangeClientRecord(AccountNumber);
					break;
				}
			}

			SavClientDataToFile(FileName, vClients);

			cout << "\n\nCLient Delete Successfully";

			return true;
		}
	}
	else {
		cout << "\n CLient Delete with Account Number [" << AccountNumber << "] Not Found!";
		return false;
	}
}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients) {
	sClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
		PrintClientCard(Client);

		cout << "\n\nAre you sure you want to delete this Client?Y/N? ";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y') {
			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);

			SavClientDataToFile(FileName, vClients);

			vClients = LoadCleintsDataFromFile(FileName);
			cout << "\n\nCLient Delete Successfully";

			return true;
		}
	}
	else {
		cout << "\n CLient Delete with Account Number [" << AccountNumber << "] Not Found!";
		return false;
	}
}

void PrintClientRecord(sClient Client) {
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(20) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.Phone;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ShowAllClientsScreen() {
	vector <sClient> vClients = LoadCleintsDataFromFile(FileName);

	cout << "\n\t\t\tClient List (" << vClients.size() << ") Client(s).";
	cout << "\n_____________________________________________________________________________________\n" << endl;

	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(10) << "PinCode";
	cout << "| " << left << setw(20) << "Name Client";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Account Balance";
	cout << "\n_____________________________________________________________________________________\n" << endl;

	if (vClients.size() == 0)
		cout << "\t\t\t\tNo Clients Available In the System!";
	else
		for (sClient Client : vClients) {
			PrintClientRecord(Client);
			cout << endl;
		}

	cout << "_____________________________________________________________________________________\n" << endl;
}

void ShowDeleteClientsScreen() {
	cout << "=================================\n";
	cout << "\tDelete Client\n";
	cout << "=================================\n";
	vector <sClient> vClients = LoadCleintsDataFromFile(FileName);
	string AccountNumber = ReadAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateClientsScreen() {
	cout << "=================================\n";
	cout << "\tUPdate Client\n";
	cout << "=================================\n";
	vector <sClient> vClients = LoadCleintsDataFromFile(FileName);
	string AccountNumber = ReadAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, vClients);
}

void showAddClientsScreen() {
	cout << "=================================\n";
	cout << "\tAdd New Client\n";
	cout << "=================================\n";
	AddClient();
}

void ShowFindClientsScreen() {
	cout << "=================================\n";
	cout << "\tFind Client\n";
	cout << "=================================\n";
	vector <sClient> vClients = LoadCleintsDataFromFile(FileName);
	sClient Client;
	string AccountNumber = ReadAccountNumber();
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
		PrintClientCard(Client);

	else
		cout << "\nClient With Account Number [" << AccountNumber << "] is not find.";
}

void showEndScreen() {
	cout << "=================================\n";
	cout << "\tEnd Programm:-) \n";
	cout << "=================================\n";
}

enum enMainMenueOption {
	eListClients = 1,
	eAddNewClient = 2,
	eDeleteClient = 3,
	eUpdateClient = 4,
	eFindClient = 5,
	eExit = 6
};

void GoBackToMain() {
	cout << "\nPress any key to go back to main Screen....";
	system("pause > 0");
	showMainMenu();
}

short ReadMainMenueOption() {
	short Choice = 0;
	cout << "Choose what do you want to do [1 to 6]? ";
	cin >> Choice;
	return Choice;
}

void PerfromMainMenueOptio(enMainMenueOption MainMenueOption) {

	switch (MainMenueOption) {
	case enMainMenueOption::eListClients:
		system("cls");
		ShowAllClientsScreen();
		GoBackToMain();
		break;
	case enMainMenueOption::eAddNewClient:
		system("cls");
		showAddClientsScreen();
		GoBackToMain();
		break;

	case enMainMenueOption::eDeleteClient:
		system("cls");
		ShowDeleteClientsScreen();
		GoBackToMain();
		break;

	case enMainMenueOption::eUpdateClient:
		system("cls");
		ShowUpdateClientsScreen();
		GoBackToMain();
		break;
	case enMainMenueOption::eFindClient:
		system("cls");
		ShowFindClientsScreen();
		GoBackToMain();
		break;

	case enMainMenueOption::eExit:
		system("cls");
		showEndScreen();
		break;
	}
}

void showMainMenu() {
	system("cls");
	cout << "======================================\n";
	cout << "\tMain Menue Screen\n";
	cout << "======================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add Nwe Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Exit.\n";
	cout << "======================================\n";
	PerfromMainMenueOptio((enMainMenueOption)ReadMainMenueOption());
}

int main() {

	showMainMenu();

	system("pause >0");
	return 0;

}