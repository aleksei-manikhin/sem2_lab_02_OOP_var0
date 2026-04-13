#ifndef LIST_H
#define LIST_H

#include <initializer_list>

template<typename T>
class list
{
public:
    list();
    list(const list<T>& lst);
    list(list<T>&& lst) noexcept;
    explicit list(std::initializer_list<T> lst);
    ~list();

    list<T>& operator=(const list<T>& lst);

    int get_length() const;
    void add(const T& elem);
    void add_range(const list<T>& lst);
    void add_range(T* arr, int size);
    void set_elem(int index, const T& elem);
    T& get_elem(int index);
    void remove_elem(int index);
    list<T> combine(const list<T>& lst);
    void sort(int (*comp)(const T& r1, const T& r2));
    int get_index(T& elem) const;
    T* to_array();
    T& operator[](int index);

private:
    struct Node
    {
        T value;
        Node* prev;
        Node* next;

        explicit Node(const T& value);
    };

    Node* m_head;
    Node* m_tail;
    int m_length;

    Node* get_node(int index) const;
    void clear();
    void copy_from(const list<T>& lst);
    void swap(list<T>& other) noexcept;
};


#endif // LIST_H
