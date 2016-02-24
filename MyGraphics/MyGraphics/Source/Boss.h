#ifndef BOSS_H
#define BOSS_H

#include <string>
#include <Vector3.h>

using std::string;

class Boss
{
public:

    virtual ~Boss()
    {
    }

    Boss(string name = "BOSS", int health = 100, Vector3 position = Vector3(0, 0, 0));

    string name;
    int health;
    Vector3 position;

};

#endif /*BOSS_H*/