#include <ilcplex/ilocplex.h>

ILOSTLBEGIN

int main(int argc, char **argv) {
    IloEnv env;
    try {
        // iniciando variaveis
        IloModel coloring(env, "Coloring problem");
        IloCplex cplex(coloring);

        int n_colors = 4;
        int colors[] = {1, 2, 3, 4};

        int vertices, edges;
        cin >> vertices >> edges;

        int mtz_cost[vertices][edges];
        memset(mtz_cost, 0, sizeof(mtz_cost));
        
        for (int i = 0; i < edges; i++) {
            int a, b;
            cin >> a >> b;
            mtz_cost[a - 1][b - 1] = mtz_cost[b - 1][a - 1] = 1;
        }

        IloArray<IloNumVarArray> Xij(env, vertices);
        for (int i = 0; i < vertices; i++) {
            Xij[i] = IloNumVarArray(env, vertices, 0, n_colors, ILOINT);
        }


        // adicionando restrições
        for (int i = 0; i < vertices; i++) {
            IloExpr exp(env);
            int idx = 0;
            int minCost = 0;

            for (int j = 0; j < vertices; j++) {
                coloring.add((i == j) ? Xij[i][j] == 0 : Xij[i][j] == Xij[j][i]);

                if (mtz_cost[i][j]) {
                    exp += Xij[i][j];
                    minCost += colors[idx];
                    idx += 1;
                    coloring.add(Xij[i][j] >= 1);
                }
            }
            coloring.add(exp >= minCost);
            exp.end();
        }

        // funcão objetivo
        IloExpr obj(env);
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                if (mtz_cost[i][j]) {
                    obj += Xij[i][j];
                }
            }
        }

        coloring.add(IloMinimize(env, obj));

        // resultado
        if (cplex.solve())
            cout << "best cost " << cplex.getObjValue() / 2 << endl;

        IloArray<IloNumArray> solXij(env, vertices);
        for (int i = 0; i < vertices; i++) {
            solXij[i] = IloNumArray(env, vertices, 0, n_colors, ILOINT);
            cplex.getValues(solXij[i], Xij[i]);
        }

        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++)
                cout << solXij[i][j] << ' ';
            cout << endl;
        }
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
