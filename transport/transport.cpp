#include <ilcplex/ilocplex.h>
using namespace std;
int main() {
    IloEnv env;
    try {

        int factory[] = {120, 80, 80};
        int storage[] = {150, 70, 60};
        int cost[3][3] = {{5,  5,  6},
                          {5, 10, 12},
                          {3,  9,  10}};

        IloModel transport(env, "Transport problem");
        IloCplex cplex(transport);
        IloArray <IloIntVarArray> x(env, 3);

        for (int i = 0; i < 3; i++) {
            x[i] = IloIntVarArray(env, 3, 0, IloInfinity);
        }

        IloExpr obj(env);
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++)
                obj += cost[i][j] * x[i][j];
        }
        transport.add(IloMinimize(env, obj));

        for (int i = 0; i < 3; i++) {
            IloExpr offer(env);
            for (int j = 0; j < 3; j++)
                offer += x[i][j];
            transport.add(offer <= factory[i]);
        }


        for (int i = 0; i < 3; i++) {
            IloExpr demand(env);
            for (int j = 0; j < 3; j++)
                demand += x[j][i];
            transport.add(demand == storage[i]);
        }
        if (cplex.solve())
            cout << "best cost " << cplex.getObjValue() << endl;

        IloArray <IloNumArray> solution(env, 3);

        for (int i = 0; i < 3; i++) {
            cout << "factory: " << i + 1 << " >>" << endl;
            solution[i] = IloNumArray(env, 3);
            cplex.getValues(solution[i], x[i]);

            for (int j = 0; j < 3; j++)
                cout << ">> to: " << j + 1 << " | total: " << solution[i][j] << endl;
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
