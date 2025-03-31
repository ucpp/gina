#ifndef _GINA_ACTION_H_
#define _GINA_ACTION_H_

#include <functional>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <mutex>

namespace gina
{
    template <typename... Args>
    class Action
    {
    public:
        template <typename T>
        void Subscribe(T* object, void (T::*method)(Args...))
        {
            if (!object || !method) return;

            std::lock_guard<std::mutex> lock(m_mutex);
            uintptr_t objKey = reinterpret_cast<uintptr_t>(object);
            auto it = std::find_if(m_subscribers.begin(), m_subscribers.end(),
                [objKey](const auto& sub) { return sub.first == objKey; });
            
            if (it != m_subscribers.end()) return;

            m_subscribers.emplace_back(
                objKey,
                [object, method](Args... args) { (object->*method)(args...); }
            );
        }

        void Unsubscribe(void* object)
        {
            if (!object) return;

            std::lock_guard<std::mutex> lock(m_mutex);
            uintptr_t objKey = reinterpret_cast<uintptr_t>(object);
            
            m_subscribers.erase(
                std::remove_if(m_subscribers.begin(), m_subscribers.end(),
                    [objKey](const auto& sub) { return sub.first == objKey; }),
                m_subscribers.end()
            );
        }

        void Invoke(Args... args)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            
            for (const auto& subscriber : m_subscribers)
            {
                if (subscriber.second) 
                {
                    subscriber.second(args...);
                }
            }
        }

        void Clear()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_subscribers.clear();
        }

    private:
        std::vector<std::pair<uintptr_t, std::function<void(Args...)>>> m_subscribers;
        std::mutex m_mutex;
    };
}

#endif // !_GINA_ACTION_H_