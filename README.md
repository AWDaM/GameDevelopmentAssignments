# Maid in Abyss

This is a little game made by two second year students of the Videogame Design and Development degree
It's just a small platformer with two levels, where the player can move right and left, jump, and dash.
The game has been made using C++, using SDL and pugi libraries. The maps have been made using Tiled.

2.0 Update:

We have improved the game by adding framerate cap and selection(at 30 by default), performance analyisis with brofiler,
and an entity system which can be seen at the pdf inside the release. We also have added enemies that can pathfind to the player,
and the hability to stop time.

3.0 Update

We have added an Intro screen before the game and UI. The UI System can be checked in the pdf file inside the release
In the Intro screen, the player can start a new gameload a saved game(if possible) change the volume or check the authors 
and the license. While playing, the player will have a clock that indicates the state of the ZA WARUDO hability
a lifebar with the remaining lifes, and counters for the time the pickups(we have also added those) and the score.

The score is calculated by the number of pickupsx100 - current timex10 - lost livesx50.

### Controls
-Movement with arrow keys.

-Jump with spacebar.

-Dash with X (now can be canelled, yay :D)

-ZA WARUDO(Time Stopping) with Z. The player stops time during a certain amount of time. This ability has a 10 second cooldown.

-Pause menu wit ESC

### Debug Controls
F1: Load the first level.
F2: Restart the current level.
F3: Load the second level.
F5: Load the saved game.
F6: Save the current game.
F8: View UI Logic
F9: View collider logic.
F10: Activate/Deactivate God Mode (infinite jump, cannot die when colliding with enemies).
F11: Disable/Enable 30fps cap

### Autors
-Jaume Sur�s Valhondo

-Lori�n Portella L�pez

### Github
-https://github.com/AWDaM/Maid-in-Abyss

### Disclosure

We do not own any of the sprites, tilesets, music or sfx presented in this game. Authors credited below

-Sprites: Mystical Chain by LION HEART: http://shakna.jp/
	Downloaded from http://spritedatabase.net/file/10214/Marisa_Kirisame

-UI Sprites: Golden UI bigger than ever edition by Buch: https://opengameart.org/users/buch
	Downloaded from https://opengameart.org/content/golden-ui-bigger-than-ever-edition
	
-Tilesets: Metroidvania Tileset v.2 by KIIRA https://opengameart.org/users/the-chayed-kiira
	Downloaded from https://opengameart.org/content/metroidvania-tileset-v2

-Music: 
	-Track 1: Made in Abyss OST: 13. Those Everyday Feels: https://www.youtube.com/watch?v=SF_tj5g9SIo
		Kevin Penkin: http://www.kpenkmusic.com/
	-Track 2: Made in Abyss OST: 2. The First Layer: https://www.youtube.com/watch?v=NSmukdqyvjU
		Kevin Penkin: http://www.kpenkmusic.com/

-SFX: Megaman ZX by Inti Creates and Capcom:http://inticreates.com/ && http://www.capcom.com/
	Downloaded from: https://www.vg-resource.com/thread-26823.html

### License

MIT License

Copyright (c) [2017] [Jaume Sur�s Valhondo, Lori�n Portella L�pez]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.