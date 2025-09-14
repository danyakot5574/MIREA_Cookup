#include "linked_list.h"
#include "CustomExceptions.h"

#include <iostream>
#include <fstream>

using std::find;

template <typename D>
DoublyLinkedList<D>::DoublyLinkedList() = default;

template <typename D>
DoublyLinkedList<D>::DoublyLinkedList(const DoublyLinkedList<D>& other) // Копирование связного списка
{
    shared_node_obj<D> curr = other.head; // Новому "текущему" объекту присваиваем указатель на голову прибавляемого списка

    for(; curr != nullptr; curr = curr->get_next()) // Пока указатель "текущего" объекта не будет равен нулевому, перебираем все элементы прибавляемого списка
       push((D){curr.get()->get_raw_data()}, true); // Добавляем новые элементы в конец текущего списка
    
    curr.reset(); // Отвязываем указатель от "текущего" объекта
}

template <typename D>
DoublyLinkedList<D>::DoublyLinkedList(DoublyLinkedList<D>&& move)
{
    head = move.head;
    tail = move.tail;
    length = move.length;

    move.head.reset();
    move.tail.reset();  
    length = 0;
}

template <typename D>
DoublyLinkedList<D>::DoublyLinkedList(const D& data) // Создание связного списка только с одной переданной нодой
{
    head = std::make_shared<Node<D>>(data);
    tail = head;

    length = 1;
}

template <typename D>
DoublyLinkedList<D>::DoublyLinkedList(const D& d1, const D& d2) // Создание связного списка с двумя переданными нодами
{
    head = std::make_shared<Node<D>>(d1);
    tail = std::make_shared<Node<D>>(d2);

    head->set_next(tail);
    tail->set_prev(head);
    length = 2;
}

template <typename D>
DoublyLinkedList<D>::DoublyLinkedList(const char* f_name) // Загрузка связного списка из файла
{
    load(f_name);
}

template <typename D>
DoublyLinkedList<D>::DoublyLinkedList(const DoublyLinkedList<D>& other, unsigned pos_1, unsigned pos_2)
{
    shared_node_obj<D> curr = other.head;
    int cnt {0};

    while (cnt < pos_1 && curr != nullptr)
    {
        curr = curr->get_next();
        cnt++;
    }
    

    for(int i = pos_1; curr != nullptr && i < pos_2 + 1; i++, curr = curr->get_next())   
        push(curr, true);
}

template <typename D>
DoublyLinkedList<D>::DoublyLinkedList(DoublyLinkedList<D>& other, unsigned pos_1, unsigned pos_2, bool is_shared)
{
    if(!is_shared) DoublyLinkedList<D>(other, pos_1, pos_2);
    else
    {
        head = other._get_shared_node_by_index_(pos_1);
        tail = other._get_shared_node_by_index_(pos_2);
        isShared = true;
    }

    length = _count_length_for_shared_();
}

template <typename D>
void DoublyLinkedList<D>::push(const D& data) // Добавление элемента в начало списка
{
    shared_node_obj<D> new_node = std::make_shared<Node<D>>(data); // Новая нода (создаётся указатель и сразу привязывается к объекту new_node)

    new_node->set_next(head); // Сразу присваиваем голову списка следующей для новой ноды

    if(head != nullptr) // Если голова существует - ставим её предыдущей нодой новую
        head->set_prev(new_node);

    head = new_node; // Заменяем указатель головы на новую ноду

    if(length == 0)
        tail = head;

    new_node.reset(); // Отвязываем новую ноду от созданного объекта

    length++;
}

template <typename D>
void DoublyLinkedList<D>::push(const D& data, bool inEnd) // Добавление элемента в конец списка
{
    if(!inEnd) return push(data); // Если inEnd == false - начинаем процесс добавления в начало списка

    shared_node_obj<D> new_node = std::make_shared<Node<D>>(data); // Новая нода (создаётся указатель и сразу привязывается к объекту new_node)

    if(head == nullptr)
    { // Если головы не существует, сразу привязываем к ней новый указатель
        head = new_node;
        tail = head;
    }
    else
    {
        tail->set_next(new_node); // Новую ноду ставим следующим элементом для хвоста   
        new_node->set_prev(tail); // Ставим хвост списка предыдущим элементом для новой ноды
        tail = new_node; // Перепривязываем объект хвоста к новой ноде
    }

    new_node.reset(); // Отвязываем новую ноду от созданного объекта

    length++;
}

template <typename D>
void DoublyLinkedList<D>::push(shared_node_obj<D> other)
{
    try
    {
        if(isShared) 
            throw PushToSharedDoublyLinkedListError("WARNING: push: you can't push elements to shared list");
    }
    catch(const PushToSharedDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return;
    }

    shared_node_obj<D> new_node = std::make_shared<Node<D>> (*other);

    new_node->set_next(head); // Сразу присваиваем голову списка следующей для новой ноды

    if(head != nullptr) // Если голова существует - ставим её предыдущей нодой новую
        head->set_prev(new_node);

    head = new_node; // Заменяем указатель головы на новую ноду

    if(length == 0)
        tail = head;

    new_node.reset(); // Отвязываем новую ноду от созданного объекта

    length++;
}

template <typename D>
void DoublyLinkedList<D>::push(shared_node_obj<D> other, bool is_end)
{
    try
    {
        if(isShared) 
            throw PushToSharedDoublyLinkedListError("WARNING: push: you can't push elements to shared list");
    }
    catch(const PushToSharedDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return;
    }
    
    if(!is_end) return push(other);

    shared_node_obj<D> new_node = std::make_shared<Node<D>>(other); // Новая нода (создаётся указатель и сразу привязывается к объекту new_node)

    if(head == nullptr)
    { // Если головы не существует, сразу привязываем к ней новый указатель
        head = new_node;
        tail = head;
    }
    else // иначе
    {   
        tail->set_next(new_node); // Новую ноду ставим следующим элементом для хвоста  
        new_node->set_prev(tail); // Ставим хвост списка предыдущим элементом для новой ноды
        tail = new_node; // Перепривязываем объект хвоста к новой ноде
    }

    new_node.reset(); // Отвязываем новую ноду от созданного объекта

    length++;
}

