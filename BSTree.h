/** BSTree class header file
 */

#ifndef BSTREE_H
#define BSTREE_H
#include "Node.h"

template<typename T>
class BSTree
{
private:
    Node<T>* root;
public:
    // Constructors
    BSTree() {root = nullptr;}
    BSTree(Node<T>* r) { root = r;}

    // Destructor / helper
    ~BSTree();
    void deleteTree(Node<T>* n);

    // Accessor
    Node<T>* getRoot() const {return root;}

    // Mutator
    void setRoot(Node<T>* r) {root = r;}
    Node<T>* getParent(Node<T> *parent, Node<T> *child);

    // Functions
    Node<T>* insert(T value, Node<T>* curr);
    Node<T>* search(T value, Node<T>* curr);
    Node<T>* remove(T value, BSTree<T>* tree);
    Node<T>* removeNode(BSTree<T>* tree, Node<T>* parent, Node<T>* node);
};

template<typename T>
BSTree<T>::~BSTree()
{
    deleteTree(root);
}

template<typename T>
void BSTree<T>::deleteTree(Node<T>* n)
{
    if (n == nullptr)
    {
        return;
    }

    // Deleting left subtree
    deleteTree(n->getLeft());
    // Deleting right subtree
    deleteTree(n->getRight());
    // Deleting root at the end
    delete n;
}

template<typename T>
Node<T>* BSTree<T>::getParent(Node<T>* parent, Node<T>* child)
{
    // If the parent is null, then there is no tree to search for the parent (because the root was null)
    if (parent == nullptr)
    {
        return nullptr;
    }
    // If the parent's left node isn't null and its value is equal to the child's value
    if (parent->getLeft() != nullptr && parent->getLeft()->getPayload() == child->getPayload())
    {
        // Parent has been found, so return the parent
        return parent;
    }
    // If the parent's right node isn't null and its value is equal to the child's value
    if (parent->getRight() != nullptr && parent->getRight()->getPayload() == child->getPayload())
    {
        // Parent has been found, so return the parent
        return parent;
    }
    // If the child's value is less than the parent's value
    if (child->getPayload() < parent->getPayload())
    {
        // Call get parent to find the parent starting with the parent's left child
        return getParent(parent->getLeft(), child);
    }
    // If the child's value is greater than the parent's value
    else
    {
        // Call get parent to find the parent starting with the parent's right child
        return getParent(parent->getRight(), child);
    }
}

template<typename T>
Node<T>* BSTree<T>::insert(T value, Node<T>* curr)
{
    // The current node is null, then recursion is done and node is inserted
    //      The recursion then finishes by resetting the parent links in the tree to
    //      their same links except for the parent of the new inserted node
    if (curr == nullptr)
    {
        curr = new Node<T>(value);
    }
    else
    {
        // If the value to be inserted is greater than or equal to the current node's value
        if (value < curr->getPayload() || value == curr->getPayload())
        {
            // Set the current node's left to the node returned by the insert function
            //  when called with the current node's left
            // Recursion is being used to reset a parent link to accommodate the
            //  inserted node
            curr->setLeft(insert(value, curr->getLeft()));
        }
        else // if value > curr's payload
        {
            // Same process as described above except setting current node's right to the
            //  node returned by the insert function when called with the current node's right
            curr->setRight(insert(value, curr->getRight()));
        }
    }

    // Returns the current node being checked
    return curr;
}

template<typename T>
Node<T>* BSTree<T>::search(T value, Node<T>* curr)
{
    // If the current node is null then there is nowhere left to search
    if (curr == nullptr)
    {
        return nullptr;
    }

    // If the current node's value is equal to the value to find, then search is done
    if (value == curr->getPayload())
    {
        // Return found node
        return curr;
    }
    // If the value to find is less than the the current node's value
    else if (value < curr->getPayload())
    {
        // Continue the search but to the left of the binary tree
        return search(value, curr->getLeft());
    }
    // If the value to find is greater than the the current node's value
    else
    {
        // Continue the search but to the right of the binary tree
        return search(value, curr->getRight());
    }
}

template<typename T>
Node<T>* BSTree<T>::remove(T value, BSTree<T>* tree)
{
    // Finding the node to be removed
    Node<T>* find = search(value, tree->getRoot());
    // Finding the parent of the node to be removed
    Node<T>* parent = getParent(tree->getRoot(), find);
    // Removing the node from the tree
    Node<T>* toDel = removeNode(tree, parent, find);
    // Return the node that was removed
    return toDel;
}

template<typename T>
Node<T>* BSTree<T>::removeNode(BSTree<T>* tree, Node<T>* parent, Node<T>* node)
{
    // If the node was never found, then return nullptr
    if (node == nullptr)
    {
        return nullptr;
    }

    // If the node to be removed has two children
    if (node->getLeft() != nullptr && node->getRight() != nullptr)
    {
        // Locate the successor and the successor's parent
        //      The successor is the left most node in the node's right subtree
        Node<T>* successor = node->getRight(); // Searching right subtree
        Node<T>* trackParent = node;

        // While the successor still has a left child, set successor equal to its left child
        //      while also tracking the successor's parent node
        while (successor->getLeft() != nullptr)
        {
            trackParent = successor;
            successor = successor->getLeft();
        }

        // Making a copy of the payload
        T copy = successor->getPayload();

        // Remove the successor node from the tree
        removeNode(tree, trackParent, successor);
        // Setting the node's value to the copy successor's payload
        node->setPayload(copy);

        // Return the successor mode to be deleted
        return successor;
    }

    // If the node to be removed is the root (and has either one child or none)
    else if(node == tree->getRoot())
    {
        // If the root's left child is not null, then set the root equal to its left child
        //      (making top node of the left subtree the new root)
        if (node->getLeft() != nullptr)
        {
            tree->setRoot(node->getLeft());
        }
        // Otherwise, set the root equal to its right child
        else
        {
            tree->setRoot(node->getRight());
        }
    }

    // If the node to be removed is an internal node and only has a left child
    else if (node->getLeft() != nullptr)
    {
        // If the parent's left node is the node to be removed
        if (parent->getLeft()->getPayload() == node->getPayload())
        {
            // Resetting parent's left link to node's left child
            parent->setLeft(node->getLeft());
        }
        // If the parent's right node is the node to be removed
        else
        {
            // Resetting parent's right link to node's left child
            parent->setRight(node->getLeft());
        }
    }

    // If the node to be removed is an internal node with only a right child or is a leaf
    else
    {
        // If the parent's left node is the node to be removed
        if (parent->getLeft()->getPayload() == node->getPayload())
        {
            // Resetting parent's left link to node's right child
            parent->setLeft(node->getRight());
        }
        // If the parent's right node is the node to be removed
        else
        {
            // Resetting parent's right link to node's right child
            parent->setRight(node->getRight());
        }
    }

    // Return the node that was removed to be deleted
    return node;
}


#endif // BSTREE_H
