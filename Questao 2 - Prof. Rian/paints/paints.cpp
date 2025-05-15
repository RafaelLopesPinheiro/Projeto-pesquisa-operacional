#include <ilcplex/ilocplex.h>

ILOSTLBEGIN

int main(int argc, char **argv) {
    IloEnv env;
    try {
        int n_var = 4;

        IloModel paints(env, "Paints problem");
        IloCplex cplex(paints);
        IloNumVarArray xsr(env, n_var, 0, IloInfinity);
        IloNumVarArray xsn(env, n_var, 0, IloInfinity);

        IloExpr prod_xsr(env);
        for (int i = 0; i < n_var; i++)
            prod_xsr += xsr[i];

        paints.add(prod_xsr == 1000);

        IloExpr prod_xsn(env);
        for (int i = 0; i < n_var; i++)
            prod_xsn += xsn[i];

        paints.add(prod_xsn == 250);
        paints.add(0.3 * xsr[0] + 0.6 * xsr[1] + xsr[3] >= 250);
        paints.add(0.3 * xsn[0] + 0.6 * xsn[1] + xsn[3] >= 50);
        paints.add(0.7 * xsr[0] + 0.4 * xsr[1] + xsr[2] >= 500);
        paints.add(0.7 * xsn[0] + 0.4 * xsn[1] + xsn[2] >= 125);

        IloExpr obj(env);
        obj = 1.5 * (xsr[0] + xsn[0]) + (xsr[1] + xsn[1]) + 6 * (xsr[2] + xsn[2]) + 4 * (xsr[3] + xsn[3]);

        paints.add(IloMinimize(env, obj));

        if (cplex.solve())
            cout << "best cost " << cplex.getObjValue() << endl;

        IloNumArray solxsr(env, n_var);
        IloNumArray solxsn(env, n_var);

        cplex.getValues(solxsr, xsr);
        cplex.getValues(solxsn, xsn);

        for (int i = 0; i < n_var; i++)
            cout << "Xsr" << i + 1 << ": " << solxsr[i] << endl << "para Xsn" << i + 1 << ": " << solxsn[i] << endl;

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

