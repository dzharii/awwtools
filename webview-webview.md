[webview/webview](https://github.com/webview/webview)

Debian-based systems:                      

Packages:
    - Development: apt install libgtk-3-dev libwebkit2gtk-4.0-dev
    - Production: apt install libgtk-3-0 libwebkit2gtk-4.0-37


CmakeLists:

- https://github.com/Soundux/webviewpp/blob/master/CMakeLists.txt
- https://github.com/webview/webview/issues/131
- https://alandefreitas.github.io/moderncpp/guis/webview/
- https://stackoverflow.com/questions/66146868/how-do-you-build-webview-on-windows-using-cmake


- 2022-11-25 [Github Actions: Run step on specific OS - Stack Overflow](https://stackoverflow.com/questions/57946173/github-actions-run-step-on-specific-os)
```
if: matrix.os == 'ubuntu-latest'
```
