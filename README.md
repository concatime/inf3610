# INF3610

## À EXÉCUTEZ IMPÉRATIVEMENT APRÈS CHAQUE CLONAGE

```sh
git config include.path ../.gitconfig
git submodule update --init --recursive
```

## Développement sur GNU+Linux

Installez Clang, LLD, Meson, Ninja et VSCodium.

Ouvrez le répertoire depuis GNOME Builder.

## Développement pour Windows 10

Add to user env
 - `VCToolsInstallDir`: `C:\Windows`

Delete to env user path
 - `windowsapp...`

Add to env user path
 - `%LOCALAPPDATA%\Programs\Cocktail`
 - `%LOCALAPPDATA%\Programs\LLVM\bin`
 - `%LOCALAPPDATA%\Programs\VSCodium\bin`

https://github.com/VSCodium/vscodium/releases/latest
 - VSCodiumUserSetup-x64-X.Y.Z.exe

Copy Cocktail to `%LOCALAPPDATA%\Programs`

TODO: `%LOCALAPPDATA%\Programs\VCTools` (libcmt.lib)

Install LLVM witih `noadmin`
https://github.com/llvm/llvm-project/releases/download/llvmorg-10.0.0/LLVM-10.0.0-win64.exe

https://aka.ms/windowssdk
 - Nécessite les droits administrateurs car besoin de modifier HKLM

Redémarrez

Ouvrez le dossier, installez les extensions recommendées et relancez VSCodium.

Compilez avec `Ctrl-Shift-B`.

Lancez le déverminage avec `F5`.
