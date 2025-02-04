#pragma once
#include "EventCallback.h"
template<typename  _Fty, typename... Args>
size_t  EventCallback<_Fty, Args...>::operator+=(const std::function<_Fty(Args...)>& callback)
{
    m_eventVector.push_back(callback);
    size_t id = nextId++;
    m_eventMap[id] = callback;
    return id;
}

template<typename  _Fty, typename... Args>
void EventCallback<_Fty, Args...>::operator-=(size_t callbackId)
{
    if (m_eventMap.find(callbackId) != m_eventMap.end()) {
        auto callbackToRemove = m_eventMap[callbackId];
        m_eventVector.erase(std::remove(m_eventVector.begin(), m_eventVector.end(), callbackToRemove), m_eventVector.end());
        m_eventMap.erase(callbackId);
    }
    return *this;

}

template<typename _Fty, typename... Args>
void EventCallback<_Fty, Args...>::operator()(Args... args) 
{
    for (auto& callback : m_eventVector) {
        if (callback) {
            callback(args...); 
        }
    }
}