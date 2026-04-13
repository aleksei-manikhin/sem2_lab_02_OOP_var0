#include <stdexcept>
#include <utility>
#include "list.h"

template<typename T>
list<T>::Node::Node(const T& value) : value(value), prev(nullptr), next(nullptr)
{
}

template<typename T>
list<T>::list() : m_head(nullptr), m_tail(nullptr), m_length(0)
{
}

template<typename T>
list<T>::list(const list<T>& lst) : m_head(nullptr), m_tail(nullptr), m_length(0)
{
    copy_from(lst);
}

template<typename T>
list<T>::list(list<T>&& lst) noexcept : m_head(lst.m_head), m_tail(lst.m_tail), m_length(lst.m_length)
{
    lst.m_head = nullptr;
    lst.m_tail = nullptr;
    lst.m_length = 0;
}

template<typename T>
list<T>::list(std::initializer_list<T> lst) : m_head(nullptr), m_tail(nullptr), m_length(0)
{
    for (const T& elem : lst) {
        add(elem);
    }
}

template<typename T>
list<T>::~list()
{
    clear();
}

template<typename T>
list<T>& list<T>::operator=(const list<T>& lst)
{
    if (this != &lst) {
        list<T> temp(lst);
        swap(temp);
    }
    return *this;
}

template<typename T>
int list<T>::get_length() const
{
    int result = m_length;
    return result;
}

template<typename T>
void list<T>::add(const T& elem)
{
    Node* new_node = new Node(elem);
    if (m_length == 0) {
        m_head = new_node;
        m_tail = new_node;
    } else {
        new_node->prev = m_tail;
        m_tail->next = new_node;
        m_tail = new_node;
    }
    ++m_length;
}

template<typename T>
void list<T>::add_range(const list<T>& lst)
{
    if (this == &lst) {
        list<T> copy(lst);
        add_range(copy);
    } else {
        Node* current = lst.m_head;
        while (current != nullptr) {
            add(current->value);
            current = current->next;
        }
    }
}

template<typename T>
void list<T>::add_range(T* arr, int size)
{
    if (size < 0) {
        throw std::invalid_argument("size must be non-negative");
    }
    if (arr == nullptr && size > 0) {
        throw std::invalid_argument("array pointer is null");
    }
    for (int i = 0; i < size; ++i) {
        add(arr[i]);
    }
}

template<typename T>
void list<T>::set_elem(int index, const T& elem)
{
    Node* node = get_node(index);
    node->value = elem;
}

template<typename T>
T& list<T>::get_elem(int index)
{
    Node* node = get_node(index);
    T& result = node->value;
    return result;
}

template<typename T>
void list<T>::remove_elem(int index)
{
    Node* node = get_node(index);

    if (node->prev != nullptr) {
        node->prev->next = node->next;
    } else {
        m_head = node->next;
    }

    if (node->next != nullptr) {
        node->next->prev = node->prev;
    } else {
        m_tail = node->prev;
    }

    delete node;
    --m_length;
}

template<typename T>
list<T> list<T>::combine(const list<T>& lst)
{
    list<T> result(*this);
    result.add_range(lst);
    return result;
}

template<typename T>
void list<T>::sort(int (*comp)(const T& r1, const T& r2))
{
    if (comp == nullptr) {
        throw std::invalid_argument("comparator pointer is null");
    }

    bool swapped = true;
    while (swapped) {
        swapped = false;
        Node* current = m_head;
        while (current != nullptr && current->next != nullptr) {
            if (comp(current->value, current->next->value) > 0) {
                T temp = current->value;
                current->value = current->next->value;
                current->next->value = temp;
                swapped = true;
            }
            current = current->next;
        }
    }
}

template<typename T>
int list<T>::get_index(T& elem) const
{
    int result = -1;
    int index = 0;
    Node* current = m_head;
    while (current != nullptr && result == -1) {
        if (current->value == elem) {
            result = index;
        }
        current = current->next;
        ++index;
    }
    return result;
}

template<typename T>
T* list<T>::to_array()
{
    T* result = nullptr;
    if (m_length > 0) {
        result = new T[m_length];
        Node* current = m_head;
        int i = 0;
        while (current != nullptr) {
            result[i] = current->value;
            current = current->next;
            ++i;
        }
    }
    return result;
}

template<typename T>
T& list<T>::operator[](int index)
{
    T& result = get_elem(index);
    return result;
}

template<typename T>
typename list<T>::Node* list<T>::get_node(int index) const
{
    if (index < 0 || index >= m_length) {
        throw std::out_of_range("index is out of range");
    }

    Node* current = nullptr;
    if (index < m_length / 2) {
        current = m_head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
    } else {
        current = m_tail;
        for (int i = m_length - 1; i > index; --i) {
            current = current->prev;
        }
    }
    return current;
}

template<typename T>
void list<T>::clear()
{
    Node* current = m_head;
    while (current != nullptr) {
        Node* next_node = current->next;
        delete current;
        current = next_node;
    }
    m_head = nullptr;
    m_tail = nullptr;
    m_length = 0;
}

template<typename T>
void list<T>::copy_from(const list<T>& lst)
{
    Node* current = lst.m_head;
    while (current != nullptr) {
        add(current->value);
        current = current->next;
    }
}

template<typename T>
void list<T>::swap(list<T>& other) noexcept
{
    std::swap(m_head, other.m_head);
    std::swap(m_tail, other.m_tail);
    std::swap(m_length, other.m_length);
}
