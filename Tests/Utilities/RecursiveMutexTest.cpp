/* ---------------------------------------------------------------- //
Example tests:

- lock and shared-lock:

    input:
        c 
        n l  0 s 250 d
        n sl 0 s 250 d
        n sl 0 s 250 d
        n l  0 s 250 d
        s

    possible output:
        0.000683: thread-0: locked: local0-0
        0.250888: thread-0: done, destroying all local guards...
        0.250988: thread-1: shared-locked: local1-shared-0
        0.250998: thread-2: shared-locked: local2-shared-0
        0.501267: thread-1: done, destroying all local guards...
        0.501294: thread-2: done, destroying all local guards...
        0.501589: thread-3: locked: local3-0
        0.751795: thread-3: done, destroying all local guards...

- try-lock, try-shared-lock and try-upgradable-shared-lock:

    input:
        c
        n       tl  0 s 200 d
        n       tl  0 s 200 d
        n s 100 tsl 0 s 400 tsl 0 s 250 d
        n s 100 tsl 0 s 400 tsl 0 s 250 d
        n s 100 tul 0 s 400 tul 0 s 250 d
        n s 600 tl  0 d
        n s 600 tul 0 d
        s

    possible output:
        0.000289: thread-0: try-lock successful: local0-0
        0.000571: thread-1: try-lock failed: local1-0
        0.100922: thread-2: try-shared-lock failed: local2-shared-0
        0.101466: thread-3: try-shared-lock failed: local3-shared-0
        0.101619: thread-4: try-upgradable-shared-lock failed: local4-upgradable-shared-0
        0.200371: thread-0: done, destroying all local guards...
        0.200700: thread-1: done, destroying all local guards...
        0.501036: thread-2: try-shared-lock successful: local2-shared-0
        0.501546: thread-3: try-shared-lock successful: local3-shared-0
        0.501708: thread-4: try-upgradable-shared-lock successful: local4-upgradable-shared-0
        0.601760: thread-5: try-lock failed: local5-0
        0.601778: thread-5: done, destroying all local guards...
        0.601899: thread-6: try-upgradable-shared-lock failed: local6-upgradable-shared-0
        0.601916: thread-6: done, destroying all local guards...
        0.751147: thread-2: done, destroying all local guards...
        0.751621: thread-3: done, destroying all local guards...
        0.751790: thread-4: done, destroying all local guards..

- invalid operations (example: transition from shared-lock to lock or upgradable-shared-lock):

    input:
        c
        n sl 0 s 200 l   0 s 800 d
        n sl 0 s 400 tl  0 s 800 d
        n sl 0 s 600 ul  0 s 800 d
        n sl 0 s 800 tul 0 s 800 d
        s

    possible output:
        0.000366: thread-0: shared-locked: local0-shared-0
        0.000653: thread-1: shared-locked: local1-shared-0
        0.001076: thread-2: shared-locked: local2-shared-0
        0.001311: thread-3: shared-locked: local3-shared-0
        0.200511: thread-0: exception on lock attempt, local0-0: Invalid operation, possible deadlock: Cannot acquire lock after shared-lock in a single thread. Use upgradable-shared-lock instead, to lock afterwards.
        0.400782: thread-1: exception on try-lock attempt, local1-0: Invalid operation, possible livelock: A thread is trying to lock after it has shared-locked. Use upgradable-shared-lock instead, to lock afterwards.
        0.601207: thread-2: exception on upgradable-shared-lock attempt, local2-upgradable-shared-0: Invalid operation: Cannot acquire upgradable-shared-lock after shared-lock in a single thread.
        0.801438: thread-3: exception on try-upgradable-shared-lock attempt, local3-upgradable-shared-0: Invalid operation: Cannot acquire upgradable-shared-lock after shared-lock in a single thread.
        1.000614: thread-0: done, destroying all local guards...
        1.200890: thread-1: done, destroying all local guards...
        1.401316: thread-2: done, destroying all local guards...
        1.601545: thread-3: done, destroying all local guards...

- transition from upgradable-shared-lock to lock:

    input:
        c
        n       ul 0 s 200 l 0 d
        n s 100 sl 0 s 400 d
        n s 600 sl 0 s 400 d
        n s 800 ul 0 s 100 tl 0 s 200 tl 0 d
        s

    possible output:
        0.000300: thread-0: upgradable-shared-lock: local0-upgradable-shared-0
        0.100684: thread-1: shared-locked: local1-shared-0
        0.500789: thread-1: done, destroying all local guards...
        0.500833: thread-0: locked: local0-0
        0.500845: thread-0: done, destroying all local guards...
        0.600931: thread-2: shared-locked: local2-shared-0
        0.801160: thread-3: upgradable-shared-lock: local3-upgradable-shared-0
        0.901273: thread-3: try-lock failed: local3-0
        1.001035: thread-2: done, destroying all local guards...
        1.101377: thread-3: try-lock successful: local3-0
        1.101395: thread-3: done, destroying all local guards...

- transition from lock to shared-lock:

    input:
        c
        n l  0 s 200 sl 0 u 0 s 200 d
        n l  0 s 200 sl 0 u 0 s 200 d
        n sl 0 s 250 d
        n sl 0 s 300 d
        s

    possible output:
        0.000703: thread-0: locked: local0-0
        0.200915: thread-0: shared-locked: local0-shared-0
        0.200986: thread-0: unlocked: local0-0
        0.201014: thread-2: shared-locked: local2-shared-0
        0.201113: thread-3: shared-locked: local3-shared-0
        0.401189: thread-0: done, destroying all local guards...
        0.451216: thread-2: done, destroying all local guards...
        0.501295: thread-3: done, destroying all local guards...
        0.501406: thread-1: locked: local1-0
        0.701677: thread-1: shared-locked: local1-shared-0
        0.701742: thread-1: unlocked: local1-0
        0.901963: thread-1: done, destroying all local guards...

- recursive locking

    input:
        c
        n l 0 l 0 l 1 s 200 u 1 u 0 s 800 l 0 d
        n     l 0 l 1 s 200 u 0 u 1 d
        n s 400 l  0 l  1 s 250 d
        n s 600 sl 0 sl 1 s 250 d
        n s 800 ul 0 ul 1 s 250 d
        s

    possible output:
        0.000373: thread-0: locked: local0-0
        0.000392: thread-0: locked: local0-0
        0.000400: thread-0: locked: local0-1
        0.200491: thread-0: unlocked: local0-1
        0.200515: thread-0: unlocked: local0-0
        0.200524: thread-1: locked: local1-0
        0.200549: thread-1: locked: local1-1
        0.400636: thread-1: unlocked: local1-0
        0.400660: thread-1: unlocked: local1-1
        0.400665: thread-1: done, destroying all local guards...
        0.401332: thread-2: locked: local2-0
        0.401355: thread-2: locked: local2-1
        0.651430: thread-2: done, destroying all local guards...
        0.651482: thread-3: shared-locked: local3-shared-0
        0.651504: thread-3: shared-locked: local3-shared-1
        0.801810: thread-4: upgradable-shared-lock: local4-upgradable-shared-0
        0.801847: thread-4: upgradable-shared-lock: local4-upgradable-shared-1
        0.901574: thread-3: done, destroying all local guards...
        1.051921: thread-4: done, destroying all local guards...
        1.051987: thread-0: locked: local0-0
        1.052001: thread-0: done, destroying all local guards...

RecursiveMutexTest tests:
    print locking:   c  n l 0 s 500 d  n sl 0 s 500 d  n sl 0 s 500 d  n sl 0 s 500 d  n ul 0 s 500 d  s
    lock exceptions: c  n sl 0 l 0 tl 0 ul 0 gl 0 gtl 0 gul 0 d  s
    dict exceptions: c  n u 0 l 0 u 0 u 0 u 1 su 0 uu 0 gu 0 gsu 0 guu 0 d  s

// ---------------------------------------------------------------- */

