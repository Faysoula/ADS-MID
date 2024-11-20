//
// Created by Faysal on 11/19/2024.
//

#ifndef ADS_MIDTERM_PROJECT_TREENODE_H
#define ADS_MIDTERM_PROJECT_TREENODE_H

#include <iostream>
#include <string>
#include "Account.h"
using namespace std;

typedef Account* AccountPtr;
typedef class TreeNode* NodePtr;
//didnt use elemtntypr account cause its confusing for no reason
class TreeNode {
private:
    AccountPtr account;
    NodePtr leftChild;
    NodePtr rightSibling;

public:
    //constructors
    TreeNode(); //aadeye
    TreeNode(const Account& acc); //with acc
    TreeNode(const TreeNode& other); // copy

    ~TreeNode();

    //all the gets
    Account getData() const;
    NodePtr getLeftChild() const;
    NodePtr getRightSibling() const;

    //setters
    void setData(const Account& acc);
    void setLeftChild(NodePtr left);
    void setRightSibling(NodePtr right);

    //node stuffies
    bool isLeaf() const;
    bool hasSibling() const;

    TreeNode& operator=(const TreeNode&);

    void addchild(const Account&);

    void addSibling(const Account&);

    void updateBalance(double);//propagates up

    bool isParent(int accNum) const;

    int getLevel(NodePtr) const;

    void print() const;

private:
    void copyForm(const TreeNode& other);
    void clean();
    bool checkParent(int parentNum, int childNum) const;

};

#endif //ADS_MIDTERM_PROJECT_TREENODE_H
