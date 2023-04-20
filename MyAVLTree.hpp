#ifndef __PROJ_THREE_AVL_HPP
#define __PROJ_THREE_AVL_HPP

#include "runtimeexcept.hpp"
#include <string>
#include <vector>
#include <iostream>

class ElementNotFoundException : public RuntimeException
{
public:
    ElementNotFoundException(const std::string & err) : RuntimeException(err) {}
};


template<typename Key, typename Value>
class MyAVLTree
{
private:
    struct Node {
    public:
        Key key;
        Value value;
        Node* left;
        Node* right;
        Node* parent;
        int height;
        ~Node(){
            delete left;
            delete right;
        }
    };
    size_t total;
    Node* root;
    void recurse_insert(const Key & k, const Value & v, Node*& parent);
    int updateHeight(Node*& n);
    int getBF(Node*& n);
    void leftRotate(Node*& n);
    void rightRotate(Node*& n);
    void rec_in(Node*& n, std::vector<Key>& v);





public:
    MyAVLTree();

    // In general, a copy constructor and assignment operator
    // are good things to have.
    // For this summer, I am not requiring these.
    //	MyAVLTree(const MyAVLTree & st);
    //	MyAVLTree & operator=(const MyAVLTree & st);


    // The destructor is, however, required.
    ~MyAVLTree();

    // size() returns the number of distinct keys in the tree.
    size_t size() const noexcept;

    // isEmpty() returns true if and only if the tree has no values in it.
    bool isEmpty() const noexcept;

    // contains() returns true if and only if there
    //  is a (key, value) pair in the tree
    //	that has the given key as its key.
    bool contains(const Key & k) const;

    // find returns the value associated with the given key
    // If !contains(k), this will throw an ElementNotFoundException
    // There needs to be a version for const and non-const MyAVLTrees.
    Value & find(const Key & k);
    const Value & find(const Key & k) const;

    // Inserts the given key-value pair into
    // the tree and performs the AVL re-balance
    // operation, as described in lecture.
    // If the key already exists in the tree,
    // you may do as you please (no test cases in
    // the grading script will deal with this situation)
    void insert(const Key & k, const Value & v);

    // in general, a "remove" function would be here
    // It's a little trickier with an AVL tree
    // and I am not requiring it for this quarter's ICS 46.
    // You should still read about the remove operation
    // in your textbook.

    // The following three functions all return
    // the set of keys in the tree as a standard vector.
    // Each returns them in a different order.
    std::vector<Key> inOrder() const;
    std::vector<Key> preOrder() const;
    std::vector<Key> postOrder() const;


};


template<typename Key, typename Value>
MyAVLTree<Key,Value>::MyAVLTree()
{
    root = nullptr;
    this->total = 0;
}

template<typename Key, typename Value>
MyAVLTree<Key,Value>::~MyAVLTree()
{
    delete root;
}

template<typename Key, typename Value>
size_t MyAVLTree<Key, Value>::size() const noexcept
{
    return this->total;
}

template<typename Key, typename Value>
bool MyAVLTree<Key, Value>::isEmpty() const noexcept
{
    if (root == nullptr){
        return true;
    }
    return false;
}


template<typename Key, typename Value>
bool MyAVLTree<Key, Value>::contains(const Key &k) const
{
    Node* temp = root;
    while (temp != nullptr) {
        if (k > temp->key){
            temp = temp->right;
        } else if (k < temp->key) {
            temp = temp->left;
        } else {
            return true;
        }
    }
    return false;
}

template<typename Key, typename Value>
Value & MyAVLTree<Key, Value>::find(const Key & k)
{
    Node* temp = root;
    while (temp != nullptr) {
        if (k > temp->key){
            temp = temp->right;
        } else if (k < temp->key) {
            temp = temp->left;
        } else {
            return temp->value;
        }
    }
    throw ElementNotFoundException("Could not find node.");
}

template<typename Key, typename Value>
const Value & MyAVLTree<Key, Value>::find(const Key & k) const
{
    Node* temp = root;
    while (temp != nullptr) {
        if (k > temp->key){
            temp = temp->right;
        } else if (k < temp->key) {
            temp = temp->left;
        } else {
            return temp->value;
        }
    }
    throw ElementNotFoundException("Could not find node.");
}

template<typename Key, typename Value>
int MyAVLTree<Key, Value>::getBF(Node*& n){
    int leftHeight = -1;
    if (n->left != nullptr) {
        leftHeight = n->left->height;
    }
    int rightHeight = -1;
    if (n->right != nullptr) {
        rightHeight = n->right->height;
    }
    return leftHeight - rightHeight;
}


template<typename Key, typename Value>
int MyAVLTree<Key, Value>::updateHeight(Node*& n){
    int leftHeight = -1;
    if (n->left != nullptr){
        leftHeight = n->left->height;
    }
    int rightHeight = -1;
    if (n->right != nullptr){
        rightHeight = n->right->height;
    }
    n->height = std::max(leftHeight, rightHeight) + 1;

    return n->height;

}


