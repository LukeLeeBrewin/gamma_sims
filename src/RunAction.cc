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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
: G4UserRunAction(),
  fEdep(0.), 
  filename("_llb_gammasim_run_")
{ 
  // Register accumulable to the accumulable manager
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fEdep);



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run*)
{ 
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  
  // reset accumulables to their initial values
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();



  G4int runID = G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID();
  std::string runIDstr = std::to_string(runID);

  // Get time of first run 
  std::time_t currentTime = std::time(nullptr);
  std::tm* localTime = std::localtime(&currentTime);


  // Extract components
  if (runID == 0){
    
    std::string day = std::to_string(localTime->tm_mday);
    std::string month = std::to_string(localTime->tm_mon + 1);  // Months are 0-based
    std::string year = std::to_string(localTime->tm_year + 1900);  // Years are since 1900
    std::string hours = std::to_string(localTime->tm_hour);
    std::string minutes = std::to_string(localTime->tm_min);
    std::string seconds = std::to_string(localTime->tm_sec);


    filename.prepend(year.c_str());
    filename.prepend(month.c_str());
    filename.prepend(day.c_str());
    filename.prepend("_");
    filename.prepend(hours.c_str());
    filename.prepend(minutes.c_str());
    filename.prepend(seconds.c_str());
  }
  
  filename.append(runIDstr.c_str()); 


  // Create ROOT file
  G4cout << "\n\n\n Starting Run: " << runID << "\n\n\n" << G4endl;
  G4AnalysisManager *man = G4AnalysisManager::Instance();

  man->OpenFile(filename.data());
  man->CreateNtuple("Hits", "Hits");
  man->CreateNtupleIColumn("runID");
  man->CreateNtupleIColumn("fEvent");
  man->CreateNtupleDColumn("edep");
  man->CreateNtupleDColumn("shielding");
  man->CreateNtupleSColumn("shield_mat");

  man->FinishNtuple(0);

  filename = filename.substr(0, filename.size() - runIDstr.size());

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // Merge accumulables 
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  // Total energy deposit in a run
  //
  G4double edep  = fEdep.GetValue();
  

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const PrimaryGeneratorAction* generatorAction
   = static_cast<const PrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  if (generatorAction)
  {
    const G4GeneralParticleSource* GeneralParticleSource = generatorAction->GetGeneralParticleSource();
    runCondition += GeneralParticleSource->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    G4double particleEnergy = GeneralParticleSource->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy,"Energy");
  }
        
  // Print
  //  
  if (IsMaster()) {
    G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------";
  }
  else {
    G4cout
     << G4endl
     << "--------------------End of Local Run------------------------";
  }
  

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

