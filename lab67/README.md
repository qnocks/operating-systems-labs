# OS #6. Interworking between processes
## Objective of the work
Organization of interconnection by means of WinAPI and POSIX.

## Task
Organize client-server interaction using WinAPI and POSIX in accordance with an individual task.

1. Calculate the variant number from the list in the log and save it to the file [`TASKID.txt`](TASKID.txt) in the repository.
2. Select an individual task according to the option number. Also determine by variant number: 
- the protocol that should be used to exchange data between the server and the client;
- the operating system for which the server application needs to be developed;
- the operating system for which the client application is to be developed.
3. In the [Issues](../../issues) section of the repository, create at least three issues. For example: "Server application development", "Client application development", "Debugging client-server interaction". Perform these tasks sequentially by writing code and committing it to a repository. The solution of each issue must have its own separate commit, which should be [linked](https://github.blog/2011-04-09-issues-2-0-the-next-generation/#commits-issues) to a specific issue . In total, the repository must have at least three commits. Create additional tasks if necessary.

4. The client application code must be placed in the `client.cpp` file in the repository root, the server application code - in the `server.cpp` file in the same place. If necessary, use additional header files. Code intended to run on Linux must be built with the command `g++ client.cpp` or `g++ server.cpp`. 

5. The repository has a test that checks the correctness of the code format in accordance with the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html). This test launches [linter](https://ru.wikipedia.org/wiki/Lint), which checks if the code complies with the language standard and formatting rules (indents, delimiters, comments, etc.). In some cases, the linter can give advice on how to improve the code using generally accepted practices.

6. There is no automatic testing of code performance. You need to upload the working code to the repository, and then secure the instructor's lab by demonstrating that the client and server applications work together correctly.

7. It is allowed to complete the task not in full. The rules for calculating the rating for this case are given in the [Rating](#Rating) section.

---

**NB1!** To get the maximum possible number of points on the defense of laboratory work, you must demonstrate the operation of the system on several computers connected to a local network. To do this, you can use a local network between machines in a university classroom or a virtual local area network in a VM Ware or Virtual Box virtualization system. You should demonstrate the correct networking of client and server applications running on different operating systems (Windows and Linux/Mac OS).

**NB2!** A commit will not be counted as a solution to a specific issue from the [Issues](../../issues) section of the repository if it is not linked to it. To bind a commit to an issue, when creating a commit in the commit message, mention the issue number using [special syntax](https://docs.github.com/en/github/writing-on-github/autolinked-references-and-urls#issues -and-pull-requests). For example, `Add server code for #2`, `Fix issue #9`, etc.

---

## Variants
| Variant number    | Individual task | Protocol  | Server  | Client  |
| ------------- |:----------------------:|:---------:|:-------:|:-------:|
| **1**         | 1                      | TCP       | Windows |  Linux  |
| **2**         | 2                      | UDP       | Linux   | Windows |
| **3**         | 3                      | TCP       | Windows |  Linux  |
| **4**         | 4                      | UDP       | Linux   | Windows |
| **5**         | 5                      | TCP       | Windows |  Linux  |
| **6**         | 6                      | UDP       | Linux   | Windows |
| **7**         | 7                      | TCP       | Windows |  Linux  |
| **8**         | 8                      | UDP       | Linux   | Windows |
| **9**         | 9                      | TCP       | Windows |  Linux  |
| **10**        | 10                     | UDP       | Linux   | Windows |
| **11**        | 11                     | TCP       | Windows |  Linux  |
| **12**        | 12                     | UDP       | Linux   | Windows |
| **13**        | 13                     | TCP       | Windows |  Linux  |
| **14**        | 14                     | UDP       | Linux   | Windows |
| **15**        | 15                     | TCP       | Windows |  Linux  |
| **16**        | 15                     | UDP       | Linux   | Windows |
| **17**        | 14                     | TCP       | Windows |  Linux  |
| **18**        | 13                     | UDP       | Linux   | Windows |
| **19**        | 12                     | TCP       | Windows |  Linux  |
| **20**        | 11                     | UDP       | Linux   | Windows |
| **21**        | 10                     | TCP       | Windows |  Linux  |
| **22**        | 9                      | UDP       | Linux   | Windows |
| **23**        | 8                      | TCP       | Windows |  Linux  |
| **24**        | 7                      | UDP       | Linux   | Windows |
| **25**        | 6                      | TCP       | Windows |  Linux  |
| **26**        | 5                      | UDP       | Linux   | Windows |
| **27**        | 4                      | TCP       | Windows |  Linux  |
| **28**        | 3                      | UDP       | Linux   | Windows |
| **29**        | 2                      | TCP       | Windows |  Linux  |
| **30**        | 1                      | UDP       | Linux   | Windows |

## Individual tasks
General requirements for all tasks:
- client and server are console applications;
- the port number on which the server is running is specified when it is started as a command line argument;
- the domain name (ip-address) on which the server is running, as well as its port number, are specified as command line arguments when starting the client;
- the server prints to the console all messages it receives from the client;
- the client prints to the console all messages it receives from the server.

1.  **Calculator**. The client application prompts the user for a string of the form `<number> <mathematical_operation> <number>` and passes it to the server. The server performs the mathematical operation and returns the result to the client. Examples:  
    The user enters: `1 + 2`. Server response: `3`  
    The user enters: `2 * 3`. Server response: `6`  
    The user enters: `4 - 7`. Server response: `-3`  
    The user enters: `30 / 10`. Server response: `3`
2.  **Remote viewing of files**. The client application prompts the user for one of three commands: `dir`, `get <filename>`, `quit` and sends it to the server. The server returns the result of executing the received command to the client. Teams:  
    `dir` - the server asks the operating system for a list of files in the current directory and sends the result to the client; file names must be separated by a newline character;  
    `get <filename>` - the server sends the contents of the file with the specified name to the client; if the file is not found or the server does not have access to it, the server sends the number 1 to the client; if the file is found, the server sends the number 0, then the size of the file in bytes, and then the contents of the file;  
    `quit` - the client requests the end of work; the server sends the number 0 to the client and exits; the client receives the server's response and exits as well.
3.  **Word count**. The client application prompts the user for the name of a text file and sends the contents of this file to the server. To do this, the size of the file in bytes is passed first, and then its contents. The server returns the number of words in the received file to the client.
4.  **File type check**. The client application prompts the user for a filename and sends the contents of that file to the server. The server parses the [file signature (magic number)](https://ru.wikipedia.org/wiki/List_of_file_signatures) and returns the file type to the client as one of the following strings: `exe`, `elf`, `png`, ` pdf`, `zip`, `unknown`.
5.  **Search for files by mask**. The client application prompts the user for two strings: a path and a file extension. These strings are passed to the server, which asks the operating system for a list of all files with the specified extension in the specified path. In response to the client, the server sends the version of the operating system, the number of files found and their names. The separator between file names is the newline character. You can use [`GetVersionEx()`](https://docs.microsoft.com/en-gb/windows/win32/api/sysinfoapi/nf-sysinfoapi-getversionexa) or `Version Helper to determine the operating system version on Windows OS. functions` (see [example](https://docs.microsoft.com/en-gb/windows/win32/sysinfo/getting-the-system-version)), on Linux and MacOS the [`uname ()`](https://linux.die.net/man/2/uname);  
6.  **Triangles**. The client application prompts the user for a number, which is sent to the server. The server generates 3 random numbers. If these numbers form the sides of a triangle, then they are passed to the client, otherwise the server prints them to the console. The procedure is repeated until the server generates as many triplets of numbers forming the sides of the triangle as the client originally requested.
7.  **Word repetitions**. The client application prompts the user for two strings: a word and the name of a text file located on the server. These strings are passed to the server, which counts the number of occurrences of the specified word in the selected text file. The resulting number is returned to the client.
8.  **Case change**. The client application prompts the user for a string, which is passed to the server. The server reverses the case of all alphabetic characters in the received string and returns the result to the client. Examples:  
    The user enters: `AbCd`. Server response: `aBcD`.  
    The user enters: `1q2/A`. Server response: `1Q2/a`.  
    The user types: `i-g`. Server response: `i-G`.
9.  **Server Specifications**. The client application prompts the user for one of three commands: `date`, `ps`, `quit` and sends it to the server. The server returns the result of executing the received command to the client. Teams:  
    `date` - the server returns the current date and time to the client as a string in [ISO format](https://ru.wikipedia.org/wiki/ISO_8601) `YYYY-MM-DDThh:mm:ss±hh`;  
    `ps` - the server sends to the client the number of running processes and their names, separated by a newline character; on Windows, you can use the [`EnumProcess()`](https://docs.microsoft.com/en-us/windows/win32/api/psapi/nf-psapi-enumprocesses) function (see [example](https ://docs.microsoft.com/en-us/windows/win32/psapi/enumerating-all-processes)), on Linux, read the contents of the files `/proc/<pid>/cmdline` for all available process IDs ` <pid>`, and on MacOS X and BSD use the code from [QA1123](https://developer.apple.com/library/archive/qa/qa2001/qa1123.html) (see also [answer with example] (https://stackoverflow.com/a/12433047) using the `GetBSDProcessList` function and [full code](https://pastebin.com/etd3kxJD) example);  
    `quit` - the client requests the end of work; the server sends the number 0 to the client and exits; the client receives the server's response and exits as well.
10. **Authorization**. The client application asks the user to enter two strings: login and password. These strings are passed to the server, which checks the correctness of the entered data. After the third unsuccessful attempt in a row to send the correct `<login, password>` pair, the server should block the client's IP address for 1 minute. Demonstrate that while blocking one client, a client with a different IP address can connect to the server.
11. **Reminder**. The client application prompts the user for a string and an integer N and passes them to the server. The server starts sending the received string to the client every N seconds until the client disconnects.
12. **Encryption**. The client application prompts the user for a string and an integer N and passes them to the server. The server converts the string using [Caesar cipher](https://ru.wikipedia.org/wiki/Caesar_cipher) with shift N and sends the encrypted string back to the client. The client verifies that it can recover the received string.
13. **Prime numbers**. The client application prompts the user for a number and passes it to the server. The server checks if the received number is prime and reports the result to the client. The server also sends the client a string containing the previously received number, written in words. For example:
    The user enters: `37`. Server response: `1 thirty seven` (or `1 thirty seven`).
    The user enters: `10`. Server response: `0 ten` (or `0 ten`).
14. **Registration**. The client application prompts the user for one of three commands: `reg <username>`, `list`, `quit` and sends it to the server. The server returns the result of executing the received command to the client. Teams:  
    `reg <username>` - the server adds a new user with the specified name to the list of registered users, and also saves the IP address and port identifying this client; if a user with the same name already exists, the server sends the number 1 to the client; if this is a new user, the server sends the number 0 and stores the triplet `<username>`, client IP address, client port;  
    `list` - the server sends to the client the number of all registered users and their list with the username, IP address and connection port for each of them; information about different users must be separated by a newline character;  
    `quit` - the client requests the end of work; the server sends the number 0 to the client and goes into the waiting mode for new clients to connect; the client receives the server's response and exits.
15. **Free place**. The client application prompts the user for a path and passes it to the server. The server, using a system call, determines the free space on the disk that the received path points to. Two numbers are sent to the client: information about the amount of free and used space on the server's disk (in bytes). On Linux and MacOS use [`statvfs`]() (see [example](https://stackoverflow.com/a/12686166)), on Windows use [`GetDiskFreeSpaceA`](https://docs.microsoft. com/en-gb/windows/win32/api/fileapi/nf-fileapi-getdiskfreespacea).


# OS #7. Continuous Integration in the Cloud Using GitHub Actions
## Objective of the work
Studying the principles of organizing continuous integration of applications using cloud technologies.

## Task
1. Add the configuration file `.github/workflows/tests.yml` to the repository of the previous lab, which controls the creation of a virtual machine in the cloud, on which the build and test of the project will automatically start. Examples of configuration file design can be found in the repositories of labs ## 1-6, in the [official documentation] (https://docs.github.com/en/actions), as well as by searching the Internet. The task (`job`) in the configuration file must be named `test`, using other names will result in loss of points. The commands in the `steps` section of the task should run the build of that part of the previous lab, which, according to the option, was performed on Linux (or Mac OS). For example, if according to the task variant for the previous laboratory work, it was necessary to develop the server part of the application under Linux OS, and the client part of the application under Windows OS, then in this laboratory work it is necessary to automatically build the server part of the application. The build should succeed (no errors).
2. (**NB**! This task item is optional) *Develop at least one test to verify the correct functioning of the application, which was automatically built in the previous step. Add build and run test(s) to `tests.yml`.* configuration file
3. By analogy with the previous work, the task execution process should be broken down into tasks, which should be described in the Issues section of the repository. At least one commit must correspond to the solution of each problem. Task names must begin with the prefix `Lab6:`, the same prefix must appear in the name of the commit(s). Commits and issues without a prefix will not be counted.


---

<a name="footnote1">Пример</a> подхода к тестированию аргументов командной строки. 

Пусть необходимо протестировать работу сервера. Если сервер принимает на вход одно число (номер порта), то это число должно быть целым и лежать в определенном диапазоне. В этом случае потенциальными тестами могут быть: 
- проверка, что программа завершится аварийно, если количество аргументов командной строки равно нулю или более одного;
- проверка, что программа завершится аварийно, если аргумент командной строки не является целым числом;
- проверка, что программа завершится аварийно, если аргумент командной строки является числом, не соответствующим допустимым значениям для номера порта.

В случае тестирования аргументов командной строки клиента действовать по аналогии.
