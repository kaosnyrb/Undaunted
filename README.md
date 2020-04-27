<p align="center">
  <img src="./docs/logo.jpg" alt="Size Limit CLI" width="720">
</p>

# Undaunted
SkyrimSE Mod for Dynamic Quest/bounties.
Inspired by the ESO Guild The Undaunted and the Diablo 3 Adventure mode.

I wanted to see if I could dynamically create quest content without having to place thousand of markers over skyrim.
When the bounty is started the SKSE Plugin loads the Region data and grabs all the worldspaces and the unk088 cells which appears to contain all the persistant references in the worldspace.

It then filters these down by Worldspace name as most of the worldspaces are one off quest spaces.

Next it selects a random persistant reference and moves an XMarker to that location. The XMarker is the objective of the Quest and it's used as the target of the placeatme calls.

Finally any of the enemies created are stored in memory in the plugin and checked every 20 seconds to see if they are dead. If they are the objective is marked as completed.

As the mod is using the Region data from memory any mods which add these would also be accessable from this mod. Currently there is a property on the Pillar Activator which says which worldspace to spawn the bounty in.


## Requirements
* SKSE SE build 2.0.17 (runtime 1.5.97)
* JContainers (Currently bundled with the release)

## How to play

1. Visit the Undaunted Camp northwest of Windhelm.
2. Use the Pillar to start a bounty.
3. A Quest will start called Undaunted.
3. Somewhere in the Skyrim Overworld a group of enemies will appear with a quest marker pointing to them
4. Go and kill the enemies. It doesn't matter who gets the killing blow, just that they die.
5. Return to the Undaunted camp to pickup another bounty and return to step 3.


## Spawning Groups

* Bandits
* Vampires
* Draugr
* Falmer
* Dwemer
* Necromancers


## What the mod can do

* Dynamically place forms in any Cell in any Worldspace. Skyrim's main world has roughly 18,000 different places this mod can spawn.
* Code for placing in Interior cells also exists, but currently isn't used to stop bandit raids of Belethor's General Goods.
* Json Configuration for Enemy groups. Currently using default levelled lists but any mod that edits them should work with Undaunted.
* Quest tracking for dynamically placed enemies.
* Very mod compatible, currently only adds 2 items to the world.

## Known issues/WIP

* Rewards other than the thrill of killing
* Bounty tracking doesn't persist between sessions. Meaning you can only complete a bounty in the session you started it.
* Support for Mod created enemies that aren't placed into the default leveled lists.
* More advanced encounters. This system is able to spawn objects with scripts attached to them. (Oblivion gates...?)
