// Handling Entity objects for OOP game instance code

// Inherited from instance_container.h/.cpp

#include "./entity.h"

Entity::Entity(std::string ClassName, std::string Name, bool IsNPC, double Health, double Speed, Vector3 newPos) : Instance(ClassName, Name), isNPC(isNPC), Health(Health), Speed(Speed), Position(newPos) {
    
};

void Entity::TakeDamage(double Damage) {
    Health = Health - Damage;

    std::cout << "Take damage!" << std::endl;

    if (Health <= 0.0) {
        OnDeath.Fire();
        std::cout << "Entity dead" << std::endl;
    }
}