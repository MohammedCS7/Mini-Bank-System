#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;

const string CLIENTS_FILE = "Clients.txt";

enum enMainMenuOptions { Show = 1, Add = 2, Delete = 3, Update = 4, Find = 5, Transact = 6, Exit = 7 };

enum enMenus { Main = 1, Transactions = 2 };

enum enTransMenuOptions { Deposit = 1, Withdraw = 2, Balances = 3, MainMenu = 4 };

struct Client
{
    string AccountNumber;
    string PinCode;
    string FullName;
    string PhoneNumber;
    double Balance = 0;
    bool MarkForDelete = false;
};

void TransMenu(vector <Client>& clients);

string ReadString(string message)
{
    string str;

    cout << message;
    getline(cin >> ws, str);

    return str;
}

double ReadDouble(string message)
{
    double num;
    
    cout << message;
    cin >> num;

    return num;
}

int ReadNumberInRange(string message, int from, int to)
{
    int num;

    do
    {
        cout << message;
        cin >> num;
    } 
    while (num < from || num > to);

    return num;
}

char ReadChar(string message)
{
    char ch;

    cout << message;
    cin >> ch;

    return ch;
}

int ReadChoice(int from, int to)
{
    cout << "Choose what do you want to do ? [" << from << "] to [" << to << "] : ";
    int choice = ReadNumberInRange("", from, to);
    return choice;
}

string ReadAccountNumber()
{
    return ReadString("Enter Account Number : ");
}


enMainMenuOptions ReadMainOption(int from,int to)
{
    int choice = ReadChoice(from, to);
    return (enMainMenuOptions)choice;
}

enTransMenuOptions ReadTransOption(int from, int to)
{
    int choice = ReadChoice(from, to);
    return (enTransMenuOptions)choice;
}

bool IsFound(const vector<Client>& clients,string accNum)
{
    for (const Client& client : clients)
    {
        if (client.AccountNumber == accNum)
            return true;
    }
    return false;
}

string ReadAccNumUntilFound(const vector <Client>& clients)
{
    string accNum = ReadAccountNumber();
    while (!IsFound(clients, accNum))
    {
        cout << "Client with [" << accNum << "] does not exist.\n";
        accNum = ReadAccountNumber();
    }
    return accNum;
}

double ReadWithdrawAmount(Client client)
{
    double amount = ReadDouble("Please enter withdraw amount : ");
    while (amount > client.Balance)
    {
        cout << "Amount Exceeds the balance, you can withdraw up to : " << client.Balance << endl;
        amount = ReadDouble("Please enter another amount : ");
    }
    return amount;
}

Client ReadClientData()
{
    Client client;

    client.AccountNumber = ReadString("Enter Account Number : ");
    client.PinCode = ReadString("Enter PIN Code       : ");
    client.FullName = ReadString("Enter Full Name      : ");
    client.PhoneNumber = ReadString("Enter Phone Number   : ");
    client.Balance = ReadDouble("Enter Balance        : ");

    return client;
}

Client ReadClientCommonData(string accountNumber = "")
{
    Client client;
    client.AccountNumber = accountNumber;
    client.PinCode = ReadString("Enter PIN Code       : ");
    client.FullName = ReadString("Enter Full Name      : ");
    client.PhoneNumber = ReadString("Enter Phone Number   : ");
    client.Balance = ReadDouble("Enter Balance        : ");

    return client;

}

void PrintLine(int length, char ch)
{
    for (int i = 0; i < length; i++)
    {
        cout << ch;
    }
    cout << endl;
}

vector <string> SplitString(string str, string sep)
{
    vector <string> words;
    int pos;
    string word;
    while ((pos = str.find(sep)) != string::npos)
    {
        word = str.substr(0, pos);
        if (word != "")
        {
            words.push_back(word);
        }
        str.erase(0, pos + sep.length());
    }
    if (str != "")
        words.push_back(str);
    return words;
}

string JoinString(vector <string>& words, string sep)
{
    if (words.empty() || sep.empty())
        return "";
    if (words.size() == 1)
        return words.at(0);
    string joined = words.at(0);
    vector <string>::iterator iter;
    for (iter = words.begin() + 1; iter != words.end() ; ++iter)
    {
        joined += sep + *iter;
    }
    return joined;
}

Client ReadClientData(string record, string sep = "#//#")
{
    vector <string> data = SplitString(record, sep);
    Client client;

    client.AccountNumber = data[0];
    client.PinCode = data[1];
    client.FullName = data[2];
    client.PhoneNumber = data[3];
    client.Balance = stod(data[4]);

    return client;
}

