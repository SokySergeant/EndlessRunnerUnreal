# EndlessRunner

PART 2:

The spec changes:
I started with the spec change to the obstacles since it seemed like the easier of the two. The way I decided to solve this problem was by having a second box collider on every obstacle (the first one being the one encompassing the obstacle mesh and damaging the player upon collision). This second box collider is placed slightly behind the obstacle, so that the player collides with it only after having either hit or dodged the obstacle. When this second collider is touched, it will run a check to see if the obstacle was hit or not. If it wasn't hit, it'll broadcast a delegate which will inform its level segment to destroy one of its obstacles, with a 25% chance of occuring. This value can be changed. When an obstacle gets destroyed, it will also play an explosion particle effect.
The second requirement, adding another player, proved to be really difficult to me. I spent a lot of time trying to spawn in two player actors and controllers for them within a GameMode class, as to have them use different input devices (i.e. one player on keyboard, other player on controller). After a lot of trying, testing, and googling, me and some colleagues found out that there is actually a bug within the 5.1 version of the engine that basically makes handling multiple input devices impossible. The workaround I came up with is as such: The first player gets spawned in normally, and it will instead spawn in another character. All inputs are handled by the first player; inputs for the second player will get passed down to it from the first player. This way, only one player controller is required, and both players can play on the same input device. There was also a some design challenges to having two players, such as "When should the game end?", "How does the highscore work?", or "Do you collaborate or are you playing against each other?". I decided that the two players would be fighting over who can get most score, and that each player would have their own highscore value. The game would end when both players have died, and the player with most score would win. 

I've also changed the contols a bit:
- Player 1: A / D to move left / right, W to jump
- Player 2: Left arrow / right arrow to move left / right, up arrow to jump

Other improvements and things I changed:
- Made the Obstacle class inherit from the PickupBase since they both handled very similar cases (colliding with player, hiding self, and performing an action upon collision).
- I had to change the way I stored certain info because of the requirement of having two players. For example, the way I previously stored the location of lanes was within a GameInstance, but since now each player needs to have their own set of lanes, I had to move those values to within the player itself. 
- I also had to change how the LevelSegmentsManager gets spawned in. Instead of having one already in the level, the player will now instead spawn its own manager beneath it.
- Made the player model disappear when they died.
- Added a saving and loading system to locally store the highscore of each player. This is done using the UGameplayStatics save game functions.
- Added a little fluff to the UI, such as showing the highscores of the two players on the main menu, and a piece of text on the game over widget that says which player won (whoever has most score after both players die).
- I added a value for the max amount of obstacles that can be spawned on each level segment, so that the map doesn't become extremely cluttered after playing for a while.
- Added a projectile obstacle that will move towards the player at a given speed.
