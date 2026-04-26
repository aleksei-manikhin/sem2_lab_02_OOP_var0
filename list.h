#ifndef LIST_H
#define LIST_H

#include <initializer_list>
#include <utility>
#include "list_exceptions.h"

template<typename T>
class List
{
public:
    List();
    List(const List<T>& lst);
    List(List<T>&& lst) noexcept;
    explicit List(std::initializer_list<T> lst);
    List<T>& operator=(const List<T>& lst);
    List<T>& operator=(List<T>&& lst) noexcept;
    int getLength() const;
    void add(const T& elem);
    void addRange(const List<T>& lst);
    void addRange(T* arr, int size);
    void setElem(int index, const T& elem);
    T& getElem(int index);
    void removeElem(int index);
    void sort(int (*comp)(const T& r1, const T& r2));
    int getIndex(const T& elem) const;
    List<T> combine(const List<T>& lst);
    ~List();

private:
    struct Node
    {
        T value;
        Node* prev;
        Node* next;
    };

    Node* pHead;
    Node* pTail;
    int length;

    void swap(List<T>& other) noexcept;
    void appendNodesFrom(const List<T>& source);
    Node* getNodeByIndex(int index);
};

template<typename T>
List<T>::List() : pHead(nullptr), pTail(nullptr), length(0)
{
}

template<typename T>
List<T>::List(const List<T>& lst) : pHead(nullptr), pTail(nullptr), length(0)
{
    appendNodesFrom(lst);
}

template<typename T>
List<T>::List(List<T>&& lst) noexcept : pHead(lst.pHead), pTail(lst.pTail), length(lst.length)
{
    lst.pHead = nullptr;
    lst.pTail = nullptr;
    lst.length = 0;
}

template<typename T>
List<T>::List(std::initializer_list<T> lst) : pHead(nullptr), pTail(nullptr), length(0)
{
    for (const T& elem : lst) {
        add(elem);
    }
}

template<typename T>
int List<T>::getLength() const
{
    int result = length;
    return result;
}

template<typename T>
void List<T>::add(const T& elem)
{
    Node* pNewNode = new Node{elem, pTail, nullptr};
    if (pTail == nullptr)
        pHead = pNewNode;
    else
        pTail->next = pNewNode;
    pTail = pNewNode;
    ++length;
}

template<typename T>
void List<T>::addRange(const List<T>& lst)
{
    if (this == &lst) {
        List<T> temp(lst);
        appendNodesFrom(temp);
    } else {
        appendNodesFrom(lst);
    }
}

template<typename T>
void List<T>::addRange(T* arr, int size)
{
    if (size < 0) {
        throw ListInvalidArgument("size must be non-negative");
    }
    if (arr == nullptr && size > 0) {
        throw ListInvalidArgument("array pointer is null");
    }
    for (int i = 0; i < size; ++i) {
        add(arr[i]);
    }
}

template<typename T>
void List<T>::setElem(int index, const T& elem)
{
    Node* pNode = getNodeByIndex(index);
    pNode->value = elem;
}

template<typename T>
T& List<T>::getElem(int index)
{
    Node* pNode = getNodeByIndex(index);
    return pNode->value;
}

template<typename T>
void List<T>::removeElem(int index)
{
    Node* pNode = getNodeByIndex(index);

    if (pNode->prev != nullptr) {
        pNode->prev->next = pNode->next;
    } else {
        pHead = pNode->next;
    }

    if (pNode->next != nullptr) {
        pNode->next->prev = pNode->prev;
    } else {
        pTail = pNode->prev;
    }

    delete pNode;
    --length;
}

template<typename T>
void List<T>::sort(int (*comp)(const T& r1, const T& r2))
{
    if (comp == nullptr)
        throw ListInvalidArgument("comparator pointer is null");
    bool swapped = true;
    while (swapped) {
        swapped = false;
        for (Node* pNode = pHead; pNode != nullptr && pNode->next != nullptr; pNode = pNode->next) {
            if (comp(pNode->value, pNode->next->value) > 0) {
                T temp = pNode->value;
                pNode->value = pNode->next->value;
                pNode->next->value = temp;
                swapped = true;
            }
        }
    }
}

template<typename T>
int List<T>::getIndex(const T& elem) const
{
    int result = 0;
    for (const Node* pNode = pHead; pNode != nullptr && pNode->value != elem; pNode = pNode->next, ++result);
    return (result < length) ? result : -1;
}

template<typename T>
void List<T>::appendNodesFrom(const List<T>& source)
{
    const Node* pOrigNode = source.pHead;
    while (pOrigNode != nullptr) {
        add(pOrigNode->value);
        pOrigNode = pOrigNode->next;
    }
}

template<typename T>
typename List<T>::Node* List<T>::getNodeByIndex(int index) //typename показывает, что List<T>::Node* это тип
{
    if (index < 0 || index >= length) {
        throw ListIndexOutOfRange("index is out of range");
    }

    Node* pNode = nullptr;
    if (index < length / 2) {
        pNode = pHead;
        for (int i = 0; i < index; ++i) {
            pNode = pNode->next;
        }
    } else {
        pNode = pTail;
        for (int i = length - 1; i > index; --i) {
            pNode = pNode->prev;
        }
    }

    return pNode;
}

template<typename T>
List<T> List<T>::combine(const List<T>& lst)
{
    List<T> result(*this);
    result.addRange(lst);
    return result;
}

template<typename T>
List<T>& List<T>::operator=(const List<T>& lst)
{
    if (this != &lst) {
        List<T> temp(lst);
        swap(temp);
    }
    List<T>& result = *this;
    return result;
}

template<typename T>
List<T>& List<T>::operator=(List<T>&& lst) noexcept
{
    if (this != &lst) {
        List<T> temp(std::move(lst));
        swap(temp);
    }
    List<T>& result = *this;
    return result;
}

template<typename T>
void List<T>::swap(List<T>& other) noexcept
{
    Node* pTempHead = pHead;
    pHead = other.pHead;
    other.pHead = pTempHead;

    Node* pTempTail = pTail;
    pTail = other.pTail;
    other.pTail = pTempTail;

    int tempLength = length;
    length = other.length;
    other.length = tempLength;
}

template<typename T>
List<T>::~List()
{
    Node* pCurrent = pHead;
    while (pCurrent != nullptr) {
        Node* pNext = pCurrent->next;
        delete pCurrent;
        pCurrent = pNext;
    }
    pHead = nullptr;
    pTail = nullptr;
    length = 0;
}

#endif // LIST_H
