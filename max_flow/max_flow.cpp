#include <ilcplex/ilocplex.h>
#include <iostream>

ILOSTLBEGIN

int main(int argc, char **argv) {
    IloEnv env;
    try {
        IloModel max_flow(env, "Max flow problem");
        IloCplex cplex(max_flow);

        int size, edges;
        cin >> size >> edges;

        int edge_capacity[edges];
        for (int i = 0; i < edges; i++)
            cin >> edge_capacity[i];

        int edge_map[size][size];
        memset(edge_map, 0, sizeof(edge_map));

        int si, sj, ti, tj;
        cin >> si >> sj >> ti >> tj;

        int Cij[size][size];
        int counter = 0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                cin >> Cij[i][j];
                if (Cij[i][j] > 0) {
                    counter += 1;
                    if (!edge_map[i][j])
                        edge_map[i][j] = edge_map[j][i] = counter;
                }
            }
        }

        IloNumVarArray X(env, edges, 0, IloInfinity);

        for (int i = 0; i < edges; i++)
            max_flow.add(X[i] <= edge_capacity[i]);

        for (int i = 0; i < size; i++) {

            int inputSum = 0;
            IloExpr inputSumExp(env);

            int outputSum = 0;
            IloExpr outputSumExp(env);

            for (int j = 0; j < size; j++) {
                int index = edge_map[i][j];
                if (i != si && i != ti) {
                    if (Cij[i][j] > 0) {
                        outputSum += Cij[i][j];
                        outputSumExp += X[index - 1];
                    } else if (Cij[i][j] < 0) {
                        int index = edge_map[i][j];
                        inputSum += Cij[i][j];
                        inputSumExp += X[index - 1];
                    }
                }
            }

            if (outputSum > abs(inputSum)) {
                max_flow.add(inputSumExp == abs(inputSum));
                max_flow.add(outputSumExp == abs(inputSum));
            } else {
                max_flow.add(inputSumExp == outputSum);
                max_flow.add(outputSumExp == outputSum);
            }

            inputSumExp.end();
            outputSumExp.end();
        }
        IloExpr obj(env);
        for (int j = 0; j < size; j++)
            if (Cij[si][j] > 0) {
                int index = edge_map[si][j] - 1;
                obj += X[index];
            }

        max_flow.add(IloMaximize(env, obj));

        if (cplex.solve())
            cout << "best cost " << cplex.getObjValue() << endl;


        IloNumArray solX(env, edges);
        cplex.getValues(solX, X);

        for (int i = 0; i < solX.getSize(); i++)
            cout << "flow X[" << i + 1 << "]: " << solX[i] << "\n";
    }
    catch (const IloException &e) {
        cerr << "Exception caught: " << e << endl;
    }
    catch (...) {
        cerr << "Unknown exception caught!" << endl;
    }

    env.end();
    return 0;
}
