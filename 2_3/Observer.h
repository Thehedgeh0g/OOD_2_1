#pragma once

#include <map>
#include <functional>
#include <unordered_set>

/*
Шаблонный интерфейс IObserver. Его должен реализовывать класс, 
желающий получать уведомления от соответствующего IObservable
Параметром шаблона является тип аргумента,
передаваемого Наблюдателю в метод Update
*/
template <typename T>
class IObserver
{
public:
	virtual void Update(T const& data) = 0;
	virtual ~IObserver() = default;
};

/*
Шаблонный интерфейс IObservable. Позволяет подписаться и отписаться на оповещения, а также
инициировать рассылку уведомлений зарегистрированным наблюдателям.
*/
template <typename T>
class IObservable
{
public:
	virtual ~IObservable() = default;
	virtual void RegisterObserver(IObserver<T> & observer, int priority = 0) = 0;
	virtual void NotifyObservers() = 0;
	virtual void RemoveObserver(IObserver<T> & observer) = 0;
};

// Реализация интерфейса IObservable
template <class T>
class CObservable : public IObservable<T>
{
public:
	typedef IObserver<T> ObserverType;

	// Регистрация наблюдателя с приоритетом
	void RegisterObserver(ObserverType& observer, int priority = 0) override
	{
		auto result = m_observersByPriority.insert({priority, {}});
		result.first->second.insert(&observer);
		m_observerToPriority[&observer] = priority;
	}

	void NotifyObservers() override
	{
		T data = GetChangedData();
		auto observersCopy = m_observersByPriority;
		for (auto it = observersCopy.rbegin(); it != observersCopy.rend(); ++it)
		{
			for (auto& observer : it->second)
			{
				observer->Update(data);
			}
		}
	}

	void RemoveObserver(ObserverType& observer) override
	{
		auto it = m_observerToPriority.find(&observer);
		if (it != m_observerToPriority.end())
		{
			int priority = it->second;
			m_observersByPriority[priority].erase(&observer);

			if (m_observersByPriority[priority].empty())
			{
				m_observersByPriority.erase(priority);
			}

			m_observerToPriority.erase(it);
		}
	}

protected:
	virtual T GetChangedData() const = 0;

private:
	std::unordered_map<ObserverType*, int> m_observerToPriority;
	std::map<int, std::unordered_set<ObserverType *>> m_observersByPriority;
};
