#include <iostream>
#include <vector>
#include <functional>

// sinetric

class ScriptConnection{
    public:
       // void (*_connections[5])();

        void Connect(void (*callback)());

        template <class T> void Fire(std::vector <T>);
    private:
        std::vector <void (*)()> _connections;
};
