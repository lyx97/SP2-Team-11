#pragma once

class Singleton
{
public:
	static Singleton* getInstance()
	{
		static Singleton instance;

		return &instance;
	}

	bool single;
	int test;
	double testy;

private:
	Singleton(){}
};

