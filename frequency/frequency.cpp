#include <ilcplex/ilocplex.h>

ILOSTLBEGIN

int main(int argc, char **argv) {
    IloEnv env;
    try {
        IloModel frequency(env, "Frequency problem");
        IloCplex cplex(frequency);

        int towers, edges, frequencies;
        cin >> towers >> edges >> frequencies;

        int tower_graph[towers][towers];
        memset(tower_graph, 0, sizeof(tower_graph));

        for (int i = 0; i < edges; i++) {
            int a, b;
            cin >> a >> b;
            tower_graph[a - 1][b - 1] = tower_graph[b - 1][a - 1] = 1;
        }

        IloArray <IloNumVarArray> Xij(env, towers);
        for (int i = 0; i < towers; i++)
            Xij[i] = IloNumVarArray(env, frequencies, 0, 1, ILOINT);


        IloNumVarArray Yj(env, frequencies, 0, 1, ILOINT);

        for (int i = 0; i < towers; i++) {
            IloExpr exp(env);

            for (int j = 0; j < frequencies; j++)
                exp += Xij[i][j];
            frequency.add(exp == 1);
            exp.end();
        }


        for (int i = 0; i < towers; i++) {

            for (int j = 0; j < towers; j++)
                if (tower_graph[i][j]) {
                    for (int k = 0; k < frequencies; k++)
                        frequency.add(Xij[i][k] + Xij[j][k] <= 1);
                }
        }

        IloExpr obj(env);
        for (int i = 0; i < towers; i++) {
            for (int j = 0; j < frequencies; j++)
                obj += Xij[i][j];
        }

        frequency.add(IloMinimize(env, obj));

        if (cplex.solve())
            cout << "best cost" << cplex.getObjValue() << endl;

        IloArray <IloNumArray> solXij(env, towers);
        for (int i = 0; i < towers; i++) {
            solXij[i] = IloNumArray(env, frequencies, 0, 1, ILOINT);
            cplex.getValues(solXij[i], Xij[i]);
        }

        for (int i = 0; i < towers; i++) {
            for (int j = 0; j < frequencies; j++)
                if (solXij[i][j])
                    cout << "Tower " << i + 1 << " | frequency " << j + 1 << endl;
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

