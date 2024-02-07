#pragma once

#ifndef DATACONTAINER_H
#define DATACONTAINER_H

#include <iostream>
#include <vector>
#include <string>

template <typename T> class DataContainerClass {
    protected:
        std::vector<T> DataContainer;
        void FillContainer(T value);
        std::vector<T> GetContents();
};

#endif