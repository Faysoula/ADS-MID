//
// Created by Faysal on 11/19/2024.
//

#ifndef ADS_MIDTERM_PROJECT_TREENODE_H
#define ADS_MIDTERM_PROJECT_TREENODE_H

#include <iostream>
#include <string>
#include "Account.h"
using namespace std;

class TreeNode {
private:
    Account* account;
    TreeNode* leftChild;
    TreeNode* rightSibling;

public:
    //constructors
    TreeNode(); //aadeye
    TreeNode(const Account& acc); //with acc
    TreeNode(const TreeNode& other); // copy

    ~TreeNode();

    //all the gets
    Account getData() const;
    TreeNode* getLeftChild() const;
    TreeNode* getRightSibling() const;

    //setters
    void setData(const Account& acc);
    void setLeftChild(TreeNode* left);
    void setRightSibling(TreeNode* right);

    //node stuffies
    bool isLeaf() const;
    bool hasSibling() const;

    TreeNode& operator=(const TreeNode&);

    void addchild(TreeNode*);

    void addSibling(TreeNode*);

    void updateBalance(double);//propagates up

    bool isParent(int accNum) const;

    int getLevel(TreeNode*) const;

    void print() const;

private:
    void copyForm(const TreeNode& other);
    void clean();
    bool checkParent(int parentNum, int childNum) const;

};

#endif //ADS_MIDTERM_PROJECT_TREENODE_H
