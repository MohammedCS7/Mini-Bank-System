#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;

const string CLIENTS_FILE = "Clients.txt";
const string USERS_FILE = "Users.txt";

enum enMainMenuOptions { Show = 1, Add = 2, Delete = 3, Update = 4, Find = 5, Transact = 6, Users = 7, Logout = 8 };

enum enMenus { Main = 1, Transactions = 2, UserMenu = 3 };

enum enTransMenuOptions { Deposit = 1, Withdraw = 2, Balances = 3, MainMenuTrans = 4 };

enum enUserMenuOptions { UserShow = 1, UserAdd = 2, UserDelete = 3, UserUpdate = 4, UserFind = 5, UserMainMenu = 6 };

enum enPermissions { Admin = -1,ShowCli = 1, AddCli = 2, DeleteCli = 4, UpdateCli = 8, FindCli = 16, Transaction = 32 };

struct Client
{
    string AccountNumber;
    string PinCode;
    string FullName;
    string PhoneNumber;
    double Balance = 0;
    bool MarkForDelete = false;
};

struct User
{
    string UserName;
    string Password;
    short Permissions;
    bool MarkForDelete = false;
};

User CurrentUser;

void ShowUserMenu(vector <Client>& clients, vector <User>& users,const User& user);
void ShowTransMenu(vector <Client>& clients,vector <User>& users,const User& user);
void Login(vector <Client>& clients, vector <User>& users);
void GoBackToUserMenu(vector <Client>& clients, vector <User>& users,const User& user);
void ShowMainMenu(vector <Client>& clients, vector <User>& users,const User& user);

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
    cout << endl;
    return ch;
}

int ReadChoice(int from, int to)
{
    cout << "Choose what do you want to do ? [" << from << "] to [" << to << "] : ";
    int choice = ReadNumberInRange("", from, to);
    return choice;
}

