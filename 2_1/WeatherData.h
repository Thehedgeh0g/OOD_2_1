#pragma once
#include <iostream>
#include <algorithm>
#include <limits>
#include <map>
#include <utility>
#include "Observer.h"

struct SWeatherInfo {
    double temperature = 0;
    double humidity = 0;
    double pressure = 0;
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
    }

    [[nodiscard]] double GetMin() const {
        return m_min;
    }

    [[nodiscard]] double GetMax() const {
        return m_max;
    }

    [[nodiscard]] double GetAverage() const {
        return m_accumulatedValue / m_count;
    }

private:
    double m_min = std::numeric_limits<double>::infinity();
    double m_max = -std::numeric_limits<double>::infinity();
    double m_accumulatedValue = 0;
    unsigned m_count = 0;
};

class CStatsDisplay : public IObserver<SWeatherInfo> {
private:
    void Update(SWeatherInfo const &data) override {
        m_calculatedStats["Temperature"].Update(data.temperature);
        m_calculatedStats["Humidity"].Update(data.humidity);
        m_calculatedStats["Pressure"].Update(data.pressure);

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
        "Pressure"
    };

    CalculatedStat m_temperatureStats;
    CalculatedStat m_humidityStats;
    CalculatedStat m_pressureStats;
    std::map<std::string, CalculatedStat> m_calculatedStats;
};

class CDisplay : public IObserver<SWeatherInfo> {
private:
    /* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
        Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
        остается публичным
    */
    void Update(SWeatherInfo const &data) override {
        std::cout << "Current Temp " << data.temperature << std::endl;
        std::cout << "Current Hum " << data.humidity << std::endl;
        std::cout << "Current Pressure " << data.pressure << std::endl;
        std::cout << "----------------" << std::endl;
    }
};

class CWeatherData : public CObservable<SWeatherInfo> {
public:
    // Температура в градусах Цельсия
    double GetTemperature() const {
        return m_temperature;
    }

    // Относительная влажность (0...100)
    double GetHumidity() const {
        return m_humidity;
    }

    // Атмосферное давление (в мм.рт.ст)
    double GetPressure() const {
        return m_pressure;
    }

    void MeasurementsChanged() {
        NotifyObservers();
    }

    void SetMeasurements(double temp, double humidity, double pressure) {
        m_humidity = humidity;
        m_temperature = temp;
        m_pressure = pressure;

        MeasurementsChanged();
    }

protected:
    SWeatherInfo GetChangedData() const override {
        SWeatherInfo info;
        info.temperature = GetTemperature();
        info.humidity = GetHumidity();
        info.pressure = GetPressure();
        return info;
    }

private:
    double m_temperature = 0.0;
    double m_humidity = 0.0;
    double m_pressure = 760.0;
};
