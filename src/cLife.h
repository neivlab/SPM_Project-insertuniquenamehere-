#pragma once
#include <string>
#include <vector>
#include "ofMain.h"

class cLife
{
public:
    cLife();
    virtual ~cLife() {};
    virtual void setup() {};
    virtual void destroy();
    virtual int  simulate(std::array<cLife*, 8>& simNeighbours);
    virtual void draw();
    virtual cLife* spawn(int x, int y);
    virtual std::string getName() { return m_name; }
    virtual bool isAlive() { return m_health > 0; }
    virtual int  addHealth(int health);
    virtual int  getHealth() { return m_health;  }
    virtual void setPosition(int x, int y);
    virtual void getPosition(int& x, int& y);

    static std::string  getLifeName() { return ms_lifeName; }


protected:
    static std::string ms_lifeName;
    int         m_health{ 0 };                  // health value. if <=0 it will die.
    std::string m_name{ cLife::ms_lifeName };               // name of this life
    int         m_xPos{ 0 };                    // x/horizontal - centre cell
    int         m_yPos{ 0 };                    // y/vertical centre of cell
    int         m_drawSize{ 8 };                // size/radius when displayed on screen     
    ofColor     m_color{ ofColor::fireBrick };  // display colour

private:
    //Unused. how far to check for neighbours?
    //  virtual cLife* spawn(int x, int y);
    //  virtual int  getNeighbourThreshold() { return neighbourThreshold; }
    //  const int neighbourThreshold{ 1 };      
};

