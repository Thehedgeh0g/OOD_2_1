//
// Created by thehedgeh0g on 02.10.24.
//

#include <gtest/gtest.h>
#include "WeatherData.h"


class CSelfRemovingObserver : public IObserver<SWeatherInfo>
{
public:
    CSelfRemovingObserver(CObservable<SWeatherInfo> & weatherData)
        : m_weatherData(weatherData)
    {
    }

    void Update(SWeatherInfo const& data) override
    {
        m_weatherData.RemoveObserver(*this);
        std::cout << "CSelfRemovingObserver Notifed" << std::endl;
    }

private:
    CObservable<SWeatherInfo> & m_weatherData;
};

class WeatherStationTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        weatherData.RegisterObserver(display);
        weatherData.RegisterObserver(selfRemovingObserver);
    }

    CWeatherData weatherData;
    CDisplay display;
    CSelfRemovingObserver selfRemovingObserver{ weatherData };
};

TEST_F(WeatherStationTest, SelfRemovingObserverTest)
{
    ::testing::internal::CaptureStdout();
    weatherData.SetMeasurements(3, 0.7, 760);
    std::string output = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "Current Temp 3\nCurrent Hum 0.7\nCurrent Pressure 760\n----------------\nCSelfRemovingObserver Notifed\n");

    ::testing::internal::CaptureStdout();
    weatherData.SetMeasurements(4, 0.8, 761);
    output = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "Current Temp 4\nCurrent Hum 0.8\nCurrent Pressure 761\n----------------\n");
}


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}