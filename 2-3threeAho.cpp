#include <iostream>

struct node {
private:
public:
    int k;
    int size;
    int k1;
    int k2;
    node* first;
    node* second;
    node* third;
    node* fourth;
    node* parent;

    bool is_leaf() {
        return (first == nullptr) && (second == nullptr) && (third == nullptr);
    }

    bool is_parent_of_leafs() {
        return first != nullptr && first->is_leaf();
    }

    node(int k) : size(1), k(k), k1(NULL), k2(NULL), first(nullptr), second(nullptr),
        third(nullptr), fourth(nullptr), parent(nullptr) {}
    
    node(int k, int k1, int k2, node* first_, node* second_, node* third_, node* fourth_, node* parent_) :
        size(1), k(k), k1(k1), k2(k2), first(first_), second(second_), third(third_), fourth(fourth_), parent(parent_) {}

    friend node* search(node* p, int k);
    friend node* insert(node* p, int k);
    friend void remove(node* p, int k);
    friend void correctLabels(node* p);
    friend void addSonAho(node* p);
    friend void balanceTree(node* p);
    friend void printTreeVertexKeys(node* p);
    friend void printTree(node* p);
};

void printTreeVertexKeys(node* p) {
    if (p != nullptr) {
        std::cout << '(' << ' ';
        if (p->is_leaf()) {
            std::cout << p->k << ' ';
        }
        else {
            std::cout << p->k1 << ':' << p->k2 << ' ';
        }
        std::cout << ')';
    }
}

void printTree(node* p) {
    if (p != nullptr) {
        printTreeVertexKeys(p);
        if (!p->is_leaf()) {
            std::cout << ' ' << '-' << '>' << ' ';
        }
        printTreeVertexKeys(p->first);
        printTreeVertexKeys(p->second);
        printTreeVertexKeys(p->third);
        std::cout << std::endl;
        printTree(p->first);
        printTree(p->second);
        printTree(p->third);
    }
}

void printSeparator() {
    for (size_t i = 0; i < 20; i++) {
        std::cout << '-';
    }
    std::cout << std::endl;
}

node* search(node* p, int k) {
    if (p->is_leaf()) return p;

    if (p->is_parent_of_leafs()) {
        node* v = new node(-1);
        if (p->first->k == k) {
            return p->first;
        }
        else if (p->second->k == k) {
            return p->second;
        }
        else if (p->third != nullptr && p->third->k == k) {
            return p->third;
        }
        return v;
    }
    else {
        if (k > p->k2) {
            if (p->third != nullptr) {
                search(p->third, k);
            }
            else {
                search(p->second, k);
            }
        }
        else if (k <= p->k2 && k > p->k1) {
            search(p->second, k);
        }
        else if (k <= p->k1) {
            search(p->first, k);
        }
    }
}

int miniCorrectLabels(node* p) {
    if (p->third != nullptr) {
        if (p->is_parent_of_leafs()) {
            return p->third->k;
        }
        else {
            return miniCorrectLabels(p->third);
        }
    }
    else if (p->is_leaf()) {
        return p->k;
    }
    else {
        return p->second->k != NULL ? p->second->k : p->second->k2;
    }
}

void correctLabels(node* p) {
    if (p->parent != nullptr) {
        if (p->parent->first == p) {
            if (p->third != nullptr) {
                int thirdK = miniCorrectLabels(p->third);
                if (p->parent->k1 != thirdK) {
                    p->parent->k1 = thirdK;
                }
            }
            else {
                if (p->parent->k1 != p->k2) {
                    p->parent->k1 = p->k2;
                }
            }

            if (p->parent->second->third != nullptr) {
                int thirdK = p->parent->second->third->k != NULL ? p->parent->second->third->k : p->parent->second->third->k2;
                if (p->parent->k2 != thirdK) {
                    p->parent->k2 = thirdK;
                }
            }

            correctLabels(p->parent);
        }
        else if (p->parent->second == p) {
            if (p->third != nullptr) {
                //int thirdK = p->third->k != NULL ? p->third->k : p->third->k2;
                int thirdK = miniCorrectLabels(p->third);
                if (p->parent->k2 != thirdK) {
                    p->parent->k2 = thirdK;
                    correctLabels(p->parent);
                }
            }
            else {
                if (p->parent->k2 != p->k2) {
                    p->parent->k2 = p->k2;
                    correctLabels(p->parent);
                }
            }
        }
    }
    /*else {
        if (!p->is_parent_of_leafs() || !p->is_leaf()) {
            if (p->first->third != nullptr) {
                p->k1 = p->first->third->k != NULL ? p->first->third->k : p->first->third->k2;
            }
            else if (p->second->third != nullptr) {
                p->k2 = p->second->third->k != NULL ? p->second->third->k : p->second->third->k2;
            }
        }
    }*/
}

