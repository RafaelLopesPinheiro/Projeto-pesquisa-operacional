#include <ilcplex/ilocplex.h>

ILOSTLBEGIN

int main(int argc, char **argv) {
    IloEnv env;
    try {
        // iniciando variaveis
        int n, weightMax;
        cin >> n >> weightMax;

        int value[n];
        int weight[n];

        for (int i = 0; i < n; i++)
            cin >> value[i];


        for (int i = 0; i < n; i++)
            cin >> weight[i];


        IloModel bag(env, "Bag problem");
        IloCplex cplex(bag);
        IloIntVarArray x(env, n, 0, 1);
        //função objetivo
        IloExpr obj(env);
        for (int i = 0; i < n; i++)
            obj += value[i] * x[i];

        bag.add(IloMaximize(env, obj));
        // restrições
        IloExpr sum(env);
        for (int i = 0; i < n; i++)
            sum += weight[i] * x[i];
        bag.add(sum <= weightMax);

        // resultado
        if (cplex.solve())
            env.out() << "best cost " << cplex.getObjValue() << endl;

        IloNumArray sol(env, n);
        cplex.getValues(sol, x);

        int weightTotal = 0;
        for (int i = 0; i < n; i++)
            if (sol[i] == 1)
                weightTotal += weight[i];

        cout << "Total = " << weightTotal << endl;

        for (int i = 0; i < n; i++)
            cout << "id " << i + 1 << ": cost = " << value[i] << " weight = " << weight[i] << " in solution " << sol[i]
                 << endl;
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

