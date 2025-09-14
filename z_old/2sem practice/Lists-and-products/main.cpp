#include <iostream>

#include "linked_list.cpp"

template <typename T>
void print_list(const DoublyLinkedList<T>& array, const std::string& name)
{
    size_t curr_length = array.get_length();

    std::cout << name << ':' << std::endl;

    if(curr_length == 0)
        std::cout << "<EMPTY>\n";
    
    for(int i = 0; i < curr_length; i++)
        std::cout << i << ") " << array[i].get_raw_data() << std::endl;
    
    std::cout << std::endl;
}

template <>
void print_list(const DoublyLinkedList<Product>& array, const std::string& name)
{
    size_t curr_length = array.get_length();

    std::cout << name << ':' << std::endl;

    if(curr_length == 0)
        std::cout << "<EMPTY>\n";
    
    for(int i = 0; i < curr_length; i++)
        std::cout << i << ") " << array[i].get_raw_data().get_id() << ' ' 
                                << array[i].get_raw_data().get_name() << ' ' 
                                << array[i].get_raw_data().get_price() << ' ' 
                                << array[i].get_raw_data().get_supplier() << std::endl;
    
    std::cout << std::endl;
}

int main()
{
    Node<int> dflt_node_init;
    
    Node trans_node_init_1 {-5};

    Node copy_node_init {trans_node_init_1};
    
    Node move_node_init {(Node<int>){-10}};

    std::cout << "Node variables: " << std::endl;
    std::cout << dflt_node_init.get_raw_data() << ' ' << trans_node_init_1.get_raw_data() << 
            ' ' << copy_node_init.get_raw_data() << ' ' << move_node_init.get_raw_data() << std::endl;
    std::cout << std::endl;

    DoublyLinkedList<int> list_int;
    list_int.push(1);
    list_int.push(2);
    list_int.push(3);
    list_int.push(4, true);
    list_int.remove(2);

    std::cout << "list_int created\n";

    DoublyLinkedList<std::string> list_string;
    list_string.push("one", true);
    list_string.push("two", true);
    list_string.push("three", true);
    list_string.insert("one/two", 1);
    list_string.pop(true);

    std::cout << "list_string created\n";

    DoublyLinkedList<double> list_double;
    list_double.push(1); 
    list_double.push(3); 
    list_double.push(2, true);
    list_double.insert(4, 2);
    list_double.pop();

    std::cout << "list_double created\n";

    DoublyLinkedList list_string_copy = list_string;

    std::cout << "list_string_copy created\n";

    DoublyLinkedList list_one_elem_init {1};

    std::cout << "list_one_elem_init created\n";

    DoublyLinkedList list_two_elem_init {3, 4};

    std::cout << "list_two_elem_init created\n";

    DoublyLinkedList<int> cycle_filled_list;

    for(int i = 0; i < 100; i++)
    {
        cycle_filled_list.push(i, (i % 2 == 0 ? true : false));
    }

    std::cout << "cycle_filled_list created\n";

    DoublyLinkedList cycle_filled_sublist = cycle_filled_list.sublist(10, 20);

    std::cout << "cycle_filled_sublist\n";

    DoublyLinkedList cycle_filled_filtered = cycle_filled_list.filter_by_values({1, 2, 50, 90});

    std::cout << "cycle_filled_filtered created\n";

    unsigned index_1 = cycle_filled_list.search(81);

    std::cout << "Index of 81 (" << index_1 << ") finded\n";

    bool res = cycle_filled_sublist.swap(4, 6);

    std::cout << "Swap result: " << res << std::endl;

    cycle_filled_list.clear();

    std::cout << "cycle_filled_list cleared\n";

    cycle_filled_sublist.sort(false);

    print_list(list_int, "list_int");
    print_list(list_string, "list_string");
    print_list(list_double, "list_double");
    print_list(list_string_copy, "list_string_copy");
    print_list(list_one_elem_init, "list_one_elem_init");
    print_list(list_two_elem_init, "list_two_elem_init");
    print_list(cycle_filled_list, "cycle_filled_list");
    print_list(cycle_filled_sublist, "cycle_filled_sublist");
    print_list(cycle_filled_filtered, "cycle_filled_filtered");

    std::cout << "===============" << std::endl;
    std::cout << "OPERATORS CHECK" << std::endl;
    std::cout << "===============" << std::endl;

    DoublyLinkedList<std::string> lst_1;
    DoublyLinkedList<std::string> lst_2;    
    DoublyLinkedList<std::string> lst_3;
    DoublyLinkedList<std::string> lst_4;
    DoublyLinkedList<std::string> lst_5;

    lst_1.push("1) Variables", true);
    lst_1.push("2) Operators", true);
    lst_1.push("3) Pointers", true);

    lst_2.push("trash");
    lst_2.push("garbage");

    std::cout << list_string[20].get_raw_data() << std::endl;


    lst_3.push("4) Conditional operators", true);
    lst_3.push("5) Cycles", true);
    lst_3.push("6) Functions", true);

    lst_4 += (const std::string)"-1) Computer's architecture";
    lst_4.push("0) Binary code", true);

    lst_5 += (const std::string)("trash");
    lst_5 += (const std::string)("garbage");

    print_list(lst_1, "lst_1");
    print_list(lst_2, "lst_2");
    print_list(lst_3, "lst_3");
    print_list(lst_4, "lst_4");
    print_list(lst_5, "lst_5");

    lst_2 -= lst_5;

    std::cout << "Removes lst_5 elements from lst_2\n";
    print_list(lst_2, "lst_2");

    lst_2 = lst_3;

    DoublyLinkedList lst_res = (lst_4 + lst_1) + lst_2;

    print_list(lst_res, "lst_res");

    lst_res.save("Roadmap");
    cycle_filled_filtered.save("Digits");

    DoublyLinkedList<std::string> load_res_1;
    DoublyLinkedList<int> load_res_2;

    load_res_1.load("Roadmap");
    load_res_2.load("Digits");

    print_list(load_res_1, "load_res_1");
    print_list(load_res_2, "load_res_2");

    puts("==================");
    puts("Products and lists");
    puts("==================");

    Product p1 {1, "Alabaster", 300, double_r},
        p2 {2, "Coconut", 100, barabulka},
        p3 {3, "Drawer", 4000, unknown_supplier},
        p4 {4, "Bomb", 1300, rnb_club},
        p5 {5, "Error", 1, bratyxi_42};

    DoublyLinkedList<Product> product_list;

    product_list.push(p1, true);
    product_list.push(p2, true);
    product_list.push(p3, true);
    product_list.push(p4, true);
    product_list.push(p5, true);

    product_list.sort(id, false);
    print_list(product_list, "product_list id");

    product_list.sort(name, false);
    print_list(product_list, "product_list name");

    product_list.sort(price, false);
    print_list(product_list, "product_list price");

    product_list.sort(supplier, false);
    print_list(product_list, "product_list supplier");

    return 0;
}
