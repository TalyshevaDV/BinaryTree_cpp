#include <iostream>
  
using namespace std;

// Интерфейс
class Container
{
public:
    // Виртуальные методы, будут реализованы далее 
    virtual void insert(int value) = 0;
    virtual bool exists(int value) = 0;
    virtual void remove(int value) = 0;
    // Это потом заменим на перегруженный оператор <<
    virtual void print() = 0;

    // Виртуальный деструктор
    virtual ~Container();
};

Container::~Container() { }

// Реализация интерфейса
// Struct to store tree element
typedef struct tree_element {
    int value;          // Element value
    struct tree_element* left;  // Pointer to left subtree
    struct tree_element* right; // Pointer to right subtree
    struct tree_element* parent; //Pointer to parent
} tree_element;


class SimpleTree: public Container
{
public:
    SimpleTree();
    ~SimpleTree();
    void insert(int value);
    bool exists(int value);
    void remove(int value);
    void print();

protected:
    void deletion(tree_element* element);
    tree_element* insert_pre(tree_element* elem, tree_element* root);
    void print_element(tree_element* i);
    tree_element* create(int i);
    tree_element* find(int value);
    tree_element* find1(int value, tree_element* root);
    tree_element* root;
    tree_element* search(tree_element* elem);
    tree_element* search1(tree_element* elem);
};


SimpleTree::SimpleTree()
{
    this->root = NULL;
}

SimpleTree::~SimpleTree()
{    
    deletion(this->root);
}

void SimpleTree::deletion(tree_element* element) 
{
    if (element != NULL){
        deletion(element->left);
        deletion(element->right);
        free(element);
    }
}

tree_element* SimpleTree::create(int value)
{
    
    tree_element* elem = (tree_element*)malloc(sizeof(tree_element));
    elem->value = value;
    elem->right = NULL;
    elem->left = NULL;
    return elem;
}

void SimpleTree::insert(int value)
{
    tree_element* elem = this->create(value);
       
    if(this->root == NULL)
    {
        this->root = elem;
    }
    insert_pre(elem, this->root);
}

tree_element* SimpleTree::insert_pre(tree_element* elem, tree_element* root) {
    tree_element* cur_root = root;
    if (elem < cur_root && cur_root->left == NULL) {
        cur_root->left = elem;
        elem->parent = cur_root;

    } else if (elem > cur_root && cur_root->right == NULL) {
        cur_root->right = elem;
        elem->parent = cur_root;
    } else if (elem < cur_root && cur_root != NULL) {
        insert_pre(elem, cur_root->left);

    } else if (elem > cur_root && cur_root != NULL) {
        insert_pre(elem, cur_root->right);
    }
    return (cur_root);
}
bool SimpleTree::exists(int value)
{
    tree_element* cur_element = this->find(value);
    return find(value) != NULL ;
}

tree_element* SimpleTree::find1(int value, tree_element* root) 
{
    tree_element* cur_element = root;
    if (cur_element->value == value) {
        return cur_element;
    } else if (cur_element == NULL) {
        return NULL;
    
     
    } else if (cur_element->value < value && cur_element->right != NULL) {
        return find1(value, cur_element->right);
    } else if (cur_element->value > value && cur_element->left != NULL) {
        return find1(value, cur_element->left);
    } 
}

tree_element* SimpleTree::find(int value)
{
    tree_element* cur_root = root;
    if (cur_root != NULL && value == cur_root->value) {
        return(cur_root);
    } else {
        return find1(value, cur_root);
    }
     
}

tree_element* SimpleTree::search(tree_element* elem)
{   
    int A[10];
    int i;
    tree_element* next_elem = elem;
    while (next_elem->value > elem->value){
        A[i]=next_elem->value;
        next_elem = search(elem->right);
    }

    for (i=1; i<10; i++) {
        if (A[i]<A[0]) {
            A[i] = A[0];
        }
        next_elem->value = A[i];
    }
    return(next_elem);
}

tree_element* SimpleTree::search1(tree_element* elem)
{   
    int A[10];
    int i;
    tree_element* next_elem = elem;
    while (next_elem->value < elem->value){
        A[i]=next_elem->value;
        next_elem = search1(elem->left);
    }

    for (i=1; i<10; i++) {
        if (A[i]>A[0]) {
            A[i] = A[0];
        }
        next_elem->value = A[i];
    }
    return(next_elem);
}

void SimpleTree::remove(int value)
{
    tree_element* elem = this->find(value);
    printf("%d\n", elem->value);
    printf("%d\n", elem->parent->value);

    if(elem == NULL)
    {
        printf("Element doesn't found: %d\n", elem->value);
        return;
   } else if(elem->left==NULL && elem->right==NULL) {
        if (elem->parent->left== elem){
            free(elem);
            printf("%d\n", elem->parent->left->value);
            elem->parent->left=NULL;
        } else if(elem->parent->right == elem) {
            free(elem);
            printf("элемент здесь\n");
            printf("%d\n", elem->parent->right->value);
            elem->parent->right=NULL;
        }
    } else if (elem->left!=NULL || elem->right!=NULL) {
        if (elem->left!=NULL) {
            if (elem->value < elem->parent->value) {
                elem->parent->left = elem->left->left;
                free(elem);
            } else if (elem->value > elem->parent->value) {
                elem->parent->right = elem->left->left;
                free(elem);
            }
        } else if (elem->right!=NULL) {
            if (elem->value < elem->parent->value){
                elem->parent->left = elem->right->right;
                free(elem);
            } else if (elem->value > elem->parent->value){
                elem->parent->right = elem->right->right;
                free(elem);
            }
        } else if (elem->left!=NULL && elem->right!=NULL) {
            if (elem->left != NULL) {
                if (elem->value < elem->parent->value) {
                    elem->parent->left = search1(elem);
                    free(elem);
                } else if (elem->value > elem->parent->value) {
                    elem->parent->left = search(elem);
                    free(elem);
                } else if (elem->right != NULL) {
                    if (elem->value < elem->parent->value) {
                    elem->parent->left = search1(elem);
                    free(elem);
                } else if (elem->value > elem->parent->value) {
                    elem->parent->left = search(elem);
                    free(elem);
                }
                }
            }
        }
    }
}

void SimpleTree::print_element(tree_element* cur_elem) {
    // printf("2\n");
    printf("%d\n", cur_elem->value);
    if (cur_elem->left != NULL) {
        // printf("2\n");
        print_element(cur_elem->left);

    }
    // printf("%d\n", cur_elem->value);
    if (cur_elem->right != NULL) {
        print_element(cur_elem->right);
    }
    // printf("%d\n", cur_elem->value);
    // printf("2\n");
}
void SimpleTree::print() {
    tree_element* cur_elem = this->root;
    print_element(cur_elem);
    }
// TODO

int main()
{
    Container* c = new SimpleTree();

    for(int i = 1; i < 10; i++)
        {
            // printf("1\n");
            c->insert(i*i);
            // printf("1\n");
        }


    cout << "List after creation:" << endl;
    // printf("6\n");
    c->print();

    if(c->exists(25))
        cout << "Search for value 25: found" << endl;

    if(c->exists(111))
        cout << "Search for value 111: not found" << endl;
    
    c->remove(25);
   
    

    cout << "List after deletion of the element:" << endl;
    c->print();

    delete c;
    return 0;
}