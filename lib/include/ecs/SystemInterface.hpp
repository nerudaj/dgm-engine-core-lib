#pragma once

#include <utility>

namespace ecs
{
    /**
     * Static-type inheritance interface for ECS System objects.
     */
    template<class System, class Event>
    concept SystemType = requires(System&& object, float dt, Event event)
    {
        { std::visit(object, event) };   
        { object.update(dt) };
    };

    /**
     * Dynamic-type inheritance interface for ECS System objects.
     */
    template<class TimeType>
    class SystemInterface
    {
    public:
        virtual ~SystemInterface() = default;

    public:
        virtual void update(const TimeType& dt) = 0;
    };

} // namespace ecs
