# File-Management-System
File Management System Using Threaded Binary Search Tree
A C++ application implementing a file management system using a Threaded Binary Search Tree (BST). This project demonstrates efficient file storage, retrieval, and deletion while maintaining the tree's sorted structure. The system uses threads for optimized traversal without recursion or stacks.

## Features
- File Properties:
  - Name
  - Size (in KB)
  - Type (e.g., .txt, .jpg)

  
- Functionalities:

    - Add File: Insert files into the tree in lexicographical order.
    - Delete File: Remove files with proper handling of all node cases (leaf, one child, two children).
    - Display Files: Show all files sorted by name using in-order traversal.
    - Exit: Quit the application.
 
      
- Threaded BST:
   - Uses left and right threads to simplify traversal, avoiding stack-based or recursive methods.
- Tech Stack
   - Programming Language: C++
   - Data Structure: Threaded Binary Search Tree (Threaded BST)

## How to Run

### Clone the repository:
```bash
git clone https://github.com/alok1304/File-Management-System.git
cd File-Management-System
```

### Compile the code using a C++ compiler:
```bash
g++ -o FMS FMS.cpp
```
### Run the application:
```bash
./FMS
```
