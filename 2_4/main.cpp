#include "WeatherData.h"

int main()
{
	CWeatherData weatherStationIn("Inside");
	CWeatherData weatherStationOut("Outside");

	CDisplay display;
	CStatsDisplay statsDisplay;

	// Регистрируем наблюдателей для обеих станций
	weatherStationIn.RegisterObserver(display);
	weatherStationOut.RegisterObserver(display);

	weatherStationIn.RegisterObserver(statsDisplay);
	weatherStationOut.RegisterObserver(statsDisplay);

	// Обновляем данные внутренней станции
	weatherStationIn.SetMeasurements(22.5, 0.55, 765);
	weatherStationOut.SetMeasurements(15.0, 0.80, 755);

	return 0;
}