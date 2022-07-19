#include "StatBoostMgr.h"

StatType StatBoostMgr::GetStatTypeFromSubClass(Item* item)
{
    switch (item->GetTemplate()->SubClass)
    {
    case ITEM_SUBCLASS_ARMOR_CLOTH:
        return STAT_TYPE_SPELL;


    case ITEM_SUBCLASS_ARMOR_LEATHER:
    case ITEM_SUBCLASS_ARMOR_MAIL:
    case ITEM_SUBCLASS_ARMOR_PLATE:
        switch (urand(0, 3))
        {
        case 0:
            return STAT_TYPE_TANK;

        case 1:
            return STAT_TYPE_PHYS;

        case 2:
            return STAT_TYPE_HYBRID;

        case 3:
            return STAT_TYPE_SPELL;

        default:
            return STAT_TYPE_NONE;
        }
    }
}

StatType StatBoostMgr::AnalyzeItem(Item* item)
{
    auto iTemplate = item->GetTemplate();

    uint32 spellsCount = 0;
    for (int i = 0; i < (sizeof(iTemplate->Spells) / sizeof(iTemplate->Spells[0])); i++)
    {
        if (iTemplate->Spells[i].SpellId)
        {
            spellsCount++;
        }
    }

    if (iTemplate->StatsCount < 1 && spellsCount < 1)
    {
        return GetStatTypeFromSubClass(item);
    }

    return STAT_TYPE_NONE;
}

bool StatBoostMgr::BoostItem(Player* player, Item* item)
{
    if (!IsEquipment(item))
    {
        return false;
    }

    StatType statType = AnalyzeItem(item);

    return true;
}

bool StatBoostMgr::IsEquipment(Item* item)
{
    auto itemTemplate = item->GetTemplate();

    if (itemTemplate->Class != ITEM_CLASS_WEAPON &&
        itemTemplate->Class != ITEM_CLASS_ARMOR)
    {
        return false;
    }

    return true;
}

bool StatBoostMgr::EnchantItem(Player* player, Item* item, EnchantmentSlot slot, uint32 enchantId, bool overwrite)
{
    if (item->GetEnchantmentId(slot) && !overwrite)
    {
        return false;
    }

    player->ApplyEnchantment(item, false);
    item->SetEnchantment(EnchantmentSlot(slot), enchantId, 0, 0);
    player->ApplyEnchantment(item, true);

    return true;
}
