import numpy as np
import os
from pyne import nucname


class MacroGenerator:
    def __init__(self, isotope_list, shielding_range, activity_range):
        print("Macro Generator")
        self.master_str = "\n"
        self.isotope_list = isotope_list
        self.shielding_range = shielding_range
        self.activity_range = activity_range

    def prelude(self):
        print("Prelude")
        self.master_str += "/vis/disable/\n"
        self.master_str += "/tracking/storeTrajectory 0\n"
        self.master_str += "/gps/particle ion\n\n"
    
    def applyActivity(self):
        print("Activity")

    def applyIsotope(self):
        for i, isotope_name in enumerate(self.isotope_list):
            nuclide = nucname.id(isotope_name)

            atomic_number = nucname.anum(nuclide)
            protons = nucname.znum(nuclide)


            for thickness in self.shielding_range:

                for counts in self.activity_range:
                    self.master_str += f"\n\n#--------------------------------------------------------------\n"
                    self.master_str += f"# {isotope_name}\n"
                    self.master_str += f"/shielding/thickness {np.round(thickness)}\n"
                    self.master_str += f"/gps/ion {protons} {atomic_number}\n"
                    self.master_str += f"/run/beamOn {int(counts)}\n"






        

    def assembleMacro(self):
        self.prelude()
        self.applyIsotope()

        with open("macro.mac", "w") as f:
            f.write(self.master_str)



if __name__ == "__main__":

    activity_array = [1e4]
    shielding_array = np.linspace(1, 50, 3)
    isotope_list = ["Co60", "Cs137"]

    mg = MacroGenerator(isotope_list = isotope_list, shielding_range=shielding_array, activity_range=activity_array)

    mg.assembleMacro()