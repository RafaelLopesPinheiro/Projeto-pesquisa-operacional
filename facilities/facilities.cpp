#include <ilcplex/ilocplex.h>

ILOSTLBEGIN


int main(int argc, char **argv) {
    IloEnv env;
    try {
        // iniciando variaveis
        IloModel facilities(env, "Facilities problem");
        IloCplex cplex(facilities);

        int centers, clients;
        cin >> centers >> clients;

        int storage[centers];
        int client[centers][clients];

        for (int i = 0; i < centers; i++)
            cin >> storage[i];

        for (int i = 0; i < centers; i++) {
            for (int j = 0; j < clients; j++)
                cin >> client[i][j];
        }

        //restição
        IloArray <IloNumVarArray> Xij(env, centers);
        for (int i = 0; i < centers; i++)
            Xij[i] = IloNumVarArray(env, clients, 0, 1, ILOINT);

        for (int i = 0; i < clients; i++) {
            IloExpr exp(env);

            for (int j = 0; j < centers; j++)
                exp += Xij[j][i];

            facilities.add(exp == 1);
            exp.end();
        }

        for (int i = 0; i < centers; i++) {
            IloExpr exp(env);

            for (int j = 0; j < clients; j++)
                exp += Xij[i][j];

            facilities.add(exp >= 0);
            facilities.add(exp <= clients);
            exp.end();
        }
        //objetivo
        IloExpr obj(env);
        for (int i = 0; i < centers; i++) {
            for (int j = 0; j < clients; j++)
                obj += (storage[i] * Xij[i][j]) + (client[i][j] * Xij[i][j]);
        }

        facilities.add(IloMinimize(env, obj));

        // resultado
        if (cplex.solve())
            cout << "best cost " << cplex.getObjValue() << endl;

        IloArray <IloNumArray> solXij(env, centers);
        for (int i = 0; i < centers; i++) {
            solXij[i] = IloNumArray(env, clients, 0, 1, ILOINT);
            cplex.getValues(solXij[i], Xij[i]);
        }

        for (int i = 0; i < centers; i++) {
            for (int j = 0; j < clients; j++)
                if (solXij[i][j])
                    cout << "id " << j + 1 << " | center " << i + 1 << " | build cost: " << storage[i]
                         << " | service cost: " << client[i][j] << endl;
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

