// sinetric * sulthnn

#include "script_connection.h"

ScriptConnection::ScriptConnection(void (*callback)()) {
    _connections[sizeof(_connections) + 1] = callback;
};