#include "../../Engine/Engine.h"
#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

using namespace std::literals::chrono_literals;
using namespace Engine::Utilities;

#define print(context) (std::cout << context << '\n')
#define input(var) (std::cin >> var)

RecursiveMutex<false, false> r1;
RecursiveMutex<true, false> r2;
RecursiveMutex<true, true> r3;
//RecursiveMutex<false, true> r4; // error

void StaticTest()
{
    r1.GetLock();
    //r1.GetSharedLock(); // error
    //r1.GetUpgradableSharedLock(); // error

    r2.GetLock();
    r2.GetSharedLock();
    //r2.GetUpgradableSharedLock(); // error

    r3.GetLock();
    r3.GetSharedLock();
    r3.GetUpgradableSharedLock();

    //r1.GetSharedLock<false>(); // error
    //r1.GetSharedLock<true>(); // error
}

std::mutex PrintingMutex;

#define print_locked(context) { PrintingMutex.lock(); (std::cout << context << '\n'); PrintingMutex.unlock(); }

/// @brief The mutex that is tested by TestThreads.
RecursiveMutex<> GlobalTestMutex;

/// @brief Used to assign IDs to new TestThreads.
Shared<int, true> NextThreadID = 0;

/// @brief Gets the next ID to be assigned to the newly created TestThreads.
int GetNextThreadID()
{
    auto guard = NextThreadID.Mutex.GetLock();
    int id = NextThreadID;
    NextThreadID = NextThreadID + 1;
    return id;
}

