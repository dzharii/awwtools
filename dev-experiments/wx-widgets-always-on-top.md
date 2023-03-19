> are there any open-source frameworks or libraries that allows to create a Desctop HUD in C++?

There are several open-source frameworks or libraries that can help you create a desktop HUD in C++. [One of them is **openFrameworks**, which is a C++ toolkit for creative coding](https://en.wikipedia.org/wiki/OpenFrameworks)[1](https://en.wikipedia.org/wiki/OpenFrameworks)[2](https://github.com/openframeworks/openFrameworks). [It runs on various platforms, including Windows, macOS, Linux, iOS and Android](https://en.wikipedia.org/wiki/OpenFrameworks)[1](https://en.wikipedia.org/wiki/OpenFrameworks). [Another option is **wxWidgets**, which is a cross-platform GUI toolkit that supports OpenGL](https://www.slant.co/topics/12868/~cross-platform-c-gui-toolkits)[3](https://www.slant.co/topics/12868/~cross-platform-c-gui-toolkits). [It also runs on multiple platforms, such as Windows, Linux, macOS and more](https://www.slant.co/topics/12868/~cross-platform-c-gui-toolkits)[3](https://www.slant.co/topics/12868/~cross-platform-c-gui-toolkits).

What kind of desktop HUD do you want to create? What features are you looking for in a framework or library?



>  how do I create a borderless window in wxWidgets, transparent background and always on top?

[To create a borderless window in wxWidgets, you can use **wxFrame::SetWindowStyleFlag** with **wxFRAME_NO_TASKBAR** and **wxFRAME_SHAPED** flags](https://stackoverflow.com/questions/5082507/wxwidgets-creating-a-borderless-frame-movable)[1](https://stackoverflow.com/questions/5082507/wxwidgets-creating-a-borderless-frame-movable). [To make it transparent, you can use **wxTopLevelWindow::SetTransparent** with a value between 0 (invisible) and 255 (opaque)](https://forums.wxwidgets.org/viewtopic.php?t=12369)[2](https://forums.wxwidgets.org/viewtopic.php?t=12369). [To make it always on top, you can use **wxSTAY_ON_TOP** flag](https://www.ghacks.net/2020/06/01/set-any-window-to-stay-on-top-shrink-transparent-or-dark-mode-with-windowtop/)[3](https://www.ghacks.net/2020/06/01/set-any-window-to-stay-on-top-shrink-transparent-or-dark-mode-with-windowtop/).

[To make a panel transparent, you can access the background bitmap directly and draw it on the panel’s paint event handler](https://forums.wxwidgets.org/viewtopic.php?t=39939)[4](https://forums.wxwidgets.org/viewtopic.php?t=39939)[5](https://forums.wxwidgets.org/viewtopic.php?t=39939&start=15). [Alternatively, you can use a PNG image with transparency and draw it on the panel using **wxDC::DrawBitmap**](https://stackoverflow.com/questions/3581955/wxwidgets-transparent-background-image)[6](https://stackoverflow.com/questions/3581955/wxwidgets-transparent-background-image).

Do you need more help with this task? Do you have any other questions about wxWidgets?



