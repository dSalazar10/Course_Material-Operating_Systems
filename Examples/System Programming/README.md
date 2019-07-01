# CS_3560
Intro to systems programming

--------------------------------------------------------
CHAPTER 1:  C Programming
--------------------------------------------------------

Key differences with C++
Command-line Compiling
Basic I/O:  Input, Output
Dynamic Memory Allocation

--------------------------------------------------------
CHAPTER 2: USERS, FILES, AND THE MANUAL
--------------------------------------------------------
System Calls and Functions:
- open, read, write, creat, lseek, close
- perror

Commands
- man, who, cp

Ideas and Skills
- The role and use of on-line documentation
- The Unix file interface:  open, read, write, lseek, close
- Reading, creating, and writing files
- File descriptors
- Buffering:  user level and kernel level
- Kernel mode, user mode, and the cost of system calls
- How Unix represents time, how to format Unix time
- Using the utmp file to find list of current users
- Detecting and reporting errors in system calls


----------------------------------------------------------------
CHAPTER 3:  DIRECTORIES AND FILE PROPERTIES
-----------------------------------------------------------------

System Calls and Functions:
- opendir, readdir, closedir, seekdir
- stat
- chmod, chown, utime
- rename

Commands
- ls

Ideas and Skills
- A directory is a list of files
- How to read a directory
- Types of files and how to determine their type
- Properties of files and how to determine properties of a file
- Bit sets and bit masks
- User and group ID numbers and the passwd database


------------------------------------------------
CHAPTER 4:  FOCUS ON FILE SYSTEMS
------------------------------------------------

System Calls and Functions
- mkdir, rmdir, chdir
- link, unlink, rename symlink

Commands
- pwd

Ideas and Skills
- User's view of the Unix file system tree
- Internal structure of Unix file system:  inodes and data blocks
- How directories are connected
- Hard links, symbolic links:  ideas and system calls

--------------------------------------------------------
CHAPTER 5:  Connection Control, Studying stty
--------------------------------------------------------

Ideas and Skills
* Similarities between files and devices
* Differences between files and devices
* Attributes of Connections
* Race Conditions and atomic operations
* Controlling device drivers
* Streams

System Calls and Functions
* fcntl
* ioctl
* tcsetattr
* tcgetattr


--------------------------------------------------------
CHAPTER 6 - Programming for Humans:  Terminal Control
            and Signals
--------------------------------------------------------

Ideas and Skills
* Software tools vs. user programs
* Reading and changing settings of the terminal driver
* Modes of the terminal driver
* Nonblocking input
* Timeouts on user input
* Introduction to signals:  How Ctrl-C works

System Calls
* fcntl
* signal


--------------------------------------------------------
CHAPTER 8 - Processes and Programs:  Studying sh
--------------------------------------------------------

Ideas and Skills
* What a Unix shell does
* The Unix model of a process
* How to run a program
* How to create a process
* How parent and child processes communicate

System Calls, Functions, Commands
* fork
* exec
* wait
* exit
* sh
* ps


--------------------------------------------------------
CHAPTER 10 - I/O Redirection and Pipes
--------------------------------------------------------

Ideas and Skills
* I/O Redirection:  What and why?
* Definitions of standard input, output, and error.
* Redirecting standard I/O to files.
* Using for to redirect I/O for other programs.
* Pipes
* Using fork with pipes.

System Calls and Functions
* dup, dup2
* pipe

