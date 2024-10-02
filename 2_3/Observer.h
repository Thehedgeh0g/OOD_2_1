#pragma once

#include <map>
#include <set>
#include <functional>

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
		auto it = m_observersByPriority.find(priority);
		for (auto& [prio, obs] : m_observersByPriority)
		{
			if (obs == &observer)
			{
				return; // Наблюдатель уже подписан
			}
		}
		m_observersByPriority.insert({priority, &observer});
	}

	void NotifyObservers() override
	{
		T data = GetChangedData();
		// Уведомляем наблюдателей по порядку приоритетов от высокого к низкому
		for (auto& [priority, observer] : m_observersByPriority)
		{
			observer->Update(data);
		}
	}

	void RemoveObserver(ObserverType& observer) override
	{
		for (auto it = m_observersByPriority.begin(); it != m_observersByPriority.end(); ++it)
		{
			if (it->second == &observer)
			{
				m_observersByPriority.erase(it);
				return;
			}
		}
	}

protected:
	virtual T GetChangedData() const = 0;

private:
	struct Compare {
		bool operator()(const int& lhs, const int& rhs) const {
			return lhs > rhs;
		}
	};

	std::map<int, ObserverType*, Compare> m_observersByPriority;
};