/// @brief Test start time, used to print the time passed since the start of the test.
std::chrono::time_point<std::chrono::steady_clock> TestStartTime;

/// @brief Sets the TestStartTime to now
void SetTestStartTime()
{
    TestStartTime = std::chrono::steady_clock::now();
}

/// @brief Gets the time passed since the start of the test.
std::string GetStrTimeSinceStart()
{
    auto duration = std::chrono::steady_clock::now() - TestStartTime;
    double time = (double)std::chrono::duration_cast<std::chrono::microseconds>(duration).count() / 1000000.0;
    return std::to_string(time);
}

Collections::Dictionary<std::string, RecursiveMutex<>::LockGuard*> GlobalLockGuards;
Collections::Dictionary<std::string, RecursiveMutex<>::SharedLockGuard*> GlobalSharedLockGuards;
Collections::Dictionary<std::string, RecursiveMutex<>::UpgradableSharedLockGuard*> GlobalUpgradableSharedLockGuards;

/// @brief The command types available to use in a TestThread.
enum CommandType
{
    Sleep,
    Lock,
    Unlock,
    SharedLock,
    SharedUnlock,
    UpgradableSharedLock,
    UpgradableSharedUnlock,
    GlobalLock,
    GlobalUnlock,
    GlobalSharedLock,
    GlobalSharedUnlock,
    GlobalUpgradableSharedLock,
    GlobalUpgradableSharedUnlock,
    TryLock,
    TrySharedLock,
    TryUpgradableSharedLock,
    GlobalTryLock,
    GlobalTrySharedLock,
    GlobalTryUpgradableSharedLock
};

/// @brief Converts a short command name from CLI to CommandType to use in TestThread.
CommandType NameToCommandType(std::string name)
{
    if (name == "s") return Sleep;
    else if (name == "l") return Lock;
    else if (name == "u") return Unlock;
    else if (name == "sl") return SharedLock;
    else if (name == "su") return SharedUnlock;
    else if (name == "ul") return UpgradableSharedLock;
    else if (name == "uu") return UpgradableSharedUnlock;
    else if (name == "gl") return GlobalLock;
    else if (name == "gu") return GlobalUnlock;
    else if (name == "gsl") return GlobalSharedLock;
    else if (name == "gsu") return GlobalSharedUnlock;
    else if (name == "gul") return GlobalUpgradableSharedLock;
    else if (name == "guu") return GlobalUpgradableSharedUnlock;
    else if (name == "tl") return TryLock;
    else if (name == "tsl") return TrySharedLock;
    else if (name == "tul") return TryUpgradableSharedLock;
    else if (name == "gtl") return GlobalTryLock;
    else if (name == "gtsl") return GlobalTrySharedLock;
    else if (name == "gtul") return GlobalTryUpgradableSharedLock;
    else throw std::domain_error("Undefined command");
}

