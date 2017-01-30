cd ..

"C:/Program Files/7-Zip/7z" x _extlibs-win.zip

copy _extlibs-win\SDL_image-1.2.12\lib\x86\*.dll *.* /y
copy _extlibs-win\SDL_mixer-1.2.12\lib\x86\*.dll *.* /y
copy _extlibs-win\SDL_ttf-2.0.11\lib\x86\*.dll *.* /y
copy _extlibs-win\SDL-1.2.15\bin\*.dll *.* /y

