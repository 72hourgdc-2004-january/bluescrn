
The code is pretty messy as it was so rushed.

Things go wrong in optimized release builds - there's a nasty bug somewhere :(

Should be easyish to port to Linux, if you've got compatible SDL, DevIL, and FMOD libs.
Just remove the windows-specific frame rate limiting, and the one MessageBox() in main.cpp!
(Might have problems with slashes in paths??)