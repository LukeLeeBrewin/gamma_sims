import numpy as np
import os
from pyne import nucname
import pandas as pd


class MacroGenerator:
    def __init__(self, isotope_list, shielding_range, activity_range, distance_to_source, source_speed, unit = "cm"):
        print("\nMacro Generator\n")
        self.master_str = "\n"
        self.isotope_list = []
        self.shielding_range = []
        self.activity_range = []
        self.x_step = []
        self.y_step = []
        self.z_step = []
        self.unit = unit

        # Calculate source positions
        self.z = distance_to_source
        self.y = 0
        self.x = np.linspace(-5, 5, 5) # Test 



        # Generate combinations
        for i, isotope_name in enumerate(isotope_list):
            for thickness in shielding_range:
                for counts in activity_range:
                    for x in self.x:
                        for z in self.z:
                            self.isotope_list.append(isotope_name)
                            self.activity_range.append(counts)
                            self.shielding_range.append(thickness)
                            self.x_step.append(np.round(x, 2))
                            self.y_step.append(np.round(self.y, 2))
                            self.z_step.append(np.round(z, 2))

        print("----------------------------------------------------")
        print(f"Isotope List: {len(isotope_list)}")
        print(f"Activity List: {len(activity_range)}")
        print(f"Shielding Range List: {len(shielding_range)}")
        print(f"X Positions: {len(self.x_step)}")
        print(f"Z Positions: {len(self.z_step)}")
        print(f"Total Combinations: {len(self.isotope_list)}")
        print("----------------------------------------------------\n")

        df_dict = {
            "Isotope":self.isotope_list, 
            "Activity":self.activity_range,
            "Shielding":self.shielding_range
        }

        self.df = pd.DataFrame(df_dict)
        self.df.index.name = "runID"
        print("\nHead\n", self.df.head())
        print("\nTail\n", self.df.tail())

        self.df.to_csv("runDF.csv")


    def prelude(self):
        # Setup at top of macro file
        self.master_str += "/vis/disable\n"
        self.master_str += "/tracking/storeTrajectory 0\n"
        self.master_str += "/gps/particle ion\n\n"
    

    def applyRuns(self):
        for i in range(0, len(self.isotope_list)):
            nuclide = nucname.id(self.isotope_list[i])

            atomic_number = nucname.anum(nuclide)
            protons = nucname.znum(nuclide)

            self.master_str += f"\n\n#--------------------------------------------------------------\n"
            self.master_str += f"# {self.isotope_list[i]}\n"
            # self.master_str += f"/run/initialize\n"
            self.master_str += f"/gps/ion {protons} {atomic_number}\n"
            self.master_str += f"/gps/position {self.x_step[i]} {self.y_step[i]} {self.z_step[i]} {self.unit}\n"

            self.master_str += f"/run/beamOn {int(self.activity_range[i])}\n"

    def assembleMacro(self):
        self.prelude()
        self.applyRuns()

        with open("./isotope.mac", "w") as f:
            f.write(self.master_str)



if __name__ == "__main__":

    activity_array = [1e3]
    shielding_array = [0]
    isotope_list = ["Co60", "Cs137", "Eu152"]
    distance_to_source = [1]
    source_speed = []




    mg = MacroGenerator(isotope_list = isotope_list, shielding_range=shielding_array, activity_range=activity_array, distance_to_source=distance_to_source, source_speed=[1])

    mg.assembleMacro()

