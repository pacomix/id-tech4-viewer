id-tech4-viewer is a small project coded during the spring/summer of 2010 for learning purposes.
It is a map viewer for games developed with the id Tech 4 game engine coded from scratch.

Features implemented include:
* Portals
* DirectX 9 renderer (lol) Why not?
* Binary space partitioning
* Parallax texture rendering, bumpmapping, lights, etc...
* Multi camera system.
* Diverse debug rendering for lights, volumes, portals, camera frustum culling, etc...

# About
Everything started as an attempt to learn DirectX programming since my background back then consisted of J2ME game programming (the cut version of Java for Mobile phones) and Nintendo DS.
My only help was the help files of DirectX and some internet forums but... did you know that even back then it was already difficult to find useful information in internet? ;)
And yes as you can see the quality of the code is very low taking in account the context in which it was developed.

Setting up artificial scenarios for testing the different concepts I was learning was a bit tedious and, at least for me, it wasn't possible to source good ones so I remembered that I used to hack games by peeking inside the files they contain. Depending on the game that could mean changing SPAWN locations, change the music/FX (you can turn a serious game into a comedy with that one lol) or changing game events (Blade: The Edge of Darkness counted with a vast script collection written in Python), etc...  In my search for texture files and so on I accidentally realized that the vast majority of Doom3/Quake IV maps apparently were in plain text files so I started pulling the string and I ended up building a pretty decent viewer for, in theory, map games developed with the id tech 4 engine.

With some patience I reverse engineered all the map format, the material system, portals, etc... although I searched for some help specially with the interpretation of the values of the BSP Tree used for the collisions and the meaning of the areas where the geometry resides. So yes this means that there is a portal system implemented.

Additionally I came across with a set of textures (wulfen texture pack) that included parallax so I added support for it too together with cheap light reflections :P. All the pixel/vertex shaders have been developed by me too.

This repository contains solution and project files adapted to work with Visual Studio 2019 and the old VS2008/2010 platform toolsets so if you want to build it its up to you to configure the environment in order to build it yourself.

The entire code was developed back then using Windows XP in a hacked MacMini that had an integrated nVidia card with shared system memory. I was lucky since it supported the Shader Model 3 so developing shaders was kind of pleasant in comparison with older versions although due to the graphic performance I had to optimize everything as much as possible.


# Building
You need Visual Studio 2019 with the old platform toolset vs2008/2010 and DirectX 9 latest SDK.

* Open the Gae solution file and build.
* Open the MapViewer solution file and build.
* Search for the executables in the Output folder and the shader folder and copy them to the installation directory of Quake 4/Doom 3
* Unzip the game .pak files taking as root the installation folder of the games.
* It is recommended that the Wulfen texture packs are there too.
* Go to a command line and type: MapViewer.exe game/maps/${MAP_NAME} (please take into account you shouldn't write the extension of the map file just the base name instead)

