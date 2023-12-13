#include "DetectorMessenger.hh"
#include "G4ThreeVector.hh"

DetectorMessenger* DetectorMessenger::instance = nullptr;

DetectorMessenger* DetectorMessenger::GetInstance() {
    if (!instance) {
        instance = new DetectorMessenger();
    }
    return instance;
}

DetectorMessenger::DetectorMessenger() {
    // Initialize your variable here
    thickness = 0; // Initializing with default value
    shield_mat = "G4_Al";
    position = G4ThreeVector(0., 0., 0.);
}

void DetectorMessenger::SetThickness(G4double newThickness) {
    thickness = newThickness;
}

G4double DetectorMessenger::GetThickness() const {
    return thickness;
}

void DetectorMessenger::SetShieldMat(G4String newShieldMat) {
    shield_mat = newShieldMat;
}

G4String DetectorMessenger::GetShieldMat() const {
    return shield_mat;
}

void DetectorMessenger::SetPosition(G4ThreeVector newPosition){
    position = newPosition;
}

G4ThreeVector DetectorMessenger::GetPosition() const {
    return position; 
}