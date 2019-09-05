#ifndef RAILGRAPHNODESELECTER_H
#define RAILGRAPHNODESELECTER_H

#include "Map/Graph/RailGraphIter.h"

class RailGraphNodeSelecter
{
public:
   virtual bool isSatisfy(RailGraphIter &) = 0;
};

#endif // RAILGRAPHNODESELECTER_H