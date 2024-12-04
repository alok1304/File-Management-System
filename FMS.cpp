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

    Node *inSucc(Node *ptr)
    {
        if (ptr->rthread)
        {
            return ptr->right;
        }
        ptr = ptr->right;
        while (!ptr->lthread)
        {
            ptr = ptr->left;
        }
        return ptr;
    }

    Node *inPred(Node *ptr)
    {
        if (ptr->lthread)
        {
            return ptr->left;
        }
        ptr = ptr->left;
        while (!ptr->rthread)
        {
            ptr = ptr->right;
        }
        return ptr;
    }

    Node *caseA(Node *par, Node *ptr)
    {
        if (!par)
        {
            root = nullptr;
        }
        else if (par->left == ptr)
        {
            par->lthread = true;
            par->left = ptr->left;
        }
        else
        {
            par->rthread = true;
            par->right = ptr->right;
        }
        delete ptr;
        return root;
    }

    Node *caseB(Node *par, Node *ptr)
    {
        Node *child = ptr->lthread ? ptr->right : ptr->left;

        if (!par)
        {
            root = child;
        }
        else if (par->left == ptr)
        {
            par->left = child;
        }
        else
        {
            par->right = child;
        }

        Node *s = inSucc(ptr);
        Node *p = inPred(ptr);

        if (!ptr->lthread)
        {
            p->right = s;
        }
        else if (!ptr->rthread)
        {
            s->left = p;
        }

        delete ptr;
        return root;
    }

    Node *caseC(Node *par, Node *ptr)
    {
        Node *succ = ptr->right;
        Node *parsucc = ptr;

        while (!succ->lthread)
        {
            parsucc = succ;
            succ = succ->left;
        }

        ptr->info = succ->info;

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

        Node *ptr = node;
        while (!ptr->lthread)
        {
            ptr = ptr->left;
        }

        while (ptr)
        {
            displayFile(ptr);
            ptr = inSucc(ptr);
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
        Node *tmp = new Node(file);
        Node *ptr = root;
        Node *par = nullptr;

        while (ptr)
        {
            par = ptr;
            if (file.name == ptr->info.name)
            {
                cout << "\nFile \"" << file.name << "\" already exists.\n";
                delete tmp;
                return;
            }
            if (file.name < ptr->info.name)
            {
                if (!ptr->lthread)
                {
                    ptr = ptr->left;
                }
                else
                {
                    break;
                }
            }
            else
            {
                if (!ptr->rthread)
                {
                    ptr = ptr->right;
                }
                else
                {
                    break;
                }
            }
        }
        if (!par)
        {
            root = tmp;
        }
        else if (file.name < par->info.name)
        {
            tmp->left = par->left;
            tmp->right = par;
            par->lthread = false;
            par->left = tmp;
        }
        else
        {
            tmp->left = par;
            tmp->right = par->right;
            par->rthread = false;
            par->right = tmp;
        }
        cout << "\nFile \"" << file.name << "\" inserted successfully.\n";
    }

    void remove(const string &fname)
    {
        Node *ptr = root;
        Node *par = nullptr;

        while (ptr)
        {
            if (fname == ptr->info.name)
            {
                break;
            }
            par = ptr;
            if (fname < ptr->info.name)
            {
                if (!ptr->lthread)
                {
                    ptr = ptr->left;
                }
                else
                {
                    break;
                }
            }
            else
            {
                if (!ptr->rthread)
                {
                    ptr = ptr->right;
                }
                else
                {
                    break;
                }
            }
        }

        if (!ptr || ptr->info.name != fname)
        {
            cout << "\nFile not found: \"" << fname << "\".\n";
            return;
        }

        if (!ptr->lthread && !ptr->rthread)
        {
            root = caseC(par, ptr);
        }
        else if (!ptr->lthread || !ptr->rthread)
        {
            root = caseB(par, ptr);
        }
        else
        {
            root = caseA(par, ptr);
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
    int choice;

    while (true)
    {
        cout << "\nFile Management System\n";
        cout << "1. Add File\n";
        cout << "2. Delete File\n";
        cout << "3. Display All Files (Inorder)\n";
        cout << "4. Quit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            File file;
            cout << "Enter file name: ";
            cin >> file.name;
            cout << "Enter file size (in KB): ";
            cin >> file.size;
            cout << "Enter file type: ";
            cin >> file.type;
            tree.insert(file);
            break;
        }
        case 2:
        {
            string fname;
            cout << "Enter the file name to delete: ";
            cin >> fname;
            tree.remove(fname);
            break;
        }
        case 3:
        {
            tree.displayInorder();
            break;
        }
        case 4:
        {
            cout << "\nExiting...\n";
            return 0;
        }
        default:
        {
            cout << "\nInvalid choice. Try again.\n";
        }
        }
    }
}