bool IsChoiceCorrect(char choice)
{
    return (choice == 'y' || choice == 'Y');
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

enUserMenuOptions ReadUserOption(int from, int to)
{
    int choice = ReadChoice(from, to);
    return (enUserMenuOptions)choice;
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

short ReadPermissions()
{
    short per = 0;
    char choice;
    choice = ReadChar("Do you want admin permission ? y/n : ");
    if (IsChoiceCorrect(choice))
    {
        per += enPermissions::Admin;
        return per;
    }

    cout << "\nDo you want to get access to : \n";

    choice = ReadChar("Do you want to show all clients ? y/n : ");
    if (IsChoiceCorrect(choice))
    {
        per += enPermissions::ShowCli;
    }
    choice = ReadChar("Add New Clients ? y/n : ");
    if (IsChoiceCorrect(choice))
    {
        per += enPermissions::AddCli;
    }
    choice = ReadChar("Delete Clients ? y/n : ");
    if (IsChoiceCorrect(choice))
    {
        per += enPermissions::DeleteCli;
    }
    choice = ReadChar("Update Clients ? y/n : ");
    if (IsChoiceCorrect(choice))
    {
        per += enPermissions::UpdateCli;
    }
    choice = ReadChar("Find Clients ? y/n : ");
    if (IsChoiceCorrect(choice))
    {
        per += enPermissions::FindCli;
    }
    choice = ReadChar("Transactions ? y/n : ");
    if (IsChoiceCorrect(choice))
    {
        per += enPermissions::Transaction;
    }

    return per;
}

bool CheckPermission(User user, enMainMenuOptions option)
{
    return ((user.Permissions & option) == option || user.Permissions == -1);
}

User ReadUserCommonData(string username = "")
{
    User user;
    user.UserName = username;
    user.Password = ReadString("Enter Password : ");
    user.Permissions = ReadPermissions();

    return user;

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

User ReadUserData(string dataLine)
{
    vector <string> data = SplitString(dataLine, "//#");

    User user;

    user.UserName = data[0];
    user.Password = data[1];
    user.Permissions = stoi(data[2]);

    return user;
}



User ChangeUserRecord(string username)
{
    User user;

    user.UserName = username;
    user.Password = ReadString("Enter New Password : ");
    user.Permissions = ReadPermissions();

    return user;
}

string GetUserRecord(User user, string sep = "//#")
{
    return user.UserName + sep + user.Password + sep + to_string(user.Permissions);
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

void SaveUsersToFile(vector<User>& users, string fileName)
{
    fstream File;

    File.open(fileName, ios::out);

    if (File.is_open())
    {
        for (const User& user : users)
        {
            if (user.MarkForDelete == false)
            {
                string dataLine = GetUserRecord(user);
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

void SaveUsersToFile(const vector <User>& users, string fileName)
{
    fstream File;

    File.open(fileName, ios::out);

    if (File.is_open())
    {
        string dataLine;
        for (User user : users)
        {
            dataLine = GetUserRecord(user);
            File << dataLine << endl;
        }
        File.close();
    }
}

vector <User> LoadUsersFromFile(string fileName)
{
    fstream File;
    vector <User> users;

    File.open(fileName, ios::in);

    if (File.is_open())
    {
        string dataLine;

        while (getline(File, dataLine))
        {
            users.push_back(ReadUserData(dataLine));
        }
        File.close();
    }

    return users;
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

void PrintUserCard(const User& user)
{
    cout << "\nThe following are the user details : \n";
    PrintLine(40, '-');
    cout << "\nUsername   : " << user.UserName << endl;
    cout << "Password       : " << user.Password << endl;
    cout << "Permissions    : " << user.Permissions << endl;
    PrintLine(40, '-');
}

string GetMenuHeader(enMenus menu)
{
    string menus[3] = { "Main Menu Screen","Transactions Menu Screen","Manage Users Screen"};
    return menus[menu - 1];
}

string GetTransTaskHeader(enTransMenuOptions option)
{
    string options[3] = { "Deposit Screen","Withdraw Screen","" };
    return options[option - 1];
}

string GetUserTaskHeader(enTransMenuOptions option)
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
    default:
        return "HAHAHAHAH";
    }
}

string GetUserTaskHeader(enUserMenuOptions option)
{
    string options[6] = {"","","Add New User Screen","Delete User Screen","Update User Screen","Find User Screen"};
    return options[option];
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

bool FindUser(vector <User>& users, User& target, string userName)
{
    for (User& user : users)
    {
        if (user.UserName == userName)
        {
            target = user;
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

bool FindTask(vector <User>& users)
{
    User target;

    target.UserName = ReadString("Enter the Username : ");

    if (FindUser(users, target, target.UserName))
    {
        PrintUserCard(target);
        return true;
    }
    else
    {
        cout << "User with UserName (" << target.UserName << ") Not Found!" << endl;
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

bool MarkUserForDelete(vector <User>& users, string username)
{
    for (User& user : users)
    {
        if (user.UserName == username)
        {
            user.MarkForDelete = true;
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

void DeleteTask(vector <User>& users)
{
    User target;
    target.UserName = ReadString("Enter Username : ");
    char choice;
    if (FindUser(users, target, target.UserName))
    {
        PrintUserCard(target);
        choice = ReadChar("\nAre you sure you want to delete user ? : ");
        if (choice == 'y' || choice == 'Y')
        {
            MarkUserForDelete(users, target.UserName);
            SaveUsersToFile(users, USERS_FILE);
            cout << "\nUser Deleted Successfully!" << endl;
        }
    }
    else
    {
        cout << "User with Username (" << target.UserName << ") Not Found!" << endl;
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

bool UpdateTask(vector <User>& users)
{
    User target;
    target.UserName = ReadString("Enter Username : ");
    char choice;
    if (FindUser(users, target, target.UserName))
    {
        PrintUserCard(target);
        choice = ReadChar("\nAre you sure to update client ? : ");
        if (choice == 'y' || choice == 'Y')
        {
            system("cls");
            for (User& user : users)
            {
                if (user.UserName == target.UserName)
                {
                    user = ChangeUserRecord(target.UserName);
                    break;
                }
            }
            SaveUsersToFile(users, USERS_FILE);
            cout << "\User Updated Successfully!" << endl;
            return true;
        }
    }
    else
    {
        cout << "User with Account Number (" << target.UserName << ") Not Found!" << endl;
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

void AddUser(vector <User>& users)
{
    User user;

    user.UserName = ReadString("Enter Username : ");

    while (FindUser(users, user, user.UserName))
    {
        cout << "User with [" << user.UserName << "] already exists, Enter another Account Number : ";
        user.UserName = ReadString("");
    }

    user = ReadUserCommonData(user.UserName);

    users.push_back(user);
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

void AddTask(vector <User>& users)
{

    char choice;

    do
    {


        cout << "Adding New User Data : \n\n";

        AddUser(users);


        choice = ReadChar("User Added Successfully, Do you want to add another user ? : ");

    } while (choice == 'y' || choice == 'Y');
    SaveUsersToFile(users, USERS_FILE);
}

bool UpdateClientBalance(vector <Client>& clients,Client& target, double amount, string accountNumber)
{
    for (Client& client : clients)
    {
        if (client.AccountNumber == accountNumber)
        {
            client.Balance += amount;
            target = client;
            return true;
        }
    }
    return false;
}

bool DepositTask(vector <Client>& clients)
{
    Client target;
    string accNum = ReadAccNumUntilFound(clients);
    FindClient(clients, target, accNum);
    PrintClientCard(target);
    double amount = ReadDouble("Please enter deposit amount : ");
    char choice = ReadChar("Are you sure to perform this transaction ? y/n : ");
    if (choice == 'y' || choice == 'Y')
    {
        UpdateClientBalance(clients,target,amount,accNum);
        cout << "Done Successfully!, New Balance is : " << target.Balance << ".\n";
        SaveClientsToFile(clients, CLIENTS_FILE);
        return true;
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
            UpdateClientBalance(clients, client, -amount, accNum);
            cout << "Done Successfully!, New Balance is : " << client.Balance << ".\n";
            SaveClientsToFile(clients, CLIENTS_FILE);
        }
    }
    return false;
}

bool CheckUsernameAndPassword(vector <User>& users,User& target)
{
    for (User user : users)
    {
        if (user.UserName == target.UserName && user.Password == target.Password)
        {
            target.Permissions = user.Permissions;
            return true;
        }
    }
    return false;
}

bool LoginWithUsernameAndPassword(vector <User>& users, User& target)
{
     target.UserName = ReadString("Enter the username : ");
     target.Password = ReadString("Enter the password : ");
     return CheckUsernameAndPassword(users,target);
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

void PrintUserRecord(User data)
{
    cout << left;
    cout << "| " << setw(20) << data.UserName;
    cout << "| " << setw(50) << data.Password;
    cout << "| " << setw(40) << data.Permissions;
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

void ShowAllUsersHeader(const vector<User>& users)
{
    system("cls");
    cout << "\t\t\t\t\t\Users List (" << users.size() << ") User(s)." << endl;
    PrintLine(120, '_');
    cout << "\n";
    cout << left;
    cout << "| " << setw(20) << "UserName";
    cout << "| " << setw(50) << "Password";
    cout << "| " << setw(40) << "Permissions";
    cout << endl;
    PrintLine(120, '_');
}

void ShowAllUsers(const vector <User>& users)
{
    ShowAllUsersHeader(users);
    cout << endl;
    if (users.size() == 0)
    {
        cout << "\t\t\t\tThere are No Users in the System!.\n";
    }
    else
    {
        for (User user : users)
        {
            PrintUserRecord(user);
        }
    }
    cout << endl;
    PrintLine(120, '_');
}

void ShowAllBalances(const vector <Client>& clients,double total)
{
    ShowAllBalanceHeader(clients);
    cout << endl;
    if (clients.size() == 0)
    {
        cout << "\t\t\t\tThere is NO Clients in the System!.\n";
        cout << endl;
        PrintLine(120, '_');
    }
    else
    {
        for (const Client& client : clients)
        {
            PrintBalanceRecord(client);
        }
        cout << endl;
        PrintLine(120, '_');
        cout << endl;
        cout << right << setw(60) << "Total Balances = " << total << endl;
    }
    
}

void ShowAllClients(const vector <Client>& clients)
{
    ShowAllClientsHeader(clients);
    cout << endl;
    if (clients.size() == 0)
    {
        cout << "\t\t\t\tThere is NO Clients in the System!.\n";
    }
    else
    {
        
        for (const Client& client : clients)
        {
            PrintClientRecord(client);
        }
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
        cout << "          [7] Manage Users.\n";
        cout << "          [8] Logout.\n";
        break;
    case enMenus::Transactions:
        cout << "          [1] Deposit.\n";
        cout << "          [2] Withdraw.\n";
        cout << "          [3] Total Balances.\n";
        cout << "          [4] Main Menu.\n";
        break;
    case enMenus::UserMenu:
        cout << "          [1] List Users.\n";
        cout << "          [2] Add New Users.\n";
        cout << "          [3] Delete User.\n";
        cout << "          [4] Update User.\n";
        cout << "          [5] Find User.\n";
        cout << "          [6] Main Menu.\n";
        break;
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

void PrintUsersMenu()
{
    PrintMenuHeader(GetMenuHeader(enMenus::UserMenu));
    PrintMenuOptions(enMenus::UserMenu);
    PrintLine(50, '=');
}

void GoBackToMainMenu(vector <Client>& clients,vector <User>& users,const User& user)
{
    cout << "\n\nPress any key to go back to Main Menu...";
    system("pause>0");
    ShowMainMenu(clients,users,user);
}

void GoBackToTransMenu(vector <Client>& clients,vector <User>& users,const User& user)
{
    cout << "\n\nPress any key to go back to Transaction Menu...";
    system("pause>0");
    ShowTransMenu(clients,users,user);
}

void RunMainOption(vector <Client>& clients,vector <User>& users,const User& user, enMainMenuOptions option)
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
        ShowTransMenu(clients,users,user);
        break;
    case enMainMenuOptions::Users:
        ShowUserMenu(clients,users,user);
        break;
    case enMainMenuOptions::Logout:
        Login(clients,users);
        return;
    }
    GoBackToMainMenu(clients,users,user);
}

void RunTransOption(vector <Client>& clients,vector <User>& users,const User& user, enTransMenuOptions option)
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
    case enTransMenuOptions::MainMenuTrans:
        ShowMainMenu(clients,users,user);
    }
    GoBackToTransMenu(clients,users,user);
}

void RunUsersOption(vector <Client>& clients,vector <User>& users,const User& user, enUserMenuOptions option)
{
    system("cls");
    switch (option)
    {
    case enUserMenuOptions::UserShow:
        ShowAllUsers(users);
        break;
    case enUserMenuOptions::UserAdd:
        PrintTaskHeader(GetUserTaskHeader(option));
        AddTask(users);
        break;
    case enUserMenuOptions::UserDelete:
        PrintTaskHeader(GetUserTaskHeader(option));
        DeleteTask(users);
        break;
    case enUserMenuOptions::UserUpdate:
        PrintTaskHeader(GetUserTaskHeader(option));
        UpdateTask(users);
        break;
    case enUserMenuOptions::UserFind:
        PrintTaskHeader(GetUserTaskHeader(option));
        FindTask(users);
        break;
    case enUserMenuOptions::UserMainMenu:
        ShowMainMenu(clients, users, user);
    }
    GoBackToUserMenu(clients,users,user);
}


void ShowUserMenu(vector <Client>& clients,vector <User>& users,const User& user)
{
    system("cls");
    PrintUsersMenu();
    RunUsersOption(clients,users,user, ReadUserOption(1, 6));
}

void GoBackToUserMenu(vector <Client>& clients, vector <User>& users,const User& user)
{
    cout << "\n\nPress any key to go back to Users Menu...";
    system("pause>0");
    ShowUserMenu(clients, users, user);
}

void ShowTransMenu(vector <Client>& clients,vector <User>& users,const User& user)
{
        system("cls");
        PrintTransMenu();
        RunTransOption(clients,users,user,ReadTransOption(1, 4));
}

void AccessDeniedMessage()
{
    system("cls");
    PrintLine(50, '-');
    cout << "          Access Denied," << endl;
    cout << "You Don't Have Permission To Do this,\n";
    cout << "Please Contact Your Admin." << endl;
    PrintLine(50, '-');
    cout << endl;
}

void CheckOptionPermissionAndRun(vector <Client>& clients, vector <User>& users,const User& user, enMainMenuOptions option)
{
    if (!CheckPermission(user, option) && option != enMainMenuOptions::Logout)
    {
        AccessDeniedMessage();
        GoBackToMainMenu(clients, users, user);
    }
    else
    {
        RunMainOption(clients, users, user, option);
    }
}

void ShowMainMenu(vector <Client>& clients,vector <User>& users,const User& user)
{
        system("cls");
        PrintMainMenu();
        CheckOptionPermissionAndRun(clients, users, user,ReadMainOption(1,8));
}

void Login(vector <Client>& clients,vector <User>& users)
{
    bool login = true;
    do
    {
        system("cls");
        PrintTaskHeader("Login Screen");
        if (login == false)
            cout << "Invalid Username/Password" << endl;
        login = LoginWithUsernameAndPassword(users, CurrentUser);
    } while (!login);
    ShowMainMenu(clients, users, CurrentUser);
}

int main() {

    vector <Client> clients;
    vector <User> users;
    users = LoadUsersFromFile(USERS_FILE);
    clients = LoadClientsFromFile(CLIENTS_FILE);
    Login(clients, users);
    return 0;
}