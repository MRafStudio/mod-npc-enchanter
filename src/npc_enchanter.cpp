/*

# Enchanter NPC #

_This module was created for [StygianCore](https://rebrand.ly/stygiancoreproject). A World of Warcraft 3.3.5a Solo/LAN repack by StygianTheBest | [GitHub](https://rebrand.ly/stygiangithub) | [Website](https://rebrand.ly/stygianthebest))_

### Data ###
------------------------------------------------------------------------------------------------------------------
- Type: NPC (ID: 601015)
- Script: npc_enchantment
- Config: Yes
- SQL: No


### Version ###
------------------------------------------------------------------------------------------------------------------
- v2019.04.15 - Ported to AC by gtao725 (https://github.com/gtao725/)
- v2019.02.21 - Add AI/Phrases/Emotes, Update Menu
- v2018.12.05 - Fix broken menu. Replace 'Enchant Weapon' function. Add creature AI and creature text.
- v2018.12.01 - Update function, Add icons, Fix typos, Add a little personality (Emotes don't always work)
- v2017.08.08 - Release


### CREDITS
------------------------------------------------------------------------------------------------------------------
![Styx](https://stygianthebest.github.io/assets/img/avatar/avatar-128.jpg "Styx")
![StygianCore](https://stygianthebest.github.io/assets/img/projects/stygiancore/StygianCore.png "StygianCore")

##### This module was created for [StygianCore](https://rebrand.ly/stygiancoreproject). A World of Warcraft 3.3.5a Solo/LAN repack by StygianTheBest | [GitHub](https://rebrand.ly/stygiangithub) | [Website](https://rebrand.ly/stygianthebest))

#### Additional Credits

- [Blizzard Entertainment](http://blizzard.com)
- [TrinityCore](https://github.com/TrinityCore/TrinityCore/blob/3.3.5/THANKS)
- [SunwellCore](http://www.azerothcore.org/pages/sunwell.pl/)
- [AzerothCore](https://github.com/AzerothCore/azerothcore-wotlk/graphs/contributors)
- [OregonCore](https://wiki.oregon-core.net/)
- [Wowhead.com](http://wowhead.com)
- [OwnedCore](http://ownedcore.com/)
- [ModCraft.io](http://modcraft.io/)
- [MMO Society](https://www.mmo-society.com/)
- [AoWoW](https://wotlk.evowow.com/)
- [More credits are cited in the sources](https://github.com/StygianTheBest)

### LICENSE
------------------------------------------------------------------------------------------------------------------
This code and content is released under the [GNU AGPL v3](https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3).

*/

#include "ScriptMgr.h"
#include "Configuration/Config.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GameEventMgr.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Unit.h"
#include "GameObject.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "InstanceScript.h"
#include "CombatAI.h"
#include "PassiveAI.h"
#include "Chat.h"
#include "DBCStructure.h"
#include "DBCStores.h"
#include "ObjectMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Player.h"

enum Enchants
{
    ENCHANT_WEP_BERSERKING = 3789,
    ENCHANT_WEP_BLADE_WARD = 3869,
    ENCHANT_WEP_BLOOD_DRAINING = 3870,
    ENCHANT_WEP_ACCURACY = 3788,
    ENCHANT_WEP_AGILITY_1H = 1103,
    ENCHANT_WEP_SPIRIT = 3844,
    ENCHANT_WEP_BATTLEMASTER = 2675,
    ENCHANT_WEP_BLACK_MAGIC = 3790,
    ENCHANT_WEP_ICEBREAKER = 3239,
    ENCHANT_WEP_LIFEWARD = 3241,
    ENCHANT_WEP_MIGHTY_SPELL_POWER = 3834, // One-hand
    ENCHANT_WEP_EXECUTIONER = 3225,
    ENCHANT_WEP_POTENCY = 3833,
    ENCHANT_WEP_TITANGUARD = 3851,
    ENCHANT_2WEP_MASSACRE = 3827,
    ENCHANT_2WEP_SCOURGEBANE = 3247,
    ENCHANT_2WEP_GIANT_SLAYER = 3251,
    ENCHANT_2WEP_GREATER_SPELL_POWER = 3854,
    ENCHANT_2WEP_AGILITY = 2670,
    ENCHANT_2WEP_MONGOOSE = 2673,

    ENCHANT_SHIELD_DEFENSE = 1952,
    ENCHANT_SHIELD_INTELLECT = 1128,
    ENCHANT_SHIELD_RESILIENCE = 3229,
    ENCHANT_SHIELD_BLOCK = 2655,
    ENCHANT_SHIELD_STAMINA = 1071,
    ENCHANT_SHIELD_TOUGHSHIELD = 2653,
    ENCHANT_SHIELD_TITANIUM_PLATING = 3849,

    ENCHANT_HEAD_BLISSFUL_MENDING = 3819,
    ENCHANT_HEAD_BURNING_MYSTERIES = 3820,
    ENCHANT_HEAD_DOMINANCE = 3796,
    ENCHANT_HEAD_SAVAGE_GLADIATOR = 3842,
    ENCHANT_HEAD_STALWART_PROTECTOR = 3818,
    ENCHANT_HEAD_TORMENT = 3817,
    ENCHANT_HEAD_TRIUMPH = 3795,
    ENCHANT_HEAD_ECLIPSED_MOON = 3815,
    ENCHANT_HEAD_FLAME_SOUL = 3816,
    ENCHANT_HEAD_FLEEING_SHADOW = 3814,
    ENCHANT_HEAD_FROSTY_SOUL = 3812,
    ENCHANT_HEAD_TOXIC_WARDING = 3813,

    ENCHANT_SHOULDER_MASTERS_AXE = 3835,
    ENCHANT_SHOULDER_MASTERS_CRAG = 3836,
    ENCHANT_SHOULDER_MASTERS_PINNACLE = 3837,
    ENCHANT_SHOULDER_MASTERS_STORM = 3838,
    ENCHANT_SHOULDER_GREATER_AXE = 3808,
    ENCHANT_SHOULDER_GREATER_CRAG = 3809,
    ENCHANT_SHOULDER_GREATER_GLADIATOR = 3852,
    ENCHANT_SHOULDER_GREATER_PINNACLE = 3811,
    ENCHANT_SHOULDER_GREATER_STORM = 3810,
    ENCHANT_SHOULDER_DOMINANCE = 3794,
    ENCHANT_SHOULDER_TRIUMPH = 3793,

    ENCHANT_CLOAK_DARKGLOW_EMBROIDERY = 3728,
    ENCHANT_CLOAK_SWORDGUARD_EMBROIDERY = 3730,
    ENCHANT_CLOAK_LIGHTWEAVE_EMBROIDERY = 3722,
    ENCHANT_CLOAK_SPRINGY_ARACHNOWEAVE = 3859,
    ENCHANT_CLOAK_WISDOM = 3296,
    ENCHANT_CLOAK_TITANWEAVE = 1951,
    ENCHANT_CLOAK_SPELL_PIERCING = 3243,
    ENCHANT_CLOAK_SHADOW_ARMOR = 3256,
    ENCHANT_CLOAK_MIGHTY_ARMOR = 3294,
    ENCHANT_CLOAK_MAJOR_AGILITY = 1099,
    ENCHANT_CLOAK_GREATER_SPEED = 3831,

    ENCHANT_LEG_EARTHEN = 3853,
    ENCHANT_LEG_FROSTHIDE = 3822,
    ENCHANT_LEG_ICESCALE = 3823,
    ENCHANT_LEG_BRILLIANT_SPELLTHREAD = 3719,
    ENCHANT_LEG_SAPPHIRE_SPELLTHREAD = 3721,
    ENCHANT_LEG_DRAGONSCALE = 3331,
    ENCHANT_LEG_WYRMSCALE = 3332,

    ENCHANT_GLOVES_GREATER_BLASTING = 3249,
    ENCHANT_GLOVES_ARMSMAN = 3253,
    ENCHANT_GLOVES_CRUSHER = 1603,
    ENCHANT_GLOVES_AGILITY = 3222,
    ENCHANT_GLOVES_PRECISION = 3234,
    ENCHANT_GLOVES_EXPERTISE = 3231,
    ENCHANT_GLOVES_HYPERSPEED = 3604,

    ENCHANT_BRACERS_MAJOR_STAMINA = 3850,
    ENCHANT_BRACERS_SUPERIOR_SP = 2332,
    ENCHANT_BRACERS_GREATER_ASSUALT = 3845,
    ENCHANT_BRACERS_MAJOR_SPIRT = 1147,
    ENCHANT_BRACERS_EXPERTISE = 3231,
    ENCHANT_BRACERS_GREATER_STATS = 2661,
    ENCHANT_BRACERS_INTELLECT = 1119,
    ENCHANT_BRACERS_FURL_ARCANE = 3763,
    ENCHANT_BRACERS_FURL_FIRE = 3759,
    ENCHANT_BRACERS_FURL_FROST = 3760,
    ENCHANT_BRACERS_FURL_NATURE = 3762,
    ENCHANT_BRACERS_FURL_SHADOW = 3761,
    ENCHANT_BRACERS_FURL_ATTACK = 3756,
    ENCHANT_BRACERS_FURL_STAMINA = 3757,
    ENCHANT_BRACERS_FURL_SPELLPOWER = 3758,

