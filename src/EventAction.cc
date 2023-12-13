#include "EventAction.hh"
#include "RunAction.hh"
#include "g4root.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fEdep(0.)
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{    
  fEdep = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event*)
{   
  // accumulate statistics in run action
  fRunAction->AddEdep(fEdep);

  G4double thickness = DetectorMessenger::GetInstance()->GetThickness();
  G4String shield_mat = DetectorMessenger::GetInstance()->GetShieldMat();
  G4ThreeVector pos = DetectorMessenger::GetInstance()->GetPosition(); 
  G4int runID = G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID();
  G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
  

  G4AnalysisManager *man = G4AnalysisManager::Instance();

  if (fEdep > 0){

    man->FillNtupleIColumn(0, runID);
    man->FillNtupleIColumn(1, evt);
    man->FillNtupleDColumn(2, fEdep);
    man->FillNtupleDColumn(3, pos[0]);
    man->FillNtupleDColumn(4, pos[1]);
    man->FillNtupleDColumn(5, pos[2]);
    man->AddNtupleRow(0);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


