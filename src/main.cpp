#include <iostream>

#include "modules/util/vector3.h"
#include "modules/util/script_connection.h"
#include "modules/util/data_container.h"
#include "modules/instances/main/instance_container.h"
#include "modules/instances/entities/entity.h"

void OnDeath() {
	std::cout << "Death signal received" << std::endl;
}

int main() {
	Entity Human("Human", "Human1", true, 100, 10, Vector3(0, 0, 0));

	Vector3 origin(2, 2, 2);

	std::cout << origin.X << origin.Y << origin.Z << std::endl;
	std::cout << "\n" << std::endl;
	std::cout << origin.GetMagnitude() << "\n" << std::endl;

	Vector3 _unitVector = origin.GetUnitVector();

	std::cout << _unitVector.GetMagnitude() << "\n" << std::endl;

	Human.OnDeath.Connect(&OnDeath);

	Human.TakeDamage(50);
	Human.TakeDamage(50);

	std::cout << Human.GetMemoryAddress() << "\n" << std::endl;

	std::cout << "AAAAAA!!!!!" << std::endl;

	return 0;
}
