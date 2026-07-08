#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>

using namespace std;

const string CLIENTS_FILE = "Clients.txt";

enum enMenuOptions { Show = 1, Add = 2, Delete = 3, Update = 4, Find = 5, Exit = 6 };

struct Client
{
    string AccountNumber;
    string PinCode;
    string FullName;
    string PhoneNumber;
    double Balance = 0;
    bool MarkForDelete = false;
};

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

enMenuOptions ReadOption(int choice)
{
    return (enMenuOptions)choice;
}

enMenuOptions ReadOption()
{
    int choice = ReadNumberInRange("Choose what do you want to do ? [1 to 6] : ",1,6);
    return ReadOption(choice);
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

string JoinString(const vector <string>& words, string sep)
{
    if (words.empty() || sep.empty())
        return "";
    if (words.size() == 1)
        return words.at(0);
    string joined = words.at(0);
    for (const string& word : words)
    {
        joined += sep + word;
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
    PrintLine(40, '-');
    cout << "\nThe following are the client details : \n";
    cout << "\nAccount Number     : " << client.AccountNumber << endl;
    cout << "PIN Code           : " << client.PinCode << endl;
    cout << "Full Name          : " << client.FullName << endl;
    cout << "Phone Number       : " << client.PhoneNumber << endl;
    cout << "Account Balance    : " << client.Balance << endl;
    PrintLine(40, '-');
}

string GetTaskHeader(enMenuOptions option)
{
    switch (option)
    {
    case enMenuOptions::Add:
        return "Add Clients Screen";
    case enMenuOptions::Delete:
        return "Delete Client Screen";
    case enMenuOptions::Update:
        return "Update Client Screen";
    case enMenuOptions::Find:
        return "Find Client Screen";
    case enMenuOptions::Exit:
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

void ShowAllClientsHeader(const vector <Client>& clients)
{
    system("cls");
    cout << right << setw(60) << "Client List (" << clients.size() << ") Client(s)." << endl;
    PrintLine(120, '_');
    cout << "\n";
    cout << left;
    cout << "| " << setw(15) << "Account Number";
    cout << "| " << setw(15) << "PIN Code";
    cout << "| " << setw(40) << "Client Name";
    cout << "| " << setw(25) << "Phone";
    cout << "| " << setw(15) << "Balance";
    PrintLine(120, '_');
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

void PrintMainMenuHeader()
{
    PrintLine(50, '=');
    cout << setw(35) << "Main Menu Screen" << endl;
    PrintLine(50, '=');
}

void PrintMainMenuOptions()
{
    cout << "          [1] Show Client List.\n";
    cout << "          [2] Add New Client.\n";
    cout << "          [3] Delete Client.\n";
    cout << "          [4] Update Client Info.\n";
    cout << "          [5] Find Client.\n";
    cout << "          [6] Exit.\n";
}

void PrintMainMenu()
{
    PrintMainMenuHeader();
    PrintMainMenuOptions();
    PrintLine(50, '=');
}




void RunTask(vector <Client>& clients, enMenuOptions option)
{
    system("cls");
    switch (option)
    {
    case enMenuOptions::Show:
        ShowAllClients(clients);
        cout << "\n\nPress any key to go back to Main Menu...";
        system("pause>0");
        break;
    case enMenuOptions::Add:
        PrintTaskHeader(GetTaskHeader(option));
        AddTask(clients);
        break;
    case enMenuOptions::Delete:
        PrintTaskHeader(GetTaskHeader(option));
        DeleteTask(clients);
        break;
    case enMenuOptions::Update:
        PrintTaskHeader(GetTaskHeader(option));
        UpdateTask(clients);
        break;
    case enMenuOptions::Find:
        PrintTaskHeader(GetTaskHeader(option));
        FindTask(clients);
        cout << "\n\nPress any key to go back to Main Menu...";
        system("pause>0");
        break;
    case enMenuOptions::Exit:
        PrintTaskHeader(GetTaskHeader(option));
        break;
    }
}

void Bank()
{
    vector <Client> clients;
    enMenuOptions task;
    do
    {
        clients = LoadClientsFromFile(CLIENTS_FILE);
        system("cls");
        PrintMainMenu();
        task = ReadOption();
        RunTask(clients, task);
    } 
    while (task != enMenuOptions::Exit);
}


//_______________________________________________________


int main() {


    Bank();
    

    return 0;
}