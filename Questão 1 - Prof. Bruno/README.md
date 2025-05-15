# Bin Packing com Tempo Limite

## Descrição
Este projeto implementa uma solução para o problema de Bin Packing utilizando uma meta-heurística de busca local. O objetivo é minimizar o número de recipientes (bins) utilizados para armazenar os itens, respeitando um limite de tempo definido pelo usuário.

## Representação da Solução
A solução é representada por uma lista de recipientes (bins), onde cada bin é uma lista de itens alocados. Inicialmente, cada item é colocado em um recipiente próprio, e a otimização ocorre através da realocação dos itens entre os bins.

## Função de Avaliação
A função de avaliação mede a qualidade da solução contando o número total de bins utilizados. O objetivo é minimizar este valor.

## Estratégia de Busca Local
A busca local implementada segue a estratégia Best Improvement, onde:
- São geradas soluções vizinhas movendo itens entre bins.
- A solução vizinha com menor número de bins é escolhida.
- O processo continua até que nenhuma melhoria seja encontrada ou o tempo limite seja atingido.

## Critério de Parada
O algoritmo recebe um tempo limite em segundos como argumento via linha de comando. A execução é interrompida assim que o tempo limite é atingido, retornando a última solução válida.

## Como Executar
Para rodar o script, execute o seguinte comando:
```bash
python script.py <tempo_limite>
```
Onde `<tempo_limite>` é o tempo máximo permitido para a execução do algoritmo, em segundos.

## Exemplo de Saída
```bash
Caso de Teste 1:
Bins utilizados: 4
Distribuição: [[0.9, 0.1], [0.8, 0.2], [0.7, 0.3], [0.6, 0.5]]
Tempo de execução: 15.32 ms
```

## Dependências
O código utiliza apenas bibliotecas padrão do Python (random, time, sys), não sendo necessária nenhuma instalação adicional.

## Autor
Este projeto foi desenvolvido para fins educacionais e de experimentação com técnicas de busca local aplicadas ao problema de Bin Packing.