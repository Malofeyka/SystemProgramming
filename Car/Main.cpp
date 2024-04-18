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

};


void main()
{
	setlocale(LC_ALL, "");
	Tank tank(-50);
	tank.info();

}