#pragma once
#include "cLife.h"

class cBlob :
    public cLife
{
public:
    cBlob(int xpos, int ypos);
    virtual ~cBlob() = default;
    virtual void draw();   
    virtual void applySimulationChanges();

    static const std::string  m_LifeName;
    static std::string getLifeName() { return { "blob" }; }
    // note - each class that derives from cLife should have its own spawn()
    static cLife* spawn(int x, int y, int health); 


protected:
    //std::string m_name{ "Blob"};               // name of this life
    virtual NeighbourClassMap countNeighbours(const std::vector<cLife*>& simNeighbours);
    virtual int interactWithNeighbours(const std::vector<cLife*>& simNeighbours, NeighbourClassMap& neighbourMap);
    virtual int addPendingHealthChange(int health);
};

