 
#!/usr/bin/python
# -*- coding: utf-8 -*-

import os
import multiprocessing

random_seeds = [ 269070,  99470, 126489, 644764, 547617, 642580,  73456, 462018, 858990, 756112, 
                 701531, 342080, 613485, 131654, 886148, 909040, 146518, 782904,   3075, 974703, 
                 170425, 531298, 253045, 488197, 394197, 519912, 606939, 480271, 117561, 900952, 
                 968235, 345118, 750253, 420440, 761205, 130467, 928803, 768798, 640300, 871462, 
                 639622,  90614, 187822, 594363, 193911, 846042, 680779, 344008, 759862, 661168, 
                 223420, 959508,  62985, 349296, 910428, 964420, 422964, 384194, 985214,  57575, 
                 639619,  90505, 435236, 465842, 102567, 189997, 741017, 611828, 699223, 335142, 
                  52119,  49256, 324523, 348215, 651525, 517999, 830566, 958538, 880422, 390645, 
                 148265, 807740, 934464, 524847, 408760, 668587, 257030, 751580,  90477, 594476, 
                 571216, 306614, 308010, 661191, 890429, 425031,  69108, 435783,  17725, 335928, ]

def launcher(instanceID, area, container, number_of_items, number_of_iterations_without_improvement, run):
    os.system("./vns --pickuparea %s --deliveryarea %s --container %s --n %d --iter %d --seed %d --outputsolution %s" % 
                (
                    "../../inputs/areas/%sp.tsp" % (area, ),
                    "../../inputs/areas/%sd.tsp" % (area, ),
                    "../../inputs/containers/%s" % (container, ),
                    number_of_items,
                    number_of_iterations_without_improvement,
                    random_seeds[run-1],
                    "../../solutions/vns/ID%03d_%s_%s_%02d.sol" % (instanceID, container.replace(".txt", ""), area, run)
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
 
    runs = list(range(1, 11))

    os.system("make clean")
    os.system("make")

    pool = multiprocessing.Pool(processes=max(1, multiprocessing.cpu_count() - 2))

    instanceID = 1
    for type in ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',]:
        container, number_of_items = types_of_instances[type]
        for area in areas:
            for run in runs:
                number_of_iterations_without_improvement = 7 * number_of_items
                pool.apply_async(launcher, args=(instanceID, area, container, number_of_items, number_of_iterations_without_improvement, run))
            instanceID += 1

    pool.close()
    pool.join()