void addSonAho(node* v) {
    int rightK1 = v->third->k != NULL ? v->third->k : v->third->k2;
    int rightK2 = v->fourth->k != NULL ? v->fourth->k : v->fourth->k2;
    node* vs = new node(NULL, rightK1, rightK2, v->third, v->fourth, nullptr, nullptr, v);
    v->third->parent = vs;
    v->fourth->parent = vs;

    if (v->parent == nullptr) {
        int leftK1 = v->first->k != NULL ? v->first->k : v->k1;
        int leftK2 = v->second->k != NULL ? v->second->k : v->k2;
        node* left = new node(NULL, leftK1, leftK2, v->first, v->second, nullptr, nullptr, v);
        v->first->parent = left;
        v->second->parent = left;

        v->first = left;
        v->second = vs;
        v->third = nullptr;
        v->fourth = nullptr;
        v->k1 = left->k2;
        v->k2 = vs->k2;
        left->parent = v;
        vs->parent = v;
    }
    else {
        node* parentV = v->parent;

        v->third = nullptr;
        v->fourth = nullptr;
        v->k1 = v->first->k != NULL ? v->first->k : v->first->k2;
        v->k2 = v->second->k != NULL ? v->second->k : v->second->k2;

        if (parentV->first == v) {
            parentV->fourth = parentV->third;
            parentV->third = parentV->second;
            parentV->second = vs;
            parentV->k1 = v->k2;
            parentV->k2 = vs->k2;
        }
        else if (parentV->second == v) {
            parentV->fourth = parentV->third;
            parentV->third = vs;
            parentV->k2 = v->k2;
        }
        else if (parentV->third == v) {
            parentV->fourth = vs;
        }
        vs->parent = parentV;

        if (parentV->fourth != nullptr) {
            addSonAho(parentV);
        }
    }
}

node* insert(node* p, int k) {
    node* v = new node(k);
    
    if (p->is_leaf()) {
        if (p->parent == nullptr) {
            node* newRight = new node(k);
            if (k > p->k) {
                p->k1 = p->k;
                p->k2 = k;
                p->first = v;
                p->second = newRight;
                v->k = p->k1;
            } else {
                p->k2 = p->k;
                p->k1 = k;
                p->second = v;
                p->first = newRight;
                v->k = p->k2;
            }
            v->parent = p;
            newRight->parent = p;
            p->k = NULL;
            return p;
        }
    } else { // node
        if (p->is_parent_of_leafs()) { // vertex with leaf children
            if (p->third == nullptr) {
                if (k > p->k2) {
                    p->third = v;
                }
                else if (k < p->k2 && k > p->k1) {
                    p->third = p->second;
                    p->second = v;
                    p->k2 = v->k != NULL ? v->k : v->k2;
                }
                else if (k < p->k1) {
                    p->third = p->second;
                    p->second = p->first;
                    p->first = v;
                    p->k1 = v->k != NULL ? v->k : v->k2;
                    p->k2 = p->second->k != NULL ? p->second->k : p->second->k2;
                }
                v->parent = p;
            }
            else {
                if (k < p->first->k) {
                    p->fourth = p->third;
                    p->third = p->second;
                    p->second = p->first;
                    p->first = v;
                }
                else if (k < p->second->k) {
                    p->fourth = p->third;
                    p->third = p->second;
                    p->second = v;
                }
                else if (k < p->third->k) {
                    p->fourth = p->third;
                    p->third = v;
                }
                else if (k > p->third->k) {
                    p->fourth = v;
                }
                v->parent = p;
                if (p->fourth != nullptr) {
                    addSonAho(p);
                }
            }
            correctLabels(p);
        } else {
            if (k > p->k2) {
                if (p->third != nullptr) {
                    insert(p->third, k);
                }
                else {
                    insert(p->second, k);
                }
            }
            else if (k < p->k2 && k > p->k1) {
                insert(p->second, k);
            }
            else if (k < p->k1) {
                insert(p->first, k);
            }
        }
    }
}

