# pmt

Projeto 1 da disciplina de Processamento de Cadeias de Caracteres que tem como objetivo criar uma ferramenta chamada **pmt** que realiza uma busca de padrão ou padrões de texto em um arquivo ou conjunto de arquivos de texto.

### Pré requisitos

Para este projeto você somente necessita de um compilador para c++. Porém facilita possuir a ferramenta make, para auxiliar a compilação.

### Instalando

Para instalar a ferramenta você pode utilizar duas maneiras:
* Se possuir a ferramenta **make** instalada basta digitar o comando *make* no diretório pmt/ que será instalado automaticamente.
* Se não possuir o comando make basta seguir estes comandos a partir do diretório pmt/:
```
$ mkdir bin
$ cd src
$ g++ shiftor.cpp aho_corasick.cpp ukkonen.cpp main.cpp -o ../bin/pmt
$ cd ..
```

### Rodando

Para rodar a ferramenta pmt basta ir ao diretório *bin* através do comando 'cd bin' e rodar a ferramenta, seguindo seu caso geral de uso:
- $ ./pmt [Opções] padrão arquivo1 [arquivo2...]

Como por exemplo:
```
$ ./pmt love shakespeare.txt
```

### Opções

A ferramenta pmt suporta uma série de opções descritas abaixo:

* -e, --edit e-max: Realiza o casamento aproximado de padrão com uma distância de edição máxima de e-max;
* -p, --pattern arquivoDePadrao: A ferramenta suporta o recebimento de um arquivo que definem os padrões. Cada padrão deve ser colocado numa linha do arquivo arquivoDePadrao. **Ao utilizar esta opção, não se deve passar o padrão como argumento.**
* -a, --algorithm nomeAlgoritmo: É uma opção para a escolha do algoritmo que deseja ser utilizado. Ao escolher esta opção deve-se passar o nome do algoritmo **respeitando letras maiúsculas e minúsculas.** Dentre as opções tem-se:
    - ShiftOr - Algoritmo para casamento exato do padrão;
    - AhoCorasick - Segundo algoritmo para casamento exato;
    - WuManber - Algoritmo para casamento aproximado do padrão;
    - Ukkonen - Segundo algoritmo para casamento aproximado do padrão;
* -c, --count: Esta opção não recebe argumento, e quando ativada imprime somente a quantidade de ocorrências encontradas do(s) padrão(ões) no(s) texto(s);
* -h, --help: Imprime estas opções; 


### Autores

Esta ferramenta foi desenvolvida por:
* **Jônatas de Oliveira Clementino** - joc@cin.ufpe.br
* **Valdemiro Rosa Vieira Santos** - vrvs@cin.ufpe.br
