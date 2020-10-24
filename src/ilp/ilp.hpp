#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <ilcplex/ilocplex.h>

#include "../vns/data.hpp"

#define PICKUP 0
#define DELIVERY 1

inline void solveILPModel(const std::string& outputFileName) {
    
    using namespace std::chrono;
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    const Data& data = Data::getInstance();

    IloEnv env;
    IloModel model(env);
    IloExpr obj(env);
    IloConstraintArray constraints(env);
    
    char name[1000];

    IloArray < IloArray < IloArray < IloNumVarArray > > > x = IloArray < IloArray < IloArray < IloNumVarArray > > > (env, data.numItems+1);
    for(int i = 0; i <= data.numItems; ++i) {
        x[i] = IloArray < IloArray < IloNumVarArray > > (env, data.numItems+1);
        for(int j = 0; j <= data.numItems; ++j) {
            x[i][j] = IloArray < IloNumVarArray > (env, data.numVehicles); // PICKUP and DELIVERY
            for(int k = 0; k < data.numVehicles; ++k) {
                x[i][j][k] = IloNumVarArray(env, 2, 0, 1, ILOINT); // PICKUP and DELIVERY
                for(int T = PICKUP; T <= DELIVERY; ++T) {
                    sprintf(name, "x_%02d_%02d_%02d_%c", i, j, k, (T == PICKUP ? 'P' : 'D'));
                    x[i][j][k][T].setName(name);
                }
            }
        }
    }

    IloArray < IloArray < IloNumVarArray > > y = IloArray < IloArray < IloNumVarArray > > (env, data.numItems+1);
    for(int i = 0; i <= data.numItems; ++i) {
        y[i] = IloArray < IloNumVarArray > (env, data.numItems+1);
        for(int j = 0; j <= data.numItems; ++j) {
            y[i][j] = IloNumVarArray(env, 2, 0, 1, ILOINT); // PICKUP and DELIVERY
            for(int T = PICKUP; T <= DELIVERY; ++T) {
                sprintf(name, "y_%02d_%02d_%c", i, j, (T == PICKUP ? 'P' : 'D'));
                y[i][j][T].setName(name);
            }
        }
    }

    IloArray < IloNumVarArray > w = IloArray < IloNumVarArray > (env, data.numItems+1);
    for(int i = 0; i <= data.numItems; ++i) {
        w[i] = IloNumVarArray(env, data.numVehicles, 0, 1, ILOINT);
        for(int k = 0; k < data.numVehicles; ++k) {
            sprintf(name, "w_%02d_%02d", i, k);
            w[i][k].setName(name);
        }
    }

    IloArray < IloArray < IloNumVarArray > > z = IloArray < IloArray < IloNumVarArray > >(env, data.numItems+1);
    for(int i = 0; i <= data.numItems; ++i) {
        z[i] = IloArray < IloNumVarArray > (env, data.numVehicles);
        for(int k = 0; k < data.numVehicles; ++k) {
            z[i][k] = IloNumVarArray(env, data.dimContainers[k].first, 0, 1, ILOINT);
            for(int r = 0; r < data.dimContainers[k].first; ++r) {
                sprintf(name, "z_%02d_%02d_%02d", i, k, r);
                z[i][k][r].setName(name);
            }
        }
    }

    // Minimize function
    // 1
    for(int k = 0; k < data.numVehicles; ++k) {
        for(int T = PICKUP; T <= DELIVERY; ++T) {
            for(int i = 0; i <= data.numItems; ++i) {
                for(int j = 0; j <= data.numItems; ++j) {
                    obj += (T == PICKUP ? data.pickupDistances[i][j] : data.deliveryDistances[i][j]) * x[i][j][k][T];
                }
            }
        }
    }
    model.add(IloMinimize(env, obj));

    // Constraints

    // 2
    for(int k = 0; k < data.numVehicles; ++k) {
        for(int T = PICKUP; T <= DELIVERY; ++T) {
            IloExpr expr(env);
            for(int j = 0; j <= data.numItems; ++j) {
                expr += x[0][j][k][T];
            }
            constraints.add(expr == 1);
            expr.end();
        }
    }

    // 3
    for(int k = 0; k < data.numVehicles; ++k) {
        for(int T = PICKUP; T <= DELIVERY; ++T) {
            IloExpr expr(env);
            for(int i = 0; i <= data.numItems; ++i) {
                expr += x[i][0][k][T];
            }
            constraints.add(expr == 1);
            expr.end();
        }
    }

    // 4
    for(int k = 0; k < data.numVehicles; ++k) {
        for(int T = PICKUP; T <= DELIVERY; ++T) {
            for(int j = 1; j <= data.numItems; ++j) {
                IloExpr expr(env);
                for(int i = 0; i <= data.numItems; ++i) {
                    if(i == j) continue;
                    expr += x[i][j][k][T];
                }
                constraints.add(expr - w[j][k] == 0);
                expr.end();
            }
        }
    }

    // 5
    for(int k = 0; k < data.numVehicles; ++k) {
        for(int T = PICKUP; T <= DELIVERY; ++T) {
            for(int i = 1; i <= data.numItems; ++i) {
                IloExpr expr0(env);
                IloExpr expr1(env);
                for(int j = 0; j <= data.numItems; ++j) {
                    if(i == j) continue;
                    expr0 += x[i][j][k][T];
                }
                for(int j = 0; j <= data.numItems; ++j) {
                    if(i == j) continue;
                    expr1 += x[j][i][k][T];
                }
                constraints.add(expr0 - expr1 == 0);
                expr0.end();
                expr1.end();
            }
        }
    }

    // 6
    for(int i = 1; i <= data.numItems; ++i) {
        IloExpr expr(env);
        for(int k = 0; k < data.numVehicles; ++k) {
            expr += w[i][k];
        }
        constraints.add(expr == 1);
        expr.end();
    }

    // 7
    for(int k = 0; k < data.numVehicles; ++k) {
        for(int r = 0; r < data.dimContainers[k].first; ++r) {
            IloExpr expr(env);
            for(int i = 1; i <= data.numItems; ++i) {
                expr += z[i][k][r];
            }
            constraints.add(expr - data.dimContainers[k].second <= 0);
            expr.end();
        }
    }
    
    // 8
    for(int k = 0; k < data.numVehicles; ++k) {
        for(int i = 1; i <= data.numItems; ++i) {
            IloExpr expr(env);
            for(int r = 0; r < data.dimContainers[k].first; ++r) {
                expr += z[i][k][r];
            }
            constraints.add(expr - w[i][k] == 0);
            expr.end();
        }
    }

    // 9
    for(int T = PICKUP; T <= DELIVERY; ++T) {
        for(int i = 1; i <= data.numItems; ++i) {
            for(int j = 1; j <= data.numItems; ++j) {
                if(i == j) continue;
                constraints.add(y[i][j][T] + y[j][i][T] == 1);
            }
        }
    }

    // 10
    for(int T = PICKUP; T <= DELIVERY; ++T) {
        for(int l = 1; l <= data.numItems; ++l) {
            for(int i = 1; i <= data.numItems; ++i) {
                for(int j = 1; j <= data.numItems; ++j) {
                    if(j == i || j == l) continue;
                    constraints.add(y[i][l][T] + y[l][j][T] - (y[i][j][T] + 1) <= 0);
                }
            }
        }
    }

    // 11
    for(int k = 0; k < data.numVehicles; ++k) {
        for(int T = PICKUP; T <= DELIVERY; ++T) {
            for(int i = 0; i <= data.numItems; ++i) {
                for(int j = 0; j <= data.numItems; ++j) {
                    if(i == j) continue;
                    constraints.add(x[i][j][k][T] - y[i][j][T] <= 0);
                }
            }
        }
    }

    // 12
    for(int k = 0; k < data.numVehicles; ++k) {
        for(int i = 1; i <= data.numItems; ++i) {
            for(int j = 1; j <= data.numItems; ++j) {
                if(i == j) continue;
                for(int r = 0; r < data.dimContainers[k].first; ++r) {
                    constraints.add(y[i][j][PICKUP] + z[i][k][r] + z[j][k][r] - (3 - y[i][j][DELIVERY]) <= 0);
                }
            }
        }
    }
    
    // 13
    std::vector < int > idContainerType(data.numVehicles);
    for(int k = 0; k < data.numVehicles; ++k) {
        idContainerType[k] = k;
        for(int i = k-1; i >= 0; --i) {
            if(data.dimContainers[k].first == data.dimContainers[i].first && data.dimContainers[k].second == data.dimContainers[i].second) {
                idContainerType[k] = idContainerType[i];
            }
        }
    }    
    for(int k = 0; k < data.numVehicles; ++k) {
        for(int kLine = k+1; kLine < data.numVehicles; ++kLine) {
            if(idContainerType[k] == idContainerType[kLine]) {
                IloExpr expr0(env);
                IloExpr expr1(env);
                for(int j = 1; j <= data.numItems; ++j) {
                    expr0 += j * x[0][j][k][PICKUP];
                }
                for(int j = 1; j <= data.numItems; ++j) {
                    expr1 += j * x[0][j][kLine][PICKUP];
                }
                constraints.add(expr0 - expr1 <= 0);
                expr0.end();
                expr1.end();
            }
        }
    }

    model.add(constraints);

    IloCplex cplex(model);
    
    std::string logFileOut = outputFileName;
    logFileOut.erase(--logFileOut.end());
    logFileOut.erase(--logFileOut.end());
    logFileOut.erase(--logFileOut.end());
    logFileOut.erase(--logFileOut.end());
    logFileOut += ".log";
            

    std::ofstream logFile(logFileOut.c_str());
    cplex.setOut(logFile);
    
    using namespace std::chrono;
    system_clock::time_point today = system_clock::now();

    time_t tt;
    tt = system_clock::to_time_t ( today );
    logFile << ctime(&tt) << std::endl;
    
    std::string modelOut = outputFileName;
    modelOut.erase(--modelOut.end());
    modelOut.erase(--modelOut.end());
    modelOut.erase(--modelOut.end());
    modelOut.erase(--modelOut.end());
    modelOut += ".lp";
    
    cplex.exportModel(modelOut.c_str());
 
    cplex.setParam(IloCplex::ClockType, 2); // cputime 1 to specify CPU time or 2 to specify wall-clock time
    cplex.setParam(IloCplex::TiLim, 3*3600);
    
    cplex.setParam(IloCplex::MIPDisplay, 3);
    //cplex.setParam(IloCplex::SimDisplay, 0);

    if(!cplex.solve()) { std::clog << "Error: model was not solved!" << std::endl; std::abort(); }

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration < double > time_span = duration_cast < duration < double > >(t2 - t1);

    double result = (int)(cplex.getObjValue()+0.5);
    double gap = cplex.getMIPRelativeGap();
    double bestBound = cplex.getBestObjValue();

    logFile << std::endl << std::endl << "Solution status: " << cplex.getStatus() << std::endl;
    logFile << "Execution time: " << std::fixed << std::setprecision(1) << time_span.count() << " sec" << std::endl;
    logFile << "Gap: " << std::fixed << std::setprecision(5) << gap << std::endl;

    printf("%-60s %10.3lf %10.3lf %10.5lf %10.1lf\n", outputFileName.c_str(), result, bestBound, gap, time_span.count());  
           
    std::vector < std::vector < int > > containerTmp;

    std::ofstream fout(outputFileName.c_str()); 
    
    fout << "Total routing cost: " << result << std::endl << std::endl;
    
    for(int k = 0; k < data.numVehicles; ++k) {
    
        fout << "Vehicle " << k+1 <<  " (" << data.dimContainers[k].first << " x " << data.dimContainers[k].second << "): " << std::endl;
        
        containerTmp.resize(data.dimContainers[k].first);
        for(int r = 0; r < (int)containerTmp.size(); ++r) {
            containerTmp[r].clear();
        }
        
        for(int r = 0; r < data.dimContainers[k].first; ++r) {
            for(int i = 1; i <= data.numItems; ++i) {
                if(cplex.getValue(z[i][k][r]) > 0.5) {
                    containerTmp[r].push_back(i);
                }
            }
        }
        
        for(int r = 0; r < (int)containerTmp.size(); ++r) {
            for(int l = 0; l < (int)containerTmp[r].size();l++) {    
                for(int l_line=l+1;l_line<(int)containerTmp[r].size();l_line++) {    
                    if(containerTmp[r][l] != containerTmp[r][l_line] && cplex.getValue(y[containerTmp[r][l_line]][containerTmp[r][l]][PICKUP])) {
                        std::swap(containerTmp[r][l], containerTmp[r][l_line]);
                    }
                }
            }
        }
        
        for(int r = 0; r < (int)containerTmp.size(); ++r) {
            while((int)containerTmp[r].size() < data.dimContainers[k].second) {
                containerTmp[r].push_back(-1);
            }
        }
        
        for(int l=data.dimContainers[k].second-1;l>=0;l--) {  
            fout << "                  ";
            for(int r = 0; r < data.dimContainers[k].first; ++r) {
                fout << std::setfill('0') << std::setw(2) << containerTmp[r][l] << ' ';
            }
            fout << std::endl;
        }
        fout << std::endl;
    
        if(cplex.getValue(x[0][0][k][PICKUP]) > 0.5) {
            fout << "Pickup  : -1" << std::endl;
            fout << "Delivery: -1" << std::endl;
        }
        else {
            for(int T = PICKUP; T <= DELIVERY; ++T) {
                int prev = 0;
                if(T == PICKUP) {
                    fout << "Pickup  : 00";
                }
                else {
                    fout << "Delivery: 00";
                }            
                while(1) {
                    for(int j = 0; j <= data.numItems; ++j) {
                        if(prev != j && cplex.getValue(x[prev][j][k][T]) > 0.5) {
                            fout << " --> " << std::setfill('0') << std::setw(2) << j;
                            prev = j;
                            break;
                        }
                    }
                    if(prev == 0) {
                        fout << std::endl;
                        break;
                    }
                }
            }
        }
        fout << std::endl << std::endl; 
    }
    
    fout.close();
    
    env.end();
}
