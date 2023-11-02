#include "Detector.hh"


SensitiveDetector::SensitiveDetector(G4String name) : 
G4VSensitiveDetector(name)
{}

SensitiveDetector::~SensitiveDetector()
{}

G4bool SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *R0hist)
{
    G4Track *track = aStep->GetTrack();
    G4double edepStep = aStep->GetTotalEnergyDeposit();

    // G4cout << "\n\n\n Process Hits Called \n\n\n" << G4endl;


return true;
}

