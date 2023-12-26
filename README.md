## Lista de Tarefas - C

Fiz uma lista de tarefa feita em C com a utilização da estrutura de dadoos `Lista Ligada` para a melhor compreenção vou fazer uma breve introdução:

### lista ligada/encadeada

Essa estrutura de dado ela é semelhante a um `Array` porem com ela podemos ter uma alocação de momoria dinamica onde o elemanto ira apontar para o proximo elemento e esse por sua vez ira faazer o mesmo ate o ultimo que esse pode definir as duas variação da lista ligada que é a simplismente ligada que o ultimo retorna NULL e a duplamente ligada que o ultimo retorna o primeiro elemento voltando para o inicio da lista.

### Sobre a Lista de Tarefas

A lista de tarefas ela funciona apartir de uma lista ligada onde cada elemento posui um nome e se vai trabalhar com horas e retorna uma nova lista ligada dos seus elementos e esse lista casa elemento possuia um texto e a hora.

### Funções Internas

Como é usado uma lista ligada é necessario ter as sequites funções principais:

- Criar
- Editar
- Deletar
- Listar
- Contar

Essas funções são cruciais para o funcionamento do programa porem para que podece ser usado corretamente foi necessario fazer alterações para que podessem funcionar com as listas de listas e de itens.

### Alocação de Memoria

Um fator de muito cuidado é o tratamento de memoria onde para cria o elemento usamos `malloc()` para alocar a memoria e passar o ponteiro da memoria do elemento por conta disso é necessario para encerrar o programa usarmos a função `free()` que limpa a momoria indicada e elem disso é necessario umcidado maior com as listas de listas ja que alen de limpar o elemento lista temos que limpar todos os elementos itens pentenciente a lista e não deixar memorias sem serem limpas.