#pragma once

#ifndef INSTANCECONTAINER_H
#define INSTANCECONTAINER_H

#include "../../util/data_container.h"

// Instance class

class Instance {
    public:
        std::string ClassName;
        std::string Name;

        Instance(std::string className, std::string name);

        Instance* GetMemoryAddress();

        void Destroy();
    private:
        Instance* memory_address;
};

// Instance DataContainer

class InstanceContainer : public DataContainerClass<Instance*> {
    public:
        using DataContainerClass<Instance*>::FillContainer;

        void FillContainer(Instance* value) override;
        std::vector<Instance*> GetContents() override;
        
        int GetIndexOfInstanceId(Instance* InstanceId);
};

extern InstanceContainer instanceContainer;

#endif