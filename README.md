# FGInfiniteRunnerGame
 
This is my base for the endless runner assignment, my first rendition. I have run into project-related
corruption issues that have set me back during development, so many of my desired features are yet to be
introduced.

Find my binary under the shipping version Windows 10 package under Releases tab as a ZIP.

## Part 1

OBSTACLES - I currently have two spawnable obstacle classes - both are almost identical with the exception that they inflict
different types of damage. These are SmallObstacle and LargeObstacle. In future releases I plan on having an obstacle class which they inherit from which already have
these fields accessible. The small obstacles use the OnOverlap functionality to reduce the player's
health upon collision. 

LEVEL - My main way of spawning the level also utilizes the OnOverlap functionality inside of Unreal, in which a box component
detects player collision and spawns a new chunk in front of the current one, effectively creating an infinite loop.
Since I am moving the player and not the level, some areas like the skybox bug out when you achieve a certain score.
The level tiles are currently spawned randomly through an array, but going forward I only want the floor to be spawned
this way, whereas the planned coins and obstacles will be generated separately in a random fashion, instead of having
preset level chunks that the player can familiarize themselves with.

PLAYER - The player character is constantly moving forward in the world space, and with future releases I am planning on having
a lane-switch system instead of the free movement (including jumping) that the game has right now.

GAME MANAGER - My game manager script handles the scoring and lives of the player, and is the main access point for
other classes. Some functionality is temporarily in other classes (such as spawning the initial tile when game starts),
but the plan is to have this as the main access point. I could not get UI to display lives lost and score increasing over time,
but that is the focus going forward as there is no communication given to the player outside of debug messages in their place.

I have run into some issues with inheriting from the correct base classes when making my game components, and have found
it to be particularly difficult to handle UI through C++.

## Part 2