    ENCHANT_CHEST_POWERFUL_STATS = 3832,
    ENCHANT_CHEST_SUPER_HEALTH = 3297,
    ENCHANT_CHEST_GREATER_MAINA_REST = 2381,
    ENCHANT_CHEST_EXCEPTIONAL_RESIL = 3245,
    ENCHANT_CHEST_GREATER_DEFENSE = 1953,

    ENCHANT_BOOTS_GREATER_ASSULT = 1597,
    ENCHANT_BOOTS_TUSKARS_VITLIATY = 3232,
    ENCHANT_BOOTS_SUPERIOR_AGILITY = 983,
    ENCHANT_BOOTS_GREATER_SPIRIT = 1147,
    ENCHANT_BOOTS_GREATER_VITALITY = 3244,
    ENCHANT_BOOTS_ICEWALKER = 3826,
    ENCHANT_BOOTS_GREATER_FORTITUDE = 1075,
    ENCHANT_BOOTS_NITRO_BOOTS = 3606,
    ENCHANT_BOOTS_PYRO_ROCKET = 3603,
    ENCHANT_BOOTS_ARMOR_WEBBING = 3860,

    ENCHANT_RING_ASSULT = 3839,
    ENCHANT_RING_GREATER_SP = 3840,
    ENCHANT_RING_STAMINA = 3791,
};

uint32 roll;
uint32 EnchanterAnnounceModule;
uint32 EnchanterNumPhrases;
uint32 EnchanterMessageTimer;
uint32 EnchanterEmoteSpell;
uint32 EnchanterEmoteCommand;

class EnchanterConfig : public WorldScript
{
public:
    EnchanterConfig() : WorldScript("EnchanterConfig_conf") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        if (!reload) {
            EnchanterAnnounceModule = sConfigMgr->GetOption<bool>("Enchanter.Announce", 1);
            EnchanterNumPhrases = sConfigMgr->GetOption<uint32>("Enchanter.NumPhrases", 3);
            EnchanterMessageTimer = sConfigMgr->GetOption<uint32>("Enchanter.MessageTimer", 60000);
            EnchanterEmoteSpell = sConfigMgr->GetOption<uint32>("Enchanter.EmoteSpell", 44940);
            EnchanterEmoteCommand = sConfigMgr->GetOption<uint32>("Enchanter.EmoteCommand", 3);

            // Enforce Min/Max Time
            if (EnchanterMessageTimer != 0)
            {
                if (EnchanterMessageTimer < 60000 || EnchanterMessageTimer > 300000)
                {
                    EnchanterMessageTimer = 60000;
                }
            }
        }
    }
};

class EnchanterAnnounce : public PlayerScript
{

public:

    EnchanterAnnounce() : PlayerScript("EnchanterAnnounce") {}

    void OnLogin(Player* player)
    {
        // Announce Module
        if (EnchanterAnnounceModule)
        {
            WorldSession* session = player->GetSession();
            switch (session->GetSessionDbLocaleIndex())
            {
            case LOCALE_ruRU:
            {
                ChatHandler(player->GetSession()).SendSysMessage("На сервере запущен модуль |cff4CFF00EnchanterNPC |r");
                break;
            }
            default:
                ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00EnchanterNPC |rmodule.");
                break;
            }
        }
    }
};

class npc_enchantment : public CreatureScript
{

public:

    npc_enchantment() : CreatureScript("npc_enchantment") { }

