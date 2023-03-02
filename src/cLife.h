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
    virtual std::string getName() { return m_name; }
    virtual bool isAlive() { return m_health > 0; }
    virtual int  addHealth(int health);
    virtual int  getHealth() { return m_health;  }
    virtual void setPosition(int x, int y);
    virtual void getPosition(int& x, int& y);
    
    // note - each class that derives from cLife should have its own name and spawn()
    static std::string getLifeName() { return mk_LifeName; }
    static cLife* spawn(int x, int y);   

protected:
    // this constructor is protected -  only for internal use by the spawn() function or similar.
    cLife::cLife(int x, int y);

    static const std::string  mk_LifeName;
    static const int  mk_MaxLife;
    int         m_health{ 0 };                  // health value. if <=0 it will die.
    std::string m_name{ mk_LifeName };           // name of this life
    int         m_xPos{ 0 };                    // x/horizontal - centre cell
    int         m_yPos{ 0 };                    // y/vertical centre of cell
    int         m_drawSize{ 8 };                // size/radius when displayed on screen     
    ofColor     m_color{ ofColor::fireBrick };  // display colour

private:

    //Unused stuff. how far to check for neighbours?
    //  virtual cLife* spawn(int x, int y);
    //  virtual int  getNeighbourThreshold() { return neighbourThreshold; }
    //  const int neighbourThreshold{ 1 };      
};

template<class T, class Compare>
constexpr const T& clamp(const T& v, const T& lo, const T& hi, Compare comp)
{
    return comp(v, lo) ? lo : comp(hi, v) ? hi : v;
}