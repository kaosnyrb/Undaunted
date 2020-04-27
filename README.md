<p align="center">
  <img src="./docs/logo.jpg" alt="Size Limit CLI" width="720">
</p>

# Undaunted
SkyrimSE Mod for Dynamic Quest/bounties.

## Requirements
* SKSE SE build 2.0.17 (runtime 1.5.97)
* JContainers (Currently bundled with the release)

## How It Works

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

* Dynamiclly place forms in any Cell in any Worldspace. Skyrim's main world has roughly 18,000 different places this mod can spawn.
* Json Configuration for Enemy groups. Currently using default levelled lists but any mod that edits them should work with Undaunted.
* Quest tracking for dynamiclly places enemies.
* Very mod compatible, currently only adds 2 items to the world.

## Known issues/WIP

* Bounty tracking doesn't persist between sessions. Meaning you can only complete a bounty in the session you started it.
* Support for Mod created enemies that aren't placed into the default leveled lists.
* More advanced encounters. This system is able to spawn objects with scripts attached to them. (Oblivion gates...?)
