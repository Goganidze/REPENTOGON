# Callbacks
## ModCallbacks.MC_PRE_ADD_COLLECTIBLE(CollectibleType Type, int Charge, bool FirstTime, int Slot, int VarData)
Optional argument: `CollectibleType Type`
Accepts a table of parameters: `{Type, Charge, FirstTime, Slot, VarData}`

Example, which turns every item into Money = Power: `return {CollectibleType.COLLECTIBLE_MONEY_EQUALS_POWER, Charge, FirstTime, Slot, VarData}`

## ModCallbacks.MC_POST_TAKE_DMG(Entity Ent, float Damage, int DamageFlags, EntityRef Source, int DamageCooldown)
Accepts no return parameters.

## ModCallbacks.MD_GRID_ROCK_UPDATE(GridEntityRock Rock)
Accepts no return parameters.

## ModCallbacks.MC_HUD_UPDATE()
Accepts no return parameters.

## ModCallbacks.MC_HUD_POST_UPDATE()
Accepts no return parameters.

## ModCallbacks.MC_HUD_RENDER()
Accepts no return parameters.

## ModCallbacks.MC_CHARACTER_MENU_RENDER()
Accepts no return parameters.

## ModCallbacks.MC_PRE_SFX_PLAY(int ID, float Volume, int FrameDelay, bool Loop, float Pitch, float Pan)
Accepts a table of parameters: `{ID, Volume, FrameDelay, Loop, Pitch, Pan}`

Example, which forcibly loops every sound: `return {ID, Volume, FrameDelay, true, Pitch, Pan}`

Alternatively accepts `false` to cancel the sound

## ModCallbacks.MC_POST_SFX_PLAY(int ID, float Volume, int FrameDelay, bool Loop, float Pitch, float Pan)
Accepts no return parameters.

## ModCallbacks.MC_PRE_MUSIC_PLAY(int ID, float Volume OR float FadeRate (see below), bool IsFade)
Accepts a table of parameters: `{ID, Volume}`

Example, which replaces every song with the Flooded Caves track: `return {Music.MUSIC_FLOODED_CAVES, Volume}`

Alternatively accepts a single number to change the track without changing volume (effectively shorthand for the above)

Alternatively accepts `false` to cancel the track

This callback is called for *both* MusicManager::Play *and* MusicManager::Crossfade! `IsFade` is provided to distinguish the two.


## ModCallbacks.MC_PRE_ENTITY_THROW(Vector Velocity)
Accepts a `Vector` which will modify the velocity of the thrown entity.

## ModCallbacks.MC_POST_ENTITY_THROW(Entity ThrownEntity, Vector Velocity)
Accepts no return parameters.

## ModCallbacks.MC_PLAYER_INIT_POST_LEVEL_INIT_STATS()
Accepts no return parameters.

## ModCallbacks.MC_PRE_ROOM_EXIT(EntityPlayer Player, Bool Unknown)
Accepts no return parameters.

## ModCallbacks.MC_PRE_LEVEL_INIT()
Accepts no return parameters.

## ModCallbacks.MC_PRE_TRIGGER_PLAYER_DEATH(EntityPlayer Player)
Accepts `false` to cancel the death, reviving the player in-place, `true` or `nil` to allow the death to go through.

**BUG!** Much like the vanilla Lua Revive() function, this removes the current run's ability to save. This is being investigated.

## ModCallbacks.MC_PRE_RESTOCK_SHOP(bool Partial)
Accepts `false` to cancel the restock, blocking shop rerolls from restock machines or restocks from Restock altogether.

This callback is called for *both* Room::ShopRestockFull *and* Room::ShopRestockPartial! `Partial` is provided to distinguish the two.

## ModCallbacks.MC_POST_RESTOCK_SHOP(bool Partial)
Accepts no return parameters.

This callback is called for *both* Room::ShopRestockFull *and* Room::ShopRestockPartial! `Partial` is provided to distinguish the two.

## ModCallbacks.MC_PRE_CHANGE_ROOM(int TargetRoomIdx, int Dimension)
Accepts a table of parameters: `{TargetRoomIdx, Dimension}`

