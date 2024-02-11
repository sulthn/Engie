// Handling Entity objects for OOP game instance code

// Inherited from instance_container.h/.cpp

#include "../main/instance_container.h"
#include "../../util/script_connection.h"
#include "../../util/vector3.h"

class Entity : public Instance {
    public:
        bool isNPC;
        std::string State;
        double Health;
        double Speed;

        Vector3 Position;

        ScriptConnection OnDeath;

        Entity(std::string ClassName, std::string Name, bool isNPC, double Health, double Speed, Vector3 newPos);

        void SetSpeed(double Speed);

        bool IsDead();

        void TakeDamage(double Damage);

        void SetState(std::string State);
    private:
};