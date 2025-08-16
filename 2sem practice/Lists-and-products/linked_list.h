#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include <vector>
#include <algorithm>

#include "Node.cpp" // Вставляем node.cpp со всеми реализациями
// Вставлять product.cpp уже нет смысла, тк это было сделано в файле node.cpp

enum _extra_return_values_ // Особые значения, которые будут возвращаться при возникновении исключений при работе программы
{
    _cant_find_object_ = 128128128
};

enum _sort_parameters_
{
    id,
    name,
    price,
    supplier
};

union _filter_dt_
{
    _suppliers_ sup;
    unsigned id;
    double price;
    std::string name;

};

// Класс двусвязного списка
template <typename D>
class DoublyLinkedList
{
    shared_node_obj<D> head {nullptr}; // Умный указатель на голову (первый объект связного списка)
    shared_node_obj<D> tail {nullptr}; // Умный указатель на хвост (последний объект связного списка)

    unsigned length {0};

    bool isShared {false};     

    DoublyLinkedList(const DoublyLinkedList<D>&, unsigned, unsigned);
    DoublyLinkedList(DoublyLinkedList<D>&, unsigned, unsigned, bool);

    void push(shared_node_obj<D>);
    void push(shared_node_obj<D>, bool);

    unsigned _count_length_for_shared_();
    shared_node_obj<D> _get_shared_node_by_index_(unsigned);
    DoublyLinkedList<D> _get_shared_list_(unsigned, unsigned);
    
    unsigned _partition_by_obj_(unsigned, unsigned, bool); // +
    unsigned _partition_by_price_(unsigned, unsigned, bool);
    unsigned _partition_by_id_(unsigned, unsigned, bool);
    unsigned _partition_by_name_(unsigned, unsigned, bool);
    unsigned _partition_by_supplier_(unsigned, unsigned, bool);

    void _sort_by_obj_(unsigned, unsigned, bool); // +
    void _sort_by_price_(unsigned, unsigned, bool);
    void _sort_by_id_(unsigned, unsigned, bool);
    void _sort_by_name_(unsigned, unsigned, bool);
    void _sort_by_supplier(unsigned, unsigned, bool);

public:

    // Конструкторы

    DoublyLinkedList(); // +

    DoublyLinkedList(const DoublyLinkedList<D>&); // +

    DoublyLinkedList(DoublyLinkedList<D>&&); // +

    DoublyLinkedList(const D&); // +

    DoublyLinkedList(const D&, const D&); // +

    DoublyLinkedList(const char*); // +

    // Методы вставки элементов

    void push(const D&); // + Вставка в начало
    void push(const D&, bool); // + Вставка в конец

    void insert(const D&, int); // + Вставка в любую доступную позици (в текущих границах списка)

    shared_node_obj<D> pop(); // + Удаление объекта из конца
    shared_node_obj<D> pop(bool); // + Удаление объекта из начала

    shared_node_obj<D> remove(int); // + Удаление объекта под любым индексом в пределах текущих грациц списка

    unsigned get_length() const; // +

    unsigned search(const D&); // + Поиск конкретного объекта в связном списке

    DoublyLinkedList<D> sublist(unsigned, unsigned); // +

    DoublyLinkedList<D> filter_by_values(const std::vector<D>); // +

    void sort(bool); // +
    void sort(_sort_parameters_, bool); // +

    bool swap(unsigned, unsigned); // +
    

    void clear(); // +

    const DoublyLinkedList<D>& operator =(const DoublyLinkedList<D>&); // + Переопределение операции присваивания связных списков
    const DoublyLinkedList<D>& operator =(DoublyLinkedList<D>&&); // +

    DoublyLinkedList<D> operator +(const DoublyLinkedList<D>&); // + Позволяет получить конкатенацию двух списков
    DoublyLinkedList<D> operator +(const D&); // + Позволяет получить конкатенацию списка и продукта

    DoublyLinkedList<D> operator -(const DoublyLinkedList<D>&); // + Вычитает из левого списка те объекты, которые присутствуют в правом (если они есть в левом)
    DoublyLinkedList<D> operator -(const D&); // + Вычитает продукт из списка (если он в списке есть)

    const DoublyLinkedList<D>& operator +=(const DoublyLinkedList<D>&); // + Добавляет к текущему списку элементы другого списка
    const DoublyLinkedList<D>& operator +=(const D&); // + Добавляет к текущему списку новый продукт

    const DoublyLinkedList<D>& operator -=(const DoublyLinkedList<D>&); // + Удаляет из текущего списка объекты из правого списка (если они есть в текущем)
    const DoublyLinkedList<D>& operator -=(const D&); // + Удаляет из текущего списка продукт (если тот есть в списке)

    Node<D>& operator [] (unsigned); // (+-) В случае неверно выставленного индекса, будет передана голова списка
    const Node<D>& operator [] (unsigned) const; // (+-)

    bool save(const char*); // Сохранение списка в файл
    bool load(const char*); // Загрузка списка в файл
    /* Если список не пустой, то объекты будут добавляться справа, ничего удалено из не будет*/

    ~DoublyLinkedList();

    friend void _test_();
};

#endif