Client ChangeClientRecord(string accountNumber)
{
    Client client;

    client.AccountNumber = accountNumber;
    client.PinCode = ReadString("Enter PIN Code     : ");
    client.FullName = ReadString("Enter Full Name    : ");
    client.PhoneNumber = ReadString("Enter Phone Number : ");
    client.Balance = ReadDouble("Enter Balance      : ");

    return client;
}

string GetClientRecord(Client client, string sep = "#//#")
{
    string record = client.AccountNumber + sep +
        client.PinCode + sep +
        client.FullName + sep +
        client.PhoneNumber + sep +
        to_string(client.Balance);
    return record;
}

void SaveClientsToFile(const vector<Client>& clients, string fileName)
{
    fstream File;

    File.open(fileName, ios::out);

    if (File.is_open())
    {
        for (const Client& client : clients)
        {
            if (client.MarkForDelete == false)
            {
                string dataLine = GetClientRecord(client);
                File << dataLine << endl;
            }
        }
        File.close();
    }
}

vector <Client> LoadClientsFromFile(string fileName)
{
    fstream File;
    Client client;

    vector <Client> clients;

    File.open(fileName, ios::in);

    if (File.is_open())
    {
        string dataLine;

        while (getline(File, dataLine))
        {
            client = ReadClientData(dataLine);
            clients.push_back(client);
        }

        File.close();
    }
    return clients;
}

void PrintClientCard(const Client& client)
{
    cout << "\nThe following are the client details : \n";
    PrintLine(40, '-');
    cout << "\nAccount Number     : " << client.AccountNumber << endl;
    cout << "PIN Code           : " << client.PinCode << endl;
    cout << "Full Name          : " << client.FullName << endl;
    cout << "Phone Number       : " << client.PhoneNumber << endl;
    cout << "Account Balance    : " << client.Balance << endl;
    PrintLine(40, '-');
}

string GetMenuHeader(enMenus menu)
{
    string menus[2] = { "Main Menu Screen","Transactions Menu Screen" };
    return menus[menu - 1];
}

string GetTransTaskHeader(enTransMenuOptions option)
{
    string options[3] = { "Deposit Screen","Withdraw Screen","" };
    return options[option - 1];
}

string GetMainTaskHeader(enMainMenuOptions option)
{
    switch (option)
    {
    case enMainMenuOptions::Add:
        return "Add Clients Screen";
    case enMainMenuOptions::Delete:
        return "Delete Client Screen";
    case enMainMenuOptions::Update:
        return "Update Client Screen";
    case enMainMenuOptions::Find:
        return "Find Client Screen";
    case enMainMenuOptions::Exit:
        return "SEE YOU LATER";
    default:
        return "HAHAHAHAH";
    }
}

void PrintTaskHeader(string task)
{
    PrintLine(50, '-');
    cout << "          " << task << endl;
    PrintLine(50, '-');
}

bool FindClient(vector <Client>& clients, Client& target,  string accountNumber)
{
    for (Client& client : clients)
    {
        if (client.AccountNumber == accountNumber)
        {
            target = client;
            return true;
        }
    }
    return false;
}

bool FindTask(vector <Client>& clients)
{
    Client target;

    target.AccountNumber = ReadString("Enter Account Number : ");

    if (FindClient(clients, target, target.AccountNumber))
    {
        PrintClientCard(target);
        return true;
    }
    else
    {
        cout << "Client with Account Number (" << target.AccountNumber << ") Not Found!" << endl;
        return false;
    }
}

