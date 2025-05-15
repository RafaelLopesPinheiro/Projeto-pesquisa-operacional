#include <ilcplex/ilocplex.h>

using namespace std;


int main() {
    IloEnv env;
    try {
        // iniciar variaveis
        IloModel pattern(env, "Pattern problem");
        IloCplex cplex(pattern);
        IloIntVar y(env, 0, IloIntMax);
        IloNumVarArray x(env, 4, 0, IloIntMax, ILOINT);

        // definir funçao objetivo
        IloExpr obj(env);

        obj = 50 * y - (5 * (1 * x[0] + 2 * x[1] + 4 * x[3] - y)) +
              (3 * (7 * x[0] + 3 * x[1] + 9 * x[2] + 4 * x[3] - 2 * y));
        // restrições do problema
        pattern.add(IloMaximize(env, obj));
        pattern.add(2 * x[0] + 3 * x[1] + 2 * x[2] + 1 * x[3] <= 400);
        pattern.add(x[0] + x[2] + x[3] <= 20);
        pattern.add(x[1] <= 90);
        pattern.add(y <= 7 * x[0] + 3 * x[1] + 9 * x[2] + 4 * x[3]);
        pattern.add(y <= 1 * x[0] + 2 * x[1] + 4 * x[3]);

        // resultado
        if (cplex.solve())
            cout << "best cost " << cplex.getObjValue() << endl;

        IloNumArray solution(env, 4, 0, IloIntMax);
        cplex.getValues(solution, x);

        for (int i = 0; i < 4; i++) {
            cout << solution[i] << " pattern " << i + 1 << endl;
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