template<typename Key, typename Value>
void MyAVLTree<Key, Value>::leftRotate(Node*& n){
    Node* rchild = n->right;
    Node* rlchild = rchild->left;
    rchild->left = n;
    n->right = rlchild;
    n = rchild;
    updateHeight(rchild);
    updateHeight(n);

}

template<typename Key, typename Value>
void MyAVLTree<Key, Value>::rightRotate(Node*& n){
    Node* lchild = n->left;
    Node* lrchild = lchild->right;
    lchild->right = n;
    n->left = lrchild;
    n = lchild;
    updateHeight(lchild);
    updateHeight(n);

}

template<typename Key, typename Value>
void MyAVLTree<Key, Value>::insert(const Key & k, const Value & v)
{
    if (!this->contains(k)) this->total++;
    if (root == nullptr){
        root = new Node();
        root->key = k;
        root->value = v;
        root->left = nullptr;
        root->right = nullptr;
        root->height = 1;
        return;
    }else {
        recurse_insert(k,v,this->root);
    }

}

template<typename Key, typename Value>
void MyAVLTree<Key, Value>::recurse_insert(const Key & k, const Value & v, Node*& parent){
    // sourced from zybooks 7.10, 8.2(rotations)

    // handles duplicates by replacing the existing value with the new one
    if (parent != nullptr && parent->key == k){

        parent->value = v;
        std::cout << parent->value << std::endl;
        return;
    }

    if(parent == nullptr) {
        parent = new Node();
        parent->key = k;
        parent->value = v;
        parent->left = nullptr;
        parent->right = nullptr;
        parent->height = 0;
//        return;
    }
    else if (k < parent->key){
        recurse_insert(k,v,parent->left);
        updateHeight(parent);
        int bf = getBF(parent);
        if (bf > 1) {
            if (k > parent->left->key) {
                leftRotate(parent->left);
                return rightRotate(parent);
            } else { //if (k > parent->left->key) {
                return rightRotate(parent);
            }
        }
    }
    else {
        recurse_insert(k,v,parent->right);
        updateHeight(parent);
        int bf = getBF(parent);
        if (bf < -1) {
            if (k < parent->right->key){
                rightRotate(parent->right);
                return leftRotate(parent);
            } else{ //if(k < parent->right->key){
                return leftRotate(parent);
            }
        }
    }
    // updateHeight(parent);
    // int bf = getBF(parent);
    // if (bf > 1) {
    //     if (k < parent->left->key) {
    //         return rightRotate(parent);
    //     } else { //if (k > parent->left->key) {
    //         leftRotate(parent->left);
    //         return rightRotate(parent);
    //     }
    // }
    // if (bf < -1) {
    //     if (k > parent->right->key){
    //         return leftRotate(parent);
    //     } else{ //if(k < parent->right->key){
    //         rightRotate(parent->right);
    //         return leftRotate(parent);
    //     }
    // }


}


template<typename Key, typename Value>
std::vector<Key> MyAVLTree<Key, Value>::inOrder() const
{

    std::vector<Key> final;
    /*std::vector<Node*> inOrder;
    std::vector<Key> final;
    Node* curNode = root;

    while(!inOrder.empty() || curNode != nullptr)
    {
        if (curNode != nullptr) {
            inOrder.push_back(curNode);
            curNode = curNode->left;
            std::cout << curNode << std::endl;
        }
        else
        {
            curNode = inOrder.back();
            inOrder.pop_back();
            final.push_back(curNode->key);
            curNode = curNode->right;
        }

    }
    return final;*/
    // return rec_in(this->root,final);
    // return final;

}

template<typename Key, typename Value>
void MyAVLTree<Key, Value>::rec_in(Node*& n, std::vector<Key>& v){
    if(n == nullptr){
        return;
    } else {
        rec_in(n->left, v);
        v.push_back(n->key);
        rec_in(n->right, v);
    }

}

template<typename Key, typename Value>
std::vector<Key> MyAVLTree<Key, Value>::preOrder() const
{
    std::vector<Node*> pre;
    std::vector<Key> final;
    Node* curNode;
    if (root == nullptr){
        throw ElementNotFoundException("Could not find node.");
    }
    pre.push_back(root);
    while (!pre.empty()) {
        curNode = pre.back();
        pre.pop_back();
        final.push_back(curNode->key);
        if (curNode->right) {
            pre.push_back(curNode->right);
        }
        if (curNode->left) {
            pre.push_back(curNode->left);
        }

    }
//    for (Key i: final){
//        std::cout << i << ' ';
//    }
    return final;
}


template<typename Key, typename Value>
std::vector<Key> MyAVLTree<Key, Value>::postOrder() const
{
    std::vector<Node*> post;
    std::vector<Key> final;
    Node* curNode = root;

    if (root == nullptr) {
        throw ElementNotFoundException("Could not find node.");
    }
    post.push_back(root);
    while (!post.empty())
    {
        curNode = post.back();
        post.pop_back();
        final.push_back(curNode->key);
        if (curNode->left) {
            post.push_back(curNode->left);
        }
        if (curNode->right) {
            post.push_back(curNode->right);
        }
    }
    std::reverse(final.begin(), final.end());
    return final;
}







#endif