struct Command
{
    CommandType Type;
    /// @brief Ignored if the CommandType is Sleep.
    std::string GuardId;
    /// @brief Sleep duration in milliseconds. Only used if the CommandType is Sleep.
    double SleepDuration;
    Command() {}
    /// @brief Constructs a Sleep type command.
    Command(double SleepDuration) : Type(CommandType::Sleep), SleepDuration(SleepDuration) {}
    Command(CommandType Type, std::string GuardId) : Type(Type), GuardId(GuardId) {}
};

/// @brief Used to create test threads containing command lists by user.
class TestThread
{
private:
    int ID;
    Collections::Dictionary<std::string, RecursiveMutex<>::LockGuard*> LockGuards;
    Collections::Dictionary<std::string, RecursiveMutex<>::SharedLockGuard*> SharedLockGuards;
    Collections::Dictionary<std::string, RecursiveMutex<>::UpgradableSharedLockGuard*> UpgradableSharedLockGuards;
    std::vector<Command> Commands;
    std::thread Thread;

    /// @brief Ensures that a guard dictionary contains a GuardID
    template <typename GuardType> void EnsureGuardExistence(
            Collections::Dictionary<std::string, GuardType*>& Guards,
            const std::string& GuardID
        )
    {
        if (!Guards.Contains(GuardID)) Guards.SetValue(GuardID, new GuardType());
    }

