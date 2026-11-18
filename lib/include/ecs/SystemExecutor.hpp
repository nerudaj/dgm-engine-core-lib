#pragma once

#include "ecs/SystemInterface.hpp"
#include <tuple>
#include <type_traits>

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
     * Systems passed to the constructor are sequentially executed for each
     * update() call. Furthermore, the operator() forwards its parameter to each
     * system, so it can be used in std::visit calls.
     *
     * Executor also manages the lifetime of the systems it holds.
     */
    template<Updateable... Systems>
    class [[nodiscard]] SystemExecutor final
    {
    public:
        /**
         * Constructs a SystemExecutor with a list of systems to manage and
         * execute.
         */
        constexpr SystemExecutor(Systems&&... args)
            : systems(std::forward<Systems>(args)...)
        {
        }

    public:
        /**
         * Forwards the given event to all managed systems.
         */
        constexpr void operator()(const auto& event)
        {
            std::apply(
                [this, &event](auto&... systems)
                { (passEventToSystem(systems, event), ...); },
                systems);
        }

    public:
        /**
         * Updates all managed systems with the given delta time.
         */
        constexpr void update(const dgm::Time& dt)
        {
            std::apply(
                [&dt](auto&... systems) { (systems.update(dt), ...); },
                systems);
        }

        template<Updateable System>
        constexpr System& get() noexcept
        {
            return std::get<System>(systems);
        }

        template<Updateable System>
        constexpr const System& get() const noexcept
        {
            return std::get<System>(systems);
        }

    private:
        template<Updateable System, class Event>
        constexpr passEventToSystem(System& system, const Event& event)
        {
            if constexpr (Callable<System, Event>)
            {
                system(event);
            }
        }

    private:
        std::tuple<Systems...> systems;
    };
} // namespace ecs
