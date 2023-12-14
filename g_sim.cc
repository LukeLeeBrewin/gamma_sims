#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "G4RunManagerFactory.hh"

#include "G4UImanager.hh"
#include "LBE.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include <getopt.h>
#include "Randomize.hh"
#include "DetectorMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = 0;
  ui = new G4UIExecutive(argc, argv);

  //construct the default run manager
  #ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager;
    runManager->SetNumberOfThreads(G4Threading::G4GetNumberOfCores()-2); 
  #else
    //my Verbose output class
    //G4VSteppingVerbose::SetInstance(new SteppingVerbose);
    G4RunManager* runManager = new G4RunManager;
  #endif

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  // Define options
  static struct option longOptions[] = {
      {"file", required_argument, 0, 'b'},
      {"shielding", required_argument, 0, 's'},
      {"shieldmat", required_argument, 0, 'm'},
      {"output", required_argument, 0, 'o'},
      {"interactive", no_argument, 0, 'i'},
      {0, 0, 0, 0} // Last element must be filled with zeros
  };

  int option; 
  bool batch_mode = false;
  G4String fileName;
  while ((option = getopt_long(argc, argv, "b:s:m:o:i", longOptions, NULL)) != -1) {
      switch (option) {
          case 'b': {
              // Batch mode
              std::cout << "\n\n\nArg - File Option: "<< optarg << "\n" << std::endl;
              batch_mode = true;
              fileName = optarg;
              break;
          }
          case 's': {
              // Shielding Thickness
              std::cout << "\nArg - Shielding Thickness: "<< optarg << std::endl;
              G4double thickness = std::stod(optarg);
              DetectorMessenger::GetInstance()->SetThickness(thickness);
              break;
          }
          case 'm': {
              //Shielding Material
              std::cout << "\nArg - Shielding Material: "<< optarg << "\n\n\n" << std::endl;
              G4String material = optarg;  
              DetectorMessenger::GetInstance()->SetShieldMat(material);
              break;
          }

          case 'o': {
              //Shielding Material
              std::cout << "\nArg - Output File Name: "<< optarg << "\n\n\n" << std::endl;  
              DetectorMessenger::GetInstance()->SetOutputFile(optarg);
              break;
          }
          case 'i':
              batch_mode = false; 
              break;
          default: {
              break; 
          }
      }
  }



  // Set mandatory initialization classes
  //
  // Detector construction
  runManager->SetUserInitialization(new DetectorConstruction());

  // Physics list
  G4VModularPhysicsList* physicsList = new LBE;
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);
    
  // User action initialization
  runManager->SetUserInitialization(new ActionInitialization());
  
  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();



  // Must be done here so world can be constructed with correct shielding
  if (batch_mode) {
    G4String command = "/control/execute ";
    std::cout << command+fileName << std::endl;
    UImanager->ApplyCommand(command+fileName);
  }
  else{
    // interactive mode
    std::cout << "\nInteractive Mode\n" << std::endl;
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;

  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....




