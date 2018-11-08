#pragma once

#include <functional>
#include <mutex>
#include <shared_mutex>
#include <chrono>

namespace Engine
{
    namespace Data
    {
        class HandledMutex;
        template <typename Type, bool HasSetter = true, bool HasGetter = true> class Property;

        namespace Collections
        {
            template <typename ItemsType, bool UseMutex = true> class ResizableArray;
            template <typename ItemsType, bool UseMutex = true> class List;
            template <typename ItemsType, bool UseMutex = true> class Stack;
            template <typename ItemsType, bool UseMutex = true> class Queue;
            template <typename ItemsType, typename PriorityType = int, bool LessPriorityFirst = true, bool UseMutex = true> class PriorityQueue;
            template <typename KeyType, typename ValueType, bool UseMutex = true> class Dictionary;
        }
    }

    namespace Core
    {
        class Container;
        class Behavior;
    }
}