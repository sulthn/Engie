#pragma once

#ifndef SCRIPTCONNECTION_H
#define SCRIPTCONNECTION_H

#include <iostream>
#include <vector>
#include <functional>

// sinetric

class ScriptConnection{
    public:
       // void (*_connections[5])();

        void Connect(void (*callback)());

        void Fire();
    private:
        std::vector <void (*)()> _connections;
};

#endif