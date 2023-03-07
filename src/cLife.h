#pragma once
#include <string>
#include <vector>
#include "ofMain.h"
#include "iCellQuery.h"

class cLife
{
public:
    cLife(int xCentre, int yCentre);
    virtual ~cLife();   
    cLife(const cLife& other);                      // copy constructor
    virtual cLife& operator=(const cLife& other);    // copy assignment
    virtual void setup();
    virtual void destroy();
    virtual int  simulate(std::array<cLife*, 8>& simNeighbours);
    virtual void draw();
    virtual std::string getName() { return m_name; }
    virtual bool isAlive() { return m_health != 0; }
    virtual int  addHealth(int health);
    virtual int  getHealth() { return m_health;  }
    virtual void updateHealth();
    virtual void setPosition(int x, int y);
    virtual void getPosition(int& x, int& y);
    
    // note - each class that derives from cLife should have its own name and spawn()
    static std::string getLifeName() { return mk_LifeName; }
    static void setupQuery(iCellQuery& query);
    static cLife* spawn(int x, int y);   

protected:
    static const std::string    mk_LifeName;
    static iCellQuery*          msp_query;
    const int   mk_MaxLife{ 1 };
    int         m_health{ 0 };                  // health value. if <=0 it will die.
    int         m_healthChange{ 0 };            // add this amount to health at the end of the update
    std::string m_name{ mk_LifeName };          // name of this life
    int         m_xCentre{ 0 };                 // x/horizontal - centre cell
    int         m_yCentre{ 0 };                 // y/vertical centre of cell
    int         m_drawSize{ 8 };                // size/radius when displayed on screen     
    ofColor     m_color{ ofColor::fireBrick };  // display colour


private:

    //Unused stuff. how far to check for neighbours?
    //  virtual cLife* spawn(int x, int y);
    //  virtual int  getNeighbourThreshold() { return neighbourThreshold; }
    //  const int neighbourThreshold{ 1 };      
};
