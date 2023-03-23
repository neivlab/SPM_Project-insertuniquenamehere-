#pragma once
#include "cLife.h"

class cTracker :
    public cLife
{
public:
    cTracker(int xpos, int ypos);
    virtual ~cTracker() = default;
    virtual void draw();
    virtual void applySimulationChanges();
    cLife* m_nearestBlob;

    static const std::string  m_LifeName;
    static std::string getLifeName() { return { "tracker" }; }
    static cLife* spawn(int x, int y, int health);

protected:
    virtual NeighbourClassMap countNeighbours(const std::vector<cLife*>& simNeighbours);
    virtual int interactWithNeighbours(const std::vector<cLife*>& simNeighbours, NeighbourClassMap& neighbourMap);
    virtual int addPendingHealthChange(int health);
};
