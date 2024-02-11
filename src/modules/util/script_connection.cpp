// sinetric * sulthnn

#include "script_connection.h"

void ScriptConnection::Connect(void (*callback)()) {
    _connections.push_back(callback);
};

void ScriptConnection::Fire() {
    for (int i = 0; i < _connections.size(); i++) {
        _connections[i]();
    }
};
