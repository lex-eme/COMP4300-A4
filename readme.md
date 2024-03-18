Assets:
- Entities in the game will be rendered using various Textures and Animations which we will be callin Assets (along with Fonts and Sounds)
- Assets are loaded once at the beginning of the program and stored in the Assets class, which is stored by the GameEngine class
- All Assets are defined in assets.txt, with the syntax defined below

Player:
- The player Entity in the game is represented by NotLink, which has several different Animations: RunDow, RunUp, StandDown, StandUp, StandRight, AtkUp, AtkDOwn, AtkRight. You must determine which direction and state the player is currently in and assign the correct Animation.
- The player is assigned the following animations in the direction facing.
Please note that left wil be accomplished by mirroring Right (x scale -1)
StandDir    - When no input (or both opposite) is being given to the player
RunDir      - When move input is given to the player in a direction
AttackDir   - When the player's sword is currently visible from an attack
- The player moves with the following controls:
  Left: Q, Right: D, Up: Z, Down: S, Attack: Space key
- The player can move up, left, down, right, at any time during the game
- The player can only move either horizontally or vertically at a given time
- If opposite directions are held, the player will be given a Stand anim
- The player collides with 'Tile' entities in the level (see level syntax) whose bounding box blocks movement and cannot move through them.
- The player will be given a CBoundingBox of a size specified in the level file
- The player will be given a CHealth component with a max health that is specified in the level file.
- When the player's current health is <= 0, the player respawns, and the LinkDie sound should play.
- If the player has a CInvincibility component, it takes no damage from enemies and its sprite is made transparent. The duration of the component is reduced each frame, and when the duration reaches 0, the component is removed, making it vulnerable to attacks again.
- When the player collides with an enemy while not invincible:
  - player's current health is reduced by the enemy damage component
  - the LinkHit sound should player
  - player is given a CInvincibility component which lasts for 30 frames

Other Entities:
- Each tile and npc in the game specifies two boolean values of the whether it blocks movement and whether it blocks vision. If a tile blocks movement, the player collides with it and cannot pass through. If an entity blocks vision then it affects enemy line of sight. See spec below.
- CBoundingBox has two new booleans to reflect these changes.
- When the player steps onto a tile with the "Black" animation, they are teleported to a random "Black" tile on the map.

Attacking;
- When the player attacks, a sword appears for 10 frames and then disappears approximately 1 tile away from the player in the direction they are facing.
- The player's sword should be given a bounding box equal to the anim size.
- When the sword collides with an enemy, it deals its damage component worth of damage to the health of the enemy that it collides with. The sword should only do damage for one frame, and then its damage component should be removed so that it does not deal damage for every frame it overlaps.
- When the sword is swung, the Slash sound should play.
- When the sword collides with an enemy, the EnemyHit sound should play.
- When an enemy's health is reduced to 0, it is destroyed.
- When an enemy is destroyed, the EnemyDie sound should play.

NPC Entities:
- npc entities will each be given an 'ai' behavior in the level file, either 'Follow' or 'Patrol'.
- 'Follow' means it will follow the player when the player is in direct line of sight, or head back to its original position when not in line of sight.
- 'Patrol' means it will move toward a series of patrol positions, looping back around to the start when the final one is reached. An npc has reached its patrol position if it is within a distance of 5 from it.

Tiles:
- Tiles are Entities that define the level geometry and interact with players.
- Tiles can be given a CBoundingBox equal to the size of the animation tile->get<CAnimation>().animation.getSize()
- The current animation displayed for a tile can be retrieved with: tile->get<CAnimation>().animation.getName()
- If a tile is given the Heart animation, any npc that collides with it should have its health filled to maximum, and the tile is destroyed.

Drawing:
- The game window is given a fixed size of 1280x768 pixels (20x12 tiles).
- Rendering of the entities is provided for you, as well as a 'debug' rendering mode.
- You are required to change the camera position (window.view()) of the game window to alternate between two modes based on the m_Follow variable
  true: Follow mode - the camera is always centered on NotLink.
  false: Room mode - the camera is centered on the middle of the room.

Rooms:
- Each level is split up into individual 'rooms' given an (rx, ry) coordinate.
- Room (rx,ry) is defined by the rectangle (rx*w, ry*h, w, h) where w and h are the window width and height respectively.
- Each tile is given a (tx, ty) grid position within a room. By default, the grid cells and all tiles have size 64x64 pixels.
- When NotLink moves to a new toom, the camera should center to that room.

GUI:
- You must implement the "Animations" and "EntityManager" tabs in ImGui so that they have the same functionality as the solution.

