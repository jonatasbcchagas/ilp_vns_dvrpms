 
#!/usr/bin/python
# -*- coding: utf-8 -*-

import os
import multiprocessing

def launcher(instanceID, area, container, number_of_items):
    os.system("./ilp --pickuparea %s --deliveryarea %s --container %s --n %d --outputsolution %s" % 
                (
                    "../../inputs/areas/%sp.tsp" % (area, ),
                    "../../inputs/areas/%sd.tsp" % (area, ),
                    "../../inputs/containers/%s" % (container, ),
                    number_of_items,
                    "../../solutions/ilp/ID%03d_%s_%s.sol" % (instanceID, container.replace(".txt", ""), area)
                )
    )

if __name__ == "__main__":
    
    types_of_instances = { # (container, number of items)
                            "a": ("C2_2x3_2x3.txt", 12),
                            "b": ("C2_2x2_2x4.txt", 12),
                            "c": ("C3_2x2_2x2_2x2.txt", 12),
                            "d": ("C2_2x4_2x4.txt", 16),
                            "e": ("C3_2x2_2x3_2x3.txt", 16),
                            "f": ("C4_2x2_2x2_2x2_2x2.txt", 16),
                            "g": ("C2_2x3_3x4.txt", 18),
                            "h": ("C3_2x3_2x3_2x3.txt", 18),
                            "i": ("C4_2x2_2x2_2x2_2x3.txt", 18),
                            "j": ("C2_2x4_3x4.txt", 20),
                            "k": ("C3_2x3_2x3_2x4.txt", 20),
                            "l": ("C4_2x2_2x2_2x3_2x3.txt", 20),
                            "m": ("C2_3x4_3x4.txt", 24),
                            "n": ("C3_2x4_2x4_2x4.txt", 24),
                            "o": ("C4_2x3_2x3_2x3_2x3.txt", 24),                          
                            "p": ("C2_4x4_4x4.txt", 18),
                            "q": ("C3_3x4_3x4_3x4.txt", 18),
                            "r": ("C4_2x4_2x4_2x4_2x4.txt", 18),
                            "s": ("C2_4x4_4x4.txt", 20),
                            "t": ("C3_3x4_3x4_3x4.txt", 20),
                            "u": ("C4_2x4_2x4_2x4_2x4.txt", 20),
                            "v": ("C2_4x4_4x4.txt", 24),
                            "w": ("C3_3x4_3x4_3x4.txt", 24),
                            "x": ("C4_2x4_2x4_2x4_2x4.txt", 24),
    }

    areas = ["R05", "R06", "R07", "R08", "R09", ]
 
    os.system("make clean")
    os.system("make")

    instanceID = 1
    for type in ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',]:
        container, number_of_items = types_of_instances[type]
        for area in areas:
            launcher(instanceID, area, container, number_of_items)
            instanceID += 1
