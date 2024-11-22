//
// Created by Faysal on 11/19/2024.
//

#ifndef ADS_MIDTERM_PROJECT_TREENODE_H
#define ADS_MIDTERM_PROJECT_TREENODE_H

#include <iostream>
#include <string>
#include "Account.h"
#include "Transaction.h"

using namespace std;

typedef Account *AccountPtr;
typedef class TreeNode *NodePtr;

//didnt use elemtntypr account cause its confusing for no reason
class TreeNode {
private:
    AccountPtr account;
    NodePtr leftChild;
    NodePtr rightSibling;

public:
    //constructors
    TreeNode(); //aadeye
    TreeNode(const Account &acc); //with acc
    TreeNode(const TreeNode &other); // copy

    ~TreeNode();

    //all the gets
    Account getData() const;

    NodePtr getLeftChild() const;

    NodePtr getRightSibling() const;


    //setters
    void setData(const Account &acc);

    void setLeftChild(NodePtr left);

    void setRightSibling(NodePtr right);

    //node stuffies
    bool isLeaf() const;

    bool hasSibling() const;

    //account hierarchy

    bool isValidChild(int parentNum, int childNum) const;

    void addChild(const Account &);

    void addSibling(const Account &);

    bool addAccountNode(NodePtr root, const Account &newAcc);

    void updateBalance(const Transaction &t);

    NodePtr findNode(NodePtr root, int accNum);

    int getLevel(NodePtr) const;

    TreeNode &operator=(const TreeNode &);

    void print() const;

private:
    void copyForm(const TreeNode &other);

    void clean();

    int getLevelHelper(NodePtr node, int currentLevel) const;

//    NodePtr findLastChild() const;
//    NodePtr findLastSibling() const;
//    bool hasValidPrefix(const string& parent, const string& child) const;

};

#endif //ADS_MIDTERM_PROJECT_TREENODE_H
