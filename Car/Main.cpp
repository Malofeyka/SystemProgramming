#include <iostream>
#include <conio.h>
#include <thread>
#include <Windows.h>

using namespace std;


#define Enter 13
#define Escape 27
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

	Tank(int volume) :VOLUME(check_fuel_volume(volume))
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
#define MAX_ENGINE_CONSUMPTION 30

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

	double set_consumption_per_second(int speed)
	{
		if (speed > 200)consumption_per_second = CONSUMPTION * 3e-5 * 100 / 3;
		else if (speed > 140)consumption_per_second = CONSUMPTION * 3e-5 * 25 / 3;
		else if (speed > 100)consumption_per_second = CONSUMPTION * 3e-5 * 20 / 3;
		else if (speed > 60)consumption_per_second = CONSUMPTION * 3e-5 * 14 / 3;
		else if (speed > 0)consumption_per_second = CONSUMPTION * 3e-5 * 20 / 3;
		
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
		cout << "Engine is " << (is_started ? "started" : "stopped") << endl;
	}

};

#define MAX_SPEED_LOW_LIMIT 50
#define MAX_SPEED_HIGH_LIMIT 400

class Car
{
	Engine engine;
	Tank tank;
	bool driver_inside;
	struct //анонимный класс(у структуры нет названия)
	{
		thread panel_thread;
		thread engine_idle_thread;
		thread free_wheeling_thread;
	}threads;
	int speed;
	const int MAX_SPEED;
	const int ACCELERATION;
	struct
	{
		bool reverse = false;
	}transmisson;


public:
	Car(int consumption = 10, int volume = 60, int max_speed = 250) :
		engine(consumption),
		tank(volume),
		driver_inside(false),
		speed(0),
		MAX_SPEED
		(
			max_speed < MAX_SPEED_LOW_LIMIT ? MAX_SPEED_LOW_LIMIT :
			max_speed > MAX_SPEED_HIGH_LIMIT ? MAX_SPEED_HIGH_LIMIT :
			max_speed
		),
		ACCELERATION(MAX_SPEED / 20)
	{
		cout << "Your car is ready to go" << endl;
	}
	~Car()
	{
		cout << "Your car is over" << endl;
	}

	void info()const
	{
		engine.info();
		tank.info();
	}

	void get_in()
	{
		driver_inside = true;
		threads.panel_thread = thread(&Car::panel, this);
	}

	void get_out()
	{
		driver_inside = false;
		if (threads.panel_thread.joinable()) threads.panel_thread.join();
		system("cls");
		cout << "Out of the car" << endl;
	}

	void control()
	{
		char key;
		do
		{
			key = 0;
			if (_kbhit())key = _getch();
			switch (key)
			{
			case Enter: driver_inside ? get_out() : get_in(); break;
			case Escape: speed = 0; stop(); get_out(); break;
			case 'f':
			case 'F':
			{
				if (driver_inside)
				{
					cout << "Get out first!" << endl;
				}
				else
				{
					double fuel;
					cout << "Сколько заправить: "; cin.clear(); cin >> fuel;
					tank.fill(fuel);
				} break;
			}

			case 'I':
			case 'i': engine.started() ? stop() : start(); break;

			case 'W': case 'w': accelerate(); break;
			case 'S': case 's': slow_down(); break;

			case 'R': case 'r': if (speed == 0)transmisson.reverse = !transmisson.reverse; break;
			}			
			if (tank.get_fuel_level() == 0)stop();
			if (speed < 0)speed = 0;
			if (speed == 0 && threads.free_wheeling_thread.joinable()) threads.free_wheeling_thread.join();

		} while (key != 27);
	}

	void free_wheeling()
	{
		while (speed--)
		{
			
			if (speed < 0)speed = 0;
			
			this_thread::sleep_for(1s);
		}
	}

	void accelerate() {
		if (engine.started()) {
			speed += ACCELERATION;
			if (speed > MAX_SPEED) speed = MAX_SPEED;
			if (transmisson.reverse && speed > MAX_SPEED / 7)speed = MAX_SPEED / 7;
			if (!threads.free_wheeling_thread.joinable()) threads.free_wheeling_thread = thread(&Car::free_wheeling, this);
		}
		this_thread::sleep_for(1s);
	}

	void slow_down() {
		speed -= ACCELERATION;
		if (speed < 0)speed = 0;
		if (speed == 0 && threads.free_wheeling_thread.joinable())
			threads.free_wheeling_thread.join();
		this_thread::sleep_for(1s);
	}

	void panel()const
	{
		while (driver_inside)
		{
			system("CLS");
			for (int i = 0; i < speed / 3; i++)cout << "|"; cout << endl;
			cout << "Speed:	" << speed << endl;
			cout << "Fuel level:\t" << tank.get_fuel_level() << " liters.\t";
			if (tank.get_fuel_level() < 5)
			{
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(hConsole, 0xC7);
				cout << "LOW FUEL!";
				SetConsoleTextAttribute(hConsole, 0x07);
			}
			cout << endl;
			cout << "Engine is " << (engine.started() ? "started" : "stopped") << endl;
			cout << "Consumption " << engine.get_consumption_per_second() << " liters/s\n";
			cout << "Revers " << (transmisson.reverse ? "ON" : "OFF") << endl;
			std::this_thread::sleep_for(100ms);

		}
	}

	void start()
	{
		if (tank.get_fuel_level()) {
			engine.start();
			threads.engine_idle_thread = thread(&Car::engine_idle, this);
		}
	}

	void stop() {
		engine.stop();
		if (threads.engine_idle_thread.joinable())threads.engine_idle_thread.join();
	}


	void engine_idle() {
		while (engine.started() && tank.give_fuel(engine.set_consumption_per_second(speed)))this_thread::sleep_for(1s);
	}

};
//#define TANK_CHECK
//#define ENGINE

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
#ifdef ENGINE
	Engine engine(10);
	engine.info();
#endif

	Car bmw(25,85,400);
	bmw.info();

	bmw.control();

}