## ModCallbacks.MC_POST_PICKUP_SHOP_PURCHASE(EntityPickup Pickup, EntityPlayer Player, int MoneySpent)
Accepts no return parameters.

## ModCallbacks.MC_PRE_PICKUP_MORPH(EntityPickup Pickup,int EntityType,int Variant,int SubType,bool KeepPrice,bool KeepSeed,bool IgnoreModifiers)
Accepts a table of parameters: `{EntityType, Variant, SubType, KeepPrice, KeepSeed, IgnoreModifiers}` or just `{EntityType, Variant, SubType}`. Returning `false` cancels the morph.

## ModCallbacks.MC_PRE_NPC_MORPH(int EntityType,int Variant,int SubType,int Championid)
Accepts a table of parameters: `{EntityType, Variant, SubType, Championid}` or just `{EntityType, Variant, SubType}`. Returning `false` cancels the morph.

# Functions
## PosVel EntityPlayer:GetMultiShotPositionVelocity(int LoopIndex, WeaponType WeaponType, Vector ShotDirection, Vector ShotSpeed, MultiShotParams Params)
This magically vanished from the API some time after 1.7.8.

Compared to the vanilla function, this implementation has been further augmented to throw an error if LoopIndex is higher than MultiShotParams.NumTears.

MultiShotParams has not been directly exposed to the API yet- it's on the to-do list, for now any mods that want to experiment with this can use this error in pcall to get the amount of tears.

## bool Game:AchievementUnlocksDisallowed()
Returns `true` if achievements can't be unlocked this run (challenges, seeded, etc)

## RoomConfigHolder Game:GetRoomConfigHolder()

## RoomConfigRoom RoomConfigHolder::GetRoomByStageTypeAndVariant(StbType Stage, int Type, int Variant, int Difficulty)
Returns a RoomConfigRoom corresponding to the given params.

## LevelGeneratorEntry Isaac.LevelGeneratorEntry
Fetches a blank LevelGeneratorEntry from the game.

## void LevelGeneratorEntry:SetColIdx(LevelGeneratorEntry Room, int ColIdx)

## void LevelGeneratorEntry:SetLineIdx(LevelGeneratorEntry Room, int ColIdx)

## void LevelGeneratorEntry:SetAllowedDoors(LevelGeneratorEntry Room, int Doors)
I *believe* Doors here is a bitmask (1 << DoorSlot)

## bool Level:PlaceRoom(LevelGeneratorEntry Room, RoomConfigRoom RoomConfig, int Seed)
Places a room into the game. Returns `true` if successful.

## PlayerManager Game:GetPlayerManager()

## EntityPlayer PlayerManager:FirstCollectibleOwner(CollectibleType Collectible, RNG RNG, bool Unknown)
Returns `nil` if the specified collectible has never been picked up.

## EntityPlayer PlayerManager:SpawnCoPlayer2(PlayerType PlayerType)

## EntityPlayer EntityPlayer:InitTwin(PlayerType PlayerType)
I believe this is used by J&E, Strawman etc

## void EntityPlayer:InitPostLevelInitStats()
Call this after spawning characters with "special" tears (Forgotten, Lilith, Azazel etc) with InitTwin, or they won't have their proper tear type.

## void EntityPlayer:SetItemState(CollectibleType CollectibleType)
This is used for thrown/2-input items (e.g. Bob's Rotten Head).

## PersistentGameData Isaac:GetPersistentGameData()
Should probably be moved to Game for consistency.

## bool PersistentGameData:TryUnlock(int Unlock)
Returns `true` if successful, `false` if unlocking failed or the secret was already unlocked.
Could *maybe* benefit from an enum?

## Console Game:GetConsole()

## void Console:PrintError(string Error)
Prints an error to the console.

# Enumerations
## StbType
Taken by RoomConfigHolder::GetRoomByStageTypeAndVariant.

## BossType
Return value of Entity::GetBossID and Room::GetBossID.

## NullPickupSubType
5.0.x

## KnifeVariant

## KnifeSubType
*VERY* incomplete.

## SlotVariant

## RoomSubType
Excludes Super Secret Room backdrop (use [BackdropType]), and Boss Room boss (use [BossType]).