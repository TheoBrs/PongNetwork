#pragma once
#include <vector>
#include <functional>

namespace Utils 
{
template<class _Fty>
class EventCallback
{
private:
	std::vector<std::function<_Fty>> m_eventVector;

public:
	EventCallback& operator+=(const std::function<_Fty>& callback);
	EventCallback& operator-=(const std::function<_Fty>& callback);
	EventCallback& operator()();
};

template<class _Fty>
EventCallback<_Fty>& EventCallback<_Fty>::operator+=(const std::function<_Fty>& callback)
{
	this->m_eventVector.push_back(callback);
	return *this;
}

template<class _Fty>
EventCallback<_Fty>& EventCallback<_Fty>::operator-=(const std::function<_Fty>& callback)
{
	this->m_eventVector.erase(std::remove(this->m_eventVector.begin(), this->m_eventVector.end(), 
		callback), this->m_eventVector.end());
	return *this;

}

template<class _Fty>
EventCallback<_Fty>& EventCallback<_Fty>::operator()() 
{
	for (int i = 0; i < this->m_eventVector.size(); i++)
	{
		if (this->m_eventVector[i]) 
		{
			this->m_eventVector[i]();
		}
	}
	return *this;
}
}

