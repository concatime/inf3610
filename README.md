# INF3610

## À EXÉCUTEZ IMPÉRATIVEMENT APRÈS CHAQUE CLONAGE

```sh
git config include.path ../.gitconfig
git submodule update --init --recursive
```

## Développement sur GNU+Linux

Installez GCC, Meson, Ninja et GNOME Builder (la version [FlatHub][1] de
préférence).

Ouvrez le répertoire depuis GNOME Builder.

## Développement pour Windows 10

https://aka.ms/vscode

https://aka.ms/buildtools
 - Individual components
   - Compilers, build tools, and runtimes
     - MSVC v142 - VS 2019 C++ x64/x86 build tools (v14.27)

https://aka.ms/windowssdk
 - Debugging Tools for Windows
 - Windows SDK for Desktop C++ amd64 Apps

https://github.com/mesonbuild/meson/releases/download/0.55.3/meson-0.55.3-64.msi

Redémarrez

Menu démarrer
 - x64 Native Tools Command Prompt for VS 2019
   - `cd %HOMEPATH%\Workspace\inf3610\lab1`
   - `code .`

Installez les extensions recommendées et relancez VSCode.

Compilation: `Ctrl-Shift-B`

Déverminage: `F5`

[1]: https://flathub.org/apps/details/org.gnome.Builder
