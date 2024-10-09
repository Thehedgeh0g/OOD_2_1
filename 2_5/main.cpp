#include "WeatherData.h"

int main()
{
	CWeatherData wd("inside");
	CWeatherData wdPro("Outside");

	CDisplay display;
	wd.RegisterObserver(display);
	wdPro.RegisterObserver(display);
	display.AddProStationNameIntoList("Outside");

	CStatsDisplay statsDisplay;
	wd.RegisterObserver(statsDisplay);
	wdPro.RegisterObserver(statsDisplay);
	statsDisplay.AddProStationNameIntoList("Outside");
	wd.SetMeasurements(20, 0.3, 760);
	wdPro.SetMeasurements(3, 0.7, 760, 10, 0);
	wdPro.SetMeasurements(4, 0.8, 761, 5, 180);

	wd.RemoveObserver(statsDisplay);

	wdPro.SetMeasurements(10, 0.8, 761, 10, 0);
	wdPro.SetMeasurements(-10, 0.8, 761, 10, 0);
	return 0;
}