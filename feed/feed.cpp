#include <ilcplex/ilocplex.h>

ILOSTLBEGIN

int main(int argc, char **argv) {
    IloEnv env;
    try {
        int number_of_variables = 2;

        IloModel feed(env, "Feed problem");
        IloCplex cplex(feed);
        IloNumVarArray Xamgs(env, number_of_variables, 0, IloInfinity);
        IloNumVarArray Xre(env, number_of_variables, 0, IloInfinity);

        feed.add(Xamgs[0] + 4 * Xre[0] <= 10000);
        feed.add(5 * Xamgs[1] + 2 * Xre[1] <= 30000);

        IloExpr obj(env);
        obj = 20 * (Xamgs[0] + 5 * Xamgs[1]) + 30 * (4 * Xre[0] + 2 * Xre[1]);

        feed.add(IloMaximize(env, obj));

        if (cplex.solve())
            cout << "best cost " << cplex.getObjValue() << endl;

        IloNumArray solXamgs(env, number_of_variables);
        IloNumArray solXre(env, number_of_variables);

        cplex.getValues(solXamgs, Xamgs);
        cplex.getValues(solXre, Xre);

        for (int i = 0; i < number_of_variables; i++)
            cout << "Xamgs" << i + 1 << ": " << solXamgs[i] << endl << "Xre" << i + 1 << ": " << solXre[i] << endl;

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