template <typename D>
void DoublyLinkedList<D>::insert(const D& data, int pos) // Добавление элемента на любую позицию внутри списка
{
    try
    {
        if(isShared) 
            throw InsertIntoSharedDoublyLinkedListError("WARNING: insert: You can't insert into shared list");

        if(pos < 0) // Если позиция меньше нуля - добавить не может
            throw IncorrectIndexDoublyLinkedListError("WARNING: insert: Operation was canceled, incorrect index");

    }
    catch(const InsertIntoSharedDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return;
    }
    catch(const IncorrectIndexDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return;
    }
    
    if(pos == 0) return push(data); // Если позиция равна нулю - просто добавляем объект в начало

    shared_node_obj<D> curr = head; // Привязываем указатель головы к объекту curr
    int count {0}; // Счётчик позиций

    while (count < pos - 1 && curr != nullptr) // Выполняется до момента пока не дойдём до пред-позиции, либо пока curr не будет привязан к нулевому указателю
    {
        curr = curr->get_next(); // Привязываем объекту curr следующий элемент списка
        count++; // Увеличиваем на 1 счетчик
    }
    
    try
    {
        if(curr == nullptr) // Если curr привязан к нулевому указателю - значит позиция находится за границами списка
            throw IncorrectIndexDoublyLinkedListError("WARNING: insert: Operation was canceled, incorrect index");
    }
    catch(const IncomparableTypesLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return;
    }

    shared_node_obj<D> temp = std::make_shared<Node<D>>(data); // Новая нода (создаётся указатель и сразу привязывается к объекту new_node)

    temp->set_next(curr->get_next()); // Для новой ноды следующей ставим следующую от пред позиции
    temp->set_prev(curr); // Предыдущей - пред-позицию

    curr->get_next()->set_prev(temp); // Для следующей ноды от пред-позиции предыдущую ставим новой
    curr->set_next(temp); // Следующей нодой пред-позиции ставим новую

    // Отвязываем указатели от созданных внутри метода объектов
    
    temp.reset(); 
    curr.reset();

    length++;
}

template <typename D>
shared_node_obj<D> DoublyLinkedList<D>::pop() // Удаление объекта с конца
{
    try
    {
        if(isShared) 
            throw PopFromSharedDoublyLinkedListError("WARNING: pop: You can't pop from shared list");
    }
    catch(const PopFromSharedDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return head;
    }
    

    if(head == nullptr) // Если голова имеет нулевой указатель - ничего не делаем
        return nullptr;
    
    if(head->get_next() == nullptr) {
        head.reset();
        tail.reset();
        length = 0;
        return nullptr;
    }

    shared_node_obj<D> curr = tail; // Привязываем к curr предыдущий для хвоста указатель
    tail = curr->get_prev(); // Хвосту же привязываем curr (предыдущий)
    if (tail != nullptr) {
        tail->set_next(nullptr);
    } else {
        head.reset();
    }

    curr.reset(); // Отвязываем указатель от созданного объекта

    length--;

    return head;
}

template <typename D>
shared_node_obj<D> DoublyLinkedList<D>::pop(bool inStart) // Удаление объекта из начала
{
    try
    {
        if(isShared) 
            throw PopFromSharedDoublyLinkedListError("WARNING: pop: You can't pop from shared list");

    }
    catch(const PopFromSharedDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return head;
    }

    if(!inStart) return pop(); // Если inStart == false - удаляем объект из конца
    
    if(head == nullptr)
    {
        return nullptr;
    }
    
    shared_node_obj<D> temp = head;
    head = head->get_next();
    
    if(head != nullptr) 
    {
        head->set_prev(nullptr);
    } else {
        tail.reset();
    }
    
    temp.reset();
    length--;
    
    return head;
}

template <typename D>
shared_node_obj<D> DoublyLinkedList<D>::remove(int pos) // Удаление объекта из любой позиции в границах списка
{
    try
    {
        if(isShared) 
            throw RemoveFromSharedDoublyLinkedListError("WARNING: remove: You can't remove from shared list");
    }
    catch(const RemoveFromSharedDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return head;
    }
    

    if(!head) return head; // Если указатель головы нулевой - ничего не делаем

    shared_node_obj<D> curr = head; // Привязывам к новому объекту указатель головы

    for(int i = 0; curr && i < pos; ++i) // Пока он не будет нулевым, либо пока не дойдём до конца списка, перебираем элементы друг за другом
        curr = curr->get_next();

    if(!curr) return head; // Если дошли до нулевого указателя - ничего дальше не делаем тк был передан некорректный индекс

    if(curr->get_prev()) // если объект на текущей позиции имеет предыдущий объект (не нулевой указатель)
        curr->get_prev()->set_next(curr->get_next()); // то для предыдущего объекта, следующим объектом ставим следующий для текущего объекта

    if(curr->get_next()) // если объект на текущей позиции имеет следующий объект (не нулевой указатель)
        curr->get_next()->set_prev(curr->get_prev()); // то для следующего объекта, предыдущим объектом ставим предыдущий для текущего объекта

    if(head == curr) // если голова ссылается на тот же объект, что и текущий
        head = curr->get_next(); // привязываем к голове следующий для текущего объекта указатель
    
    curr.reset(); // отвязываем от созданного объекта "текущий" указатель. На него больше ничто не ссылается, поэтому память очистится

    length--;

    return head;
}

template <typename D>
unsigned DoublyLinkedList<D>::get_length() const
{ return length; }

template <typename D>
unsigned DoublyLinkedList<D>::_count_length_for_shared_()
{
    shared_node_obj<D> curr = head;
    unsigned len {0};

    while (true) 
    {
        len++;
        
        if(curr == tail) break;

        curr = curr->get_next();
    } 

    return len;
}   

template <typename D>
shared_node_obj<D> DoublyLinkedList<D>::_get_shared_node_by_index_(unsigned pos_1)
{
    shared_node_obj<D> curr = head;
    unsigned cnt {0};

    while (cnt < pos_1 && curr != nullptr)
    {
        curr = curr->get_next();
        cnt++;
    }

    return curr;
}

template <typename D>
DoublyLinkedList<D> DoublyLinkedList<D>::_get_shared_list_(unsigned pos_1, unsigned pos_2)
{
    return DoublyLinkedList<D>(*this, pos_1, pos_2);
}

