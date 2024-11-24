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
/**
 * @class TreeNode
 * @brief Represents a node in a tree structure, each containing an `Account` and pointers to its left child and right sibling.
 *
 * This class supports operations like adding new accounts, updating balances, navigating the hierarchy of accounts,
 * and performing various tree operations such as finding nodes and checking node characteristics.
 */
//didnt use elemtntypr account cause its confusing for no reason
class TreeNode {
private:
    AccountPtr account;
    NodePtr leftChild;
    NodePtr rightSibling;

public:
    //constructors
    // Constructors and Destructor
    /**
     * @brief Default constructor for the `TreeNode` class.
     * Initializes the node with default values: a null account pointer, and null pointers for left child and right sibling.
     */
    TreeNode(); //aadeye
    /**
     * @brief Parameterized constructor for the `TreeNode` class.
     * Initializes the node with the provided `Account` object.
     *
     * @param acc The `Account` object to be stored in the node
     */
    TreeNode(const Account &acc); //with acc
    /**
     * @brief Copy constructor for the `TreeNode` class.
     * Initializes the node by copying data from another node.
     *
     * @param other The `TreeNode` to copy
     */
    TreeNode(const TreeNode &other); // copy
    /**
         * @brief Destructor for the `TreeNode` class.
         * Cleans up any allocated resources associated with the node.
         */
    ~TreeNode();

    //all the gets

    /**
         * @brief Gets the left child of the node.
         *
         * @return A pointer to the left child node
         */
    NodePtr getLeftChild() const;
    /**
         * @brief Gets the right sibling of the node.
         *
         * @return A pointer to the right sibling node
         */
    NodePtr getRightSibling() const;
    /**
         * @brief Gets the account data stored in the node.
         *
         * @return A reference to the `Account` object stored in this node
         */
    Account &getData() { return *account; }
    /**
     * @brief Gets the account data stored in the node (const version).
     *
     * @return A const reference to the `Account` object stored in this node
     */
    const Account &getData() const { return *account; }


    //setters
    /**
     * @brief Sets the account data for the node.
     *
     * @param acc The new `Account` to store in the node
     */
    void setData(const Account &acc);
    /**
      * @brief Sets the left child of the node.
      *
      * @param left The new left child node pointer
      */
    void setLeftChild(NodePtr left);
    /**
      * @brief Sets the right sibling of the node.
      *
      * @param right The new right sibling node pointer
      */
    void setRightSibling(NodePtr right);
     /**
      * @brief Checks if the node is a leaf (has no children).
      *
      * @return True if the node is a leaf, false otherwise
      */
    //node stuffies
    bool isLeaf() const;
    /**
     * @brief Checks if the node has a sibling.
     *
     * @return True if the node has a right sibling, false otherwise
     */
    bool hasSibling() const;
    /**
      * @brief Checks if the given account numbers represent a valid child-parent relationship.
      *
      * @param parentNum The parent account number
      * @param childNum The child account number
      * @return True if the relationship is valid, false otherwise
      */
    //account hierarchy

    bool isValidChild(int parentNum, int childNum) const;
    /**
      * @brief Adds a new account node to the tree.
      *
      * @param root The root node of the tree to start the search for the correct position
      * @param newAcc The new `Account` to add to the tree
      * @return True if the account was successfully added, false otherwise
      */
    bool addAccountNode(NodePtr root, const Account &newAcc);
    /**
      * @brief Updates the balance of accounts in the tree based on a transaction.
      *
      * @param root The root node of the tree to start the update from
      * @param t The `Transaction` object containing the update details
      */
    void updateBalance(NodePtr root, Transaction &t);
    /**
        * @brief Retrieves all the parent nodes of the given node.
        *
        * @param root The root node of the tree
        * @return A vector of pointers to parent nodes
        */
    vector<NodePtr> getParentNodes(NodePtr root);
    /**
         * @brief Finds the node with the specified account number in the tree.
         *
         * @param root The root node to start the search from
         * @param accNum The account number to search for
         * @return A pointer to the node containing the specified account, or nullptr if not found
         */
    NodePtr findNode(NodePtr root, int accNum);
    /**
         * @brief Gets the level of the node in the tree hierarchy.
         *
         * @param node The node to get the level of
         * @return The level of the node in the tree (root has level 0)
         */
    int getLevel(NodePtr) const;
    /**
         * @brief Assignment operator for the `TreeNode` class.
         * Copies the data from another `TreeNode` into this one.
         *
         * @param other The `TreeNode` to copy
         * @return A reference to this node after assignment
         */
    TreeNode &operator=(const TreeNode &);
    /**
         * @brief Prints the account data stored in the node.
         *
         * Outputs the account data to the console.
         */
    void print() const;

private:
    /**
    * @brief Copies the data from another `TreeNode`.
    *
    * This is a helper method used by the copy constructor and assignment operator.
    *
    * @param other The `TreeNode` to copy data from
    */
    void copyForm(const TreeNode &other);
    /**
         * @brief Cleans up resources used by the node.
         *
         * This is a helper method used by the destructor.
         */
    void clean();
    /**
        * @brief Helper method to recursively get the level of a node.
        *
        * @param node The node to check
        * @param currentLevel The current level during recursion
        * @return The level of the node
        */
    int getLevelHelper(NodePtr node, int currentLevel) const;
    /**
         * @brief Adds a new child to the node.
         *
         * @param acc The account data for the new child node
         */
    void addChild(const Account &);
    /**
        * @brief Adds a new sibling to the node.
        *
        * @param acc The account data for the new sibling node
        */
    void addSibling(const Account &);
    /**
        * @brief Updates the balances of the parent nodes based on a transaction.
        *
        * @param parents A list of parent nodes to update
        * @param t The `Transaction` object containing the update details
        */
    void updateParentBalances(const vector<NodePtr> &parents, const Transaction &t);

//    NodePtr findLastChild() const;
//    NodePtr findLastSibling() const;
//    bool hasValidPrefix(const string& parent, const string& child) const;

};

#endif //ADS_MIDTERM_PROJECT_TREENODE_H
