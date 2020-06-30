#pragma once

#include "Map/Graph/RailGraphIter.h"

class RailGraphNodeSelecter
{
public:
   virtual bool isSatisfy(RailGraphIter &) = 0;
};