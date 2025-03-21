#include "nbt_interface.hpp"
#include <iostream>

static const char* get_value(const char* val)
{
    return val ? val : "NULL";
}

int main()
{
    /*
    DhNbtInstance instance("/home/dream_he/下载/Station.nbt");
    DhNbtInstance instance1(instance);
    std::cout << get_value(instance.get_key()) << "\n";
    instance.child();
    std::cout << get_value(instance.get_key()) << "\n";
    std::cout << get_value(instance1.get_key()) << "\n";
    instance.parent();
    std::cout << get_value(instance.get_key()) << "\n";
    std::cout << instance.get_nbt_rc() << "\n";
    */

    DhNbtInstance root(NULL, DH_TYPE_Compound);
    DhNbtInstance child((gint8)10, "key", true);
    child.set_free_only_instance(true);
    std::cout << (child.get_original_nbt()->key) << "\n";
    root.prepend(child);
    std::cout << root.child() << "\n";
    std::cout << root.get_key() << "\n";

    return 0;
}