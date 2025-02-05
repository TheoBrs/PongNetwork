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
        m_eventMap.erase(callbackId);

        m_eventVector.clear();
        for (const auto& pair : m_eventMap) {
            m_eventVector.push_back(pair.second);
        }
    }

}

template<typename _Fty, typename... Args>
void EventCallback<_Fty, Args...>::operator()(Args... args) 
{
    for (auto& callback : m_eventVector) {
        if (callback && callback!=nullptr) {
            callback(args...); 
        }
    }
}