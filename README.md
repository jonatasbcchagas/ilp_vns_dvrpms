# An integer linear programming formulation and a variable neighborhood search heuristic algorithm for the double vehicle routing problem with multiple stacks

This repository contains a reimplementation of the solution approaches described in the paper ["A variable neighborhood search heuristic algorithm for the double vehicle routing problem with multiple stacks"](https://onlinelibrary.wiley.com/doi/full/10.1111/itor.12623) by Jonatas B. C. Chagas, Ulisses E. F. Silveira, André G. Santos, and Marcone J. F. Souza. The paper presents an Integer Linear Programming (ILP) formulation and a heuristic algorithm based on Variable Neighborhood Search (VNS) metaheuristic for solving the Double Vehicle Routing Problem with Multiple Stacks (DVRPMS).

### Compiling the code

Before running our solution approaches, it is needed to compile their code. To this end, just run the following command in both folders (src/vns and src/ilp):

```console
$ make
```

### Usage ILP:

```console
$ ./ilp [arguments]

arguments:
           --pickuparea <pickup_area_file_name>
           --deliveryarea <delivery_area_file_name>
           --container <container_file_name>
           --n <number_of_items>
           --outputsolution <solution_file_name> 
```

### Usage VNS:

```console
$ ./vns [arguments]

arguments:
           --pickuparea <pickup_area_file_name>
           --deliveryarea <delivery_area_file_name>
           --container <container_file_name>
           --n <number_of_items>
           --iter <number_of_iter_without_improvement>
           --seed <seed_for_random_number>
           --outputsolution <solution_file_name> 
```
We provide a python script (see "src/ilp/run_ilp_experiments.py" and "src/vns/run_vns_experiments.py") for running each solution approach on instances described in the paper. As this repository contains a reimplementation of the approaches proposed by [Chagas et al. (2020)](https://onlinelibrary.wiley.com/doi/full/10.1111/itor.12623), there might be slight variations between the results computed (see folder "solutions") from the python scripts and those reported in the aforementioned paper.
