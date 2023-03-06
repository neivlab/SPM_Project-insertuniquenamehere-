#pragma once
#include "cLife.h"

class cBlob :
    public cLife
{
public:
    cBlob(int xpos, int ypos);
    virtual ~cBlob() = default;
    virtual void draw();
    
    static const std::string  m_LifeName;
    static cLife* spawn(int x, int y);     // note - each class that derives from cLife should have its own spawn()


protected:
    std::string m_name{ "Blob"};               // name of this life

};