template <typename D>
unsigned DoublyLinkedList<D>::search(const D& other) // Поиск строго определённого продукта в списке
{
    shared_node_obj<D> curr = head; // Создаём новый объект, привязываем к нему указатель на голову списка

    unsigned pos {0}; // Тут будем хранить позицию найденного продукта

    while (curr != nullptr) // Пока "текущий" объект не равен нулевому
    {
        if(curr.get()->get_raw_data() == other) // ищем совпадающий объект
        {
            curr.reset(); // Если нашли - отвязываем от "текущего" объекта указатель
            return pos; // возвращаем найденную позицию (метод заканчивает выполнение)
        }

        curr = curr->get_next(); // Если не нашли - продолжаем перебор
        pos++; // Увеличиваем позицию на 1

        if(isShared && curr == tail)
            break;
    }

    curr.reset(); // отвязываем от "текущего" объекта указатель

    return _cant_find_object_; // Возвращаем спец. значение для ненайденного объекта
}

template <typename D>
DoublyLinkedList<D> DoublyLinkedList<D>::sublist(unsigned pos_1, unsigned pos_2)
{
    DoublyLinkedList<D> sub_lst {*this, pos_1, pos_2};
    return sub_lst;
}

template <typename D>
DoublyLinkedList<D> DoublyLinkedList<D>::filter_by_values(const std::vector<D> _filter_values_)
{
    DoublyLinkedList<D> new_list;

    shared_node_obj<D> curr = head;

    for(; curr != nullptr; curr = curr->get_next())
    {
        if(std::find(_filter_values_.begin(), _filter_values_.end(), curr.get()->get_raw_data()) != _filter_values_.end())
            new_list.push(curr, true);
    }

    return new_list;
}

template <typename D>
bool DoublyLinkedList<D>::swap(unsigned pos_1, unsigned pos_2)
{
    if(pos_1 >= length || pos_2 >= length) return false;

    D temp = this->operator[](pos_1).get_raw_data();

    this->operator[](pos_1).get_raw_data() = this->operator[](pos_2).get_raw_data();
    this->operator[](pos_2).get_raw_data() = temp;

    return true;
}

template <typename D>
unsigned DoublyLinkedList<D>::_partition_by_obj_(unsigned low, unsigned high, bool reverse)
{
    D pivot = this->operator[](high).get_raw_data();

    int i = low - 1;

    for(int j = low; j < high; j++)
    {
        if(reverse)
        {
            if(this->operator[](j).get_raw_data() > pivot)
            {
                i++;
                swap(i, j);
            }
        } else {
            if(this->operator[](j).get_raw_data() < pivot)
            {
                i++;
                swap(i, j);
            }
        }
    }

    swap(i + 1, high);

    return i + 1;
}

template <typename D>
void DoublyLinkedList<D>::_sort_by_obj_(unsigned low, unsigned high, bool reverse)
{
    if (length == 0) return; // Проверка на пустой список

    if(low < high)
    {
        unsigned pivot = _partition_by_obj_(low, high, reverse);

        if (pivot > 0) // Проверка, чтобы избежать переполнения unsigned
        {
            _sort_by_obj_(low, pivot - 1, reverse);
        }
        _sort_by_obj_(pivot + 1, high, reverse);
    }
}

template <typename D>
void DoublyLinkedList<D>::sort(bool reverse)
{
    if (length == 0) return; // Проверка на пустой список

    _sort_by_obj_(0, length - 1, reverse);   
}

template <typename D>
void DoublyLinkedList<D>::clear()
{
    while (head != nullptr) // Пока голова не будет равна нулевому указателю - удаляем все элементы списка
    {
        shared_node_obj<D> temp = head;
        head = head->get_next();
        temp.reset();
    }
    tail.reset();
    length = 0;
}

template <typename D>
const DoublyLinkedList<D>& DoublyLinkedList<D>::operator =(const DoublyLinkedList<D>& other) // Переопределение оператора присваивания копированием
{
    try
    {
        if(isShared)
            throw AssignmentOperationSharedDoublyLinkedListError("WARNING: operator=: Assignment operation is not comparable with shared list");
    }
    catch(const AssignmentOperationSharedDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return *this;
    }

    if(this == &other) return *this; // Если указатели равны - ничего не делаем

    clear();

    for(shared_node_obj<D> curr = other.head; curr != nullptr; curr = curr->get_next()) // Переносим все элементы списка other в текущий
        push((D){curr->get_raw_data()}, true); // Вставка в конец

    return *this; // Возвращаем копию (константную ссылку) текущего объекта
}

template <typename D>
const DoublyLinkedList<D>& DoublyLinkedList<D>::operator =(DoublyLinkedList<D>&& move) // Переопределение оператора присваивания перемещением
{
    try
    {
        if(isShared)
            throw AssignmentOperationSharedDoublyLinkedListError("WARNING: operator=: Assignment operation is not comparable with shared list");
    }
    catch(const AssignmentOperationSharedDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return *this;
    }

    if(this == &move) return *this;

    clear();

    head = move.head;
    tail = move.tail;

    move.head.reset();
    move.tail.reset();

    return *this;
}

template <typename D>
DoublyLinkedList<D> DoublyLinkedList<D>::operator+(const DoublyLinkedList<D>& other) // Переопределение оператора суммирования для двух связных списков
{
    try
    {
        if(isShared)
            throw SummationOperationSharedDoublyLinkedListError("WARNING: operator+: Summation operation is not comparable with shared list");
    }
    catch(const SummationOperationSharedDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return *this;
    }

    DoublyLinkedList<D>& lst {*this}; // Новый связный список, равный текущему (копирование доступно благодаря переопределённому конструктору копирования)

    for(shared_node_obj<D> curr = other.head; curr != nullptr; curr = curr->get_next()) // Проходим весь список other
        lst.push((D){curr->get_raw_data()}, true); // Поочерёдно добавляя каждый элемент в конец "нового" списка

    return lst; // Возвращаем полученный связный список
}

template <typename D>
DoublyLinkedList<D> DoublyLinkedList<D>::operator+(const D& other) // Переопределение операции суммирования для связного списка и продукта
{   
    try
    {
        if(isShared)
            throw SummationOperationSharedDoublyLinkedListError("WARNING: operator+: Summation operation is not comparable with shared list");
    }
    catch(const SummationOperationSharedDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return *this;
    }

    DoublyLinkedList<D>& lst {*this}; // Новый связный список, равный текущему (копирование доступно благодаря переопределённому конструктору копирования)

    lst.push((D){other.get_raw_data()}, true); // Добавляем новый продукт в конец

    return lst; // Возвращаем новый список
}

