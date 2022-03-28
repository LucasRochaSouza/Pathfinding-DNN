Instalar vscode 2022
Instalar vcpkg: https://github.com/microsoft/vcpkg

Extrair o .zip em qualquer pasta (recomendo C:\Dev)
Entrar nesta pasta com o prompt de comando (cmd)

Executar os seguintes comandos:

vcpkg bootstrap-vcpkg.bat
vcpkg integrate install
vcpkg install libpng
vcpkg install libpng:x64-windows

Instalar o git e fazer um pull
mudar para branch de desenvolvimento: git checkout develop

/**********************************************************/

No arquivo main.cpp mostra como é feito a geração do dataset, mudar as definições para realizar testes em outros mapas.