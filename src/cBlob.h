#pragma once
#include "cLife.h"

class cBlob :
    public cLife
{
public:
    cBlob();
    virtual ~cBlob() = default;
    virtual cLife* spawn(int x, int y) override;     // note - each class that derives from cLife must override spawn()
    virtual void draw();

protected:
    std::string m_name{ "Blob"};               // name of this life

};