    /// @brief Implementation of commands of type CommandType::Lock
    void Lock(std::string guard_id)
    {
        std::string expanded_guard_id = "local" + std::to_string(ID) + "-" + guard_id;
        try
        {
            EnsureGuardExistence(LockGuards, guard_id);
            *LockGuards.GetValue(guard_id) = GlobalTestMutex.GetLock();
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": locked: " << expanded_guard_id);
        }
        catch (RecursiveMutex<>::LockAfterSharedLockException& e) // RecursiveMutex<>::DeadlockException
        {
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": exception on lock attempt, " << expanded_guard_id << ": " << e.what());
        }
    }

    /// @brief Implementation of commands of type CommandType::Unlock
    void Unlock(std::string guard_id)
    {
        std::string expanded_guard_id = "local" + std::to_string(ID) + "-" + guard_id;
        try
        {
            LockGuards.GetValue(guard_id)->Unlock();
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": unlocked: " << expanded_guard_id);
        }
        catch (std::domain_error& e) // std::domain_error("Key not found.") thrown by LockGuards
        {
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": exception on unlock attempt, "
                                                << expanded_guard_id << ": " << e.what());
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": hint: Are you unlocking an existing guard? "
                        "LockGuards are created when you lock (l or tl).");
        }
    }

    /// @brief Implementation of commands of type CommandType::SharedLock
    void SharedLock(std::string guard_id)
    {
        std::string expanded_guard_id = "local" + std::to_string(ID) + "-shared-" + guard_id;
        EnsureGuardExistence(SharedLockGuards, guard_id);
        *SharedLockGuards.GetValue(guard_id) = GlobalTestMutex.GetSharedLock();
        print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": shared-locked: " << expanded_guard_id);
    }

    /// @brief Implementation of commands of type CommandType::SharedUnlock
    void SharedUnlock(std::string guard_id)
    {
        std::string expanded_guard_id = "local" + std::to_string(ID) + "-shared-" + guard_id;
        try
        {
            SharedLockGuards.GetValue(guard_id)->Unlock();
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": shared-unlocked: " << expanded_guard_id);
        }
        catch (std::domain_error& e) // std::domain_error("Key not found.") thrown by SharedLockGuards
        {
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": exception on shared-unlock attempt, "
                                                << expanded_guard_id << ": " << e.what());
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": hint: Are you unlocking an existing guard? "
                        "SharedLockGuards are created when you shared-lock (sl or tsl).");
        }
    }

    /// @brief Implementation of commands of type CommandType::UpgradableSharedLock
    void UpgradableSharedLock(std::string guard_id)
    {
        std::string expanded_guard_id = "local" + std::to_string(ID) + "-upgradable-shared-" + guard_id;
        try
        {
            EnsureGuardExistence(UpgradableSharedLockGuards, guard_id);
            *UpgradableSharedLockGuards.GetValue(guard_id) = GlobalTestMutex.GetUpgradableSharedLock();
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": upgradable-shared-lock: " << expanded_guard_id);
        }
        catch (RecursiveMutex<>::UpgradableSharedLockAfterSharedLockException& e)
        {
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": exception on upgradable-shared-lock attempt, "
                                                << expanded_guard_id << ": " << e.what());
        }
    }

    /// @brief Implementation of commands of type CommandType::UpgradableSharedLock
    void UpgradableSharedUnlock(std::string guard_id)
    {
        std::string expanded_guard_id = "local" + std::to_string(ID) + "-upgradable-shared-" + guard_id;
        try
        {
            UpgradableSharedLockGuards.GetValue(guard_id)->Unlock();
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": upgradable-shared-unlocked: " << expanded_guard_id);
        }
        catch (std::domain_error& e) // std::domain_error("Key not found.") thrown by UpgradableSharedLockGuards
        {
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": exception on upgradable-shared-unlock attempt, "
                                                << expanded_guard_id << ": " << e.what());
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": hint: Are you unlocking an existing guard? "
                        "SharedLockGuards are created when you acquire upgradable-shared-lock (ul or tul).");
        }
    }

    /// @brief Implementation of commands of type CommandType::TryLock
    void TryLock(std::string guard_id)
    {
        std::string expanded_guard_id = "local" + std::to_string(ID) + "-" + guard_id;
        try
        {
            EnsureGuardExistence(LockGuards, guard_id);
            RecursiveMutex<>::LockGuard guard;
            if (GlobalTestMutex.TryGetLock(guard))
            {
                *LockGuards.GetValue(guard_id) = guard;
                print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": try-lock successful: " << expanded_guard_id);
            }
            else print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": try-lock failed: " << expanded_guard_id);
        }
        catch (RecursiveMutex<>::TryLockAfterSharedLockException& e) // RecursiveMutex<>::PossibleLivelockException
        {
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": exception on try-lock attempt, "
                                                << expanded_guard_id << ": " << e.what());
        }
    }

    /// @brief Implementation of commands of type CommandType::TrySharedLock
    void TrySharedLock(std::string guard_id)
    {
        std::string expanded_guard_id = "local" + std::to_string(ID) + "-shared-" + guard_id;
        EnsureGuardExistence(SharedLockGuards, guard_id);
        RecursiveMutex<>::SharedLockGuard guard;
        if (GlobalTestMutex.TryGetSharedLock(guard))
        {
            *SharedLockGuards.GetValue(guard_id) = guard;
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": try-shared-lock successful: " << expanded_guard_id);
        }
        else print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": try-shared-lock failed: " << expanded_guard_id);
    }

    /// @brief Implementation of commands of type CommandType::TryUpgradableSharedLock
    void TryUpgradableSharedLock(std::string guard_id)
    {
        std::string expanded_guard_id = "local" + std::to_string(ID) + "-upgradable-shared-" + guard_id;
        try
        {
            EnsureGuardExistence(UpgradableSharedLockGuards, guard_id);
            RecursiveMutex<>::UpgradableSharedLockGuard guard;
            if (GlobalTestMutex.TryGetUpgradableSharedLock(guard))
            {
                *UpgradableSharedLockGuards.GetValue(guard_id) = guard;
                print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": try-upgradable-shared-lock successful: " << expanded_guard_id);
            }
            else print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": try-upgradable-shared-lock failed: " << expanded_guard_id);
        }
        catch (RecursiveMutex<>::UpgradableSharedLockAfterSharedLockException& e) // RecursiveMutex<>::UpgradableSharedLockAfterSharedLockException
        {
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": exception on try-upgradable-shared-lock attempt, "
                                                << expanded_guard_id << ": " << e.what());
        }
    }

    /// @brief Implementation of commands of type CommandType::GlobalLock
    void GlobalLock(std::string guard_id)
    {
        std::string expanded_guard_id = "global-" + guard_id;
        try
        {
            EnsureGuardExistence(GlobalLockGuards, guard_id);
            *GlobalLockGuards.GetValue(guard_id) = GlobalTestMutex.GetLock();
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": locked: " << expanded_guard_id);
        }
        catch (RecursiveMutex<>::LockAfterSharedLockException& e) // RecursiveMutex<>::DeadlockException
        {
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": exception on lock attempt, "
                                                << expanded_guard_id << ": " << e.what());
        }
    }

    /// @brief Implementation of commands of type CommandType::GlobalUnlock
    void GlobalUnlock(std::string guard_id)
    {
        std::string expanded_guard_id = "global-" + guard_id;
        try
        {
            GlobalLockGuards.GetValue(guard_id)->Unlock();
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": unlocked: " << expanded_guard_id);
        }
        catch (std::domain_error& e) // std::domain_error("Key not found.") thrown by GlobalLockGuards
        {
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": exception on unlock attempt, "
                                                << expanded_guard_id << ": " << e.what());
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": hint: Are you unlocking an existing guard? "
                        "Global LockGuards are created when you lock globally (gl or gtl).");
        }
    }

    /// @brief Implementation of commands of type CommandType::GlobalSharedLock
    void GlobalSharedLock(std::string guard_id)
    {
        std::string expanded_guard_id = "global-shared-" + guard_id;
        EnsureGuardExistence(GlobalSharedLockGuards, guard_id);
        *GlobalSharedLockGuards.GetValue(guard_id) = GlobalTestMutex.GetSharedLock();
        print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": shared-locked: " << expanded_guard_id);
    }

    /// @brief Implementation of commands of type CommandType::GlobalSharedUnlock
    void GlobalSharedUnlock(std::string guard_id)
    {
        std::string expanded_guard_id = "global-shared-" + guard_id;
        try
        {
            GlobalSharedLockGuards.GetValue(guard_id)->Unlock();
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": shared-unlocked: " << expanded_guard_id);
        }
        catch (std::domain_error& e) // std::domain_error("Key not found.") thrown by GlobalSharedLockGuards
        {
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": exception on shared-unlock attempt, "
                                                << expanded_guard_id << ": " << e.what());
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": hint: Are you unlocking an existing guard? "
                        "Global SharedLockGuards are created when you shared-lock globally (gsl or gtsl).");
        }
    }

    /// @brief Implementation of commands of type CommandType::GlobalUpgradableSharedLock
    void GlobalUpgradableSharedLock(std::string guard_id)
    {
        std::string expanded_guard_id = "global-upgradable-shared-" + guard_id;
        try
        {
            EnsureGuardExistence(GlobalUpgradableSharedLockGuards, guard_id);
            *GlobalUpgradableSharedLockGuards.GetValue(guard_id) = GlobalTestMutex.GetUpgradableSharedLock();
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": upgradable-shared-lock: " << expanded_guard_id);
        }
        catch (RecursiveMutex<>::UpgradableSharedLockAfterSharedLockException& e)
        {
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": exception on upgradable-shared-lock attempt, "
                                                << expanded_guard_id << ": " << e.what());
        }
    }

    /// @brief Implementation of commands of type CommandType::GlobalUpgradableSharedLock
    void GlobalUpgradableSharedUnlock(std::string guard_id)
    {
        std::string expanded_guard_id = "global-upgradable-shared-" + guard_id;
        try
        {
            GlobalUpgradableSharedLockGuards.GetValue(guard_id)->Unlock();
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": upgradable-shared-unlocked: " << expanded_guard_id);
        }
        catch (std::domain_error& e) // std::domain_error("Key not found.") thrown by UpgradableSharedLockGuards
        {
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": exception on upgradable-shared-unlock attempt, "
                                                << expanded_guard_id << ": " << e.what());
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": hint: Are you unlocking an existing guard? "
                        "SharedLockGuards are created when you acquire upgradable-shared-lock (ul or tul).");
        }
    }

    /// @brief Implementation of commands of type CommandType::GlobalTryLock
    void GlobalTryLock(std::string guard_id)
    {
        std::string expanded_guard_id = "global-" + guard_id;
        try
        {
            EnsureGuardExistence(GlobalLockGuards, guard_id);
            RecursiveMutex<>::LockGuard guard;
            if (GlobalTestMutex.TryGetLock(guard))
            {
                *GlobalLockGuards.GetValue(guard_id) = guard;
                print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": try-lock successful: " << expanded_guard_id);
            }
            else print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": try-lock failed: " << expanded_guard_id);
        }
        catch (RecursiveMutex<>::TryLockAfterSharedLockException& e) // RecursiveMutex<>::PossibleLivelockException
        {
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": exception on try-lock attempt, "
                                                << expanded_guard_id << ": " << e.what());
        }
    }

    /// @brief Implementation of commands of type CommandType::GlobalTrySharedLock
    void GlobalTrySharedLock(std::string guard_id)
    {
        std::string expanded_guard_id = "global-shared-" + guard_id;
        EnsureGuardExistence(GlobalSharedLockGuards, guard_id);
        RecursiveMutex<>::SharedLockGuard guard;
        if (GlobalTestMutex.TryGetSharedLock(guard))
        {
            *GlobalSharedLockGuards.GetValue(guard_id) = guard;
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": try-shared-lock successful: " << expanded_guard_id);
        }
        else print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": try-shared-lock failed: " << expanded_guard_id);
    }

    /// @brief Implementation of commands of type CommandType::GlobalTryUpgradableSharedLock
    void GlobalTryUpgradableSharedLock(std::string guard_id)
    {
        std::string expanded_guard_id = "global-upgradable-shared-" + guard_id;
        try
        {
            EnsureGuardExistence(GlobalUpgradableSharedLockGuards, guard_id);
            RecursiveMutex<>::UpgradableSharedLockGuard guard;
            if (GlobalTestMutex.TryGetUpgradableSharedLock(guard))
            {
                *GlobalUpgradableSharedLockGuards.GetValue(guard_id) = guard;
                print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": try-upgradable-shared-lock successful: " << expanded_guard_id);
            }
            else print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": try-upgradable-shared-lock failed: " << expanded_guard_id);
        }
        catch (RecursiveMutex<>::UpgradableSharedLockAfterSharedLockException& e) // RecursiveMutex<>::UpgradableSharedLockAfterSharedLockException
        {
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": exception on try-upgradable-shared-lock attempt, "
                                                << expanded_guard_id << ": " << e.what());
        }
    }

    void ExecuteCommand(Command cmd)
    {
        switch (cmd.Type)
        {
            case CommandType::Sleep:               std::this_thread::sleep_for(cmd.SleepDuration * 1ms); break;
            // ------------------------------------
            case CommandType::Lock:                          Lock(cmd.GuardId);                          break;
            case CommandType::Unlock:                        Unlock(cmd.GuardId);                        break;
            case CommandType::SharedLock:                    SharedLock(cmd.GuardId);                    break;
            case CommandType::SharedUnlock:                  SharedUnlock(cmd.GuardId);                  break;
            case CommandType::UpgradableSharedLock:          UpgradableSharedLock(cmd.GuardId);          break;
            case CommandType::UpgradableSharedUnlock:        UpgradableSharedUnlock(cmd.GuardId);        break;
            case CommandType::GlobalLock:                    GlobalLock(cmd.GuardId);                    break;
            case CommandType::GlobalUnlock:                  GlobalUnlock(cmd.GuardId);                  break;
            case CommandType::GlobalSharedLock:              GlobalSharedLock(cmd.GuardId);              break;
            case CommandType::GlobalSharedUnlock:            GlobalSharedUnlock(cmd.GuardId);            break;
            case CommandType::GlobalUpgradableSharedLock:    GlobalUpgradableSharedLock(cmd.GuardId);    break;
            case CommandType::GlobalUpgradableSharedUnlock:  GlobalUpgradableSharedUnlock(cmd.GuardId);  break;
            // ------------------------------------
            case CommandType::TryLock:                       TryLock(cmd.GuardId);                       break;
            case CommandType::TrySharedLock:                 TrySharedLock(cmd.GuardId);                 break;
            case CommandType::TryUpgradableSharedLock:       TryUpgradableSharedLock(cmd.GuardId);       break;
            case CommandType::GlobalTryLock:                 GlobalTryLock(cmd.GuardId);                 break;
            case CommandType::GlobalTrySharedLock:           GlobalTrySharedLock(cmd.GuardId);           break;
            case CommandType::GlobalTryUpgradableSharedLock: GlobalTryUpgradableSharedLock(cmd.GuardId); break;
        }
    }

