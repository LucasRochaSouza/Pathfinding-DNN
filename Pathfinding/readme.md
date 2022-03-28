Instalar visual studio 2022
Instalar vcpkg: https://github.com/microsoft/vcpkg#getting-started
Rodar os seguintes comandos dentro da pasta do vcpkg

vcppkg bootstrap-vcpkg.bat
vcpkg integrate install
vcpkg install libpng
vcpkg install libpng:x64-windows

main.cpp contem o codigo para gerar o dataset 

Maze.png tem um arquivo de mapa no formato preto/branco 
O define MAZE_FILE define o nome do arquivo para gerar o dataset
Ao rodar o arquivo main.cpp e esperado que seja gerado um dataset com o nome dataset.csv