template <typename D>
DoublyLinkedList<D> DoublyLinkedList<D>::operator -(const DoublyLinkedList<D>& other) // Переопределение операции вычитания для двух связных списков
{
    try
    {
        if(isShared)
            throw SubtractionOperationSharedDoublyLinkedListError("WARNING: operator-: Subtraction operation is not comparable with shared list");
    }
    catch(const SubtractionOperationSharedDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return *this;
    }

    DoublyLinkedList<D>& lst {*this}; // Новый связный список, равный текущему (копирование доступно благодаря переопределённому конструктору копирования)

    shared_node_obj<D> curr = other.head; // Новому "текущему" объекту привязываем указатель на голову вычитаемого списка

    for(; curr != nullptr; curr = curr->get_next()) // Далее, пока "текущий" объект не равен нулевому, перебираем все элементы вычитаемого списка
    {
        unsigned pos = lst.search((D){curr->get_raw_data()}); // Проверяем, есть ли в текущем списке "текущий" объект
        
        if(pos == _cant_find_object_) // Если нет - переходим к следующему элементу перебора
            continue;
        
        lst.remove(pos); // Иначе удаляем этот элемент и переходим к следующему элементу перебора
    }

    curr.reset(); // Отвязываем от "текущего" объекта указатель

    return lst; // Возвращаем новый список
}

template <typename D>
DoublyLinkedList<D> DoublyLinkedList<D>::operator-(const D& other) // Переопределение операции вычитания для связного списка и продукта
{
    try
    {
        if(isShared)
            throw SubtractionOperationSharedDoublyLinkedListError("WARNING: operator-: Subtraction operation is not comparable with shared list");
    }
    catch(const SubtractionOperationSharedDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return *this;
    }

    DoublyLinkedList<D>& lst {*this}; // Новый связный список, равный текущему (копирование доступно благодаря переопределённому конструктору копирования)

    unsigned pos = lst.search(other); // Проверяем, есть ли переданный продукт в текущем списке

    if(pos == _cant_find_object_) return lst; // Если нет - возвращаем неизменённый новый список

    lst.remove(pos); // Иначе удаляем этот объект

    return lst; // И возвращаем новый список
}

template <typename D>
const DoublyLinkedList<D>& DoublyLinkedList<D>::operator+=(const DoublyLinkedList<D>& other) // Переопределение расширенной операции присваивания с суммированием для двух связных списков
{
    try
    {
        if(isShared)
            throw SummationOperationSharedDoublyLinkedListError("WARNING: operator+=: Summation operation is not comparable with shared list");
    }
    catch(const SummationOperationSharedDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return *this;
    }

    shared_node_obj<D> curr = other.head; // Новому "текущему" объекту присваиваем указатель на голову прибавляемого списка

    for(; curr != nullptr; curr = curr->get_next()) // Пока указатель "текущего" объекта не будет равен нулевому, перебираем все элементы прибавляемого списка
        push((D){curr->get_raw_data()}, true); // Добавляем новые элементы в конец текущего списка

    curr.reset(); // Отвязываем указатель от "текущего" объекта

    return *this; // Возвращаем копию изменённого (текущего) списка
}

template <typename D>
const DoublyLinkedList<D>& DoublyLinkedList<D>::operator+=(const D& other) // Переопределение расширенной операции присваивания с суммированием для связного списка и продукта
{
    try
    {
        if(isShared)
            throw SummationOperationSharedDoublyLinkedListError("WARNING: operator+=: Summation operation is not comparable with shared list");
    }
    catch(const SummationOperationSharedDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return *this;
    }

    push(other, true); // Добавляем продукт в конец текущего списка

    return *this; // Возвращащаем копию изменённого (текущего) списка
}

template <typename D>
const DoublyLinkedList<D>& DoublyLinkedList<D>::operator-=(const DoublyLinkedList<D>& other) // Переопределение расширенной операции присваивания с вычитанием для двух связных списков
{
    try
    {
        if(isShared)
            throw SubtractionOperationSharedDoublyLinkedListError("WARNING: operator-=: Subtraction operation is not comparable with shared list");
    }
    catch(const SubtractionOperationSharedDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return *this;
    }

    shared_node_obj<D> curr = other.head; // Привязываем к новому "текущему" объекту указатель на голову вычитаемого списка
 
    for(; curr != nullptr && head != nullptr; curr = curr->get_next()) // Пока голова текущего списка, либо пока указатель "текущего" объекта не будут равны нулевому объекту, перебираем все элементы вычитаемого списка
    {
        unsigned pos = search((D){curr->get_raw_data()}); // Ищем "текущий" объект в текущем списке

        if(pos == _cant_find_object_) continue; // Если нет - переходим к следующему объекту
        remove(pos); // Иначе удаляем из текущего списка "текущий" объект
    }

    curr.reset(); // Отвязываем от нового объекта указатель

    return *this; // Возвращаем копию изменённого (текущего) списка
}

template <typename D>
const DoublyLinkedList<D>& DoublyLinkedList<D>::operator-=(const D& other) // Переопределение расширенной операции присваивания с вычитанием для связного списка и продукта
{
    try
    {
        if(isShared)
            throw SubtractionOperationSharedDoublyLinkedListError("WARNING: operator-=: Subtraction operation is not comparable with shared list");
    }
    catch(const SubtractionOperationSharedDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return *this;
    }

    unsigned pos = search(other); // Ищем в текущем списке переданный продукт

    if(pos == _cant_find_object_) return *this; // Если нет - возвращаем копию текущего списка

    remove(pos); // Иначе удаляем продукт из списка

    return *this; // Возвращаем копию изменённого (текущего) списка
}

template <typename D>
Node<D>& DoublyLinkedList<D>::operator[] (unsigned index)
{
    try
    {
        if(index >= length)
            throw IncorrectIndexDoublyLinkedListError("WARNING: operator[]: Operation canceled, incorrect index");
    }
    catch(const IncorrectIndexDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return *head.get();
    }

    shared_node_obj<D> curr = head;

    for(int i = 0; curr != nullptr && i != index; i++, curr = curr->get_next());

    return *curr;
}

template <typename D>
const Node<D>& DoublyLinkedList<D>::operator[] (unsigned index) const
{
    try
    {
        if(index >= length)
            throw IncorrectIndexDoublyLinkedListError("WARNING: operator[]: Operation canceled, incorrect index");
    }
    catch(const IncorrectIndexDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return *head.get();
    }

    shared_node_obj<D>curr = head;

    for(int i = 0; curr != nullptr && i != index; i++, curr = curr->get_next());

    return *curr;
}

