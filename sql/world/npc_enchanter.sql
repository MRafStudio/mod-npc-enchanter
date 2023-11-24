-- --------------------------------------------------------------------------------------
--	ENCHANTER - 601015
-- --------------------------------------------------------------------------------------
SET
@Entry 		:= 601015,
@Model 		:= 9353, -- Undead Necromancer
@Name 		:= "Beauregard Boneglitter",
@Title 		:= "Enchantments",
@Icon 		:= "Speak",
@GossipMenu := 0,
@MinLevel 	:= 80,
@MaxLevel 	:= 80,
@Faction 	:= 35,
@NPCFlag 	:= 1,
@Scale		:= 1.0,
@Type 		:= 7,
@TypeFlags 	:= 0,
@FlagsExtra := 2,
@AIName		:= "SmartAI",
@Script 	:= "npc_enchantment";

-- NPC
DELETE FROM creature_template WHERE entry = @Entry;
INSERT INTO creature_template (`entry`, `modelid1`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `unit_class`, `unit_flags`, `type`, `type_flags`, `RegenHealth`, `flags_extra`, `AiName`, `ScriptName`) VALUES
(@Entry, @Model, @Name, @Title, @Icon, @GossipMenu, @MinLevel, @MaxLevel, @Faction, @NPCFlag, 1, 1.14286, @Scale, 1, 2, @Type, @TypeFlags, 1, @FlagsExtra, @AIName, @Script);

-- NPC Locale
DELETE FROM `creature_template_locale` WHERE entry = @Entry;
INSERT INTO `creature_template_locale` (`entry`, `locale`, `Name`, `Title`, `VerifiedBuild`) VALUES (@Entry, 'ruRU', 'Борегар Бонеглиттер', 'Чародей', 1);

-- NPC EQUIPPED
DELETE FROM `creature_equip_template` WHERE `CreatureID`=@Entry AND `ID`=1;
INSERT INTO `creature_equip_template` VALUES (@Entry, 1, 11343, 0, 0, 18019); -- Black/Purple Staff, None

-- NPC TEXT
DELETE FROM `npc_text` WHERE `ID`=@Entry;
INSERT INTO `npc_text` (`ID`, `text0_0`) VALUES (@Entry, 'Good day $N. Beauregard Boneglitter at your service. I offer a vast array of gear enchantments for the aspiring adventurer.');

-- NPC Text Locale
DELETE FROM `npc_text_locale` WHERE `ID`=@Entry;
INSERT INTO `npc_text_locale` (`ID`, `Locale`, `Text0_0`) VALUES (@Entry, 'ruRU', 'Добрый день, $N. Борегар Бонеглиттер к вашим услугам. Я предлагаю широкий выбор чар для снаряжения начинающего искателя приключений.');

-- NPC World ADD Allance
INSERT INTO `pb_acore_world`.`creature` (`guid`, `id1`, `id2`, `id3`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`, `CreateObject`, `Comment`) VALUES (3110427, 601015, 0, 0, 0, 0, 0, 1, 1, 1, -9129.79, 408.599, 94.2356, 0.471894, 300, 0, 0, 5342, 0, 0, 0, 0, 0, '', NULL, 0, NULL) ON DUPLICATE KEY UPDATE guid=guid;

-- NPC World ADD Horde
INSERT INTO `pb_acore_world`.`creature` (`guid`, `id1`, `id2`, `id3`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`, `CreateObject`, `Comment`) VALUES (3110426, 601015, 0, 0, 1, 0, 0, 1, 1, 1, 1495.6, -4436.38, 25.4739, 0.374149, 300, 0, 0, 5342, 0, 0, 0, 0, 0, '', NULL, 0, NULL) ON DUPLICATE KEY UPDATE guid=guid;
