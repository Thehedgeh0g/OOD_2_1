//
// Created by thehedgeh0g on 02.10.24.
//

#ifndef TEST_H
#define TEST_H

#include "gtest/gtest.h"
#include "WeatherData.h"

class CMockObserver : public IObserver<SWeatherInfo>
{
public:
    explicit CMockObserver(int id) : m_id(id) {}

    void Update(SWeatherInfo const& data) override
    {
        m_lastNotified = true;
        std::cout << "Observer " << m_id << " notified." << std::endl;
        m_notifications.push_back(m_id);
    }

    static std::vector<int> m_notifications;
    bool m_lastNotified = false;
    int m_id;
};

std::vector<int> CMockObserver::m_notifications;

class WeatherStationPriorityTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        CMockObserver::m_notifications.clear();
    }

    CWeatherData weatherData;
    CMockObserver observer1{ 1 };
    CMockObserver observer2{ 2 };
    CMockObserver observer3{ 3 };
};

TEST_F(WeatherStationPriorityTest, NotifyObserversWithPriority)
{
    weatherData.RegisterObserver(observer1, 10);
    weatherData.RegisterObserver(observer2, 5);
    weatherData.RegisterObserver(observer3, 1);

    weatherData.SetMeasurements(3, 0.7, 760);

    ASSERT_EQ(CMockObserver::m_notifications, std::vector<int>({1, 2, 3}));
}

TEST_F(WeatherStationPriorityTest, PreventDuplicateRegistration)
{
    weatherData.RegisterObserver(observer1, 10);
    weatherData.RegisterObserver(observer1, 10);

    weatherData.SetMeasurements(3, 0.7, 760);

    ASSERT_EQ(CMockObserver::m_notifications.size(), 1);
    ASSERT_EQ(CMockObserver::m_notifications[0], 1);
}

TEST_F(WeatherStationPriorityTest, RemoveObserver)
{
    weatherData.RegisterObserver(observer1, 10);
    weatherData.RegisterObserver(observer2, 5);
    weatherData.RegisterObserver(observer3, 1);

    weatherData.RemoveObserver(observer2);

    weatherData.SetMeasurements(3, 0.7, 760);

    ASSERT_EQ(CMockObserver::m_notifications, std::vector<int>({1, 3}));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#endif //TEST_H
