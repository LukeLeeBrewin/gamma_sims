#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "Detector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ 
  sZ = 0.3*cm;
  fmessenger = new G4GenericMessenger(this, "/shielding/", "Shielding Thickness");
  fmessenger->DeclareProperty("thickness", sZ, "Width of shielding material");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Envelope parameters
  //
  G4double env_sizeXY = 1*m, env_sizeZ = 1*m;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_WATER");
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     
  //     
  // Envelope
  //  
  G4Box* solidEnv =    
    new G4Box("Envelope",                    //its name
        0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ); //its size
      
  G4LogicalVolume* logicEnv =                         
    new G4LogicalVolume(solidEnv,            //its solid
                        env_mat,             //its material
                        "Envelope");         //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicEnv,                //its logical volume
                    "Envelope",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
 


  //     
  // Shielding
  //  

  G4Material* shielding_mat = nist->FindOrBuildMaterial("G4_Al");
  G4ThreeVector shielding_pos = G4ThreeVector(0, 0, 0);
  shielding_pos.setZ(-sZ*cm/2);



  G4Box* solidShield =    
    new G4Box("Shielding",                    //its name
        20*cm, 20*cm, sZ*cm/2); //its size
      
  G4LogicalVolume* logicShield =                         
    new G4LogicalVolume(solidShield,         //its solid
                        shielding_mat,       //its material
                        "Shielding");        //its name
               
  new G4PVPlacement(0,                       //no rotation
                    shielding_pos,         //at (0,0,0)
                    logicShield,             //its logical volume
                    "Shielding",             //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
 


  //     
  // Detector
  //
  G4Material* det_mat = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");



  G4double innerRadius = 0.*cm;
  G4double outerRadius = 10.*cm;
  G4double hz = 10.*cm;
  G4double startAngle = 0.*deg;
  G4double spanningAngle = 360.*deg;

  G4ThreeVector det_pos = G4ThreeVector(0, 0, 0);
  det_pos.setZ(-((sZ*cm) + hz)); // For some reason Threevectors don't update properly unless you do this, they are const.


  G4Tubs* detector
    = new G4Tubs("detector",
                innerRadius,
                outerRadius,
                hz,
                startAngle,
                spanningAngle);


  detectorLog
    = new G4LogicalVolume(detector, 
                          det_mat,
                         "detectorLog");

  // G4VPhysicalVolume* detectorPhys
    new G4PVPlacement(0,                       // no rotation
                      det_pos,                 // translation position
                      detectorLog,             // its logical volume
                      "detectorPhys",          // its name
                      logicEnv,                // its mother (logical) volume
                      false,                   // no boolean operations
                      0);                      // its copy number

                
  // Set Shape2 as scoring volume
  //
  fScoringVolume = detectorLog;

  //
  //always return the physical World
  //
  return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
  SensitiveDetector *sensDet = new SensitiveDetector("SensitiveDetector");
  detectorLog->SetSensitiveDetector(sensDet);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
