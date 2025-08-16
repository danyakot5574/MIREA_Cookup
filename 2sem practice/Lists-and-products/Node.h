#ifndef _NODE_H_
#define _NODE_H_

#include "Product.cpp" // Копируем именно .cpp файл, тк если скопируем сюда заголовочный файл - не получим реализаций методов

#include <memory>

// Класс ноды (элемента связного списка))))))))))))

template <typename T = int>
class Node
{
    T data {0}; 

    std::shared_ptr<Node<T>> prev {nullptr}; // Умный указатель на предыдущую ноду
    std::shared_ptr<Node<T>> next {nullptr}; // Умный указатель на следующую ноду

public:

    // Constructors..

    Node();
    Node(const Node<T>&);
    Node(Node<T>&&);

    Node(const T&);
    Node(std::shared_ptr<Node<T>>&);

    // Setters..

    void set_next(std::shared_ptr<Node<T>>);
    void set_prev(std::shared_ptr<Node<T>>);

    // Getters..

    T& get_raw_data();
    const T& get_raw_data() const;

    std::shared_ptr<Node<T>> get_prev();
    std::shared_ptr<Node<T>> get_next();

    // Other methods..

    // Переопределённые операции

    const Node<T>& operator=(const Node<T>&);
    const Node<T>& operator=(Node<T>&&);

    const Node<T>& operator=(const T&);

    // Destructor..

    ~Node();
    
};

#endif