public:
    /// @brief Constructs the TestThread by prompting the user to add the commands.
    TestThread()
    {
        ID = GetNextThreadID();
        while (true)
        {
            std::string str;
            double number;
            std::string guard_id;

            print("Command:");
            print("  s <milliseconds>           => Sleep");
            print("  <mutex_command> <guard_id> => Mutex commands");
            print("Local guard mutex commands:");
            print("  l:  Lock, u: Unlock,      sl:  SharedLock, su: SharedUnlock");
            print("  ul: UpgradableSharedLock, uu: UpgradableSharedUnlock");
            print("  tl: TryLock,              tsl: TrySharedLock");
            print("  tul: TryUpgradableSharedLock");
            print("Global guard mutex commands");
            print("  gl:  Lock, gu: Unlock,     gsl:  SharedLock, gsu: SharedUnlock");
            print("  gul: UpgradableSharedLock, guu: UpgradableSharedUnlock");
            print("  gtl: TryLock,              gtsl: TrySharedLock");
            print("  gtul: TryUpgradableSharedLock");
            print("Enter a command to add to thread " << ID << " or d (or q or e) to finish:");

            input(str);

            if (str == "d" || str == "q" || str == "e")
                break;
            if (str == "s")
            {
                input(number);
                Commands.push_back(Command(number));
            }
            else
            {
                try
                {
                    CommandType Type = NameToCommandType(str);
                    input(str);
                    Commands.push_back(Command(Type, str));
                }
                catch (std::domain_error&) // thrown by NameToCommandType
                {
                    print("Invalid input.");
                }
            }
        }
    }

    /// @brief Starts the thread.
    void Start()
    {
        Thread = std::thread([&] {
            for (auto command : Commands)
                ExecuteCommand(command);
            print_locked(GetStrTimeSinceStart() << ": thread-" << ID << ": done, destroying all local guards...");
            LockGuards.ForEach([](std::string key, RecursiveMutex<>::LockGuard * value) { delete value; });
            LockGuards.Clear();
            SharedLockGuards.ForEach([](std::string key, RecursiveMutex<>::SharedLockGuard * value) { delete value; });
            SharedLockGuards.Clear();
            UpgradableSharedLockGuards.ForEach([](std::string key, RecursiveMutex<>::UpgradableSharedLockGuard * value) { delete value; });
            UpgradableSharedLockGuards.Clear();
        });
    }

    /// @brief Waits for the thread to join.
    void Join()
    {
        Thread.join();
    }
};

int main()
{
    std::vector<std::shared_ptr<TestThread>> Threads;
    while (true)
    {
        while (true)
        {
            std::string str;
            print("Enter c to clear, n to create a new thread, s to start, or q (or e) to quit:");
            input(str);
            if (str == "c") { NextThreadID = 0; Threads.clear(); }
            else if (str == "n") Threads.push_back(std::shared_ptr<TestThread>(new TestThread()));
            else if (str == "s") break;
            else if (str == "q" || str == "e") return 0;
            else print("Invalid input.");
        }
        if (Threads.size() == 0) print("No threads to execute.");
        SetTestStartTime();
        for (auto t : Threads) t->Start();
        for (auto t : Threads) t->Join();
        GlobalLockGuards.ForEach([](std::string key, RecursiveMutex<>::LockGuard * value) { delete value; });
        GlobalLockGuards.Clear();
        GlobalSharedLockGuards.ForEach([](std::string key, RecursiveMutex<>::SharedLockGuard * value) { delete value; });
        GlobalSharedLockGuards.Clear();
    }
}
