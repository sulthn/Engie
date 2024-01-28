#include <iostream>

// sinetric

class ScriptConnection{
    public:
        void (*_connections[5])();

        ScriptConnection(void (*callback)()){};
};