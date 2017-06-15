Put the Gish data files in this folder.

You can buy the game on these links:

1. [Gish on Chronic Logic site](http://www.chroniclogic.com/gish.htm)
2. [Gish on Steam](http://store.steampowered.com/app/9500)

```sh
cd assets_full/
ls -alhF
total 2.6M
drwxr-xr-x 18 exl users 4.0K May 23 15:49 ./
drwxr-xr-x 11 exl users 4.0K May 23 15:45 ../
drwxr-xr-x  2 exl users  20K May 23 14:35 animation/
drwxr-xr-x  2 exl users 4.0K May 23 14:35 data/
-rw-r--r--  1 exl users  15K May 23 14:35 gamecontrollerdb.txt
drwxr-xr-x  3 exl users 4.0K May 23 14:35 Gish.app/
-rw-r--r--  1 exl users 800K May 23 14:35 gish.exe
drwxr-xr-x  2 exl users 4.0K May 23 14:35 level/
drwxr-xr-x  2 exl users 4.0K May 23 14:35 music/
-rw-r--r--  1 exl users 112K May 23 14:35 OpenAL32.dll
drwxr-xr-x  2 exl users 4.0K May 23 14:35 player/
-rw-r--r--  1 exl users 1.6K May 23 15:49 ReadMe-GishData.md
drwxr-xr-x  2 exl users 4.0K May 23 14:35 replay/
-rw-r--r--  1 exl users 982K May 23 14:35 SDL2.dll
drwxr-xr-x  2 exl users 4.0K May 23 14:35 sound/
-rw-r--r--  1 exl users 184K May 23 14:35 steam_api.dll
-rw-r--r--  1 exl users    4 May 23 14:35 steam_appid.txt
drwxr-xr-x  2 exl users  12K May 23 14:35 texture/
drwxr-xr-x  3 exl users 4.0K May 23 14:35 tile01/
drwxr-xr-x  3 exl users 4.0K May 23 14:35 tile02/
drwxr-xr-x  3 exl users 4.0K May 23 14:35 tile03/
drwxr-xr-x  3 exl users 4.0K May 23 14:35 tile04/
drwxr-xr-x  3 exl users 4.0K May 23 14:35 tile05/
drwxr-xr-x  3 exl users 4.0K May 23 14:35 tile06/
drwxr-xr-x  3 exl users 4.0K May 23 14:35 tile07/
-rw-r--r--  1 exl users 400K May 23 14:35 wrap_oal.dll
```
Tested with data files from Gish 1.6 (non-steam, MS Windows), Gish 1.53 (non-steam, Linux) and Gish 1.7 (steam version).

Use [CMake](https://cmake.org/) to build Gish:

```sh
mkdir build; cd build
cmake -DCMAKE_BUILD_TYPE=Release -DGLES=Off ../gish/src/main/cpp/
make
```
Required dependencies: SDL2, OpenAL, OpenGL, PNG and Ogg Vorbis libraries.