Misc:
- The 'P' key should pause the game.
- The 'C' key toggles debugging view.
- The 'T' key toggles texture rendering.
- The 'G' key toggles drawing the grid.
- The 'Y' key should toggle between follow camera or 'room' camera.
- The 'ESC' key should go 'back' to the Main Menu, or quit if on the Main Menu.


----------------------------------
Assets File Specification
----------------------------------

There will be three different line types in the Assets file, each of which correspond to a different type of Asset. They are as follows:

Texture Asset Specification:
Texture N P
  Texture Name      N     std::string (it will have no spaces)
  Texture FilePath  P     std::string (it will have no spaces)

Animation Asset Specification:
Animation N T F S
  Animation Name    N     std::string (it iwll have no spaces)
  Texture Name      T     std::string (refers to an existing texture)
  Frame Count       F     int (number of frames in the Animation)
  Anim Speed        S     int (number of game frames between anim frames)

Font Asset Specification:
Font N P
  Font Name         N     std::string (it will have no spaces)
  Font File Path    P     std::string (it will have no spaces)

Sound Asset Specification:
Sound N P
  Sound Name         N     std::string (it will have no spaces)
  Sound File Path    P     std::string (it will have no spaces)

----------------------------------
Level Specification File
----------------------------------

Player Specification:
Player X Y BX BY S H
  Spawn Position      X   Y       int, int
  Bounding Box Size   BX  BY      int, int
  Speed               S           float
  Max Health          H           int

Tile Specification:
Tile Name RX RY TX TY BM BV
  Animation Name    Name    string
  Room Coordinate   RX RY   int, int
  Tile Position     TX TY   int, int
  Blocks Movement   BM      int (1 = true, 0 = false)
  Blocks Vision     BV      int (1 = true, 0 = false)

NPC Specification:
NPC Name RX RY TX TY BM BV H D AI ...
  Animation Name    Name    string
  Room Coordinate   RX RY   int, int
  Tile Position     TX TY   int, int
  Blocks Movement   BM      int (1 = true, 0 = false)
  Blocks Vision     BV      int (1 = true, 0 = false)
  Max Health        H       int > 0
  Damage            D       int > 0
  AI Behavior Name  AI      string
  AI Parameters     ...     (see below)

AI = Follow
  ... = S
  Follow Speed      S       float (speed to follow player)

AI = Patrol
  ... = S N X1 Y1 X2 Y2 ... XN YN
  Patrol Speed      S       float
  Patrol Position   N       int (number of patrol positions)
  Position 1 - N    Xi Yi   int, int (Tile position of Patrol Position i)

----------------------------------
  Misc Notes
----------------------------------

1.  The m_Player variable has been removed and replaced with the player() function, which looks in the EntityManager for a player and returns it.
    This is slightly less efficient, but allows us to put in sanity checks for things like checking to see if a player object exists before using it, etc.

2.  Entity getComponent, addComponent, etc have been shorted to get, add, etc.

3.  sf::Sound objects can be played with .play() and stopped with .stop().
    However in order to play, the Sound object must persist for at least the duration of the sound that you want to be played.
    If you simply allocate a local Soung object and then call sound.play(), it will stop playing as soon as the SOund object goes out of scope.
    Calling sound.play() on a Sound object that is already playing will restart the sound, not start playing another one.

----------------------------------
  Assignment Hints
----------------------------------

You can use all of your collision code from assignment 3 in this assignment.

I recommend approaching this assignment in the following order:

0.  Implement the GUI whenever you feel you'd like to have it for debugging.
1.  Implement ZQSD movement for the player such that it moves u/l/d/r when thos keys are pressed. This will allow you to test rectangular collisions as soon as possible.
2.  Implement Scene_Zelda::loadLevel(), which will allow you to load the assets for the level given to you to test the rest of the assignment.
    To do this, you will need to implement getPosition().
3.  Implement spawnPlayer() so that it correctly uses the configuration specified by the level file so you can start testing collisions.
4.  Implement the camera changes described above.
5.  Implement spawnSword() so that it spawns a sword in the correct position with correct lifespan.
6.  Implement player attacking / sword collision with NPCs / health / damage.
7.  Implement player animations to correctly animate based on player state.
8.  Implement NPC Patrol behavior.
    Implement NPC Follow behavior without vision.
9.  Implement Physics::LineIntersect() which will allow you to test whether or not two line segments AB and CD intersect, and where they intersect.
10. Implement PHysics::EntityIntersect() which will allow you to test whether line segment AB intersects with any lines of the bounding box of entity e.
11. Implement NPC Follow behavior using 10 to check line of sight.
12. Implement invicibility frames.
13. Implement heart pickups.
14. Implement black tile teleporting.
