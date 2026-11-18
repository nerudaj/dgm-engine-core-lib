#pragma once

#include <DGM/classes/Time.hpp>
#include <utility>

namespace ecs
{
    template<class System, class Event>
    concept Callable = requires(System&& object, const Event& e) {
        { object(e) };
    };

    /**
     * Static-type inheritance interface for ECS System objects.
     */
    template<class System>
    concept Updateable = requires(System&& object, const dgm::Time& dt) {
        { object.update(dt) };
    } && std::is_move_constructible_v<System>;

    /**
     * Dynamic-type inheritance interface for ECS System objects.
     */
    template<class TimeType>
    class SystemInterface
    {
    public:
        virtual ~SystemInterface() = default;

    public:
        virtual void update(const dgm::Time& dt) = 0;
    };

} // namespace ecs
