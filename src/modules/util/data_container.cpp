#include "./data_container.h"

template <typename T>
void DataContainerClass<T>::FillContainer(T value){
    DataContainer.push_back(value);
};

template <typename T>
std::vector<T> DataContainerClass<T>::GetContents(){
    return DataContainer;
};