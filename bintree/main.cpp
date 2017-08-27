#include <iostream>
#include <stack>

struct BTNodeDouble
{
    BTNodeDouble(double data)
    {
        this->data=data;
        l=r=nullptr;
    }
    void SetL(BTNodeDouble* node)
    {
        l=node;
    }
    void SetR(BTNodeDouble* node)
    {
        r=node;
    }

    double data;
    BTNodeDouble *l,*r;

};


class BinTreeDouble
{
        public:
            BinTreeDouble();
            ~BinTreeDouble();
            BinTreeDouble(double);
            void Add(double);
            void Remove(double);
            BTNodeDouble* Find(double);
            BTNodeDouble* FindParent(BTNodeDouble*);
            void Print();

        private:
        BTNodeDouble* root;

        // Обход слева-направо рекурсивный
        void PrintLRecurs(BTNodeDouble*);
        // Обход слева-направо не рекурсивный
        void PrintLNORecurs();
};
///// MAIN
int main()
{

    /*BinTreeDouble tree(10);
    tree.Add(8);
    tree.Add(9);
    tree.Add(3);
    tree.Add(7);
    tree.Add(6);
    tree.Add(5);
    tree.Add(7.5);

    tree.Remove(3);
    tree.Print();*/

    BinTreeDouble tree;
    std::cout << " -1 to exit\n";
    double inp=0;
    while(true)
    {
        std::cout << "Next double: ";
        std::cin >> inp;
        if(inp==-1) break;
        tree.Add(inp);
    }

    std::cout << "\n";
    tree.Print();

    char c;
    std::cin >> c;
    return 0;
}

BinTreeDouble::BinTreeDouble()
{
    root=nullptr;
}
BinTreeDouble::~BinTreeDouble()
{
    BTNodeDouble* current = root;
    BTNodeDouble* del=nullptr;
    std::stack<BTNodeDouble*> stack;
    //stack.push(root);
    while(!stack.empty() || current!=nullptr)
    {
        if(current!=nullptr)
        {
            stack.push(current);
            current = current->l;
        }
        else
        {
            current = stack.top();
            stack.pop();
            del=current;
            current=current->r;
            delete del;
        }

    }
}

BinTreeDouble::BinTreeDouble(double data)
{
    root = new BTNodeDouble(data);
}

void BinTreeDouble::Add(double data)
{
    if(root==nullptr)
    {
        root = new BTNodeDouble(data);
        return;
    }
    BTNodeDouble* current = root;

    bool past=false;
    while(!past)
    {
        if(data >= current->data)
        {
            if(current->r != nullptr)
                current=current->r;
            else
            {
                current->SetR(new BTNodeDouble(data));
                past=true;
            }
        }

        if( data < current->data)
        {
            if(current->l != nullptr)
                current=current->l;
            else
            {
                current->SetL(new BTNodeDouble(data));
                past=true;
            }
        }
    }

}
void BinTreeDouble::Remove(double data)
{
    BTNodeDouble* delNode = Find(data);
    BTNodeDouble* delNodePar = FindParent(delNode);

    if(delNode->r == nullptr)   // нет правого поддерева
    {
        if(delNodePar==nullptr) // если удаляем root
            root = delNode->l;
        else
            if(delNodePar->l==delNode)
                delNodePar->l=delNode->l;
            else
                delNodePar->r=delNode->l;
    } else
    if(delNode->r->l == nullptr) // у правого поддерева нет левого поддерева
    {

        if(delNodePar==nullptr) // если удаляем root
            root = delNode->r;
        else
            if(delNodePar->l==delNode)
                delNodePar->l=delNode->r;
            else
                delNodePar->r=delNode->r;

    } else
         // у правого поддерева есть левое поддерево
    {
        BTNodeDouble* leftest = delNode->r->l;
        while(leftest->l!=nullptr)
            leftest = leftest->l;
        BTNodeDouble* leftestParent = FindParent(leftest);
        leftestParent->l=nullptr;

        leftest->l=delNode->l;
        leftest->r=delNode->r;

        if(delNodePar==nullptr) // если удаляем root
            root = leftest;
        else
            if(delNodePar->l==delNode)
                delNodePar->l=leftest;
            else
                delNodePar->r=leftest;
    }
}
BTNodeDouble* BinTreeDouble::Find(double data)
{
    if(root == nullptr) return root;
    BTNodeDouble* current = root;
    while(true)
    {
        if(current->data == data) return current;

        if(data > current->data)
        {
            if(current->r != nullptr)
                current=current->r;
            else
                return nullptr;
        }

        if( data < current->data)
        {
            if(current->l != nullptr)
                current=current->l;
            else
                return nullptr;
        }

    }
}

BTNodeDouble* BinTreeDouble::FindParent(BTNodeDouble* node)
{
    if(root == nullptr) return root;
    if(root == node) return nullptr;

    BTNodeDouble* current = root;
    while(true)
    {
        if(current->r == node || current->l == node) return current;

        if(node->data > current->data)
        {
            if(current->r != nullptr)
                current=current->r;
            else
                return nullptr;
        }

        if(node->data < current->data)
        {
            if(current->l != nullptr)
                current=current->l;
            else
                return nullptr;
        }

    }
}


// Обход слева-направо рекурсивный
void BinTreeDouble::PrintLRecurs(BTNodeDouble* node)
{
    if(node->l!=nullptr) PrintLRecurs(node->l);
    std::cout << node->data << "\n";
    if(node->r!=nullptr) PrintLRecurs(node->r);
}

// Обход слева-направо НЕ рекурсивный
void BinTreeDouble::PrintLNORecurs()
{
    BTNodeDouble* current = root;
    std::stack<BTNodeDouble*> stack;
    //stack.push(root);
    while(!stack.empty() || current!=nullptr)
    {
        if(current!=nullptr)
        {
            stack.push(current);
            current = current->l;
        }
        else
        {
            current = stack.top();
            stack.pop();
            std::cout << current->data << "\n";
            current=current->r;
        }

    }
}



void BinTreeDouble::Print()
{
    std::cout << "Recurse:\n";
    PrintLRecurs(root);
    std::cout << "\n";
    std::cout << "Iteration:\n";
    PrintLNORecurs();
}

