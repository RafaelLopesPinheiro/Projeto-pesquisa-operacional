import random
import time
import sys


class BinPacking:
    def __init__(self, items, time_limit):
        self.items = sorted(items, reverse=True)
        self.time_limit = time_limit
        self.solution = self.initial_solution()

    def initial_solution(self):
        bins = [[item] for item in self.items]
        return bins

    def evaluate(self):
        return len(self.solution)

    def get_neighbors(self, start_time):
        neighbors = []
        for i in range(len(self.solution)):
            for j in range(len(self.solution)):
                if time.time() - start_time >= self.time_limit:
                    return neighbors
                if i != j:
                    for item in self.solution[i]:
                        if sum(self.solution[j]) + item <= 1:
                            new_solution = [list(bin) for bin in self.solution]
                            new_solution[i].remove(item)
                            new_solution[j].append(item)
                            if not new_solution[i]:
                                del new_solution[i]
                            neighbors.append(new_solution)
        return neighbors

    def best_improvement(self):
        start_time = time.time()
        # print('s0:', self.solution, self.evaluate())
        while time.time() - start_time < self.time_limit:
            neighbors = self.get_neighbors(start_time)
            if not neighbors:
                break
            best_neighbor = min(neighbors, key=lambda s: len(s), default=self.solution)
            if len(best_neighbor) < self.evaluate():
                self.solution = best_neighbor
            else:
                break
            # print(f'step:', self.solution, self.evaluate())
        return self.solution, (time.time() - start_time) * 1000

    def solve(self):
        return self.best_improvement()


if __name__ == "__main__":
    try:
        time_limit = float(sys.argv[1])
    except (IndexError, ValueError):
        print("Nenhum tempo limite válido fornecido por argumento. Usando valor padrão de 1 segundo.")
        time_limit = 1.0

    test_cases = [
        ([0.7, 0.2, 0.5, 0.4, 0.3, 0.8, 0.1, 0.6, 0.7, 0.8, 0.9], time_limit),
        ([0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5], time_limit),
        ([0.9, 0.9, 0.9, 0.9, 0.9], time_limit),
        ([0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9], time_limit),
        ([round(random.uniform(0.1, 0.3), 2) for _ in range(2)], time_limit),
        ([round(random.uniform(0.1, 0.3), 2) for _ in range(4)], time_limit),
        ([round(random.uniform(0.1, 0.3), 2) for _ in range(8)], time_limit),
        ([round(random.uniform(0.1, 0.3), 2) for _ in range(16)], time_limit),
        ([round(random.uniform(0.1, 0.3), 2) for _ in range(32)], time_limit),
        ([round(random.uniform(0.1, 0.3), 2) for _ in range(64)], time_limit),
        ([round(random.uniform(0.1, 0.3), 2) for _ in range(128)], time_limit),
    ]

    for i, (items, limit) in enumerate(test_cases, 1):
        print(f"Caso de Teste {i}:")
        bp = BinPacking(items, limit)
        solution, exec_time = bp.solve()
        print("Bins utilizados:", len(solution))
        print(f"Tempo de execução: {exec_time:.2f} ms")
        print("-")
        print("Solução encontrada:", solution)
        print("--------------------")