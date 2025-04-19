#include <ilcplex/ilocplex.h>

ILOSTLBEGIN

int main(int argc, char **argv) {
    IloEnv env;
    try {
        // iniciando variaveis
        IloModel coverage(env, "Coverage problem");
        IloCplex cplex(coverage);

        int district, schools;
        cin >> district >> schools;

        int C[district][schools];
        for (int i = 0; i < district; i++) {
            for (int j = 0; j < schools; j++) {
                cin >> C[i][j];
            }
        }

        IloArray <IloNumVarArray> Xj(env, district);
        for (int i = 0; i < district; i++) {
            Xj[i] = IloNumVarArray(env, schools, 0, 1, ILOINT);
        }
        // restricões
        // escola por baixo >= 1
        for (int i = 0; i < district; i++) {
            IloExpr exp(env);

            for (int j = 0; j < schools; j++) {
                exp += Xj[i][j];
            }

            coverage.add(exp >= 1);
            exp.end();
        }

        //não construir duas schools iguais
        for (int i = 0; i < schools; i++) {
            IloExpr exp(env);

            for (int j = 0; j < district; j++) {
                exp += Xj[j][i];
            }

            coverage.add(exp <= 1);
            exp.end();
        }
        // função objetivo
        IloExpr obj(env);
        for (int i = 0; i < district; i++) {
            for (int j = 0; j < schools; j++)
                obj += Xj[i][j] * C[i][j];
        }

        coverage.add(IloMinimize(env, obj));
        // resultado
        if (cplex.solve())
            cout << "best cost " << cplex.getObjValue() << endl;

        IloArray <IloNumArray> solXj(env, district);
        for (int i = 0; i < district; i++) {
            solXj[i] = IloNumArray(env, schools, 0, 1, ILOINT);
            cplex.getValues(solXj[i], Xj[i]);
        }

        for (int i = 0; i < district; i++) {
            for (int j = 0; j < schools; j++)
                if (solXj[i][j])
                    cout << "School " << j << " will be built in the district " << i << " cost: " << C[i][j] << endl;
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

