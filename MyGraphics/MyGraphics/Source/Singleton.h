#pragma once

class Singleton
{
public:
	static Singleton* getInstance()
	{
		static Singleton instance;

		return &instance;
	}
	bool pause;
	bool buttonText;
private:
	Singleton(){}
};