    // Pick Phrase
    static std::string PickPhrase()
    {
        // Choose and speak a random phrase to the player
        // Phrases are stored in the config file
        std::string phrase = "";
        uint32 PhraseNum = urand(1, EnchanterNumPhrases); // How many phrases does the NPC speak?
        phrase = "EC.P" + std::to_string(PhraseNum);

        // Sanitize
        if (phrase == "")
        {
            phrase = "ERROR! NPC Emote Text Not Found! Check the npc_enchanter.conf!";
        }

        std::string randMsg = sConfigMgr->GetOption<std::string>(phrase.c_str(), "");
        return randMsg.c_str();
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        // enGB/ruRU
        WorldSession* session = player->GetSession();
        std::string msgMainWeapon = "";
        std::string msgOffhandWeapon = "";
        std::string msgEnchant2HWeapon = "";
        std::string msgEnchantShield = "";
        std::string msgEnchantHead = "";
        std::string msgEnchantShoulders = "";
        std::string msgEnchantCloak = "";
        std::string msgEnchantChest = "";
        std::string msgEnchantBracers = "";
        std::string msgEnchantGloves = "";
        std::string msgEnchantLegs = "";
        std::string msgEnchantBoots = "";
        std::string msgEnchantRings = "";
        switch (session->GetSessionDbLocaleIndex())
        {
        case LOCALE_ruRU:
        {
            msgMainWeapon = "|TInterface/ICONS/Inv_mace_116:24:24:-18|t[Зачаровать основное оружие]";
            msgOffhandWeapon = "|TInterface/ICONS/Inv_mace_116:24:24:-18|t[Зачаровать дополнительное оружие]";
            msgEnchant2HWeapon = "|TInterface/ICONS/Inv_axe_113:24:24:-18|t[Зачаровать двуручное оружие]";
            msgEnchantShield = "|TInterface/ICONS/Inv_shield_71:24:24:-18|t[Зачаровать щит]";
            msgEnchantHead = "|TInterface/ICONS/inv_helmet_29:24:24:-18|t[Зачаровать шлем]";
            msgEnchantShoulders = "|TInterface/ICONS/inv_shoulder_23:24:24:-18|t[Зачаровать наплечники]";
            msgEnchantCloak = "|TInterface/ICONS/Inv_misc_cape_18:24:24:-18|t[Зачаровать плащ]";
            msgEnchantChest = "|TInterface/ICONS/inv_chest_cloth_04:24:24:-18|t[Зачаровать нагрудник]";
            msgEnchantBracers = "|TInterface/ICONS/inv_bracer_14:24:24:-18|t[Зачаровать наручи]";
            msgEnchantGloves = "|TInterface/ICONS/inv_gauntlets_06:24:24:-18|t[Зачаровать перчатки]";
            msgEnchantLegs = "|TInterface/ICONS/inv_pants_11:24:24:-18|t[Зачаровать поножи]";
            msgEnchantBoots = "|TInterface/ICONS/inv_boots_05:24:24:-18|t[Зачаровать обувь]";
            msgEnchantRings = "|TInterface/ICONS/Inv_jewelry_ring_85:24:24:-18|t[Зачаровать кольца]";
            break;
        }
        default:
            msgMainWeapon = "|TInterface/ICONS/Inv_mace_116:24:24:-18|t[Enchant Main Weapon]";
            msgOffhandWeapon = "|TInterface/ICONS/Inv_mace_116:24:24:-18|t[Enchant Offhand Weapon]";
            msgEnchant2HWeapon = "|TInterface/ICONS/Inv_axe_113:24:24:-18|t[Enchant 2H Weapon]";
            msgEnchantShield = "|TInterface/ICONS/Inv_shield_71:24:24:-18|t[Enchant Shield]";
            msgEnchantHead = "|TInterface/ICONS/inv_helmet_29:24:24:-18|t[Enchant Head]";
            msgEnchantShoulders = "|TInterface/ICONS/inv_shoulder_23:24:24:-18|t[Enchant Shoulders]";
            msgEnchantCloak = "|TInterface/ICONS/Inv_misc_cape_18:24:24:-18|t[Enchant Cloak]";
            msgEnchantChest = "|TInterface/ICONS/inv_chest_cloth_04:24:24:-18|t[Enchant Chest]";
            msgEnchantBracers = "|TInterface/ICONS/inv_bracer_14:24:24:-18|t[Enchant Bracers]";
            msgEnchantGloves = "|TInterface/ICONS/inv_gauntlets_06:24:24:-18|t[Enchant Gloves]";
            msgEnchantLegs = "|TInterface/ICONS/inv_pants_11:24:24:-18|t[Enchant Legs]";
            msgEnchantBoots = "|TInterface/ICONS/inv_boots_05:24:24:-18|t[Enchant Boots]";
            msgEnchantRings = "|TInterface/ICONS/Inv_jewelry_ring_85:24:24:-18|t[Enchant Rings]";
            break;
        }

        AddGossipItemFor(player, 1, msgMainWeapon, GOSSIP_SENDER_MAIN, 1);
        if (player->HasSpell(674))
        {
            AddGossipItemFor(player, 1, msgOffhandWeapon, GOSSIP_SENDER_MAIN, 13);
        }
        AddGossipItemFor(player, 1, msgEnchant2HWeapon, GOSSIP_SENDER_MAIN, 2);
        AddGossipItemFor(player, 1, msgEnchantShield, GOSSIP_SENDER_MAIN, 3);
        AddGossipItemFor(player, 1, msgEnchantHead, GOSSIP_SENDER_MAIN, 4);
        AddGossipItemFor(player, 1, msgEnchantShoulders, GOSSIP_SENDER_MAIN, 5);
        AddGossipItemFor(player, 1, msgEnchantCloak, GOSSIP_SENDER_MAIN, 6);
        AddGossipItemFor(player, 1, msgEnchantChest, GOSSIP_SENDER_MAIN, 7);
        AddGossipItemFor(player, 1, msgEnchantBracers, GOSSIP_SENDER_MAIN, 8);
        AddGossipItemFor(player, 1, msgEnchantGloves, GOSSIP_SENDER_MAIN, 9);
        AddGossipItemFor(player, 1, msgEnchantLegs, GOSSIP_SENDER_MAIN, 10);
        AddGossipItemFor(player, 1, msgEnchantBoots, GOSSIP_SENDER_MAIN, 11);

        if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) == 450)
            AddGossipItemFor(player, 1, msgEnchantRings, GOSSIP_SENDER_MAIN, 12);

        player->PlayerTalkClass->SendGossipMenu(601015, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        Item * item;
        player->PlayerTalkClass->ClearMenus();

        // enGB/ruRU
        WorldSession* session = player->GetSession();
        // case 1
        std::string msgBladeWard = "";
        std::string msgBloodDraining = "";
        std::string msg26Agility = "";
        std::string msg45Spirit = "";
        std::string msgBerserking = "";
        std::string msg25HitRating25Critical = "";
        std::string msgBlackMagic = "";
        std::string msgBattlemaster = "";
        std::string msgIcebreaker = "";
        std::string msgLifeward = "";
        std::string msg50Stamina = "";
        std::string msg65AttackPower = "";
        std::string msg63SpellPower = "";
        std::string msgMongoose = "";
        std::string msgExecutioner = "";
        std::string msgBack = "";

        // case 2
        std::string msgWhisper2HWeaponRequireEquipped = "";
        std::string msg81SpellPower = "";
        std::string msg35Agility = "";
        std::string msg110AttackPower = "";

        // case 3
        std::string msgWhisperRequireShieldEquipped = "";
        std::string msg20Defense = "";
        std::string msg25Intellect = "";
        std::string msg12Resilience = "";
        std::string msg36Block = "";
        std::string msg18Stamina = "";
        std::string msg81Block50LessDisarm = "";

        // case 4
        std::string msg30SpellPower10Mp5 = "";
        std::string msg30SpellPower20Crit = "";
        std::string msg29SpellPower20Resilience = "";
        std::string msg30Stamina25Resilience = "";
        std::string msg37Stamina20Defense = "";
        std::string msg50AttackPower20Crit = "";
        std::string msg50AttackPower20Resilience = "";
        std::string msgArcanumOfEclipsedMoon = "";
        std::string msgArcanumOfTheFlameSoul = "";
        std::string msgArcanumOfTheFleeingShadow = "";
        std::string msgArcanumOfTheFrostySoul = "";
        std::string msgArcanumOfToxicWarding = "";

        // case 5
        std::string msg120AttackPower15Crit = "";
        std::string msg70SpellPower8Mp5 = "";
        std::string msg60Dodge15Defense = "";
        std::string msg70SpellPower15Crit = "";
        std::string msg40AttackPower15Crit = "";
        std::string msg24SpellPower8Mp5 = "";
        std::string msg30Stamina15Resilience = "";
        std::string msg20Dodge15Defense = "";
        std::string msg24SpellPower15Crit = "";
        std::string msg23SpellPower15Resilience = "";
        std::string msg40AttackPower15Resilience = "";

        // case 6
        std::string msgDarkglowEmbroidery = "";
        std::string msgLightweaveEmbroidery = "";
        std::string msgSwordguardEmbroidery = "";
        std::string msgParachute = "";
        std::string msgShadowArmor = "";
        std::string msg10Spirit2ReducedThreat = "";
        std::string msg16Defense = "";
        std::string msg35SpellPenetration = "";
        std::string msg225Armor = "";
        std::string msg22Agility = "";
        std::string msg23Haste = "";

        // case 7
        std::string msgAdd10AllStats = "";
        std::string msg225Health = "";
        std::string msg10Mp5 = "";
        std::string msg20Resilience = "";
        std::string msg22Defense = "";

        // case 8
        std::string msg40Stamina = "";
        std::string msg30SpellPower = "";
        std::string msg50AttackPower = "";
        std::string msg18Spirit = "";
        std::string msg15Expertise = "";
        std::string msgAdd6AllStats = "";
        std::string msg16Intellect = "";
        std::string msgFurLiningArcaneResist = "";
        std::string msgFurLiningFireResist = "";
        std::string msgFurLiningFrostResist = "";
        std::string msgFurLiningNatureResist = "";
        std::string msgFurLiningShadowResist = "";
        std::string msgFurLiningAttackPower = "";
        std::string msgFurLiningStamina = "";
        std::string msgFurLiningSpellpower = "";

        // case 9
        std::string msgHyperspeedAccelerators = "";
        std::string msg16CriticalStrike = "";
        std::string msg2Threat10Parry = "";
        std::string msg44AttackPower = "";
        std::string msg20Agility = "";
        std::string msg20HitRating = "";
        std::string msg15Expertise = "";

        // case 10
        std::string msg40Resilience28Stamina = "";
        std::string msg55Stamina22Agility = "";
        std::string msg75AttackPower22Critical = "";
        std::string msg50SpellPower22Spirit = "";
        std::string msg50SpellPower30Stamina = "";
        std::string msg72Stamina35Agility = "";
        std::string msg100AttackPower36Critical = "";

        // case 11
        std::string msg32AttackPower = "";
        std::string msg15StaminaMinorSpeedIncrease = "";
        std::string msg16Agility = "";
        std::string msg18Spirit = "";
        std::string msgRestore7HealthMp5 = "";
        std::string msg12HitRating12Critical = "";
        std::string msg22Stamina = "";
        std::string msgNitroBoots = "";
        std::string HandMountedPyroRocket = "";
        std::string ReticulatedArmorWebbing = "";

        // case 12
        std::string msg40AttackPower = "";
        std::string msg23SpellPower = "";
        std::string msg30Stamina = "";

        // case 13
        std::string msgWhispRequiresWeaponEquipped = "";

        // case 300
        std::string msgMainWeapon = "";
        std::string msgOffhandWeapon = "";
        std::string msgEnchant2HWeapon = "";
        std::string msgEnchantShield = "";
        std::string msgEnchantHead = "";
        std::string msgEnchantShoulders = "";
        std::string msgEnchantCloak = "";
        std::string msgEnchantChest = "";
        std::string msgEnchantBracers = "";
        std::string msgEnchantGloves = "";
        std::string msgEnchantLegs = "";
        std::string msgEnchantBoots = "";
        std::string msgEnchantRings = "";


        switch (session->GetSessionDbLocaleIndex())
        {
        case LOCALE_ruRU:
        {
            // case 1
            msgBladeWard = "Отведение удара";
            msgBloodDraining = "Высасывание крови";
            msg26Agility = "26 Ловкости";
            msg45Spirit = "45 Духа";
            msgBerserking = "Берсерк";
            msg25HitRating25Critical = "25 Рейтингу меткости + 25 Крита";
            msgBlackMagic = "Черная магия";
            msgBattlemaster = "Мастер боя";
            msgIcebreaker = "Ледорез";
            msgLifeward = "Исцеление от удара";
            msg50Stamina = "50 Выносливости";
            msg65AttackPower = "65 Силы атаки";
            msg63SpellPower = "63 Силы заклинания";
            msgMongoose = "Мангуст";
            msgExecutioner = "Палач";
            msgBack = "Назад";

            // case 2
            msgWhisper2HWeaponRequireEquipped = "Для этого зачарования требуется наличие двуручного оружия.";
            msg81SpellPower = "81 Силы заклинания";
            msg35Agility = "35 Ловкости";
            msg110AttackPower = "110 Силы атаки";

            // case 3
            msgWhisperRequireShieldEquipped = "Это зачарование требует использования щита.";
            msg20Defense = "20 Защиты";
            msg25Intellect = "25 Интеллекта";
            msg12Resilience = "12 Устойчивости";
            msg36Block = "36 Блокирования";
            msg18Stamina = "18 Выносливости";
            msg81Block50LessDisarm = "81 Блока + 50% Разоружения";

            // case 4
            msg30SpellPower10Mp5 = "30 Силы заклинания + 10 Mp5";
            msg30SpellPower20Crit = "30 Силы заклинания + 20 Крита";
            msg29SpellPower20Resilience = "29 Силы заклинания + 20 Устойчивости к криту";
            msg30Stamina25Resilience = "30 Выносливости + 25 Устойчивости к криту";
            msg37Stamina20Defense = "37 Выносливости + 20 Защиты";
            msg50AttackPower20Crit = "50 Силы атаки + 20 Крита";
            msg50AttackPower20Resilience = "50 Силы атаки + 20 Устойчивости к криту";
            msgArcanumOfEclipsedMoon = "Магический знак затменной луны";
            msgArcanumOfTheFlameSoul = "Магический знак духа пламени";
            msgArcanumOfTheFleeingShadow = "Магический знак ускользающей тени";
            msgArcanumOfTheFrostySoul = "Магический знак промерзшей души";
            msgArcanumOfToxicWarding = "Магический знак защиты от яда";

            // case 5
            msg120AttackPower15Crit = "120 Силы атаки + 15 Крита";
            msg70SpellPower8Mp5 = "70 Силы заклинания + 8 Mp5";
            msg60Dodge15Defense = "60 Уклонения + 15 Защиты";
            msg70SpellPower15Crit = "70 Силы заклинания + 15 Крита";
            msg40AttackPower15Crit = "40 Силы атаки + 15 Крита";
            msg24SpellPower8Mp5 = "24 Силы заклинания + 8 Mp5";
            msg30Stamina15Resilience = "30 Выносливости + 15 Устойчивости к криту";
            msg20Dodge15Defense = "20 Уклонения + 15 Защиты";
            msg24SpellPower15Crit = "24 Силы заклинания + 15 Крита";
            msg23SpellPower15Resilience = "23 Силы заклинания + 15 Устойчивости к криту";
            msg40AttackPower15Resilience = "40 Силы атаки + 15 Устойчивости к криту";

            // case 6
            msgDarkglowEmbroidery = "Вышивка темного сияния";
            msgLightweaveEmbroidery = "Светлотканая вышивка";
            msgSwordguardEmbroidery = "Вышивка гарды меча";
            msgParachute = "Парашют";
            msgShadowArmor = "Теневая броня";
            msg10Spirit2ReducedThreat = "10 Духа + 2% Уменьшения угрозы";
            msg16Defense = "16 Защиты";
            msg35SpellPenetration = "35 Проникновения заклинаний";
            msg225Armor = "225 Брони";
            msg22Agility = "22 Ловкости";
            msg23Haste = "23 Рейтинга скорости";

            // case 7
            msgAdd10AllStats = "+10 Всем характеристикам";
            msg225Health = "225 Здоровья";
            msg10Mp5 = "10 Mp5";
            msg20Resilience = "20 Устойчивости к криту";
            msg22Defense = "22 Защиты";

            // case 8
            msg40Stamina = "40 Выносливости";
            msg30SpellPower = "30 Силы заклинаний";
            msg50AttackPower = "50 Силы атаки";
            msg18Spirit = "18 Духа";
            msg15Expertise = "15 Мастерства";
            msgAdd6AllStats = "+6 Всем характеристикам";
            msg16Intellect = "16 Интеллекта";
            msgFurLiningArcaneResist = "Меховая подкладка - Сопротивление тайной магии";
            msgFurLiningFireResist = "Меховая подкладка - Сопротивление огню";
            msgFurLiningFrostResist = "Меховая подкладка - Сопротивление магии льда";
            msgFurLiningNatureResist = "Меховая подкладка - Сопротивление силам природы";
            msgFurLiningShadowResist = "Меховая подкладка - Сопротивление темной магии";
            msgFurLiningAttackPower = "Меховая подкладка - Сила атаки";
            msgFurLiningStamina = "Меховая подкладка - Выносливость";
            msgFurLiningSpellpower = "Меховая подкладка - Сила заклинания";

            // case 9
            msgHyperspeedAccelerators = "Гиперскоростные ускорители";
            msg16CriticalStrike = "16 Критического удара";
            msg2Threat10Parry = "2% Уменьшения угрозы + 10 Парирования";
            msg44AttackPower = "44 Силы атаки";
            msg20Agility = "20 Ловкости";
            msg20HitRating = "20 Рейтинга меткости";
            msg15Expertise = "15 Мастерства";

            // case 10
            msg40Resilience28Stamina = "40 Устойчивости к криту + 28 Выносливости";
            msg55Stamina22Agility = "55 Выносливости + 22 Ловкости";
            msg75AttackPower22Critical = "75 Силы атаки + 22 Крита";
            msg50SpellPower22Spirit = "50 Силы заклинаний + 22 Духа";
            msg50SpellPower30Stamina = "50 Силы заклинаний + 30 Выносливости";
            msg72Stamina35Agility = "72 Выносливости + 35 Ловкости";
            msg100AttackPower36Critical = "100 Силы атаки + 36 Крита";

            // case 11
            msg32AttackPower = "32 Силы атаки";
            msg15StaminaMinorSpeedIncrease = "15 Выносливости + Minor Speed Increase";
            msg16Agility = "16 Ловкости";
            msg18Spirit = "18 Духа";
            msgRestore7HealthMp5 = "Восстановить 7 Здоровья + Mp5";
            msg12HitRating12Critical = "12 Рейтинга меткости + 12 Крита";
            msg22Stamina = "22 Выносливости";
            msgNitroBoots = "Нитро Сапоги";
            HandMountedPyroRocket = "Нарукавная зажигательная ракетница";
            ReticulatedArmorWebbing = "Сетчатая броневая тесьма";

            // case 12
            msg40AttackPower = "40 Силы атаки";
            msg23SpellPower = "23 Силы заклинаний";
            msg30Stamina = "30 Выносливости";

            // case 13
            msgWhispRequiresWeaponEquipped = "Это зачарование требует, чтобы оружие было экипировано в левой руке.";

            // case 300
            msgMainWeapon = "|TInterface/ICONS/Inv_mace_116:24:24:-18|t[Зачаровать основное оружие]";
            msgOffhandWeapon = "|TInterface/ICONS/Inv_mace_116:24:24:-18|t[Зачаровать дополнительное оружие]";
            msgEnchant2HWeapon = "|TInterface/ICONS/Inv_axe_113:24:24:-18|t[Зачаровать двуручное оружие]";
            msgEnchantShield = "|TInterface/ICONS/Inv_shield_71:24:24:-18|t[Зачаровать щит]";
            msgEnchantHead = "|TInterface/ICONS/inv_helmet_29:24:24:-18|t[Зачаровать шлем]";
            msgEnchantShoulders = "|TInterface/ICONS/inv_shoulder_23:24:24:-18|t[Зачаровать наплечники]";
            msgEnchantCloak = "|TInterface/ICONS/Inv_misc_cape_18:24:24:-18|t[Зачаровать плащ]";
            msgEnchantChest = "|TInterface/ICONS/inv_chest_cloth_04:24:24:-18|t[Зачаровать нагрудник]";
            msgEnchantBracers = "|TInterface/ICONS/inv_bracer_14:24:24:-18|t[Зачаровать наручи]";
            msgEnchantGloves = "|TInterface/ICONS/inv_gauntlets_06:24:24:-18|t[Зачаровать перчатки]";
            msgEnchantLegs = "|TInterface/ICONS/inv_pants_11:24:24:-18|t[Зачаровать поножи]";
            msgEnchantBoots = "|TInterface/ICONS/inv_boots_05:24:24:-18|t[Зачаровать обувь]";
            msgEnchantRings = "|TInterface/ICONS/Inv_jewelry_ring_85:24:24:-18|t[Зачаровать кольца]";
            break;
        }
        default:
            // case 1
            msgBladeWard = "Blade Ward";
            msgBloodDraining = "Blood Draining";
            msg26Agility = "26 Agility";
            msg45Spirit = "45 Spirit";
            msgBerserking = "Berserking";
            msg25HitRating25Critical = "25 Hit Rating + 25 Critical";
            msgBlackMagic = "Black Magic";
            msgBattlemaster = "Battlemaster";
            msgIcebreaker = "Icebreaker";
            msgLifeward = "Lifeward";
            msg50Stamina = "50 Stamina";
            msg65AttackPower = "65 Attack Power";
            msg63SpellPower = "63 Spell Power";
            msgMongoose = "Mongoose";
            msgExecutioner = "Executioner";
            msgBack = "Back";

            // case 2
            msgWhisper2HWeaponRequireEquipped = "This enchant requires a 2H weapon to be equipped.";
            msg81SpellPower = "81 Spell Power";
            msg35Agility = "35 Agility";
            msg110AttackPower = "110 Attack Power";

            // case 3
            msgWhisperRequireShieldEquipped = "This enchant requires a shield to be equipped.";
            msg20Defense = "20 Defense";
            msg25Intellect = "25 Intellect";
            msg12Resilience = "12 Resilience";
            msg36Block = "36 Block";
            msg18Stamina = "18 Stamina";
            msg81Block50LessDisarm = "81 Block + 50% Less Disarm";

            // case 4
            msg30SpellPower10Mp5 = "30 Spell Power + 10 Mp5";
            msg30SpellPower20Crit = "30 Spell Power + 20 Crit";
            msg29SpellPower20Resilience = "29 Spell Power + 20 Resilience";
            msg30Stamina25Resilience = "30 Stamina + 25 Resilience";
            msg37Stamina20Defense = "37 Stamina + 20 Defense";
            msg50AttackPower20Crit = "50 Attack Power + 20 Crit";
            msg50AttackPower20Resilience = "50 Attack Power + 20 Resilience";
            msgArcanumOfEclipsedMoon = "Arcanum of Eclipsed Moon";
            msgArcanumOfTheFlameSoul = "Arcanum of the Flame's Soul";
            msgArcanumOfTheFleeingShadow = "Arcanum of the Fleeing Shadow";
            msgArcanumOfTheFrostySoul = "Arcanum of the Frosty Soul";
            msgArcanumOfToxicWarding = "Arcanum of Toxic Warding";

            // case 5
            msg120AttackPower15Crit = "120 Attack Power + 15 Crit";
            msg70SpellPower8Mp5 = "70 Spell Power + 8 Mp5";
            msg60Dodge15Defense = "60 Dodge + 15 Defense";
            msg70SpellPower15Crit = "70 Spell Power + 15 Crit";
            msg40AttackPower15Crit = "40 Attack Power + 15 Crit";
            msg24SpellPower8Mp5 = "24 Spell Power + 8 Mp5";
            msg30Stamina15Resilience = "30 Stamina + 15 Resilience";
            msg20Dodge15Defense = "20 Dodge + 15 Defense";
            msg24SpellPower15Crit = "24 Spell Power + 15 Crit";
            msg23SpellPower15Resilience = "23 Spell Power + 15 Resilience";
            msg40AttackPower15Resilience = "40 Attack Power + 15 Resilience";

            // case 6
            msgDarkglowEmbroidery = "Darkglow Embroidery";
            msgLightweaveEmbroidery = "Lightweave Embroidery";
            msgSwordguardEmbroidery = "Swordguard Embroidery";
            msgParachute = "Parachute";
            msgShadowArmor = "Shadow Armor";
            msg10Spirit2ReducedThreat = "10 Spirit + 2% Reduced Threat";
            msg16Defense = "16 Defense";
            msg35SpellPenetration = "35 Spell Penetration";
            msg225Armor = "225 Armor";
            msg22Agility = "22 Agility";
            msg23Haste = "23 Haste";

            // case 7
            msgAdd10AllStats = "+10 All Stats";
            msg225Health = "225 Health";
            msg10Mp5 = "10 Mp5";
            msg20Resilience = "20 Resilience";
            msg22Defense = "22 Defense";

            // case 8
            msg40Stamina = "40 Stamina";
            msg30SpellPower = "30 Spell Power";
            msg50AttackPower = "50 Attack Power";
            msg18Spirit = "18 Spirit";
            msg15Expertise = "15 Expertise";
            msgAdd6AllStats = "+6 All Stats";
            msg16Intellect = "16 Intellect";
            msgFurLiningArcaneResist = "Fur Lining - Arcane Resist";
            msgFurLiningFireResist = "Fur Lining - Fire Resist";
            msgFurLiningFrostResist = "Fur Lining - Frost Resist";
            msgFurLiningNatureResist = "Fur Lining - Nature Resist";
            msgFurLiningShadowResist = "Fur Lining - Shadow Resist";
            msgFurLiningAttackPower = "Fur Lining - Attack Power";
            msgFurLiningStamina = "Fur Lining - Stamina";
            msgFurLiningSpellpower = "Fur Lining - Spellpower";

            // case 9
            msgHyperspeedAccelerators = "Hyperspeed Accelerators";
            msg16CriticalStrike = "16 Critical Strike";
            msg2Threat10Parry = "2% Threat + 10 Parry";
            msg44AttackPower = "44 Attack Power";
            msg20Agility = "20 Agility";
            msg20HitRating = "20 Hit Rating";
            msg15Expertise = "15 Expertise";

            // case 10
            msg40Resilience28Stamina = "40 Resilience + 28 Stamina";
            msg55Stamina22Agility = "55 Stamina + 22 Agility";
            msg75AttackPower22Critical = "75 Attack Power + 22 Critical";
            msg50SpellPower22Spirit = "50 Spell Power + 22 Spirit";
            msg50SpellPower30Stamina = "50 Spell Power + 30 Stamina";
            msg72Stamina35Agility = "72 Stamina + 35 Agility";
            msg100AttackPower36Critical = "100 Attack Power + 36 Critical";

            // case 11
            msg32AttackPower = "32 Attack Power";
            msg15StaminaMinorSpeedIncrease = "15 Stamina + Minor Speed Increase";
            msg16Agility = "16 Agility";
            msg18Spirit = "18 Spirit";
            msgRestore7HealthMp5 = "Restore 7 Health + Mp5";
            msg12HitRating12Critical = "12 Hit Rating + 12 Critical";
            msg22Stamina = "22 Stamina";
            msgNitroBoots = "Nitro Boots";
            HandMountedPyroRocket = "Hand-Mounted Pyro Rocket";
            ReticulatedArmorWebbing = "Reticulated Armor Webbing";

            // case 12
            msg40AttackPower = "40 Attack Power";
            msg23SpellPower = "23 Spell Power";
            msg30Stamina = "30 Stamina";

            // case 13
            msgWhispRequiresWeaponEquipped = "This enchant requires a weapon to be equipped in offhand.";

            // case 300
            msgMainWeapon = "|TInterface/ICONS/Inv_mace_116:24:24:-18|t[Enchant Main Weapon]";
            msgOffhandWeapon = "|TInterface/ICONS/Inv_mace_116:24:24:-18|t[Enchant Offhand Weapon]";
            msgEnchant2HWeapon = "|TInterface/ICONS/Inv_axe_113:24:24:-18|t[Enchant 2H Weapon]";
            msgEnchantShield = "|TInterface/ICONS/Inv_shield_71:24:24:-18|t[Enchant Shield]";
            msgEnchantHead = "|TInterface/ICONS/inv_helmet_29:24:24:-18|t[Enchant Head]";
            msgEnchantShoulders = "|TInterface/ICONS/inv_shoulder_23:24:24:-18|t[Enchant Shoulders]";
            msgEnchantCloak = "|TInterface/ICONS/Inv_misc_cape_18:24:24:-18|t[Enchant Cloak]";
            msgEnchantChest = "|TInterface/ICONS/inv_chest_cloth_04:24:24:-18|t[Enchant Chest]";
            msgEnchantBracers = "|TInterface/ICONS/inv_bracer_14:24:24:-18|t[Enchant Bracers]";
            msgEnchantGloves = "|TInterface/ICONS/inv_gauntlets_06:24:24:-18|t[Enchant Gloves]";
            msgEnchantLegs = "|TInterface/ICONS/inv_pants_11:24:24:-18|t[Enchant Legs]";
            msgEnchantBoots = "|TInterface/ICONS/inv_boots_05:24:24:-18|t[Enchant Boots]";
            msgEnchantRings = "|TInterface/ICONS/Inv_jewelry_ring_85:24:24:-18|t[Enchant Rings]";
            break;
        }

        switch (action)
        {

        case 1: // Enchant Main Hand Weapon
            if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) == 450)
            {
                AddGossipItemFor(player, 1, msgBladeWard, GOSSIP_SENDER_MAIN, 102);
                AddGossipItemFor(player, 1, msgBloodDraining, GOSSIP_SENDER_MAIN, 103);
            }
            AddGossipItemFor(player, 1, msg26Agility, GOSSIP_SENDER_MAIN, 100);
            AddGossipItemFor(player, 1, msg45Spirit, GOSSIP_SENDER_MAIN, 101);
            AddGossipItemFor(player, 1, msgBerserking, GOSSIP_SENDER_MAIN, 104);
            AddGossipItemFor(player, 1, msg25HitRating25Critical, GOSSIP_SENDER_MAIN, 105);
            AddGossipItemFor(player, 1, msgBlackMagic, GOSSIP_SENDER_MAIN, 106);
            AddGossipItemFor(player, 1, msgBattlemaster, GOSSIP_SENDER_MAIN, 107);
            AddGossipItemFor(player, 1, msgIcebreaker, GOSSIP_SENDER_MAIN, 108);
            AddGossipItemFor(player, 1, msgLifeward, GOSSIP_SENDER_MAIN, 109);
            AddGossipItemFor(player, 1, msg50Stamina, GOSSIP_SENDER_MAIN, 110);
            AddGossipItemFor(player, 1, msg65AttackPower, GOSSIP_SENDER_MAIN, 111);
            AddGossipItemFor(player, 1, msg63SpellPower, GOSSIP_SENDER_MAIN, 112);
            AddGossipItemFor(player, 1, msgMongoose, GOSSIP_SENDER_MAIN, 113);
            AddGossipItemFor(player, 1, msgExecutioner, GOSSIP_SENDER_MAIN, 114);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, msgBack, GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100002, creature->GetGUID());
            return true;
            break;

        case 2: // Enchant 2H Weapon
            item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
            if (!item)
            {
                creature->Whisper(msgWhisper2HWeaponRequireEquipped, LANG_UNIVERSAL, player);
                player->PlayerTalkClass->SendCloseGossip();
                return false;
            }
            if (item->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
            {
                AddGossipItemFor(player, 1, msgBerserking, GOSSIP_SENDER_MAIN, 104);
                AddGossipItemFor(player, 1, msgMongoose, GOSSIP_SENDER_MAIN, 113);
                AddGossipItemFor(player, 1, msgExecutioner, GOSSIP_SENDER_MAIN, 114);
                AddGossipItemFor(player, 1, msg81SpellPower, GOSSIP_SENDER_MAIN, 115);
                AddGossipItemFor(player, 1, msg35Agility, GOSSIP_SENDER_MAIN, 116);
                AddGossipItemFor(player, 1, msg110AttackPower, GOSSIP_SENDER_MAIN, 117);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, msgBack, GOSSIP_SENDER_MAIN, 300);
            }
            else
            {
                creature->Whisper(msgWhisper2HWeaponRequireEquipped, LANG_UNIVERSAL, player);
                player->PlayerTalkClass->SendCloseGossip();
            }
            player->PlayerTalkClass->SendGossipMenu(100003, creature->GetGUID());
            return true;
            break;

        case 3: // Enchant Shield
            item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
            if (!item)
            {
                creature->Whisper(msgWhisperRequireShieldEquipped, LANG_UNIVERSAL, player);
                player->PlayerTalkClass->SendCloseGossip();
                return false;
            }
            if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
            {
                AddGossipItemFor(player, 1, msg20Defense, GOSSIP_SENDER_MAIN, 118);
                AddGossipItemFor(player, 1, msg25Intellect, GOSSIP_SENDER_MAIN, 119);
                AddGossipItemFor(player, 1, msg12Resilience, GOSSIP_SENDER_MAIN, 120);
                AddGossipItemFor(player, 1, msg36Block, GOSSIP_SENDER_MAIN, 121);
                AddGossipItemFor(player, 1, msg18Stamina, GOSSIP_SENDER_MAIN, 122);
                AddGossipItemFor(player, 1, msg81Block50LessDisarm, GOSSIP_SENDER_MAIN, 123);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, msgBack, GOSSIP_SENDER_MAIN, 300);
            }
            else
            {
                creature->Whisper(msgWhisperRequireShieldEquipped, LANG_UNIVERSAL, player);
                player->PlayerTalkClass->SendGossipMenu(100004, creature->GetGUID());

            }
            player->PlayerTalkClass->SendGossipMenu(100004, creature->GetGUID());
            return true;
            break;

        case 4: // Enchant Head
            AddGossipItemFor(player, 1, msg30SpellPower10Mp5, GOSSIP_SENDER_MAIN, 124);
            AddGossipItemFor(player, 1, msg30SpellPower20Crit, GOSSIP_SENDER_MAIN, 125);
            AddGossipItemFor(player, 1, msg29SpellPower20Resilience, GOSSIP_SENDER_MAIN, 126);
            AddGossipItemFor(player, 1, msg30Stamina25Resilience, GOSSIP_SENDER_MAIN, 127);
            AddGossipItemFor(player, 1, msg37Stamina20Defense, GOSSIP_SENDER_MAIN, 128);
            AddGossipItemFor(player, 1, msg50AttackPower20Crit, GOSSIP_SENDER_MAIN, 129);
            AddGossipItemFor(player, 1, msg50AttackPower20Resilience, GOSSIP_SENDER_MAIN, 130);
            AddGossipItemFor(player, 1, msgArcanumOfEclipsedMoon, GOSSIP_SENDER_MAIN, 131);
            AddGossipItemFor(player, 1, msgArcanumOfTheFlameSoul, GOSSIP_SENDER_MAIN, 132);
            AddGossipItemFor(player, 1, msgArcanumOfTheFleeingShadow, GOSSIP_SENDER_MAIN, 133);
            AddGossipItemFor(player, 1, msgArcanumOfTheFrostySoul, GOSSIP_SENDER_MAIN, 134);
            AddGossipItemFor(player, 1, msgArcanumOfToxicWarding, GOSSIP_SENDER_MAIN, 135);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, msgBack, GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100005, creature->GetGUID());
            return true;
            break;

        case 5: // Enchant Shoulders
            if (player->HasSkill(SKILL_INSCRIPTION) && player->GetSkillValue(SKILL_INSCRIPTION) == 450)
            {
                AddGossipItemFor(player, 1, msg120AttackPower15Crit, GOSSIP_SENDER_MAIN, 136);
                AddGossipItemFor(player, 1, msg70SpellPower8Mp5, GOSSIP_SENDER_MAIN, 137);
                AddGossipItemFor(player, 1, msg60Dodge15Defense, GOSSIP_SENDER_MAIN, 138);
                AddGossipItemFor(player, 1, msg70SpellPower15Crit, GOSSIP_SENDER_MAIN, 139);
            }
            AddGossipItemFor(player, 1, msg40AttackPower15Crit, GOSSIP_SENDER_MAIN, 140);
            AddGossipItemFor(player, 1, msg24SpellPower8Mp5, GOSSIP_SENDER_MAIN, 141);
            AddGossipItemFor(player, 1, msg30Stamina15Resilience, GOSSIP_SENDER_MAIN, 142);
            AddGossipItemFor(player, 1, msg20Dodge15Defense, GOSSIP_SENDER_MAIN, 143);
            AddGossipItemFor(player, 1, msg24SpellPower15Crit, GOSSIP_SENDER_MAIN, 144);
            AddGossipItemFor(player, 1, msg23SpellPower15Resilience, GOSSIP_SENDER_MAIN, 145);
            AddGossipItemFor(player, 1, msg40AttackPower15Resilience, GOSSIP_SENDER_MAIN, 146);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, msgBack, GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100006, creature->GetGUID());
            return true;
            break;

        case 6: // Enchant Cloak
            if (player->HasSkill(SKILL_TAILORING) && player->GetSkillValue(SKILL_TAILORING) == 450)
            {
                AddGossipItemFor(player, 1, msgDarkglowEmbroidery, GOSSIP_SENDER_MAIN, 149);
                AddGossipItemFor(player, 1, msgLightweaveEmbroidery, GOSSIP_SENDER_MAIN, 150);
                AddGossipItemFor(player, 1, msgSwordguardEmbroidery, GOSSIP_SENDER_MAIN, 151);
            }
            if (player->HasSkill(SKILL_ENGINEERING) && player->GetSkillValue(SKILL_ENGINEERING) == 450)
            {
                AddGossipItemFor(player, 1, msgParachute, GOSSIP_SENDER_MAIN, 147);
            }
            AddGossipItemFor(player, 1, msgShadowArmor, GOSSIP_SENDER_MAIN, 148);
            AddGossipItemFor(player, 1, msg10Spirit2ReducedThreat, GOSSIP_SENDER_MAIN, 152);
            AddGossipItemFor(player, 1, msg16Defense, GOSSIP_SENDER_MAIN, 153);
            AddGossipItemFor(player, 1, msg35SpellPenetration, GOSSIP_SENDER_MAIN, 154);
            AddGossipItemFor(player, 1, msg225Armor, GOSSIP_SENDER_MAIN, 155);
            AddGossipItemFor(player, 1, msg22Agility, GOSSIP_SENDER_MAIN, 156);
            AddGossipItemFor(player, 1, msg23Haste, GOSSIP_SENDER_MAIN, 157);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, msgBack, GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100007, creature->GetGUID());
            return true;
            break;

        case 7: //Enchant chest
            AddGossipItemFor(player, 1, msgAdd10AllStats, GOSSIP_SENDER_MAIN, 158);
            AddGossipItemFor(player, 1, msg225Health, GOSSIP_SENDER_MAIN, 159);
            AddGossipItemFor(player, 1, msg10Mp5, GOSSIP_SENDER_MAIN, 160);
            AddGossipItemFor(player, 1, msg20Resilience, GOSSIP_SENDER_MAIN, 161);
            AddGossipItemFor(player, 1, msg22Defense, GOSSIP_SENDER_MAIN, 162);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, msgBack, GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100008, creature->GetGUID());
            return true;
            break;

        case 8: //Enchant Bracers
            AddGossipItemFor(player, 1, msg40Stamina, GOSSIP_SENDER_MAIN, 163);
            AddGossipItemFor(player, 1, msg30SpellPower, GOSSIP_SENDER_MAIN, 164);
            AddGossipItemFor(player, 1, msg50AttackPower, GOSSIP_SENDER_MAIN, 165);
            AddGossipItemFor(player, 1, msg18Spirit, GOSSIP_SENDER_MAIN, 166);
            AddGossipItemFor(player, 1, msg15Expertise, GOSSIP_SENDER_MAIN, 167);
            AddGossipItemFor(player, 1, msgAdd6AllStats, GOSSIP_SENDER_MAIN, 168);
            AddGossipItemFor(player, 1, msg16Intellect, GOSSIP_SENDER_MAIN, 169);
            if (player->HasSkill(SKILL_LEATHERWORKING) && player->GetSkillValue(SKILL_LEATHERWORKING) == 450)
            {
                AddGossipItemFor(player, 1, msgFurLiningArcaneResist, GOSSIP_SENDER_MAIN, 170);
                AddGossipItemFor(player, 1, msgFurLiningFireResist, GOSSIP_SENDER_MAIN, 171);
                AddGossipItemFor(player, 1, msgFurLiningFrostResist, GOSSIP_SENDER_MAIN, 172);
                AddGossipItemFor(player, 1, msgFurLiningNatureResist, GOSSIP_SENDER_MAIN, 173);
                AddGossipItemFor(player, 1, msgFurLiningShadowResist, GOSSIP_SENDER_MAIN, 174);
                AddGossipItemFor(player, 1, msgFurLiningAttackPower, GOSSIP_SENDER_MAIN, 175);
                AddGossipItemFor(player, 1, msgFurLiningStamina, GOSSIP_SENDER_MAIN, 176);
                AddGossipItemFor(player, 1, msgFurLiningSpellpower, GOSSIP_SENDER_MAIN, 177);
            }
            AddGossipItemFor(player, GOSSIP_ICON_TALK, msgBack, GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100009, creature->GetGUID());
            return true;
            break;

        case 9: //Enchant Gloves
            if (player->HasSkill(SKILL_ENGINEERING) && player->GetSkillValue(SKILL_ENGINEERING) == 400)
            {
                AddGossipItemFor(player, 1, msgHyperspeedAccelerators, GOSSIP_SENDER_MAIN, 200);
            }
            AddGossipItemFor(player, 1, msg16CriticalStrike, GOSSIP_SENDER_MAIN, 178);
            AddGossipItemFor(player, 1, msg2Threat10Parry, GOSSIP_SENDER_MAIN, 179);
            AddGossipItemFor(player, 1, msg44AttackPower, GOSSIP_SENDER_MAIN, 180);
            AddGossipItemFor(player, 1, msg20Agility, GOSSIP_SENDER_MAIN, 181);
            AddGossipItemFor(player, 1, msg20HitRating, GOSSIP_SENDER_MAIN, 182);
            AddGossipItemFor(player, 1, msg15Expertise, GOSSIP_SENDER_MAIN, 183);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, msgBack, GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100010, creature->GetGUID());
            return true;
            break;

        case 10: //Enchant legs
            AddGossipItemFor(player, 1, msg40Resilience28Stamina, GOSSIP_SENDER_MAIN, 184);
            AddGossipItemFor(player, 1, msg55Stamina22Agility, GOSSIP_SENDER_MAIN, 185);
            AddGossipItemFor(player, 1, msg75AttackPower22Critical, GOSSIP_SENDER_MAIN, 186);
            AddGossipItemFor(player, 1, msg50SpellPower22Spirit, GOSSIP_SENDER_MAIN, 187);
            AddGossipItemFor(player, 1, msg50SpellPower30Stamina, GOSSIP_SENDER_MAIN, 188);
            AddGossipItemFor(player, 1, msg72Stamina35Agility, GOSSIP_SENDER_MAIN, 189);
            AddGossipItemFor(player, 1, msg100AttackPower36Critical, GOSSIP_SENDER_MAIN, 190);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, msgBack, GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100011, creature->GetGUID());
            return true;
            break;

        case 11: //Enchant Boots
            AddGossipItemFor(player, 1, msg32AttackPower, GOSSIP_SENDER_MAIN, 191);
            AddGossipItemFor(player, 1, msg15StaminaMinorSpeedIncrease, GOSSIP_SENDER_MAIN, 192);
            AddGossipItemFor(player, 1, msg16Agility, GOSSIP_SENDER_MAIN, 193);
            AddGossipItemFor(player, 1, msg18Spirit, GOSSIP_SENDER_MAIN, 194);
            AddGossipItemFor(player, 1, msgRestore7HealthMp5, GOSSIP_SENDER_MAIN, 195);
            AddGossipItemFor(player, 1, msg12HitRating12Critical, GOSSIP_SENDER_MAIN, 196);
            AddGossipItemFor(player, 1, msg22Stamina, GOSSIP_SENDER_MAIN, 197);
            if (player->HasSkill(SKILL_ENGINEERING) && player->GetSkillValue(SKILL_ENGINEERING) == 450)
            {
                AddGossipItemFor(player, 1, msgNitroBoots, GOSSIP_SENDER_MAIN, 198);
                AddGossipItemFor(player, 1, HandMountedPyroRocket, GOSSIP_SENDER_MAIN, 199);
                AddGossipItemFor(player, 1, ReticulatedArmorWebbing, GOSSIP_SENDER_MAIN, 201);
            }
            AddGossipItemFor(player, GOSSIP_ICON_TALK, msgBack, GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100012, creature->GetGUID());
            return true;
            break;

        case 12: //Enchant rings
            AddGossipItemFor(player, 1, msg40AttackPower, GOSSIP_SENDER_MAIN, 202);
            AddGossipItemFor(player, 1, msg23SpellPower, GOSSIP_SENDER_MAIN, 203);
            AddGossipItemFor(player, 1, msg30Stamina, GOSSIP_SENDER_MAIN, 204);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, msgBack, GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100013, creature->GetGUID());
            return true;
            break;

        case 13: // Enchant 1H Weapon
            item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
            if (!item)
            {
                creature->Whisper(msgWhispRequiresWeaponEquipped, LANG_UNIVERSAL, player);
                player->PlayerTalkClass->SendCloseGossip();
                return false;
            }
            if (item->GetTemplate()->InventoryType == INVTYPE_WEAPON)
            {
                if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) == 450)
                {
                    AddGossipItemFor(player, 1, msgBladeWard, GOSSIP_SENDER_MAIN, 207);
                    AddGossipItemFor(player, 1, msgBloodDraining, GOSSIP_SENDER_MAIN, 208);
                }
                AddGossipItemFor(player, 1, msg26Agility, GOSSIP_SENDER_MAIN, 205);
                AddGossipItemFor(player, 1, msg45Spirit, GOSSIP_SENDER_MAIN, 206);
                AddGossipItemFor(player, 1, msgBerserking, GOSSIP_SENDER_MAIN, 209);
                AddGossipItemFor(player, 1, msg25HitRating25Critical, GOSSIP_SENDER_MAIN, 210);
                AddGossipItemFor(player, 1, msgBlackMagic, GOSSIP_SENDER_MAIN, 211);
                AddGossipItemFor(player, 1, msgBattlemaster, GOSSIP_SENDER_MAIN, 212);
                AddGossipItemFor(player, 1, msgIcebreaker, GOSSIP_SENDER_MAIN, 213);
                AddGossipItemFor(player, 1, msgLifeward, GOSSIP_SENDER_MAIN, 214);
                AddGossipItemFor(player, 1, msg50Stamina, GOSSIP_SENDER_MAIN, 215);
                AddGossipItemFor(player, 1, msg65AttackPower, GOSSIP_SENDER_MAIN, 216);
                AddGossipItemFor(player, 1, msg63SpellPower, GOSSIP_SENDER_MAIN, 217);
                AddGossipItemFor(player, 1, msgMongoose, GOSSIP_SENDER_MAIN, 218);
                AddGossipItemFor(player, 1, msgExecutioner, GOSSIP_SENDER_MAIN, 219);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, msgBack, GOSSIP_SENDER_MAIN, 300);
            }
            else
            {
                creature->Whisper(msgWhispRequiresWeaponEquipped, LANG_UNIVERSAL, player);
                player->PlayerTalkClass->SendGossipMenu(100004, creature->GetGUID());

            }
            player->PlayerTalkClass->SendGossipMenu(100004, creature->GetGUID());
            return true;
            break;

        case 100:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_AGILITY_1H);
            break;

        case 101:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_SPIRIT);
            break;

        case 102:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BLADE_WARD);
            break;

        case 103:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BLOOD_DRAINING);
            break;

        case 104:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BERSERKING);
            break;

        case 105:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_ACCURACY);
            break;

        case 106:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BLACK_MAGIC);
            break;

        case 107:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BATTLEMASTER);
            break;

        case 108:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_ICEBREAKER);
            break;

        case 109:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_LIFEWARD);
            break;

        case 110:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_TITANGUARD);
            break;

        case 111:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_POTENCY);
            break;

        case 112:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_MIGHTY_SPELL_POWER);
            break;

        case 113:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_MONGOOSE);
            break;

        case 114:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_EXECUTIONER);
            break;

        case 115:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_GREATER_SPELL_POWER);
            break;

        case 116:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_AGILITY);
            break;

        case 117:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_MASSACRE);
            break;

        case 118:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_DEFENSE);
            break;

        case 119:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_INTELLECT);
            break;

        case 120:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_RESILIENCE);
            break;

        case 121:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_TITANIUM_PLATING);
            break;

        case 122:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_STAMINA);
            break;

        case 123:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_TOUGHSHIELD);
            break;

        case 124:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_BLISSFUL_MENDING);
            break;

        case 125:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_BURNING_MYSTERIES);
            break;

        case 126:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_DOMINANCE);
            break;

        case 127:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_SAVAGE_GLADIATOR);
            break;

        case 128:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_STALWART_PROTECTOR);
            break;

        case 129:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_TORMENT);
            break;

        case 130:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_TRIUMPH);
            break;

        case 131:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_ECLIPSED_MOON);
            break;

        case 132:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_FLAME_SOUL);
            break;

        case 133:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_FLEEING_SHADOW);
            break;

        case 134:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_FROSTY_SOUL);
            break;

        case 135:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_TOXIC_WARDING);
            break;

        case 136:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_MASTERS_AXE);
            break;

        case 137:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_MASTERS_CRAG);
            break;

        case 138:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_MASTERS_PINNACLE);
            break;

        case 139:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_MASTERS_STORM);
            break;

        case 140:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_AXE);
            break;

        case 141:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_CRAG);
            break;

        case 142:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_GLADIATOR);
            break;

        case 143:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_PINNACLE);
            break;

        case 144:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_STORM);
            break;

        case 145:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_DOMINANCE);
            break;

        case 146:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_TRIUMPH);
            break;

        case 147:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_SPRINGY_ARACHNOWEAVE);
            break;

        case 148:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_SHADOW_ARMOR);
            break;

        case 149:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_DARKGLOW_EMBROIDERY);
            break;

        case 150:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_LIGHTWEAVE_EMBROIDERY);
            break;

        case 151:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_SWORDGUARD_EMBROIDERY);
            break;

        case 152:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_WISDOM);
            break;

        case 153:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_TITANWEAVE);
            break;

        case 154:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_SPELL_PIERCING);
            break;

        case 155:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_MIGHTY_ARMOR);
            break;

        case 156:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_MAJOR_AGILITY);
            break;

        case 157:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_GREATER_SPEED);
            break;

        case 158:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_POWERFUL_STATS);
            break;

        case 159:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_SUPER_HEALTH);
            break;

        case 160:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_GREATER_MAINA_REST);
            break;

        case 161:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_EXCEPTIONAL_RESIL);
            break;

        case 162:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_GREATER_DEFENSE);
            break;

        case 163:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_MAJOR_STAMINA);
            break;

        case 164:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_SUPERIOR_SP);
            break;

        case 165:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_GREATER_ASSUALT);
            break;

        case 166:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_MAJOR_SPIRT);
            break;

        case 167:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_EXPERTISE);
            break;

        case 168:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_GREATER_STATS);
            break;

        case 169:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_INTELLECT);
            break;

        case 170:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_ARCANE);
            break;

        case 171:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_FIRE);
            break;

        case 172:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_FROST);
            break;

        case 173:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_NATURE);
            break;

        case 174:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_SHADOW);
            break;

        case 175:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_ATTACK);
            break;

        case 176:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_STAMINA);
            break;

        case 177:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_SPELLPOWER);
            break;

        case 178:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_GREATER_BLASTING);
            break;

        case 179:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_ARMSMAN);
            break;

        case 180:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_CRUSHER);
            break;

        case 181:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_AGILITY);
            break;

        case 182:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_PRECISION);
            break;

        case 183:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_EXPERTISE);
            break;

        case 184:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_EARTHEN);
            break;

        case 185:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_FROSTHIDE);
            break;

        case 186:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_ICESCALE);
            break;

        case 187:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_BRILLIANT_SPELLTHREAD);
            break;

        case 188:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_SAPPHIRE_SPELLTHREAD);
            break;

        case 189:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_DRAGONSCALE);
            break;

        case 190:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_WYRMSCALE);
            break;

        case 191:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_GREATER_ASSULT);
            break;

        case 192:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_TUSKARS_VITLIATY);
            break;

        case 193:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_SUPERIOR_AGILITY);
            break;

        case 194:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_GREATER_SPIRIT);
            break;

        case 195:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_GREATER_VITALITY);
            break;

        case 196:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_ICEWALKER);
            break;

        case 197:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_GREATER_FORTITUDE);
            break;

        case 198:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_NITRO_BOOTS);
            break;

        case 199:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_PYRO_ROCKET);
            break;

        case 200:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_HYPERSPEED);
            break;

        case 201:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_ARMOR_WEBBING);
            break;

        case 202:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1), ENCHANT_RING_ASSULT);
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2), ENCHANT_RING_ASSULT);
            break;

        case 203:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1), ENCHANT_RING_GREATER_SP);
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2), ENCHANT_RING_GREATER_SP);
            break;

        case 204:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1), ENCHANT_RING_STAMINA);
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2), ENCHANT_RING_STAMINA);
            break;

        case 205:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_AGILITY_1H);
            break;

        case 206:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_SPIRIT);
            break;

        case 207:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BLADE_WARD);
            break;

        case 208:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BLOOD_DRAINING);
            break;

        case 209:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BERSERKING);
            break;

        case 210:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_ACCURACY);
            break;

        case 211:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BLACK_MAGIC);
            break;

        case 212:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BATTLEMASTER);
            break;

        case 213:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_ICEBREAKER);
            break;

        case 214:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_LIFEWARD);
            break;

        case 215:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_TITANGUARD);
            break;

        case 216:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_POTENCY);
            break;

        case 217:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_MIGHTY_SPELL_POWER);
            break;

        case 218:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_2WEP_MONGOOSE);
            break;

        case 219:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_EXECUTIONER);
            break;
        
        case 300:
        {
            AddGossipItemFor(player, 1, msgMainWeapon, GOSSIP_SENDER_MAIN, 1);
            if (player->HasSpell(674))
            {
                AddGossipItemFor(player, 1, msgOffhandWeapon, GOSSIP_SENDER_MAIN, 13);
            }
            AddGossipItemFor(player, 1, msgEnchant2HWeapon, GOSSIP_SENDER_MAIN, 2);
            AddGossipItemFor(player, 1, msgEnchantShield, GOSSIP_SENDER_MAIN, 3);
            AddGossipItemFor(player, 1, msgEnchantHead, GOSSIP_SENDER_MAIN, 4);
            AddGossipItemFor(player, 1, msgEnchantShoulders, GOSSIP_SENDER_MAIN, 5);
            AddGossipItemFor(player, 1, msgEnchantCloak, GOSSIP_SENDER_MAIN, 6);
            AddGossipItemFor(player, 1, msgEnchantChest, GOSSIP_SENDER_MAIN, 7);
            AddGossipItemFor(player, 1, msgEnchantBracers, GOSSIP_SENDER_MAIN, 8);
            AddGossipItemFor(player, 1, msgEnchantGloves, GOSSIP_SENDER_MAIN, 9);
            AddGossipItemFor(player, 1, msgEnchantLegs, GOSSIP_SENDER_MAIN, 10);
            AddGossipItemFor(player, 1, msgEnchantBoots, GOSSIP_SENDER_MAIN, 11);

            if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) == 450)
                AddGossipItemFor(player, 1, msgEnchantRings, GOSSIP_SENDER_MAIN, 12);

            player->PlayerTalkClass->SendGossipMenu(601015, creature->GetGUID());
            return true;
            break;
        }
        }

        player->PlayerTalkClass->SendCloseGossip();
        return true;
    }

    void Enchant(Player* player, Creature* creature, Item* item, uint32 enchantid)
    {
        // Locales ruRU / enGB
        WorldSession* session = player->GetSession();

        if (!item)
        {
            creature->HandleEmoteCommand(EMOTE_ONESHOT_LAUGH);
            switch (session->GetSessionDbLocaleIndex())
            {
            case LOCALE_ruRU:
            {
                creature->Whisper("Пожалуйста, экипируйте предмет, который желаете зачаровать!", LANG_UNIVERSAL, player);
                break;
            }
            default:
                creature->Whisper("Please equip the item you would like to enchant!", LANG_UNIVERSAL, player);
                break;
            }
            player->PlayerTalkClass->SendCloseGossip();
            return;
        }

        if (!enchantid)
        {
            switch (session->GetSessionDbLocaleIndex())
            {
            case LOCALE_ruRU:
            {
                player->GetSession()->SendNotification("Что-то пошло не так в коде. Он зарегистрирован для разработчиков и будет рассмотрен, извините за неудобства.");
                break;
            }
            default:
                player->GetSession()->SendNotification("Something went wrong in the code. It has been logged for developers and will be looked into, sorry for the inconvenience.");
                break;
            }
            player->PlayerTalkClass->SendCloseGossip();
            creature->HandleEmoteCommand(EMOTE_ONESHOT_LAUGH);
            return;
        }

        // Roll the dice
        roll = urand(1, 100);

        item->ClearEnchantment(PERM_ENCHANTMENT_SLOT);
        item->SetEnchantment(PERM_ENCHANTMENT_SLOT, enchantid, 0, 0);

        // Random enchantment notification
        if (roll > 0 && roll < 33)
        {
            switch (session->GetSessionDbLocaleIndex())
            {
            case LOCALE_ruRU:
            {
                player->GetSession()->SendNotification("|cff00ff00Костлявый палец Борегарда потрескивает от энергии во время прикосновения |cffDA70D6%s|cff00ff00!", item->GetTemplate()->Name1.c_str());
                break;
            }
            default:
                player->GetSession()->SendNotification("|cff00ff00Beauregard's bony finger crackles with energy when he touches |cffDA70D6%s|cff00ff00!", item->GetTemplate()->Name1.c_str());
                break;
            }
            creature->HandleEmoteCommand(EMOTE_ONESHOT_POINT);
            player->PlayerTalkClass->SendCloseGossip();
        }
        else if (roll > 33 && roll < 75)
        {
            switch (session->GetSessionDbLocaleIndex())
            {
            case LOCALE_ruRU:
            {
                player->GetSession()->SendNotification("|cff00ff00Борегар замирает |cffDA70D6%s |cff00ff00поднимается в воздух и произносит странное заклинание!", item->GetTemplate()->Name1.c_str());
                break;
            }
            default:
                player->GetSession()->SendNotification("|cff00ff00Beauregard holds |cffDA70D6%s |cff00ff00up in the air and utters a strange incantation!", item->GetTemplate()->Name1.c_str());
                break;
            }
            creature->HandleEmoteCommand(EMOTE_ONESHOT_CHEER);
            player->PlayerTalkClass->SendCloseGossip();
        }
        else
        {
            switch (session->GetSessionDbLocaleIndex())
            {
            case LOCALE_ruRU:
            {
                player->GetSession()->SendNotification("|cff00ff00Борегар глубоко концентрируется, взмахивая палочкой |cffDA70D6%s|cff00ff00!", item->GetTemplate()->Name1.c_str());
                break;
            }
            default:
                player->GetSession()->SendNotification("|cff00ff00Beauregard concentrates deeply while waving his wand over |cffDA70D6%s|cff00ff00!", item->GetTemplate()->Name1.c_str());
                break;
            }
            creature->HandleEmoteCommand(EMOTE_ONESHOT_WAVE);
            player->PlayerTalkClass->SendCloseGossip();
        }
    }

    // Passive Emotes
    struct NPC_PassiveAI : public ScriptedAI
    {
        NPC_PassiveAI(Creature * creature) : ScriptedAI(creature) { }

        uint32 MessageTimer;

        // Called once when client is loaded
        void Reset()
        {
            MessageTimer = urand(EnchanterMessageTimer, 300000); // 1-5 minutes
        }

        // Called at World update tick
        void UpdateAI(const uint32 diff)
        {
            // If Enabled
            if (EnchanterMessageTimer != 0)
            {
                if (MessageTimer <= diff)
                {
                    std::string Message = PickPhrase();
                    me->Say(Message.c_str(), LANG_UNIVERSAL);

                    // Use gesture?
                    if (EnchanterEmoteCommand != 0)
                    {
                        me->HandleEmoteCommand(EnchanterEmoteCommand);
                    }

                    // Alert players?
                    if (EnchanterEmoteSpell != 0)
                    {
                        me->CastSpell(me, EnchanterEmoteSpell);
                    }

                    MessageTimer = urand(EnchanterMessageTimer, 300000);
                }
                else { MessageTimer -= diff; }
            }
        }
    };

    // CREATURE AI
    CreatureAI * GetAI(Creature * creature) const
    {
        return new NPC_PassiveAI(creature);
    }
};

void AddNPCEnchanterScripts()
{
    new EnchanterConfig();
    new EnchanterAnnounce();
    new npc_enchantment();
}
