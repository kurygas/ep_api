#pragma once

#include <Wt/Dbo/ptr.h>
#include <Wt/Dbo/collection.h>

template<typename T>
using Ptr = Wt::Dbo::ptr<T>;

template<typename T>
using List = Wt::Dbo::collection<Ptr<T>>;