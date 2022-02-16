/** Node class header file
 */

#ifndef NODE_H
#define NODE_H

template<typename T>
class Node
{
private:
    Node<T> *left;
    Node<T> *right;
    T payload;

public:
    // Constructors
    Node();
    Node(T value);

    // Accessors
    Node<T>* getLeft() const {return left;}
    Node<T>* getRight() const {return right;}
    T getPayload() const {return payload;}

    // Mutators
    void setLeft(Node<T>* l) {left = l;}
    void setRight(Node<T>* r) {right = r;}
    void setPayload(T p) {payload = p;}
};

template<typename T>
Node<T>::Node()
{
    left = nullptr;
    right = nullptr;
}

template<typename T>
Node<T>::Node(T value)
{
    left = nullptr;
    right = nullptr;
    payload = value;
}

#endif //NODE_H
