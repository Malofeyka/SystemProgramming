#include <iostream>
using namespace std;
#define MIN_TANK_VOLUME 20
#define MAX_TANK_VOLUME 120


class Tank 
{
	const int VOLUME;
	double fuel_level;

	static int check_fuel_volume(int volume) 
	{
		if (volume < MIN_TANK_VOLUME) return MIN_TANK_VOLUME;
		else if (volume > MAX_TANK_VOLUME) return MAX_TANK_VOLUME;
		else return volume;
	}


public:
	const int getVOLUME()const {
		return VOLUME;
	}
	double get_fuel_level()const 
	{
		return fuel_level;
	}
	
	
	void fill(double amount) 
	{
		if (amount < 0) return;
		if (fuel_level + amount < VOLUME) 
		{
			fuel_level += amount;
		}
		else
		{
			fuel_level = VOLUME;
		}
	}

	double give_fuel(double amount)
	{
		if (amount < 0) return fuel_level;
		fuel_level -= amount;
		if (fuel_level < 0)fuel_level = 0;
		return fuel_level;
	}

	Tank(int volume):VOLUME(check_fuel_volume(volume))
	{		
		this->fuel_level = 0;
		cout << "Tank is ready " << this << endl;
	}

	void info()const
	{
		cout << "Tank volume: " << VOLUME << " liters.\n";
		cout << "Fuel level: " << fuel_level << " liters.\n";		
	}
	~Tank()
	{
		cout << "Tank is over" << endl;
	}

};


#define MIN_ENGINE_CONSUMPTION 3
#define MAX_ENGINE_CONSUMPTION 3

class Engine
{
	const double CONSUMPTION;
	double consumption_per_second;
	bool is_started;

public:
	double get_consumprion()const 
	{
		return CONSUMPTION;
	}

	double get_consumption_per_second()const
	{
		return consumption_per_second;
	}

	void start()
	{
		is_started = true;
	}

	void stop()
	{
		is_started = false;
	}

	bool started()const
	{
		return is_started;
	}
	Engine(double consumption) :CONSUMPTION
	(
		consumption< MIN_ENGINE_CONSUMPTION ? MIN_ENGINE_CONSUMPTION :
		consumption > MAX_ENGINE_CONSUMPTION ? MAX_ENGINE_CONSUMPTION :
		consumption
	),
		consumption_per_second(consumption * 3e-5),
		is_started(false)
	{		
		cout << "Engine is ready " << this << endl;		
	}

	~Engine()
	{
		cout << "Engine is over: " << this << endl;
	}

	void info()const
	{
		cout << "Consumption: " << CONSUMPTION << " liters/100km" << endl;
		cout << "Cunsumption per second: " << consumption_per_second << " liters/100km" << endl;
		cout << "Engine is " << (is_started ? "started" : "stopped")<<endl;
	}

};


//#define TANK_CHECK

void main()
{
	setlocale(LC_ALL, "");
#ifdef TANK_CHECK

	Tank tank(120);
	tank.info();
	int fuel;
	do
	{
		cout << "Введите объём: "; cin >> fuel;
		tank.fill(fuel);
		tank.info();
	} while (fuel > 0);
#endif

	Engine engine(10);
	engine.info();


}