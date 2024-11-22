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


    NodePtr getLeftChild() const;

    NodePtr getRightSibling() const;

    Account &getData() { return *account; }

    const Account &getData() const { return *account; }


    //setters
    void setData(const Account &acc);

    void setLeftChild(NodePtr left);

    void setRightSibling(NodePtr right);

    //node stuffies
    bool isLeaf() const;

    bool hasSibling() const;

    //account hierarchy

    bool isValidChild(int parentNum, int childNum) const;

    bool addAccountNode(NodePtr root, const Account &newAcc);

    void updateBalance(NodePtr root, Transaction &t);

    vector<NodePtr> getParentNodes(NodePtr root);

    NodePtr findNode(NodePtr root, int accNum);

    int getLevel(NodePtr) const;

    TreeNode &operator=(const TreeNode &);

    void print() const;

private:
    void copyForm(const TreeNode &other);

    void clean();

    int getLevelHelper(NodePtr node, int currentLevel) const;

    void addChild(const Account &);

    void addSibling(const Account &);

    void updateParentBalances(const vector<NodePtr> &parents, const Transaction &t);

//    NodePtr findLastChild() const;
//    NodePtr findLastSibling() const;
//    bool hasValidPrefix(const string& parent, const string& child) const;

};

#endif //ADS_MIDTERM_PROJECT_TREENODE_H
