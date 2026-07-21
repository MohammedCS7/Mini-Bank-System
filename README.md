# Console Bank Management System

## 📌 Overview
This is a comprehensive, text-based Bank Management System built entirely in standard C++. It was developed as a progressive, hands-on application of foundational software engineering principles, focusing on memory-efficient data structures, procedural abstraction, role-based access control, and persistent file I/O operations.

The project evolved through three distinct development phases—moving from a basic client registry to a fully integrated transactions engine, and finally to a multi-user system with bitwise permission management. It does not pretend to be a production-ready enterprise banking system; rather, it serves as an educational codebase demonstrating a strong grasp of core C++ mechanics, software architecture, and clean code practices.

## ⚙️ Features

### 👤 Client Registry Management
* **CRUD Operations:** Add, search, update, and delete client accounts using a unique Account Number.
* **Data Integrity:** Real-time checking to prevent duplicate account creation.

### 💳 Transactions Engine
* **Deposit & Withdrawal:** Process transactions with real-time balance checks to prevent overdrafts.
* **Total Balances:** View an aggregated, calculated list of all active balances across the system.

### 🔐 User & Permission Management
* **Authentication System:** Secure login workflow verifying user credentials before granting access.
* **Bitwise Permission Control:** Granular access permissions implemented via bitwise flags (`enum`), allowing fine-grained authorization for specific sub-menus (e.g., granting transaction access while restricting client deletion).
* **User Accounts Administration:** Dedicated admin capabilities to add, edit, delete, and list system users.

### ⚙️ Technical Architecture & Memory Handling
* **RAM-First Execution:** Data (`Clients.txt` and `Users.txt`) is loaded into dynamic vectors (`std::vector`) upon startup, executing all operations in high-speed RAM and writing back to disk only during persistent saves.
* **Memory Optimization:** Heavy structures are passed exclusively by reference (`&`) or const-reference (`const &`) to prevent unnecessary object duplication and stack overhead.
* **Input Validation & Utilities:** Dedicated input parsing routines (`ReadString`, `ReadDouble`, `ReadNumberInRange`) that handle terminal streams cleanly, preventing application crashes on invalid data entry.

## 🛠️ Technical Stack
* **Language:** Standard C++ (STL: `<vector>`, `<string>`, `<fstream>`, `<iomanip>`).
* **Storage:** Plain-text files (`Clients.txt`, `Users.txt`) utilizing custom record delimiters (`#//#`).
* **Paradigm:** Procedural / Modular Programming using structured abstraction (`struct`, `enum`).
* **Version Control:** Managed via Git with dedicated feature branching (`feature/user-management`) and explicit release tags (`v1.0`, `v2.0-transactions`, `v3.0-user-management`).

## 🛑 Honest Limitations & Known Issues
In the spirit of complete architectural honesty, the following constraints exist in this codebase:

1. **Plain-Text Security:** Neither client PIN codes nor user passwords are encrypted or hashed. Data resides on disk in plain readable text.
2. **OS Dependency:** Interface clearing relies on `system("cls")` and pause mechanics (`system("pause>0")`), making the UI workflow native to Windows environments. Running on Linux/macOS requires minor modifications to terminal control commands.
3. **Flat-File Bottlenecks:** Uses sequential text parsing rather than an indexed, relational database engine. Scaling to tens of thousands of records will introduce linear I/O overhead during startup/shutdown parsing.
4. **Single-Threaded Execution:** The system is single-threaded and lacks multi-threading synchronization or file-locking primitives, making it unsuitable for concurrent multi-user access on the same physical files.
5. **Call Stack Depth:** Menu transitions rely on direct nested function calls rather than a centralized state-machine loop, which can accumulate stack frames during exceptionally long interactive sessions.

## 🚀 How to Run
1. Ensure you have a standard C++ compiler installed (e.g., GCC/MinGW, Clang, or MSVC).
2. Clone this repository to your local machine:
   ```bash
   git clone <repository-url>
   ```
3. Compile the source files via terminal:
   ```bash
   g++ -O2 main.cpp -o BankSystem
   ```
4. Run the generated executable:
   * **Windows:** `BankSystem.exe`
   * **Linux/macOS:** `./BankSystem`
5. The application will automatically initialize the required storage files (`Clients.txt` and `Users.txt`) in the working directory upon initial execution.

## 👨‍💻 Author
**Muhammad Shihab Al-Din Abdul Majid** - Undergraduate Computer Science Student