void balanceTree(node* v) {
    node* parentV = v->parent;

    if (parentV != nullptr) {
        if (parentV->first == v) {
            node* rightBrother = parentV->second;

            v->second = rightBrother->first;
            v->third = rightBrother->second;
            v->fourth = rightBrother->third;

            parentV->second->first->parent = v;
            parentV->second->second->parent = v;
            if (rightBrother->third != nullptr) parentV->second->third->parent = v;

            parentV->second = parentV->third;
            parentV->third = nullptr;

            if (v->fourth != nullptr) {
                addSonAho(v);
            }
            else if (parentV->second == nullptr) {
                balanceTree(parentV);
            }
        }

        else if (parentV->second == v) {
            node* leftBrother = parentV->first;

            if (leftBrother->third != nullptr) {
                leftBrother->fourth = v->first;
            }
            else {
                leftBrother->third = v->first;
            }

            v->first->parent = parentV->first;

            parentV->second = parentV->third;
            parentV->third = nullptr;

            if (leftBrother->fourth != nullptr) {
                addSonAho(leftBrother);
            }
            else if (parentV->second == nullptr) {
                balanceTree(parentV);
            }
        }

        else if (parentV->third == v) {
            node* leftBrother = parentV->second;

            if (leftBrother->third != nullptr) {
                leftBrother->fourth = v->first;
            }
            else {
                leftBrother->third = v->first;
            }

            v->first->parent = parentV->second;

            parentV->third = nullptr;

            if (leftBrother->fourth != nullptr) {
                addSonAho(leftBrother);
            }
        }
    }
    else {
        if (v->first->is_parent_of_leafs() || v->second == nullptr) {
            node* copyV = v->first;

            if (v->second == nullptr && !v->first->is_parent_of_leafs()) {
                node* child = v->first;
                v->k1 = child->first->k2;
                v->k2 = child->second->k2;

                v->first = copyV->first;
                v->second = copyV->second;
                v->third = copyV->third;

                copyV->first->parent = v;
                copyV->second->parent = v;
                if (copyV->third != nullptr) copyV->third->parent = v;
            }
            else {
                v->first = copyV->first;
                v->second = copyV->second;
                v->third = copyV->third;

                copyV->first->parent = v;
                copyV->second->parent = v;
                if (copyV->third != nullptr) copyV->third->parent = v;

                v->k1 = v->first->k;
                v->k2 = v->second->k;
            }
        }
    }
}

void remove(node* p, int k) {
    node* v = search(p, k);
    if (v->k == -1 || p->is_leaf()) {
        std::cout << std::endl;
        std::cout << 'N' << 'F' << std::endl;
        printSeparator();
        return;
    }

    if (p->is_parent_of_leafs()) {
        if (p->third != nullptr) {
            if (p->first == v) {
                p->first = p->second;
                p->second = p->third;
                p->third = nullptr;
                p->k1 = p->first->k;
                p->k2 = p->second->k;
            }
            else if (p->second == v) {
                p->second = p->third;
                p->third = nullptr;
                p->k2 = p->second->k;
            }
            else if (p->third == v) {
                p->third = nullptr;
            }
        }
        else {
            if (p->first == v) {
                p->k = p->second->k;
            }
            else if (p->second == v) {
                p->k = p->first->k;
            }
            p->first = nullptr;
            p->second = nullptr;
            p->k1 = NULL;
            p->k2 = NULL;
        }
    }
    else {
        node* parentV = v->parent;
        node* grandPaV = parentV->parent;

        if (parentV->third != nullptr) {
            if (parentV->first == v) {
                parentV->first = parentV->second;
                parentV->second = parentV->third;
                parentV->third = nullptr;
                parentV->k1 = parentV->first->k;
                parentV->k2 = parentV->second->k;
            }
            else if (parentV->second == v) {
                parentV->second = parentV->third;
                parentV->third = nullptr;
                parentV->k2 = parentV->second->k;
            }
            else if (parentV->third == v) {
                parentV->third = nullptr;
            }
            correctLabels(parentV);
        }
        else {
            if (parentV->first == v) {
                parentV->first = parentV->second;
                parentV->second = nullptr;
            }
            else if (parentV->second == v) {
                parentV->second = nullptr;
            }

            if (grandPaV->first == parentV) {
                node* rightBrother = grandPaV->second;
                parentV->second = rightBrother->first;
                parentV->third = rightBrother->second;
                parentV->fourth = rightBrother->third;

                grandPaV->second->first->parent = parentV;
                grandPaV->second->second->parent = parentV;
                if (rightBrother->third != nullptr) grandPaV->second->third->parent = parentV;
                if (rightBrother->fourth != nullptr) grandPaV->second->fourth->parent = parentV;

                grandPaV->second = grandPaV->third;
                grandPaV->third = nullptr;

                if (parentV->fourth == nullptr && grandPaV->second == nullptr) {
                    balanceTree(parentV->parent);
                }
                else if (parentV->fourth != nullptr) {
                    addSonAho(parentV);
                }

                correctLabels(parentV->parent);
            }
            else if (grandPaV->second == parentV) {
                node* leftBrother = grandPaV->first;
                if (leftBrother->third != nullptr) {
                    leftBrother->fourth = parentV->first;
                }
                else {
                    leftBrother->third = parentV->first;
                }
                parentV->first->parent = leftBrother;
                grandPaV->second = grandPaV->third;
                grandPaV->third = nullptr;

                if (leftBrother->fourth == nullptr && grandPaV->second == nullptr) {
                    balanceTree(leftBrother->parent);
                }
                else if (leftBrother->fourth != nullptr) {
                    addSonAho(grandPaV->first);
                }

                correctLabels(leftBrother->parent);
            }
            else if (grandPaV->third == parentV) {
                node* leftBrother = grandPaV->second;
                if (leftBrother->third != nullptr) {
                    leftBrother->fourth = parentV->first;
                }
                else {
                    leftBrother->third = parentV->first;
                }
                parentV->first->parent = leftBrother;
                grandPaV->third = nullptr;

                if (leftBrother->fourth == nullptr && grandPaV->second == nullptr) {
                    balanceTree(leftBrother->parent);
                }
                else if (leftBrother->fourth != nullptr) {
                    addSonAho(grandPaV->second);
                }

                correctLabels(leftBrother->parent);
            }
        }
    }
}