template <typename D>
bool DoublyLinkedList<D>::save(const char* file_name) // Метод записи связного списка в бинарный файл (filename - переданная строка с именем файла в который будет произведено сохранение)
{
    std::ofstream ofs(file_name, std::ios::out | std::ios::binary); // Открываем файлоый поток для файла с именем file_name в бинарном режиме записи

    try
    {
        if (!ofs.is_open()) 
            throw FileDoesNotExistDoublyLinkedListError("WARNING: save: Can't find file");
    }
    catch(const FileDoesNotExistDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }

    shared_node_obj<D> curr = head; // Привязываем к новому "текущему" объекту указатель на голову текущего связного списка
    unsigned len = length; // Сохраняем в переменной длину связного списка (чтоб было удобно загружать список из файла)
    ofs.write((char*)&len, sizeof(unsigned)); // Первым делом записываем длину связного списка в бинарном файле

    unsigned long long type = typeid(curr.get()->get_raw_data()).hash_code();
    ofs.write((char*)&type, sizeof(unsigned long long));

    while (curr != nullptr) // Далее пока не дойдём до конца связного списка
    {
        ofs.write((char*)&curr.get()->get_raw_data(), sizeof(D)); // Записываем в бинарный файл данные текущего элемента

        curr = curr->get_next(); // Переходим к следующему объекту (и так до последнего)

        if(isShared && curr == tail)
            break;
    }
    
    ofs.close(); // Закрываем файловый поток
    return true; // Запись прошла успешно - возвращаем true
}

template <typename D>
bool DoublyLinkedList<D>::load(const char* file_name) // Метод для считывания связного списка из бинарного файла
{
    std::ifstream ifs(file_name, std::ios::in | std::ios::binary); // Открываем файловый поток для файла с именем file_name в бинарном режиме считывания
    
    try
    {
        if (!ifs.is_open()) 
            throw FileDoesNotExistDoublyLinkedListError("WARNING: load: Can't find file");
    }
    catch(const FileDoesNotExistDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }

    unsigned len; // Объявляем переменную для последующего считывания в неё длины связного списка
    ifs.read((char*)&len, sizeof(unsigned)); // Считываем длину списка из файла (для этого мы её первым делом записывали в методе записи в бинарный файл)

    unsigned long long type,
        type_curr = typeid(head.get()->get_raw_data()).hash_code();
    ifs.read((char*)&type, sizeof(unsigned long long));

    if(type != type_curr)
        throw IncomparableTypesLinkedListError("WARNING: load: List's type is not comparable with list from file");

    for (int i = 0; i < len; i++) // Пока i не будет равно длине связного списка
    {
        D data; // Объявляем переменную для последующего считывания в неё айди из файла
        ifs.read((char*)&data, sizeof(D)); // Считываем из бинарного файла айди объекта

        push({data}, true); // Добавляем в текущий связный список продукт, со считанными из бинарного файла данными (объект добавляется в конец)
    }

    ifs.close(); // Закрываем файловый поток
    return true; // Считывание прошло успешно - возвращаем true
}

template <typename D>
DoublyLinkedList<D>::~DoublyLinkedList()
{
    if(isShared)
    {
        head.reset();
        tail.reset();
    } else
        clear();
}


// Specializations

// For std::string type

