#include <ilcplex/ilocplex.h>

ILOSTLBEGIN

int main(int argc, char **argv) {
    IloEnv env;
    try {
        // iniciando variaveis
        int n_ing = 6;
        int qnt_min_A = 9;
        int qnt_min_C = 19;
        int vit_A[] = {1, 0, 2, 2, 1, 2};
        int vit_C[] = {0, 1, 3, 1, 3, 2};
        int cost[] = {35, 30, 60, 50, 27, 22};

        IloModel diet(env, "Diet Problem");
        IloCplex cplex(diet);


        IloNumVarArray x(env, n_ing, 0, IloInfinity);
        // função objetivo
        IloExpr obj(env);
        for (int i = 0; i < n_ing; i++)
            obj += cost[i] * x[i];
        // restrições
        diet.add(IloMinimize(env, obj));

        IloExpr qnt_A(env);
        for (int i = 0; i < n_ing; i++)
            qnt_A += vit_A[i] * x[i];
        diet.add(qnt_A >= qnt_min_A);

        IloExpr qnt_C(env);
        for (int i = 0; i < n_ing; i++)
            qnt_C += vit_C[i] * x[i];
        diet.add(qnt_C >= qnt_min_C);

        // resultado
        if (cplex.solve())
            cout << "best cost " << cplex.getObjValue() << endl;

        IloNumArray sol(env, n_ing);
        cplex.getValues(sol, x);

        for (int i = 0; i < n_ing; i++)
            cout << "Ingredient " << i + 1 << ": " << sol[i] << endl;

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

