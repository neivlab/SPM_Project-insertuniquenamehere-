#pragma once
#include "cLife.h"

class cRgbcell :
    public cLife
{
public:
    cRgbcell(int xpos, int ypos);
    virtual ~cRgbcell() = default;
    virtual void draw();
    virtual void applySimulationChanges();
    int red = 0;
    int green = 0;
    int blue = 0;

    static const std::string  m_LifeName;
    static std::string getLifeName() { return { "rgb" }; }
    // note - each class that derives from cLife should have its own spawn()
    static cLife* spawn(int x, int y, int health);


protected:
    //std::string m_name{ "Blob"};               // name of this life
    virtual NeighbourClassMap countNeighbours(const std::vector<cLife*>& simNeighbours);
    virtual int interactWithNeighbours(const std::vector<cLife*>& simNeighbours, NeighbourClassMap& neighbourMap);
    virtual int addPendingHealthChange(int health);
};

