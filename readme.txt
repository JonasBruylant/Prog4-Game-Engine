# Github
https://github.com/JonasBruylant/Prog4-Game-Engine

# Controls
## Menu
- W/S key or Dpad up/down: select button.
- Enter: Press selected button
- Enter when game has ended: Go back to main menu. 

## Game
- Dpad Left / A: Move left.
- Dpad Right / D: Move right.
- Dpad Up / W: Move up.
- Dpad Down / S: Move down.
- ButtonA (PS: Circle) / Spacebar: Spray Pepper.

# Libraries
- rapidjson: File Reading and riding.
- SDL2
- SDL Image: Rendering images.
- SDL Mixer: Loading and playing sounds.
- SDL TTF: Loading and rendering text.
- glm: Local/World Positions.
- ImGui

<br><br>
# Design choices and why
- Observer Pattern:
    - Decouples the code.
    - Easy to know what events have taken place.
<br><br>
- Service Locator:
    - For the soundmanager, allows easily to swap the implementation of the sound system.
    - Accessible anywhere.
<br><br>
- Factory pattern:
    - Centralized way to create objects.
    - Allows fast and controllable scaling in making said objects.
<br><br>
- Component pattern:
    - Allows for small and easy functionality on gameobjects.
    - Composition over inheritance!
    - Easily reusable across multiple gameobjects.
<br><br>
- JsonFileReader:
    - Used to quickly and clearly read in level data.
    - Quickly able to make iterations of levels.
