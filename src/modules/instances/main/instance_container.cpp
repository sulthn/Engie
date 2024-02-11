#include "./instance_container.h"

InstanceContainer instanceContainer;

// Instance class

Instance::Instance(std::string ClassName, std::string Name): memory_address(reinterpret_cast<Instance*>(this)) {
    std::cout << "Storing " << memory_address << " into InstanceContainer" << std::endl;
    instanceContainer.FillContainer(memory_address); // store the memory address of the instance into the global instance list
};

Instance* Instance::GetMemoryAddress() {
    return memory_address;
}

void Instance::Destroy() {
    delete memory_address;
}

// InstanceContainer class

void InstanceContainer::FillContainer(Instance* value) {
    this->DataContainer.push_back(value);
}

std::vector<Instance*> InstanceContainer::GetContents() {
    return this->DataContainer;
}

int InstanceContainer::GetIndexOfInstanceId(Instance* InstanceId){
    auto it = std::find(this->DataContainer.begin(), this->DataContainer.end(), InstanceId);

    if (it != this->DataContainer.end()) {
        // Calculate the index using std::distance
        std::size_t index = std::distance(this->DataContainer.begin(), it);
        std::cout << "The vector contains the value " << InstanceId << " at index " << index << std::endl;

       return index;
    } else {
        std::cout << "The vector does not contain the value " << InstanceId << std::endl;

        return 0;
    }
}