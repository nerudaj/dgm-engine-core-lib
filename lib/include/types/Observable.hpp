#pragma once

#include <functional>

template<class T>
class Observable
{
public:
    Observable(T initial, std::function<void(T)> notifyCallback)
        : value(initial), notifyObserver(notifyCallback)
    {
        notifyObserver(value);
    }

public:
    Observable& operator=(T newValue)
    {
        value = newValue;
        notifyObserver(value);
        return *this;
    }

    operator T() const // NOLINT
    {
        return value;
    }

private:
    T value;
    std::function<void(T)> notifyObserver;
};

template<class T>
class UnsafeObservable
{
public:
    UnsafeObservable(T initial, std::function<void(T)> notifyCallback)
        : value(initial), mainObserver(notifyCallback)
    {
        mainObserver(value);
    }

public:
    template<class ObserverType>
    void registerObserver(ObserverType&, std::function<void(T)> callback)
    {
        dynamicObservers[typeid(ObserverType).hash_code()] = callback;
    }

    template<class ObserverType>
    void unregisterObserver(ObserverType&)
    {
        dynamicObservers.erase(typeid(ObserverType).hash_code());
    }

    UnsafeObservable& operator=(T newValue)
    {
        value = newValue;
        mainObserver(value);
        for (auto&& [_, callback] : dynamicObservers)
        {
            callback(value);
        }
        return *this;
    }

    operator T() const // NOLINT
    {
        return value;
    }

private:
    T value;
    std::function<void(T)> mainObserver;
    std::map<std::size_t, std::function<void(T)>> dynamicObservers;
};
