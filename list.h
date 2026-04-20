#ifndef LIST_H
#define LIST_H

#include <initializer_list>
#include <utility>
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
    int get_length() const; //const - нельзя менять поля объекта
    void add(const T& elem); //const - нельзя менять elem
    void add_range(const List<T>& lst);
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
};

template<typename T>
List<T>::List() : pHead(nullptr), pTail(nullptr), length(0)
{
}

template<typename T>
List<T>::List(const List<T>& lst) : pHead(nullptr), pTail(nullptr), length(0)
{
    Node* pOrigNode = lst.pHead;
    while (pOrigNode != nullptr) {
        Node* pNewNode = new Node{pOrigNode->value, pTail, nullptr};
        if (pTail == nullptr)
            pHead = pNewNode;
        else
            pTail->next = pNewNode;
        pTail = pNewNode;
        ++length;
        pOrigNode = pOrigNode->next;
    }
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
int List<T>::get_length() const
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
void List<T>::add_range(const List<T>& lst)
{
    int sourceLength = lst.length;
    int copiedCount = 0;
    Node* pOrigNode = lst.pHead;
    while (copiedCount < sourceLength) {
        add(pOrigNode->value);
        pOrigNode = pOrigNode->next;
        ++copiedCount;
    }
}

template<typename T>
List<T> List<T>::combine(const List<T>& lst)
{
    List<T> result(*this);
    result.add_range(lst);
    return result;
}

template<typename T>
List<T>& List<T>::operator=(const List<T>& lst)
{
    if (this != &lst) {
        List<T> temp(lst); //когда temp выйдет из функции, его деструктор удалит старое содержимое
        swap(temp); //Если конструктор копирования не смог создать temp, текущий объект не измениться.
    }
    return *this;
}

template<typename T>
List<T>& List<T>::operator=(List<T>&& lst) noexcept
{
    if (this != &lst) {
        List<T> temp(std::move(lst));
        swap(temp);
    }
    return *this;
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
