#include <ilcplex/ilocplex.h>

ILOSTLBEGIN

int main(int argc, char **argv) {
    IloEnv env;

    try {
        IloModel max_click(env, "Max click problem");
        IloCplex cplex(max_click);

        int n_vertices, edges;
        cin >> n_vertices >> edges;

        int edge_matrix[n_vertices][n_vertices];
        memset(edge_matrix, 0, sizeof(edge_matrix));

        for (int i = 0; i < edges; i++) {
            int a, b;
            cin >> a >> b;
            edge_matrix[a - 1][b - 1] =edge_matrix[b - 1][a - 1]= 1;
        }

        IloNumVarArray Xi(env, n_vertices, 0, 1, ILOINT);

        for (int i = 0; i < n_vertices; i++) {
            for (int j = 0; j < n_vertices; j++)
                if (!edge_matrix[i][j] && i != j)
                    max_click.add(Xi[i] + Xi[j] <= 1);
        }

        IloExpr obj(env);

        for (int i = 0; i < n_vertices; i++)
            obj += Xi[i];

        max_click.add(IloMaximize(env, obj));

        if (cplex.solve())
            cout << "best cost " << cplex.getObjValue() << endl;

        IloNumArray solXi(env, n_vertices, 0, 1, ILOINT);
        cplex.getValues(solXi, Xi);

        cout << "Solution: ";
        for (int i = 0; i < n_vertices; i++)
            if (solXi[i])
                cout << i + 1 << ' ';

        cout << endl;
    }
    catch (const IloException &e) {
        cerr << "Exception caught: " << e << endl;
    }
    catch (...) {
        cerr << "Unknown exception caught!" << endl;
    }

    return 0;
}

