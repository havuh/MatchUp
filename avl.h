#ifndef AVL_H
#define AVL_H

#include <functional>
#include <vector>
#include <iostream>
#define max(a, b) (((a) > (b))? (a) : (b))

using std::function;
using std::vector;
using std::string;

template <typename T, typename R = T>
class Avl {
    struct Node {
        T       element;
        Node*   left;
        Node*   right;
        int     height;

        Node(T element) : element(element), left(nullptr), right(nullptr), height(0) {}
    };

    typedef function<R(T)>      lbdKey;
    typedef function<R(T, int)> Key;
    typedef function<void(T)>   lbdProc;

    Node*           root;
    int             len;
    lbdKey          key;
    vector<T>       people;
    T               dummynullx;

public:
    Avl(lbdKey key = [](T a) {return a; }) : key(key), root(nullptr), len(0) {}
    ~Avl() { destroy(root); }

    int             height()    { return height(root); }
    int             size()      { return len; }
    void            clear()     { destroy(root); len = 0; }
    vector<T>       getPeople() { return people; }

    void    add(T elem) { add(root, elem); }
    void    inOrder(lbdProc proc) { inOrder(root, proc); }
    void    inOrderPersonalized(int tmn) { inOrderPersonalized(root, tmn); }
    void    preOrderPersonalized(R phase, int tmn) { preOrderPersonalized(find(root, phase), phase, tmn); }
    void    preOrderPersonalized(string phase, Key key) { preOrderPersonalized(root, phase, key); }

    void    clearList() { people.clear(); }

    T       find(R attr){
        Node*& node = find(root, attr);
        return node == nullptr ? dummynullx : node->element;
    }
    T&      findOriginal(R attr) {
        Node*& node = find(root, attr);
        return node == nullptr ? dummynullx : node->element;
    }

private:
    void destroy(Node*& node) {
        if (node != nullptr) {
            destroy(node->left);
            destroy(node->right);
            delete node;
        }
    }

    int height(Node* node) { return node == nullptr ? -1 : node->height; }
    void add(Node*& node, T elem) {
        if (node == nullptr) {
            node = new Node(elem);
            ++len;
        }
        else {
            if (key(elem) < key(node->element)) {
                add(node->left, elem);
            }
            else {
                add(node->right, elem);
            }
            balance(node);
        }
    }

    void inOrder(Node* node, lbdProc proc) {
        if (node != nullptr) {
            inOrder(node->left, proc);
            proc(node->element);
            inOrder(node->right, proc);
        }
    }

    void inOrderPersonalized(Node*& node, int tmn) {
        if (node != nullptr) {
            if (people.size() <= tmn) {
                inOrderPersonalized(node->left, tmn);
                people.push_back(node->element);
                inOrderPersonalized(node->right, tmn);
            }
        }
    }

    void preOrderPersonalized(Node*& node, R phase, int tmn) {
        if (node != nullptr) {
            if (key(node->element) == phase && people.size() <= tmn) {
                people.push_back(node->element);
                preOrderPersonalized(node->left, phase, tmn);
                preOrderPersonalized(node->right, phase, tmn);
            }
        }
    }
    void preOrderPersonalized(Node*& node, string phase, Key key) {
            if (node != nullptr) {
                if (key(node->element, phase.size()) == phase) {
                    people.push_back(node->element);
                }
                preOrderPersonalized(node->left, phase, key);
                preOrderPersonalized(node->right, phase, key);
            }
        }

    Node*& find(Node*& node, R attr) {
        if (node == nullptr) {
            return dummynull;
        }
        else if (key(node->element) == attr) {
            return node;
        }
        else if (attr <= key(node->element)) {
            return find(node->left, attr);
        }
        else {
            return find(node->right, attr);
        }
    }

    void updateHeight(Node* node) {
        if (node != nullptr) {
            node->height = max(height(node->left), height(node->right)) + 1;
        }
    }
    void rotateLeft(Node*& node) {
        Node* aux = node->right;
        node->right = aux->left;
        updateHeight(node);
        aux->left = node;
        updateHeight(aux);
        node = aux;
    }
    void rotateRight(Node*& node) {
        Node* aux = node->left;
        node->left = aux->right;
        updateHeight(node);
        aux->right = node;
        updateHeight(aux);
        node = aux;
    }
    void balance(Node*& node) {
        int hl = height(node->left);
        int hr = height(node->right);

        if (hr - hl < -1) {
            hl = height(node->left->left);
            hr = height(node->left->right);
            if (hr > hl) {
                rotateLeft(node->left);
            }
            rotateRight(node);
        }
        else if (hr - hl > 1) {
            hl = height(node->right->left);
            hr = height(node->right->right);
            if (hl > hr) {
                rotateRight(node->right);
            }
            rotateLeft(node);
        }
        else {
            updateHeight(node);
        }
    }

    Node* dummynull = nullptr;

};

#endif // AVL_H
