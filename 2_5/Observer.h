#pragma once

#include <set>
#include <string>


template <typename T>
class IObserver;
/*
Шаблонный интерфейс IObservable. Позволяет подписаться и отписаться на оповещения, а также
инициировать рассылку уведомлений зарегистрированным наблюдателям.
*/
template <typename T>
class IObservable
{
public:
	virtual ~IObservable() = default;
	virtual void RegisterObserver(IObserver<T> & observer) = 0;
	virtual void NotifyObservers() = 0;
	virtual void RemoveObserver(IObserver<T> & observer) = 0;
	virtual std::string GetName() = 0;
};

// Реализация интерфейса IObservable

template <class T>
class CObservable : public IObservable<T>
{
public:
	typedef IObserver<T> ObserverType;

	explicit CObservable(std::string stationName)
		: m_stationName(std::move(stationName))
	{
	}

	void RegisterObserver(ObserverType & observer) override
	{
		m_observers.insert(&observer);
	}

	void NotifyObservers() override
	{
		T data = GetChangedData();
		for (auto & observer : m_observers)
		{
			observer->Update(data, *this);
		}
	}

	void RemoveObserver(ObserverType & observer) override
	{
		m_observers.erase(&observer);
	}

	std::string GetName() override
	{
		return m_stationName;
	}

protected:
	// Классы-наследники должны перегрузить данный метод,
	// в котором возвращать информацию об изменениях в объекте
	virtual T GetChangedData()const = 0;

private:
	std::set<ObserverType *> m_observers;
	std::string m_stationName;
};

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
	virtual void Update(T const& data, IObservable<T> &station) = 0;
	virtual ~IObserver() = default;
};
