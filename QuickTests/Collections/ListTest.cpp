#include "../../Engine/Engine.h"
#include <iostream>
#include <string>

#define print(context) (std::cout << context << '\n')
#define input(var) (std::cin >> var)

int main()
{
    Engine::Data::Collections::List<std::string> list;
    while (true)
    {
        print("");
        print("a Item        => Add(Item)");
        print("p Item Index  => Add(Item, Index)");
        print("r Index       => Remove(Index)");
        print("s Index Value => SetItem(Index, Value)");
        print("c             => Clear()");
        print("g Index       => GetItem(Index)");
        print("C             => GetCount()");
        print("f Item From   => Find(Item, FromIndex=0)");
        print("e Item        => Exists(Item)");
        print("F             => ForEach([](Item) { print(Item); }");
        print("");
        char func;
        int arg_int;
        std::string arg_str;
        input(func);
        switch (func)
        {
        case 'a':
            input(arg_str);
            print(list.Add(arg_str));
            break;
        case 'p':
            input(arg_str);
            input(arg_int);
            print(list.Add(arg_str, arg_int));
            break;
        case 'r':
            input(arg_int);
            print(list.Remove(arg_int));
            break;
        case 's':
            input(arg_int);
            input(arg_str);
            print(list.SetItem(arg_int, arg_str));
            break;
        case 'c':
            print(list.Clear());
            break;
        case 'g':
            input(arg_int);
            print(list.GetItem(arg_int));
            break;
        case 'C':
            print(list.GetCount());
            break;
        case 'f':
            input(arg_str);
            input(arg_int);
            print(list.Find(arg_str, arg_int));
            break;
        case 'e':
            input(arg_str);
            print(list.Exists(arg_str));
            break;
        case 'F':
            list.ForEach([](std::string Item) { print(Item); });
            break;
        default:
            break;
        }
    }

    return 0;
}