# EndlessRunner
 
PART 1:

The game I have made is an endless runner with three lanes akin to Subway Surfers. The player starts with 3 health and loses one every time they hit an obstacle. There is a health pickup which will restore 1 health point. The player can switch lanes left and right with A and D, and they can also jump with the spacebar. Upon death, the player is prompted to go back to the main menu where they can play another round. The platforms, obstacles, and pickups are randomly generated. The player stands still while the environment moves past them to simulate running. 

The system is split up like this:
- LevelSegmentManager, which holds info about what level segments it's allowed to spawn, the speed to move the segments at, a difficulty value that determines how many obstacles should spawn on each segment, and the score. It also updates the score and speed values shown on the HUD.
- LevelSegment, which holds info about which obstacles and pickups it's allowed to spawn, as well as a frequency value for both of them, i.e. a level segment with an obstacle frequency value of 0.1 will spawn 10% as many as one with a frequency value of 1.0.
- Obstacle / Pickup, an actor that will become hidden upon the player touching it, and do some sort of action, such as increasing / decreasing health. There is a PickupBase class which handles collision with the player. Pickups derive from this class and can override a function to have different things happen upon collision.
- EndlessRunnerCharacter, the player, which takes care of inputs, changing lanes, and health.

There are a few other smaller classes as well, such as InGameWidget which just exists to guarantee that certain text boxes exist in a widget, so that for example, the level segment manager class can update its value with an amount of score. This is done using "meta = (BindWidget)". Both the level segment manager and the level segment classes utilize pooling for performance reasons. They hold a TArray of their already spawned actors so they can be reused, instead of deleting and respawning them all the time. The speed the environment scrolls past the player at and the amount of obstacles spawned on each segment increases as the game goes on. There are values that can be changed to have them increase faster / slower. Blueprints were also used for a few things I deemed too trivial to do in C++, such as making the start button open the second level, or spawning the in game HUD on the level's BeginPlay. Information that is needed by many different classes such as the position of each lane is stored in a custom game instance.

Known issues / things to add:
- Replace some TArrays with Tsets for optimization
- Player continues doing a run animation when they die and they can still change lanes
- Add projectiles / enemies (IMPORTANT)
- Store a high score locally and update it
- Add a value for max amount of obstacles to be spawned, as it becomes impossible to traverse the segments after a while
