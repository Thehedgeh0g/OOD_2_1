#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <map>
#include <cmath>

#include "Observer.h"

struct SWeatherInfo
{
	double temperature = 0;
	double humidity = 0;
	double pressure = 0;
	double windDirection = 0;
	double windSpeed = 0;
};
class CalculatedStat {
public:
	void Update(double value) {
		if (m_min > value) {
			m_min = value;
		}
		if (m_max < value) {
			m_max = value;
		}
		m_accumulatedValue += value;
		++m_count;
		m_aveValue = m_accumulatedValue / m_count;
	}

	[[nodiscard]] double GetMin() const {
		return m_min;
	}

	[[nodiscard]] double GetMax() const {
		return m_max;
	}

	[[nodiscard]] double GetAverage() const {
		return m_aveValue;
	}

	void UpdateAverageDirection(const double windDirection) {
		m_sumSin += std::sin(windDirection * M_PI / 180.0);
		m_sumCos += std::cos(windDirection * M_PI / 180.0);
		const double avgAngleRad = std::atan2(m_sumSin, m_sumCos);
		m_aveValue = avgAngleRad * 180.0 / M_PI;
		if (m_aveValue < 0) {
			m_aveValue += 360.0;
		}
	}

private:
	double m_min = std::numeric_limits<double>::infinity();
	double m_max = -std::numeric_limits<double>::infinity();
	double m_accumulatedValue = 0;
	double m_aveValue = 0;
	double m_sumSin = 0;
	double m_sumCos = 0;
	unsigned m_count = 0;
};

class CDisplay: public IObserver<SWeatherInfo>
{
private:
	/* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
		Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
		остается публичным
	*/
	void Update(SWeatherInfo const& data) override
	{
		std::cout << "Current Temp " << data.temperature << std::endl;
		std::cout << "Current Hum " << data.humidity << std::endl;
		std::cout << "Current Pressure " << data.pressure << std::endl;
		std::cout << "----------------" << std::endl;
	}
};

class CStatsDisplay : public IObserver<SWeatherInfo> {
private:
	void Update(SWeatherInfo const &data) override {
		m_calculatedStats["Temperature"].Update(data.temperature);
		m_calculatedStats["Humidity"].Update(data.humidity);
		m_calculatedStats["Pressure"].Update(data.pressure);
		m_calculatedStats["Wind Speed"].Update(data.windSpeed);
		m_calculatedStats["Wind Direction"].Update(data.windDirection);
		m_calculatedStats["Wind Direction"].UpdateAverageDirection(data.windDirection);

		for (auto const &name: m_statNames) {
			PrintStats(m_calculatedStats[name], name);
		}

		std::cout << "----------------" << std::endl;
	}


	static void PrintStats(const CalculatedStat &stat, const std::string &statName) {
		std::cout << statName + " - Max: " << stat.GetMax()
				<< ", Min: " << stat.GetMin()
				<< ", Average: " << stat.GetAverage() << std::endl;
	}

	const std::vector<std::string> m_statNames = {
		"Temperature",
		"Humidity",
		"Pressure",
		"Wind Speed",
		"Wind Direction"
	};

	std::map<std::string, CalculatedStat> m_calculatedStats;
};

class CWeatherData : public CObservable<SWeatherInfo>
{
public:
	// Температура в градусах Цельсия
	double GetTemperature()const
	{
		return m_temperature;
	}
	// Относительная влажность (0...100)
	double GetHumidity()const
	{
		return m_humidity;
	}
	// Атмосферное давление (в мм.рт.ст)
	double GetPressure()const
	{
		return m_pressure;
	}

	double GetWindSpeed()const
	{
		return m_windSpeed;
	}

	double GetWindDirection()const
	{
		return m_windDirection;
	}

	void MeasurementsChanged()
	{
		NotifyObservers();
	}

	void SetMeasurements(double temp, double humidity, double pressure, double windSpeed, double windDirection)
	{
		m_humidity = humidity;
		m_temperature = temp;
		m_pressure = pressure;
		m_windDirection = windDirection;
		m_windSpeed = windSpeed;

		MeasurementsChanged();
	}
protected:
	SWeatherInfo GetChangedData()const override
	{
		SWeatherInfo info;
		info.temperature = GetTemperature();
		info.humidity = GetHumidity();
		info.pressure = GetPressure();
		info.windDirection = GetWindDirection();
		info.windSpeed = GetWindSpeed();
		return info;
	}
private:
	double m_temperature = 0.0;
	double m_humidity = 0.0;	
	double m_pressure = 760.0;
	double m_windDirection = 0;
	double m_windSpeed = 0;
};
