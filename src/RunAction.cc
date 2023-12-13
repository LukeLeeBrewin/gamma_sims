#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include <iostream>
#include <ctime>
#include "DetectorMessenger.hh"
#include <string>
#include <cstring>
#include <dirent.h>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
: G4UserRunAction(),
  fEdep(0.)
{ 

  //G4RunManager::GetRunManager()->GeometryHasBeenModified();

  // Register accumulable to the accumulable manager
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fEdep);

  G4AnalysisManager *man = G4AnalysisManager::Instance();
  man->SetNtupleMerging(true);
  man->CreateNtuple("Hits", "Hits");
  man->CreateNtupleIColumn("runID");
  man->CreateNtupleIColumn("fEvent");
  man->CreateNtupleDColumn("edep");
  man->CreateNtupleDColumn("x");
  man->CreateNtupleDColumn("y");
  man->CreateNtupleDColumn("z");
  man->FinishNtuple(0);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* run)
{  
    G4int runID = run->GetRunID();
    G4int thr = G4Threading::G4GetThreadId();

    std::cout << "\n\n\nBegin of Run Action Called\nRunID: " << runID << "\nThread ID: " << thr << "\n\n\n" << std::endl;

    std::stringstream strRunID;
    strRunID << runID;
  
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->OpenFile("output"+strRunID.str()+".root");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
  G4int runID = run->GetRunID();
  G4int thr = G4Threading::G4GetThreadId();
  std::cout << "\n\n\nEnd of Run Action Called\nRunID: " << runID << "\nThread ID: " << thr << "\n\n\n" << std::endl;

  G4AnalysisManager *man = G4AnalysisManager::Instance();
  man->Write();
  man->CloseFile();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::AddEdep(G4double edep)
{
  fEdep  += edep;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