template <>
bool DoublyLinkedList<std::string>::save(const char* file_name) // Метод записи связного списка в бинарный файл (filename - переданная строка с именем файла в который будет произведено сохранение)
{
    std::ofstream ofs(file_name, std::ios::out | std::ios::binary); // Открываем файлоый поток для файла с именем file_name в бинарном режиме записи

    try
    {
        if (!ofs.is_open()) 
            throw FileDoesNotExistDoublyLinkedListError("WARNING: save: Can't find file");
    }
    catch(const FileDoesNotExistDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    

    shared_node_obj<std::string> curr = head; // Привязываем к новому "текущему" объекту указатель на голову текущего связного списка
    unsigned len = length; // Сохраняем в переменной длину связного списка (чтоб было удобно загружать список из файла)
    ofs.write((char*)&len, sizeof(unsigned)); // Первым делом записываем длину связного списка в бинарном файле

    unsigned long long type = typeid(curr.get()->get_raw_data()).hash_code();
    ofs.write((char*)&type, sizeof(unsigned long long));

    while (curr != nullptr) // Далее пока не дойдём до конца связного списка
    {
        int s = curr.get()->get_raw_data().size();
        ofs.write((char*)&s, sizeof(int));
        ofs.write(curr.get()->get_raw_data().c_str(), s); // Записываем в бинарный файл данные текущего элемента

        curr = curr->get_next(); // Переходим к следующему объекту (и так до последнего)

        if(isShared && curr == tail)
            break;
    }
    
    ofs.close(); // Закрываем файловый поток
    return true; // Запись прошла успешно - возвращаем true
}

template <>
bool DoublyLinkedList<std::string>::load(const char* file_name) // Метод для считывания связного списка из бинарного файла
{
    std::ifstream ifs(file_name, std::ios::in | std::ios::binary); // Открываем файловый поток для файла с именем file_name в бинарном режиме считывания

    try
    {
        if (!ifs.is_open()) 
            throw FileDoesNotExistDoublyLinkedListError("WARNING: load: Can't find file");
    }
    catch(const FileDoesNotExistDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }

    unsigned len; // Объявляем переменную для последующего считывания в неё длины связного списка
    ifs.read((char*)&len, sizeof(unsigned)); // Считываем длину списка из файла (для этого мы её первым делом записывали в методе записи в бинарный файл)

    unsigned long long type,
        type_curr = typeid(head.get()->get_raw_data()).hash_code();
    ifs.read((char*)&type, sizeof(unsigned long long));

    try
    {
        if(type != type_curr)
            throw IncomparableTypesLinkedListError("WARNING: load: List's type is not comparable with list from file");
    }
    catch(const IncomparableTypesLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    
    for (int i = 0; i < len; i++) // Пока i не будет равно длине связного списка
    {
        int data_len; // Объявляем переменную для последующего считывания в неё длины из файла
        ifs.read((char*)&data_len, sizeof(int)); // Считываем из бинарного файла длину имени объекта
        
        std::vector<char> data(data_len + 1); // Объявляем динамический массив символов под длину равной длине имени + 1 (+ 1 для нулевого символа)
        ifs.read(data.data(), data_len); // Считываем из бинарного файла имя объекта
        data[data_len] = '\0';  // Добавляем нулевой символ в конец строки

        push(data.data(), true); // Добавляем в текущий связный список продукт, со считанными из бинарного файла данными (объект добавляется в конец)
    }

    ifs.close(); // Закрываем файловый поток
    return true; // Считывание прошло успешно - возвращаем true
}

// For Product type

template <>
DoublyLinkedList<Product>::DoublyLinkedList(const DoublyLinkedList<Product>& other) // Копирование связного списка
{
    shared_node_obj<Product> curr = other.head; // Новому "текущему" объекту присваиваем указатель на голову прибавляемого списка

    length = other.length;

    for(; curr != nullptr; curr = curr->get_next()) // Пока указатель "текущего" объекта не будет равен нулевому, перебираем все элементы прибавляемого списка
       push((Product){curr->get_raw_data().get_id(), 
                        curr->get_raw_data().get_name(), 
                        curr->get_raw_data().get_price(), 
                        curr->get_raw_data().get_supplier()}, true); // Добавляем новые элементы в конец текущего списка
    
    curr.reset(); // Отвязываем указатель от "текущего" объекта
}

template <>
unsigned DoublyLinkedList<Product>::search(const Product& other) // Поиск строго определённого продукта в списке
{
    shared_node_obj<Product> curr = head; // Создаём новый объект, привязываем к нему указатель на голову списка

    unsigned pos {0}; // Тут будем хранить позицию найденного продукта

    while (curr != nullptr) // Пока "текущий" объект не равен нулевому
    {
        if(curr->get_raw_data().get_id() == other.get_id() && 
            (curr->get_raw_data().get_name().compare(other.get_name()) == 0) &&
            curr->get_raw_data().get_price() == other.get_price() &&
            curr->get_raw_data().get_supplier() == other.get_supplier()) // ищем совпадающий объект
        {
            curr.reset(); // Если нашли - отвязываем от "текущего" объекта указатель
            return pos; // возвращаем найденную позицию (метод заканчивает выполнение)
        }

        curr = curr->get_next(); // Если не нашли - продолжаем перебор
        pos++; // Увеличиваем позицию на 1

        if(isShared && curr == tail)
            break;
    }

    curr.reset(); // отвязываем от "текущего" объекта указатель

    return _cant_find_object_; // Возвращаем спец. значение для ненайденного объекта
}

template <>
unsigned DoublyLinkedList<Product>::_partition_by_price_(unsigned low, unsigned high, bool reverse)
{
    Product pivot = (*this)[high].get_raw_data();

    int i = low - 1;

    for(int j = low; j < high; j++)
    {
        if(reverse)
        {
            if((*this)[j].get_raw_data().get_price() > pivot.get_price())
            {
                i++;
                swap(i, j);
            }
        } else {
            if((*this)[j].get_raw_data().get_price() < pivot.get_price())
            {
                i++;
                swap(i, j);
            }
        }
    }

    swap(i + 1, high);

    return i + 1;
}

template <>
unsigned DoublyLinkedList<Product>::_partition_by_id_(unsigned low, unsigned high, bool reverse)
{
    Product pivot = (*this)[high].get_raw_data();

    int i = low - 1;

    for(int j = low; j < high; j++)
    {
        if(reverse)
        {
            if((*this)[j].get_raw_data().get_id() > pivot.get_id())
            {
                i++;
                swap(i, j);
            }
        } else {
            if((*this)[j].get_raw_data().get_id() < pivot.get_id())
            {
                i++;
                swap(i, j);
            }
        }
    }

    swap(i + 1, high);

    return i + 1;
}

template <>
unsigned DoublyLinkedList<Product>::_partition_by_name_(unsigned low, unsigned high, bool reverse)
{
    Product pivot = (*this)[high].get_raw_data();

    int i = low - 1;

    for(int j = low; j < high; j++)
    {
        if(reverse)
        {
            if((*this)[j].get_raw_data().get_name() > pivot.get_name())
            {
                i++;
                swap(i, j);
            }
        } else {
            if((*this)[j].get_raw_data().get_name() < pivot.get_name())
            {
                i++;
                swap(i, j);
            }
        }
    }

    swap(i + 1, high);

    return i + 1;
}

template <>
unsigned DoublyLinkedList<Product>::_partition_by_supplier_(unsigned low, unsigned high, bool reverse)
{
    Product pivot = (*this)[high].get_raw_data();

    int i = low - 1;

    for(int j = low; j < high; j++)
    {
        if(reverse)
        {
            if((*this)[j].get_raw_data().get_supplier() > pivot.get_supplier())
            {
                i++;
                swap(i, j);
            }
        } else {
            if((*this)[j].get_raw_data().get_supplier() < pivot.get_supplier())
            {
                i++;
                swap(i, j);
            }
        }
    }

    swap(i + 1, high);

    return i + 1;
}

template <>
void DoublyLinkedList<Product>::_sort_by_price_(unsigned low, unsigned high, bool reverse)
{
    if (length == 0) return; // Проверка на пустой список

    if(low < high)
    {
        unsigned pivot = _partition_by_price_(low, high, reverse);

        if (pivot > 0) // Проверка, чтобы избежать переполнения unsigned
        {
            _sort_by_price_(low, pivot - 1, reverse);
        }
        _sort_by_price_(pivot + 1, high, reverse);
    }
}

template <>
void DoublyLinkedList<Product>::_sort_by_id_(unsigned low, unsigned high, bool reverse)
{
    if (length == 0) return; // Проверка на пустой список

    if(low < high)
    {
        unsigned pivot = _partition_by_id_(low, high, reverse);

        if (pivot > 0) // Проверка, чтобы избежать переполнения unsigned
        {
            _sort_by_id_(low, pivot - 1, reverse);
        }
        _sort_by_id_(pivot + 1, high, reverse);
    }
}

template <>
void DoublyLinkedList<Product>::_sort_by_name_(unsigned low, unsigned high, bool reverse)
{
    if (length == 0) return; // Проверка на пустой список

    if(low < high)
    {
        unsigned pivot = _partition_by_name_(low, high, reverse);

        if (pivot > 0) // Проверка, чтобы избежать переполнения unsigned
        {
            _sort_by_name_(low, pivot - 1, reverse);
        }
        _sort_by_name_(pivot + 1, high, reverse);
    }
}

template <>
void DoublyLinkedList<Product>::_sort_by_supplier(unsigned low, unsigned high, bool reverse)
{
    if (length == 0) return; // Проверка на пустой список

    if(low < high)
    {
        unsigned pivot = _partition_by_supplier_(low, high, reverse);

        if (pivot > 0) // Проверка, чтобы избежать переполнения unsigned
        {
            _sort_by_supplier(low, pivot - 1, reverse);
        }
        _sort_by_supplier(pivot + 1, high, reverse);
    }
}

template <>
void DoublyLinkedList<Product>::sort(_sort_parameters_ _data_type_, bool reverse)
{
    if (length == 0) return; // Проверка на пустой список

    int low = 0,
        high = length - 1;

    if(low < high)
    {
        switch (_data_type_)
        {
        case id:
            _sort_by_id_(low, high, reverse);
            break;
        
        case price:
            _sort_by_price_(low, high, reverse);
            break;

        case name:
            _sort_by_name_(low, high, reverse);
            break;

        default:
            _sort_by_supplier(low, high, reverse);
            break;
        }
    }
}

template <>
const DoublyLinkedList<Product>& DoublyLinkedList<Product>::operator =(const DoublyLinkedList<Product>& other) // Переопределение оператора присваивания копированием
{
    try
    {
        if(isShared)
            throw AssignmentOperationSharedDoublyLinkedListError("WARNING: operator=: Assignment operation is not comparable with shared list");
    }
    catch(const AssignmentOperationSharedDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return *this;
    }

    if(this == &other) return *this; // Если указатели равны - ничего не делаем

    clear();

    for(shared_node_obj<Product> curr = other.head; curr != nullptr; curr = curr->get_next()) // Переносим все элементы списка other в текущий
        push((Product){curr->get_raw_data().get_id(), 
                        curr->get_raw_data().get_name(), 
                        curr->get_raw_data().get_price(), 
                        curr->get_raw_data().get_supplier()}, true); // Вставка в конец

    return *this; // Возвращаем копию (константную ссылку) текущего объекта
}

template <>
DoublyLinkedList<Product> DoublyLinkedList<Product>::operator +(const DoublyLinkedList<Product>& other) // Переопределение оператора суммирования для двух связных списков
{
    try
    {
        if(isShared)
            throw SummationOperationSharedDoublyLinkedListError("WARNING: operator+: Summation operation is not comparable with shared list");
    }
    catch(const SummationOperationSharedDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return *this;
    }

    DoublyLinkedList& lst {*this}; // Новый связный список, равный текущему (копирование доступно благодаря переопределённому конструктору копирования)

    for(shared_node_obj<Product> curr = other.head; curr != nullptr; curr = curr->get_next()) // Проходим весь список other
        lst.push((Product){curr->get_raw_data().get_id(), 
                            curr->get_raw_data().get_name(), 
                            curr->get_raw_data().get_price(), 
                            curr->get_raw_data().get_supplier()}, true); // Поочерёдно добавляя каждый элемент в конец "нового" списка

    return lst; // Возвращаем полученный связный список
}

template <>
DoublyLinkedList<Product> DoublyLinkedList<Product>::operator+(const Product& other) // Переопределение операции суммирования для связного списка и продукта
{   
    try
    {
        if(isShared)
            throw SummationOperationSharedDoublyLinkedListError("WARNING: operator+: Summation operation is not comparable with shared list");
    }
    catch(const SummationOperationSharedDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return *this;
    }

    DoublyLinkedList& lst {*this}; // Новый связный список, равный текущему (копирование доступно благодаря переопределённому конструктору копирования)

    lst.push((Product){other.get_id(), other.get_name(), other.get_price(), other.get_supplier()}, true); // Добавляем новый продукт в конец

    return lst; // Возвращаем новый список
}

template <>
DoublyLinkedList<Product> DoublyLinkedList<Product>::operator -(const DoublyLinkedList<Product>& other) // Переопределение операции вычитания для двух связных списков
{
    try
    {
        if(isShared)
            throw SubtractionOperationSharedDoublyLinkedListError("WARNING: operator-: Subtraction operation is not comparable with shared list");
    }
    catch(const SubtractionOperationSharedDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return *this;
    }

    DoublyLinkedList& lst {*this}; // Новый связный список, равный текущему (копирование доступно благодаря переопределённому конструктору копирования)

    shared_node_obj<Product> curr = other.head; // Новому "текущему" объекту привязываем указатель на голову вычитаемого списка

    for(; curr != nullptr; curr = curr->get_next()) // Далее, пока "текущий" объект не равен нулевому, перебираем все элементы вычитаемого списка
    {
        unsigned pos = lst.search((Product){curr->get_raw_data().get_id(), 
                                            curr->get_raw_data().get_name(), 
                                            curr->get_raw_data().get_price(), 
                                            curr->get_raw_data().get_supplier()}); // Проверяем, есть ли в текущем списке "текущий" объект
        
        if(pos == _cant_find_object_) // Если нет - переходим к следующему элементу перебора
            continue;
        
        lst.remove(pos); // Иначе удаляем этот элемент и переходим к следующему элементу перебора
    }

    curr.reset(); // Отвязываем от "текущего" объекта указатель

    return lst; // Возвращаем новый список
}

template <>
DoublyLinkedList<Product> DoublyLinkedList<Product>::operator-(const Product& other) // Переопределение операции вычитания для связного списка и продукта
{
    try
    {
        if(isShared)
            throw SubtractionOperationSharedDoublyLinkedListError("WARNING: operator-: Subtraction operation is not comparable with shared list");
    }
    catch(const SubtractionOperationSharedDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return *this;   
    }

    DoublyLinkedList& lst {*this}; // Новый связный список, равный текущему (копирование доступно благодаря переопределённому конструктору копирования)

    unsigned pos = lst.search(other); // Проверяем, есть ли переданный продукт в текущем списке

    if(pos == _cant_find_object_) return lst; // Если нет - возвращаем неизменённый новый список

    lst.remove(pos); // Иначе удаляем этот объект

    return lst; // И возвращаем новый список
}

template <>
const DoublyLinkedList<Product>& DoublyLinkedList<Product>::operator+=(const DoublyLinkedList<Product>& other) // Переопределение расширенной операции присваивания с суммированием для двух связных списков
{
    try
    {
        if(isShared)
            throw SummationOperationSharedDoublyLinkedListError("WARNING: operator+=: Summation operation is not comparable with shared list");
    }
    catch(const SummationOperationSharedDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
    }

    shared_node_obj<Product> curr = other.head; // Новому "текущему" объекту присваиваем указатель на голову прибавляемого списка

    for(; curr != nullptr; curr = curr->get_next()) // Пока указатель "текущего" объекта не будет равен нулевому, перебираем все элементы прибавляемого списка
        push((Product){curr->get_raw_data().get_id(), 
                        curr->get_raw_data().get_name(), 
                        curr->get_raw_data().get_price(), 
                        curr->get_raw_data().get_supplier()}, true); // Добавляем новые элементы в конец текущего списка

    curr.reset(); // Отвязываем указатель от "текущего" объекта

    return *this; // Возвращаем копию изменённого (текущего) списка
}

template <>
const DoublyLinkedList<Product>& DoublyLinkedList<Product>::operator-=(const DoublyLinkedList<Product>& other) // Переопределение расширенной операции присваивания с вычитанием для двух связных списков
{
    try
    {
        if(isShared)
            throw SubtractionOperationSharedDoublyLinkedListError("WARNING: operator-=: Subtraction operation is not comparable with shared list");
    }
    catch(const SubtractionOperationSharedDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return *this;
    }

    shared_node_obj<Product> curr = other.head; // Привязываем к новому "текущему" объекту указатель на голову вычитаемого списка
 
    for(; curr != nullptr && head != nullptr; curr = curr->get_next()) // Пока голова текущего списка, либо пока указатель "текущего" объекта не будут равны нулевому объекту, перебираем все элементы вычитаемого списка
    {
        unsigned pos = search((Product){curr->get_raw_data().get_id(), 
                                        curr->get_raw_data().get_name(), 
                                        curr->get_raw_data().get_price(), 
                                        curr->get_raw_data().get_supplier()}); // Ищем "текущий" объект в текущем списке

        if(pos == _cant_find_object_) continue; // Если нет - переходим к следующему объекту
        remove(pos); // Иначе удаляем из текущего списка "текущий" объект
    }

    curr.reset(); // Отвязываем от нового объекта указатель

    return *this; // Возвращаем копию изменённого (текущего) списка
}

template<>
bool DoublyLinkedList<Product>::save(const char* file_name) // Метод записи связного списка в бинарный файл (filename - переданная строка с именем файла в который будет произведено сохранение)
{
    std::ofstream ofs(file_name, std::ios::out | std::ios::binary); // Открываем файлоый поток для файла с именем file_name в бинарном режиме записи
    
    try
    {
        if (!ofs.is_open()) 
            throw FileDoesNotExistDoublyLinkedListError("WARNING: save: Can't find file");
    }
    catch(const FileDoesNotExistDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }

    shared_node_obj<Product> curr = head; // Привязываем к новому "текущему" объекту указатель на голову текущего связного списка
    int len = length; // Сохраняем в переменной длину связного списка (чтоб было удобно загружать список из файла)
    ofs.write((char*)&len, sizeof(int)); // Первым делом записываем длину связного списка в бинарном файле

    unsigned long long type = typeid(curr.get()->get_raw_data()).hash_code();
    ofs.write((char*)&type, sizeof(unsigned long long));

    while (curr != nullptr) // Далее пока не дойдём до конца связного списка
    {
        ofs.write((char*)&curr->get_raw_data().get_id(), sizeof(unsigned)); // Записываем в бинарный файл айди текущего объекта

        int s = curr->get_raw_data().get_name().size(); // Сохраняем в переменной длину имени объекта (чтоб было удобно записывать и загружать имя объекта)
        ofs.write((char*)&s, sizeof(int)); // Записываем в бинарный файл длину имени объекта
        ofs.write(curr->get_raw_data().get_name().c_str(), s); // Записываем в бинарный файл имя объекта (как раз используя ранее сохранённую длину) 
        ofs.write((char*)&curr->get_raw_data().get_price(), sizeof(double)); // Записываем в бинарный файл цену объекта
        ofs.write((char*)&curr->get_raw_data().get_supplier(), sizeof(_suppliers_)); // Записываем в бинарный файл производителя объекта

        curr = curr->get_next(); // Переходим к следующему объекту (и так до последнего)

        if(isShared && curr == tail)
            break;
    }
    
    ofs.close(); // Закрываем файловый поток
    return true; // Запись прошла успешно - возвращаем true
}

template <>
bool DoublyLinkedList<Product>::load(const char* file_name) // Метод для считывания связного списка из бинарного файла
{
    std::ifstream ifs(file_name, std::ios::in | std::ios::binary); // Открываем файловый поток для файла с именем file_name в бинарном режиме считывания
    
    try
    {
        if (!ifs.is_open()) 
            throw FileDoesNotExistDoublyLinkedListError("WARNING: load: Can't find file");
    }
    catch(const FileDoesNotExistDoublyLinkedListError& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }

    int len; // Объявляем переменную для последующего считывания в неё длины связного списка
    ifs.read((char*)&len, sizeof(int)); // Считываем длину списка из файла (для этого мы её первым делом записывали в методе записи в бинарный файл)

    unsigned long long type,
        type_curr = typeid(head.get()->get_raw_data()).hash_code();
    ifs.read((char*)&type, sizeof(unsigned long long));

    if(type != type_curr)
        throw IncomparableTypesLinkedListError("WARNING: load: List's type is not comparable with list from file");

    
    for (int i = 0; i < len; i++) // Пока i не будет равно длине связного списка
    {
        unsigned id; // Объявляем переменную для последующего считывания в неё айди из файла
        int name_len; // Объявляем переменную для последующего считывания в неё длины из файла
        ifs.read((char*)&id, sizeof(unsigned)); // Считываем из бинарного файла айди объекта

        ifs.read((char*)&name_len, sizeof(int)); // Считываем из бинарного файла длину имени объекта

        std::vector<char> name(name_len + 1); // Объявляем динамический массив символов под длину равной длине имени + 1 (+ 1 для нулевого символа)
        ifs.read(name.data(), name_len); // Считываем из бинарного файла имя объекта
        name[name_len] = '\0';  // Добавляем нулевой символ в конец строки

        double price; // Объявляем переменную для последующего считывания в неё цены объекта
        _suppliers_ supplier; // Объявляем переменную для последующего считывания в неё поставщика объекта
        ifs.read((char*)&price, sizeof(double)); // Считываем из бинарного файла цену объекта
        ifs.read((char*)&supplier, sizeof(_suppliers_)); // Считываем из бинарного файла поставщика объекта

        push({id, name.data(), price, supplier}, true); // Добавляем в текущий связный список продукт, со считанными из бинарного файла данными (объект добавляется в конец)
    }

    ifs.close(); // Закрываем файловый поток
    return true; // Считывание прошло успешно - возвращаем true
}
