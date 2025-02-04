#pragma once
#include <vector>
#include <functional>
#include <unordered_map>
#include <algorithm>
namespace Utils 
{
template<typename  _Fty, typename... Args>
class EventCallback
{
private:
	std::vector<std::function<_Fty(Args...)>> m_eventVector;

	std::unordered_map<size_t, std::function<_Fty(Args...)>> m_eventMap;
	size_t nextId = 0;
	
public:
	size_t  operator+=(const std::function<_Fty(Args...)>& callback);
	void operator-=(size_t callbackId);
	void operator()(Args... args);
};

#include "EventCallback.inl"
}

