#include <ilcplex/ilocplex.h>


ILOSTLBEGIN

int main(int argc, char **argv) {
    IloEnv env;
    try {
        IloModel click(env, "click max problem");
        IloCplex cplex(click);

        int n_vertices, edges;
        cin >> n_vertices >> edges;

        int edge_matrix[n_vertices][n_vertices];
        memset(edge_matrix, 0, sizeof(edge_matrix));

        for (int i = 0; i < edges; i++) {
            int a, b;
            cin >> a >> b;
            edge_matrix[a - 1][b - 1] = 1;
        }
        // fim da inicializaçao de variaveis

        IloNumVarArray x(env, n_vertices, 0, 1, ILOINT);
        IloExpr sum1(env);

        for (int i = 0; i < n_vertices; i++)
            sum1 += x[i];

        click.add(sum1 <= sum1 *(sum1 - 1)/2);

        cout << sum1 << endl;
        sum1.end();

        for (int i = 0; i < n_vertices; i++) {
            for (int j = 0; j < n_vertices; j++)
                if (!edge_matrix[i][j])
                    click.add(x[i] <= 1);
        }


        IloExpr obj(env);
        for (int i = 0, j = 0; i < n_vertices; i++, j++) {
            if (i < n_vertices)
                obj += x[i];
        }

        click.add(IloMaximize(env, obj));

        if (cplex.solve())
            cout << "best cost " << cplex.getObjValue() << endl;

        // // Obtendo a solução
        IloNumArray solx(env, n_vertices, 0, 1, ILOINT);
        cplex.getValues(solx, x);


        cout << "vertices list: ";
        for (int i = 0; i < n_vertices; i++)
            if (solx[i])
                cout << i + 1 << ' ';
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