bool MarkClientForDelete(vector <Client>& clients, string accountNumber)
{
    for (Client& client : clients)
    {
        if (client.AccountNumber == accountNumber)
        {
            client.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

void DeleteTask(vector <Client>& clients)
{
    Client target;
    target.AccountNumber = ReadString("Enter Account Number : ");
    char choice;
    if (FindClient(clients,target,target.AccountNumber))
    {
        PrintClientCard(target);
        choice = ReadChar("\nAre you sure you want to delete client ? : ");
        if (choice == 'y' || choice == 'Y')
        {
            MarkClientForDelete(clients, target.AccountNumber);
            SaveClientsToFile(clients, CLIENTS_FILE);
            cout << "\nClient Deleted Successfully!" << endl;
        }
    }
    else
    {
        cout << "Client with Account Number (" << target.AccountNumber << ") Not Found!" << endl;
    }
}

bool UpdateTask(vector <Client>& clients)
{
    Client target;
    target.AccountNumber = ReadString("Enter Account Number : ");
    char choice;
    if (FindClient(clients, target, target.AccountNumber))
    {
        PrintClientCard(target);
        choice = ReadChar("\nAre you sure to update client ? : ");
        if (choice == 'y' || choice == 'Y')
        {
            system("cls");
            for (Client& client : clients)
            {
                if (client.AccountNumber == target.AccountNumber)
                {
                    client = ChangeClientRecord(target.AccountNumber);
                    break;
                }
            }
            SaveClientsToFile(clients, CLIENTS_FILE);
            cout << "\nClients Updated Successfully!" << endl;
            return true;
        }
    }
    else
    {
        cout << "Client with Account Number (" << target.AccountNumber << ") Not Found!" << endl;
        return false;
    }
    return false;
}

void AddClient(vector <Client>& clients)
{
    Client client;

    client.AccountNumber = ReadString("Enter Account Number : ");

    while (FindClient(clients, client,  client.AccountNumber))
    {
        cout << "Client with [" << client.AccountNumber << "] already exists, Enter another Account Number : ";
        client.AccountNumber = ReadString("");
    }

    client = ReadClientCommonData(client.AccountNumber);

    clients.push_back(client);
}

void AddTask(vector <Client>& clients)
{

    char choice;

    do
    {
        

        cout << "Adding New Client Data : \n\n";

        AddClient(clients);
        

        choice = ReadChar("Client Added Successfully, Do you want to add another client ? : ");

    } while (choice == 'y' || choice == 'Y');
    SaveClientsToFile(clients, CLIENTS_FILE);
}

bool DepositTask(vector <Client>& clients)
{
    string accNum = ReadAccNumUntilFound(clients);
    double amount = 0;
    char choice = 'n';
    Client client;
    if (FindClient(clients, client, accNum))
    {
        PrintClientCard(client);
        amount = ReadDouble("Please enter deposit amount : ");
        choice = ReadChar("\nAre you sure you want to perform this transaction ? y/n : ");
        if (choice == 'y' || choice == 'Y')
        {
            for (Client& client : clients)
            {
                if (client.AccountNumber == accNum)
                {
                    client.Balance += amount;
                    cout << "Done Successfully!, New Balance is : " << client.Balance << ".\n";
                    return true;
                }
            }
        }
    }
    return false;
}

bool WithdrawTask(vector <Client>& clients)
{
    string accNum = ReadAccNumUntilFound(clients);
    double amount = 0;
    char choice = 'n';
    Client client;
    if (FindClient(clients, client, accNum))
    {
        PrintClientCard(client);
        amount = ReadWithdrawAmount(client);
        choice = ReadChar("\nAre you sure you want to perform this transaction ? y/n : ");
        if (choice == 'y' || choice == 'Y')
        {
            for (Client& client : clients)
            {
                if (client.AccountNumber == accNum)
                {
                    client.Balance -= amount;
                    cout << "Done Successfully!, New Balance is : " << client.Balance << ".\n";
                    return true;
                }
            }
        }
    }
    return false;
}

double GetTotal(vector <Client>& clients)
{
    double total = 0;
    for (Client& client : clients)
    {
        total += client.Balance;
    }
    return total;
}

void PrintClientRecord(Client data)
{
    cout << left;
    cout << "| " << setw(15) << data.AccountNumber;
    cout << "| " << setw(15) << data.PinCode;
    cout << "| " << setw(40) << data.FullName;
    cout << "| " << setw(25) << data.PhoneNumber;
    cout << "| " << setw(15) << data.Balance;
    cout << endl;
}

void PrintBalanceRecord(Client data)
{
    cout << left;
    cout << "| " << setw(20) << data.AccountNumber;
    cout << "| " << setw(50) << data.FullName;
    cout << "| " << setw(40) << data.Balance;
    cout << endl;
}

void ShowAllClientsHeader(const vector <Client>& clients)
{
    system("cls");
    cout << "\t\t\t\t\t\tClient List (" << clients.size() << ") Client(s)." << endl;
    PrintLine(120, '_');
    cout << "\n";
    cout << left;
    cout << "| " << setw(15) << "Account Number";
    cout << "| " << setw(15) << "PIN Code";
    cout << "| " << setw(40) << "Client Name";
    cout << "| " << setw(25) << "Phone";
    cout << "| " << setw(15) << "Balance";
    cout << endl;
    PrintLine(120, '_');
}

void ShowAllBalanceHeader(const vector<Client>& clients)
{
    system("cls");
    cout << "\t\t\t\t\t\tBalance List (" << clients.size() << ") Client(s)." << endl;
    PrintLine(120, '_');
    cout << "\n";
    cout << left;
    cout << "| " << setw(20) << "Account Number";
    cout << "| " << setw(50) << "Client Name";
    cout << "| " << setw(40) << "Balance";
    cout << endl;
    PrintLine(120, '_');
}

void ShowAllBalances(const vector <Client>& clients,double total)
{
    ShowAllBalanceHeader(clients);
    cout << endl;
    for (const Client& client : clients)
    {
        PrintBalanceRecord(client);
    }
    cout << endl;
    PrintLine(120, '_');
    cout << endl;
    cout << right << setw(60) << "Total Balances = " << total << endl;
}

void ShowAllClients(const vector <Client>& clients)
{
    ShowAllClientsHeader(clients);
    cout << endl;
    for (const Client& client : clients)
    {
        PrintClientRecord(client);
    }
    PrintLine(120, '_');
    
}

void PrintMenuHeader(string menuHeader)
{
    PrintLine(50, '=');
    cout << "\t\t" << menuHeader << endl;
    PrintLine(50, '=');
}

void PrintMenuOptions(enMenus menu)
{
    switch (menu)
    {
    case enMenus::Main:
        cout << "          [1] Show Client List.\n";
        cout << "          [2] Add New Client.\n";
        cout << "          [3] Delete Client.\n";
        cout << "          [4] Update Client Info.\n";
        cout << "          [5] Find Client.\n";
        cout << "          [6] Transactions.\n";
        cout << "          [7] Exit.\n";
        break;
    case enMenus::Transactions:
        cout << "          [1] Deposit.\n";
        cout << "          [2] Withdraw.\n";
        cout << "          [3] Total Balances.\n";
        cout << "          [4] Main Menu.\n";
    }
}

void PrintMainMenu()
{
    PrintMenuHeader(GetMenuHeader(enMenus::Main));
    PrintMenuOptions(enMenus::Main);
    PrintLine(50, '=');
}

void PrintTransMenu()
{
    PrintMenuHeader(GetMenuHeader(enMenus::Transactions));
    PrintMenuOptions(enMenus::Transactions);
    PrintLine(50, '=');
}

void GoBackToMainMenu()
{
    cout << "\n\nPress any key to go back to Main Menu...";
    system("pause>0");
}

void GoBackToTransMenu()
{
    cout << "\n\nPress any key to go back to Transaction Menu...";
    system("pause>0");
}

void RunMainOption(vector <Client>& clients, enMainMenuOptions option)
{
    system("cls");
    switch (option)
    {
    case enMainMenuOptions::Show:
        ShowAllClients(clients);
        break;
    case enMainMenuOptions::Add:
        PrintTaskHeader(GetMainTaskHeader(option));
        AddTask(clients);
        break;
    case enMainMenuOptions::Delete:
        PrintTaskHeader(GetMainTaskHeader(option));
        DeleteTask(clients);
        break;
    case enMainMenuOptions::Update:
        PrintTaskHeader(GetMainTaskHeader(option));
        UpdateTask(clients);
        break;
    case enMainMenuOptions::Find:
        PrintTaskHeader(GetMainTaskHeader(option));
        FindTask(clients);
        break;
    case enMainMenuOptions::Transact:
        TransMenu(clients);
        return;
    case enMainMenuOptions::Exit:
        PrintTaskHeader(GetMainTaskHeader(option));
        return;
    }
    GoBackToMainMenu();
}

void RunTransOption(vector <Client>& clients, enTransMenuOptions option)
{
    system("cls");
    switch (option)
    {
    case enTransMenuOptions::Deposit:
        PrintTaskHeader(GetTransTaskHeader(option));
        DepositTask(clients);
        break;
    case enTransMenuOptions::Withdraw:
        PrintTaskHeader(GetTransTaskHeader(option));
        WithdrawTask(clients);
        break;
    case enTransMenuOptions::Balances:
        ShowAllBalances(clients, GetTotal(clients));
        break;
    case enTransMenuOptions::MainMenu:
        return;
    }
    GoBackToTransMenu();
}

void TransMenu(vector <Client>& clients)
{
    enTransMenuOptions option;
    do
    {
        system("cls");
        PrintTransMenu();
        option = ReadTransOption(1, 4);
        RunTransOption(clients, option);
    } 
    while (option != enTransMenuOptions::MainMenu);
}

void Bank()
{
    vector <Client> clients;
    enMainMenuOptions task;
    clients = LoadClientsFromFile(CLIENTS_FILE);
    do
    {
        system("cls");
        PrintMainMenu();
        task = ReadMainOption(1,7);
        RunMainOption(clients, task);
    } 
    while (task != enMainMenuOptions::Exit);
    SaveClientsToFile(clients, CLIENTS_FILE);
}

int main() {


    Bank();
    

    return 0;
}