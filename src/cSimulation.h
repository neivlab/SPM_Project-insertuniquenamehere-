#pragma once
#include <vector>
#include "cLife.h"

class cSimulation
{
public:
    cSimulation();
    ~cSimulation() = default;

    void simulate(std::vector<cLife*>& vLife);
};