void insertAndPrint(node* root, int k) {
    std::cout << 'a' << 'd' << 'd' << ' ' << k << std::endl;
    insert(root, k);
    printTree(root);
    printSeparator();
}

void removeAndPrint(node* root, int k) {
    std::cout << 'd' << 'e' << 'l' << ' ' << k << std::endl;
    remove(root, k);
    printTree(root);
    printSeparator();
}

void searchAndPrint(node* root, int k) {
    std::cout << 's' << 'r' << 'c' << 'h' << ' ' << k << std::endl;
    node* sv = search(root, k);

    if (sv->k == -1) {
        std::cout << 'N' << 'F' << std::endl;
    }
    else {
        std::cout << sv->parent->k1 << ':' << sv->parent->k2 << ' ' << sv->k << std::endl;
    }
    printSeparator();
}

int main()
{
    printSeparator();

    node root(1);
    printTree(&root);
    printSeparator();

    insertAndPrint(&root, 3);
    insertAndPrint(&root, 5);
    insertAndPrint(&root, 6);

    insertAndPrint(&root, 7);
    insertAndPrint(&root, 8);

    insertAndPrint(&root, 4);

    removeAndPrint(&root, 8);

    insertAndPrint(&root, 9);
    insertAndPrint(&root, 10);

    insertAndPrint(&root, 12);

    removeAndPrint(&root, 7);
    //insertAndPrint(&root, 13);

    //insertAndPrint(&root, 14);
    //insertAndPrint(&root, 16);

    //insertAndPrint(&root, 18);
    //insertAndPrint(&root, 19);

    //insertAndPrint(&root, 21);
    //insertAndPrint(&root, 23);

    //searchAndPrint(&root, 9);

    //insertAndPrint(&root, 24);


    //removeAndPrint(&root, 9);
    
 
    /*searchAndPrint(&root, 7);*/
    //removeAndPrint(&root, 5);
    //insertAndPrint(&root, 7);
    //insertAndPrint(&root, 10);
    //insertAndPrint(&root, 9);
    //insertAndPrint(&root, 15);
    //insertAndPrint(&root, 12);
    //insertAndPrint(&root, 6);
    //searchAndPrint(&root, 6);
    //insertAndPrint(&root, 16);
    //searchAndPrint(&root, 16);
    //removeAndPrint(&root, 16);
    
    /*removeAndPrint(&root, 6);
    searchAndPrint(&root, 12);
    removeAndPrint(&root, 7);
    searchAndPrint(&root, 3);
    removeAndPrint(&root, 9);
    removeAndPrint(&root, 3);
    searchAndPrint(&root, 3);
    searchAndPrint(&root, 12);
    removeAndPrint(&root, 10);
    removeAndPrint(&root, 12);
    removeAndPrint(&root, 15);*/
}
