#pragma once

#include "../Engine.dec.h"
#include "RecursiveMutex.h"

namespace Engine
{
    namespace Utilities
    {
        class MutexContained
        {
        public:
            virtual ~MutexContained();
            /// @brief Calls the passed function while locking the object's mutex.
            void LockAndDo(std::function<void()> Process);
        protected:
            RecursiveMutex Mutex;
        };
    }
}
