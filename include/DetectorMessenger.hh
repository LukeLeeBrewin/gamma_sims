#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"

class DetectorMessenger {
public:
    static DetectorMessenger* GetInstance();
    void SetThickness(G4double newThickness); // Method to set the variable
    void SetShieldMat(G4String newShieldMat);


    G4double GetThickness() const; // Method to get the variable
    G4String GetShieldMat() const; 

private:
    DetectorMessenger();
    static DetectorMessenger* instance;
    G4double thickness; 
    G4String shield_mat;
};

#endif