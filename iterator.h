#ifndef ITERATOR_H
#define ITERATOR_H

#include "list.h"

template<typename T>
class Iterator
{
public:
    Iterator(List<T>& containerObj);

    Iterator<T>& next();
    T value();
    bool is_end();
    Iterator<T>& operator++();
    T& operator*();
    bool operator==(Iterator<T>& b);
    bool operator!=(Iterator<T>& b);

private:
    List<T>* pContainer;
    typename List<T>::Node* pCurrent;
};

template<typename T>
Iterator<T>::Iterator(List<T>& containerObj)
    : pContainer(&containerObj), pCurrent(containerObj.pHead)
{
}

template<typename T>
Iterator<T>& Iterator<T>::next()
{
    if (is_end()) {
        throw ListIndexOutOfRange("iterator is at end");
    }

    pCurrent = pCurrent->next;
    return *this;
}

template<typename T>
T Iterator<T>::value()
{
    if (is_end()) {
        throw ListIndexOutOfRange("iterator is at end");
    }

    return pCurrent->value;
}

template<typename T>
bool Iterator<T>::is_end()
{
    return pCurrent == nullptr;
}

template<typename T>
Iterator<T>& Iterator<T>::operator++()
{
    return next();
}

template<typename T>
T& Iterator<T>::operator*()
{
    if (is_end()) {
        throw ListIndexOutOfRange("iterator is at end");
    }

    return pCurrent->value;
}

template<typename T>
bool Iterator<T>::operator==(Iterator<T>& b)
{
    return pContainer == b.pContainer && pCurrent == b.pCurrent;
}

template<typename T>
bool Iterator<T>::operator!=(Iterator<T>& b)
{
    return !(*this == b);
}

#endif // ITERATOR_H
