#pragma once

#include "../Engine.dec.h"
#include "../Utilities/Shared.h"

namespace Engine
{
    namespace Core
    {
        class Module
        {
            friend Loop;
        public:
            /// In the derived class, use this function to set the module's ExecutionChunk
            /// to specify the execution order, using a constructor written like:
            ///
            ///     MyModule(...) : Module(ChunkNumber) {...}
            ///
            /// @param ExecutionChunk Specifies modules' execution order
            Module(std::int_fast8_t ExecutionChunk = 0);

            virtual ~Module();

            /// @brief Enables the module
            ///
            /// The OnUpdate will be called when the module is enabled
            /// and the container Loop is started.
            void Enable();
            /// @brief Disables the module
            ///
            /// The OnUpdate won't be called when the module is disabled.
            void Disable();

            /// @brief Checks whether the module is enabled.
            ///
            /// The module may not be running while enabled.
            bool IsEnabled();
            /// @brief Checks whether the module is running.
            ///
            /// Running means that the OnUpdate is being called.
            bool IsRunning();
            /// @brief Gets the module's priority in a loop cycle.
            ///
            /// This determines modules' execution order.
            int GetExecutionChunk(); // TODO: Rename again?
            // @brief Returns whether the module can be executed asynchronously.
            bool IsAsync();

            virtual std::string GetName() = 0;

            virtual ExecutionType GetExecutionType();
        protected:
            /// @brief Is called on loop start or when being added
            ///        to the loop while the loop is running.
            virtual void OnStart() = 0;
            /// @brief Is called on activation if the loop is running or just after OnStart.
            virtual void OnEnable() = 0;
            /// @brief Update called by the loop.
            virtual void OnUpdate() = 0;
            /// @brief Is called when the OnUpdate or a Schedule created by this module throws an exception.
            ///
            /// Ignores the exceptions by default if not implemented.
            virtual void OnException(std::exception&);
            /// @brief Is called on deactivation if the loop is running or just before OnStop.
            virtual void OnDisable() = 0;
            /// @brief Is called on loop stop or when being removed
            ///        from the loop while the loop is running.
            virtual void OnStop() = 0;

            /// @brief Gets the update time since the Loop is started.
            ///
            /// The value is frozen for each Loop iteration/update.
            double GetTime();
            /// @brief Gets the time difference between the last 2 updates.
            double GetTimeDiff();
            /// @brief Gets the update time since the Loop is started as float.
            ///
            /// The value is frozen for each Loop iteration/update.
            float GetTimeAsFloat();
            /// @brief Gets the time difference between the last 2 updates as float.
            float GetTimeDiffAsFloat();

            /// @brief Gets the actual time passed from the start of the Loop
            ///        to the execution of this function.
            ///
            /// DO NOT use this unless you know what you are doing. Use GetTime instead.
            /// The value is not frozen for each Loop iteration/update.
            double GetActualTime();

            /// @brief Gets the loop which this module is added to.
            Loop * GetLoop();

            /// @brief Schedules to call a function.
            ///
            /// Will not call if the Loop is stopped before the call.
            /// Is executed right before modules with ExecutionChunk=0.
            ///
            /// The exceptions thrown by the Task will be handled by this module
            ///
            /// @param Task The function that will be called.
            /// @param Time The time when the function will be called.
            ///        Time = 0 or Time <= CurrentTime results in calling the function shortly.
            void Schedule(
                std::function<void()> Task,
                double Time = 0,
                ExecutionType ExecutionType = ExecutionType::BoundedAsync
            );
            /// @brief Schedules to call a function.
            ///
            /// Will not call if the Loop is stopped before the call.
            /// Is executed right before modules with ExecutionChunk=0.
            ///
            /// The exceptions thrown by the Task will be handled by this module
            ///
            /// @param Task The function that will be called.
            /// @param Time The time when the function will be called.
            ///        Time = 0 or Time <= CurrentTime results in calling the function shortly.
            void Schedule(
                double Time,
                std::function<void()> Task,
                ExecutionType ExecutionType = ExecutionType::BoundedAsync
            );
            /// @brief Schedules to call a function.
            ///
            /// Will not call if the Loop is stopped before the call.
            /// Is executed right before modules with ExecutionChunk=0.
            ///
            /// The exceptions thrown by the Task will be handled by this module
            ///
            /// @param Task The function that will be called.
            /// @param Time The time when the function will be called.
            ///        Time = 0 or Time <= CurrentTime results in calling the function shortly.
            void Schedule(
                double Time,
                ExecutionType ExecutionType,
                std::function<void()> Task
            );
        private:
            const std::int_fast8_t ExecutionChunk;

            Utilities::Shared<bool> isEnabled;
            Utilities::Shared<Loop*> loop;

            void Acquire(Loop*);
            void Release();
            void _Start();
            void _Stop();
        };
    }
}
