#pragma once

#include <vector>
#include <array>

template<class T>
class [[nodiscard]] EventQueue final
{
public:
    template<class EventType, class... Args>
    void pushEvent(Args&&... args)
    {
        events[activeQueueIdx].template emplace_back<EventType>(
            EventType { std::forward<Args>(args)... });
    }

    template<class... Visitor>
    void processEvents(Visitor&&... visitors) // NOLINT
    {
        /**
         * Event queue is double-buffered to prevent reallocation issues
         * While one buffer is used for reading, the other is used for writing.
         *
         * After finishing reading the current buffer, the other one is checked
         * whether it is empty and if not, they are swapped.
         */
        while (!events[activeQueueIdx].empty())
        {
            activeQueueIdx = 1 - activeQueueIdx;
            processEvents(events[1 - activeQueueIdx], visitors...);
        }
    }

private:
    template<class... Visitor>
    void processEvents(std::vector<T>& queue, Visitor&&... visitors) // NOLINT
    {
        for (size_t idx = 0; idx < queue.size(); ++idx)
        {
            (std::visit(visitors, queue[idx]), ...); // NOLINT
        }

        queue.clear();
    }

private:
    uint8_t activeQueueIdx = 0;
    std::array<std::vector<T>, 2u> events;
};
