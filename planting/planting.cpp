#include <ilcplex/ilocplex.h>

ILOSTLBEGIN

int main(int argc, char **argv) {
    IloEnv env;
    try {
        int n_var = 3;

        IloModel planting(env, "Planting problem");
        IloCplex cplex(planting);
        IloNumVarArray xa(env, n_var, 0, IloInfinity);
        IloNumVarArray xf(env, n_var, 0, IloInfinity);
        IloNumVarArray xm(env, n_var, 0, IloInfinity);

        planting.add(xa[0] + xf[0] + xm[0] <= 400);
        planting.add(xa[1] + xf[1] + xm[1] <= 650);
        planting.add(xa[2] + xf[2] + xm[2] <= 350);
        planting.add(xa[0] + xa[1] + xa[2] <= 660);
        planting.add(xf[0] + xf[1] + xf[2] <= 880);
        planting.add(xm[0] + xm[1] + xm[2] <= 400);
        planting.add(4 * xa[0] + 3.5 * xf[0] + 5.5 * xm[0] <= 1800);
        planting.add(4 * xa[1] + 3.5 * xf[1] + 5.5 * xm[1] <= 2200);
        planting.add(4 * xa[2] + 3.5 * xf[2] + 5.5 * xm[2] <= 950);
        planting.add((xa[0] + xf[0] + xm[0]) / 400 == (xa[1] + xf[1] + xm[1]) / 650);
        planting.add((xa[0] + xf[0] + xm[0]) / 400 == (xa[2] + xf[2] + xm[2]) / 350);

        IloExpr obj(env);
        obj = 4000 * (xa[0] + xa[1] + xa[2])
              + 1800 * (xf[0] + xf[1] + xf[2])
              + 5000 * (xm[0] + xm[1] + xm[2]);

        planting.add(IloMaximize(env, obj));

        if (cplex.solve())
            cout << "best cost " << cplex.getObjValue() << endl;

        IloNumArray solXa(env, n_var);
        IloNumArray solXf(env, n_var);
        IloNumArray solXm(env, n_var);
        cplex.getValues(solXa, xa);
        cplex.getValues(solXf, xf);
        cplex.getValues(solXm, xm);

        for (int i = 0; i < n_var; i++) {
            cout << "Xa[" << i + 1 << "]: " << solXa[i] << endl;
            cout << "Xf[" << i + 1 << "]: " << solXf[i] << endl;
            cout << "Xm[" << i + 1 << "]: " << solXm[i] << endl;
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

