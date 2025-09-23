#include "storage.h"

template<typename T>
Storage<T>& Storage<T>::instance() {
    static Storage<T> storage;
    return storage;
}
