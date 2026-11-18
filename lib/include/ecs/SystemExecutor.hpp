#pragma once

#include <tuple>
#include <type_traits>

#include "ecs/SystemInterface.hpp"

/*
struct SystemA
{
    void operator()(const GameEvent&) {}
    void update(float) {}
};

int main()
{
    auto&& executor = ecs::SystemExecutor{
        std::type_identity<GameEvent>{},
        std::type_identity<float>{},
        SystemA()
    };

    executor.update(1/60.f);
    std::visit(executor, event);
}
*/

namespace ecs
{
    /**
     * Executor for managing ECS System objects.
     * 
     * Systems passed to the constructor are sequentially executed for each update() call.
     * Furthermore, the operator() forwards its parameter to each system, so it can be used
     * in std::visit calls.
     * 
     * Executor also manages the lifetime of the systems it holds.
     */
    template<class EventType, class TimeType, SystemType<EventType> ... Systems>
    class [[nodiscard]] SystemExecutor final
    {
    public:
        /**
         * Constructs a SystemExecutor with a list of systems to manage and execute.
         * 
         * First two parameters are type identifiers for type deduction only, so you
         * don't have to write template arguments explicitly.
         */
        constexpr SystemExecutor(
            std::type_identity<EventType>,
            std::type_identity<TimeType>,
            Systems&& ... args) : systems(std::forward<Systems>(args)...)
        {
        }

    public:
        /**
         * Forwards the given event to all managed systems.
         */
        constexpr void operator()(const auto& event)
        {
            std::apply([&event](auto&& ... systems){
                (systems(event),...);
            }, systems);
        }

    public:
        /**
         * Updates all managed systems with the given delta time.
         */
        constexpr void update(const TimeType& dt)
        {
            std::apply([&dt](auto&& ... systems){
                (systems.update(dt), ...);
            }, systems);
        }

    private:
        std::tuple<Systems...> systems;
    };

    // Deduction guide for SystemExecutor
    template<class EventType, class TimeType, class... Systems>
    SystemExecutor(std::type_identity<EventType>, std::type_identity<TimeType>, Systems&&...)
        -> SystemExecutor<EventType, TimeType, std::remove_cvref_t<Systems>...>;
} // namespace ecs
