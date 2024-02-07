#pragma once

#ifndef DATACONTAINER_H
#define DATACONTAINER_H

#include <iostream>
#include <vector>
#include <string>

template <typename T> class DataContainerClass {
    protected:
        std::vector<T> DataContainer;
    //public:
        virtual void FillContainer(T value);
        virtual std::vector<T> GetContents();
};

#endif