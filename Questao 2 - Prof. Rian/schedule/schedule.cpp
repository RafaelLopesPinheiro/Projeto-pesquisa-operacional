#include <ilcplex/ilocplex.h>
#include <iostream>

ILOSTLBEGIN

int main(int argc, char **argv) {
    IloEnv env;
    try {
        IloModel horario(env, "Schedule problem");
        IloCplex cplex(horario);

        int demand[7];
        
        for (int i = 0; i < 7; i++) {
            cin >> demand[i];
        }
        
        string days[] = {"domingo",
                         "segunda", "terca",
                         "quarta", "quinta",
                         "sexta", "sabado"};

        IloNumVarArray X(env, 7, 0, IloInfinity, ILOINT);

        horario.add(X[0] + X[3] + X[4] + X[5] + X[6] >= demand[0]);
        horario.add(X[1] + X[4] + X[5] + X[6] + X[0] >= demand[1]);
        horario.add(X[2] + X[5] + X[6] + X[1] + X[2] >= demand[2]);
        horario.add(X[3] + X[6] + X[1] + X[2] + X[3] >= demand[3]);
        horario.add(X[4] + X[1] + X[2] + X[3] + X[5] >= demand[4]);
        horario.add(X[5] + X[2] + X[3] + X[4] + X[5] >= demand[5]);
        horario.add(X[6] + X[1] + X[2] + X[3] + X[4] >= demand[6]);

        IloExpr obj(env);
        for (int j = 0; j < 7; j++) {
            obj += X[j];
        }

        horario.add(IloMinimize(env, obj));

        if (cplex.solve())
            cout << "best cost "
                 << cplex.getObjValue() << endl;

        IloNumArray solX(env, 7);
        cplex.getValues(solX, X);

        for (int i = 0; i < solX.getSize(); i++) {
            int employees = solX[i];
            for (int j = (i + 3) % 7; j < 7; j++)
                if (i != j)
                    employees += solX[j % 7];
            cout << days[i] << ": " << employees << " nurses\n";
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