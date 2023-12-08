#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "Detector.hh"
#include "G4GenericMessenger.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    G4LogicalVolume* detectorLog;
    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

  private:

    void DefineMaterials();
    
    virtual void ConstructSDandField();
    G4GenericMessenger *fmessenger;
    G4double sZ; 

    G4Box *solidWorld, *solidEnv, *solidShield;
    G4Tubs *detector;

    G4LogicalVolume *logicShield, *logicEnv, *logicWorld; 
    G4VPhysicalVolume *physWorld, *detectorPhys, *shieldPhys, *envPhys;  

    G4Material *env_mat, *world_mat, *shielding_mat, *det_mat; 

  protected:
    G4LogicalVolume*  fScoringVolume;
    

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

