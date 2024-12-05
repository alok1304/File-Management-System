#include <iostream>
#include <string>
using namespace std;

class File
{
public:
    string name;
    int size;
    string type;

    File(string name = "", int size = 0, string type = "")
        : name(name), size(size), type(type) {}
};

class ThreadedBST
{
private:
    class Node
    {
    public:
        File info;
        Node *left;
        bool lthread;
        Node *right;
        bool rthread;

        Node(File file)
            : info(file), left(nullptr), lthread(true), right(nullptr), rthread(true) {}
    };

    Node *root;

    Node *inSucc(Node *temp)
    {
        if (temp->rthread)
        {
            return temp->right;
        }
        temp = temp->right;
        while (!temp->lthread)
        {
            temp = temp->left;
        }
        return temp;
    }

    Node *inPred(Node *temp)
    {
        if (temp->lthread)
        {
            return temp->left;
        }
        temp = temp->left;
        while (!temp->rthread)
        {
            temp = temp->right;
        }
        return temp;
    }

    Node *caseA(Node *par, Node *temp)
    {
        if (!par)
        {
            root = nullptr;
        }
        else if (par->left == temp)
        {
            par->lthread = true;
            par->left = temp->left;
        }
        else
        {
            par->rthread = true;
            par->right = temp->right;
        }
        delete temp;
        return root;
    }

    Node *caseB(Node *par, Node *temp)
    {
        Node *child = temp->lthread ? temp->right : temp->left;

        if (!par)
        {
            root = child;
        }
        else if (par->left == temp)
        {
            par->left = child;
        }
        else
        {
            par->right = child;
        }

        Node *s = inSucc(temp);
        Node *p = inPred(temp);

        if (!temp->lthread)
        {
            p->right = s;
        }
        else if (!temp->rthread)
        {
            s->left = p;
        }

        delete temp;
        return root;
    }

    Node *caseC(Node *par, Node *temp)
    {
        Node *succ = temp->right;
        Node *parsucc = temp;

        while (!succ->lthread)
        {
            parsucc = succ;
            succ = succ->left;
        }

        temp->info = succ->info;

        if (succ->lthread && succ->rthread)
        {
            root = caseA(parsucc, succ);
        }
        else
        {
            root = caseB(parsucc, succ);
        }

        return root;
    }

    void inorderTraversal(Node *node)
    {
        if (!node)
        {
            cout << "\nTree is empty.\n";
            return;
        }

        Node *temp = node;
        while (!temp->lthread)
        {
            temp = temp->left;
        }

        while (temp)
        {
            displayFile(temp);
            temp = inSucc(temp);
        }
    }

    void displayFile(Node *node)
    {
        cout << "\nFile Name: " << node->info.name;
        cout << "\nSize: " << node->info.size << " KB";
        cout << "\nType: " << node->info.type << "\n";
    }

public:
    ThreadedBST() : root(nullptr) {}

    void insert(const File &file)
    {
        Node *NewNode = new Node(file);
        Node *temp = root;
        Node *par = nullptr;

        while (temp)
        {
            par = temp;
            if (file.name == temp->info.name)
            {
                cout << "\nFile \"" << file.name << "\" already exists.\n";
                delete NewNode;
                return;
            }
            if (file.name < temp->info.name)
            {
                if (!temp->lthread)
                {
                    temp = temp->left;
                }
                else
                {
                    break;
                }
            }
            else
            {
                if (!temp->rthread)
                {
                    temp = temp->right;
                }
                else
                {
                    break;
                }
            }
        }
        if (!par)
        {
            root = NewNode;
        }
        else if (file.name < par->info.name)
        {
            NewNode->left = par->left;
            NewNode->right = par;
            par->lthread = false;
            par->left = NewNode;
        }
        else
        {
            NewNode->left = par;
            NewNode->right = par->right;
            par->rthread = false;
            par->right = NewNode;
        }
        cout << "\nFile \"" << file.name << "\" inserted successfully.\n";
    }

    void remove(const string &fname)
    {
        Node *temp = root;
        Node *par = nullptr;

        while (temp)
        {
            if (fname == temp->info.name)
            {
                break;
            }
            par = temp;
            if (fname < temp->info.name)
            {
                if (!temp->lthread)
                {
                    temp = temp->left;
                }
                else
                {
                    break;
                }
            }
            else
            {
                if (!temp->rthread)
                {
                    temp = temp->right;
                }
                else
                {
                    break;
                }
            }
        }

        if (!temp || temp->info.name != fname)
        {
            cout << "\nFile not found: \"" << fname << "\".\n";
            return;
        }

        if (!temp->lthread && !temp->rthread) // The node has two children.
        {
            root = caseC(par, temp);
        }
        else if (!temp->lthread || !temp->rthread) // The node has one child.
        {
            root = caseB(par, temp);
        }
        else // The node is a leaf node.
        {
            root = caseA(par, temp);
        }

        cout << "\nFile \"" << fname << "\" deleted successfully.\n";
    }

    void displayInorder()
    {
        inorderTraversal(root);
    }
};

int main()
{
    ThreadedBST tree;
    string choice;

    while (true)
    {
        cout << "\nFile Management System\n";
        cout << "1. Add File\n";
        cout << "2. Delete File\n";
        cout << "3. Display All Files (Inorder)\n";
        cout << "4. Quit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == "1")
        {
            File file;
            cout << "Enter file name: ";
            cin >> file.name;
            cout << "Enter file size (in KB): ";
            cin >> file.size;
            cout << "Enter file type: ";
            cin >> file.type;
            tree.insert(file);
         
        }
        else if (choice == "2")
        {
            string fname;
            cout << "Enter the file name to delete: ";
            cin >> fname;
            tree.remove(fname);
           
        }
        else if (choice == "3")
        {
            tree.displayInorder();
          
        }
        else if (choice == "4")
        {
            cout << "\nExiting...\n";
            return 0;
        }
        else
        {
            cout << "\nInvalid choice. Try again.\n";
        }
    }
}
