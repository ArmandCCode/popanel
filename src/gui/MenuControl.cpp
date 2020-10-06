#include "MenuControl.h"

MenuControl::MenuControl() : m_MenuEntries(0), m_SelectedEntry(0), m_MenuWidth(0), m_StrBufEntries(0), m_StrBufValues(0)
{

}

MenuControl::~MenuControl()
{

    this->RemoveEverything();
}

void MenuControl::RemoveEverything()
{
    this->m_MenuEntries.clear();
    this->m_StrBufValues.clear();
    this->m_StrBufEntries.clear();
    m_SelectedEntry = 0;
}

bool MenuControl::AddMenuEntry(std::string p_Label, int p_EntryType, long p_DefaultValue, long p_MinValue, long p_MaxValue, std::vector<std::string> p_ValueStrings )
{
    MenuEntry tmpEntry;
    tmpEntry.IDEntry = -1; // temporaire, sera définitif après son insertion dans le tableau

    // Selon la valeur de p_EntryType, on stocke des infos différentes
    switch (p_EntryType)
    {
        case TYPE_VALIDATOR:
            // Rien de spécial a vérifier pour les validateurs
            break;
        case TYPE_SPACER:
            // Rien de spécial a vérifier pour les spacers
            break;
        case TYPE_NUMSELECTOR:
            // Vérifier que la valeur par défaut se situe entre le minimum et le maximum
            if (p_DefaultValue < p_MinValue || p_DefaultValue > p_MaxValue)
            {
                p_DefaultValue = p_MinValue;
            }
            break;
        case TYPE_STRINGLIST:
            // Vérifier que l'on a bien envoyé au moins une chaîne de caractère
            if (p_ValueStrings.size() > 0)
            {
                // Changer la valeur de minimum et maximum pour convenir au nombre de chaîne passées en paramètre
                p_MinValue = 0;
                p_MaxValue = (p_ValueStrings.size() - 1);
                // Vérifier que la valeur par défaut se situe entre le minimum et le maximum
                if (p_DefaultValue < p_MinValue || p_DefaultValue > p_MaxValue)
                {
                    p_DefaultValue = p_MinValue;
                }
            }
            else
            {
                // Si il n'y a pas de liste de chaîne de caractères, insérer une chaîne "<vide>" dans la liste
                p_ValueStrings.push_back("<empty>");
                // Et mettre les valeurs par défaut à 0
                p_MinValue = 0;
                p_MaxValue = p_MinValue;
                p_DefaultValue = p_MinValue;
            }

            break;
        case TYPE_NUMANDSTRING:
            // Vérifier que l'on a bien envoyé au moins une chaîne de caractère
            if (p_ValueStrings.size() > 0)
            {
                // Changer la valeur de minimum et maximum pour convenir au nombre de chaîne passées en paramètre
                p_MinValue = p_DefaultValue;
                p_MaxValue = p_DefaultValue;
                p_DefaultValue = p_DefaultValue;
                // N'enregistrer que la première chaîne de la liste : on enlève toutes les autres
                while (p_ValueStrings.size() > 1)
                {
                    p_ValueStrings.pop_back();
                }

            }
            else
            {
                // Si il n'y a pas de liste de chaîne de caractères, insérer une chaîne "<vide>" dans la liste
                p_ValueStrings.push_back("<empty>");
                // Et mettre les valeurs par défaut à 0
                p_MinValue = 0;
                p_MaxValue = p_MinValue;
                p_DefaultValue = p_MinValue;
            }

            break;
        default:
            // Oh, vilain, un mauvais code ! Alors ce sera un validateur !
            p_EntryType = TYPE_VALIDATOR;
            break;
    }

    // Remplir le struct
    tmpEntry.Label = p_Label;
    tmpEntry.EntryType = p_EntryType;
    tmpEntry.Value = p_DefaultValue;
    tmpEntry.MinValue = p_MinValue;
    tmpEntry.MaxValue = p_MaxValue;
    tmpEntry.ValueStrings = p_ValueStrings;

    // L'envoyer à l'arrière du tableau pour obtenir un ID
    this->m_MenuEntries.push_back(tmpEntry);
    // Assigner l'ID du tableau à l'ID du menu
    this->m_MenuEntries.back().IDEntry = (this->m_MenuEntries.size() - 1);

    // Récupérer les infos dans notre struct temporaire
    tmpEntry = this->m_MenuEntries.back();

    // Créer les string buffers de chaque option
    // Libellé
    this->m_StrBufEntries.push_back(SPStringBuffer());

    // Valeur
    this->m_StrBufValues.push_back(SPStringBuffer());

    return true;
}

void MenuControl::isIDValid(int p_IDEntry)
{
    // The returned result doesn't actually matter, because an exception will be raised if the ID is invalid
    if (p_IDEntry < 0 || p_IDEntry >= this->m_MenuEntries.size())
    {
        // Exception, can't access an entry out of bounds
        SPException e("MenuControl::isIDValid : value out of range (" + SPConv::NumToStr(p_IDEntry) + ")" );
        throw e;
    }

    // No exception thrown ? Well, let's continue
}

std::string MenuControl::GetLabel(int p_IDEntry)
{
    // Is entry ID in range
    isIDValid(p_IDEntry);

    return this->m_MenuEntries[p_IDEntry].Label;
}

long MenuControl::GetValue(int p_IDEntry)
{
    // Is entry ID in range
    isIDValid(p_IDEntry);

    return this->m_MenuEntries[p_IDEntry].Value;
}

std::string MenuControl::GetString(int p_IDEntry)
{
    // Is entry ID in range
    isIDValid(p_IDEntry);

    // Special case for TYPE_NUMANDSTRING : there is only one string value, which is the first in the array
    if (this->m_MenuEntries[p_IDEntry].EntryType == TYPE_NUMANDSTRING)
        return this->m_MenuEntries[p_IDEntry].ValueStrings[0];

    // Return string value corresponding to selected numeric value
    return this->m_MenuEntries[p_IDEntry].ValueStrings[this->GetValue(p_IDEntry)];
}

bool MenuControl::SetValue(int p_IDEntry, int p_Value)
{
    // Is entry ID in range
    isIDValid(p_IDEntry);

    // Using a reference to simplify code reading
    MenuEntry& thisEntry = this->m_MenuEntries[p_IDEntry];

    // Is forced value within the min and max range
    if (p_Value < thisEntry.MinValue)
    {
        p_Value = thisEntry.MaxValue;
    }
    else if (p_Value > thisEntry.MaxValue)
    {
        p_Value = thisEntry.MinValue;
    }
    thisEntry.Value = p_Value;

    return true;
}

bool MenuControl::NextValue(int p_IDEntry)
{
    this->SetValue(p_IDEntry, (this->GetValue(p_IDEntry) + 1) );
    return true;
}

bool MenuControl::PrevValue(int p_IDEntry)
{
    this->SetValue(p_IDEntry, (this->GetValue(p_IDEntry) - 1) );
    return true;
}

bool MenuControl::SetValueNumAndString(int p_IDEntry, int p_Value, std::string p_StringValue)
{
    // Is ID in range
    isIDValid(p_IDEntry);

    // Do nothing if entry type is not TYPE_NUMANDSTRING
    if (this->m_MenuEntries[p_IDEntry].EntryType != TYPE_NUMANDSTRING)
        return false;

    // Using a reference to simplify code reading
    MenuEntry& thisEntry = this->m_MenuEntries[p_IDEntry];

    // Force numerical value
    thisEntry.MaxValue = p_Value;
    thisEntry.MinValue = p_Value;
    thisEntry.Value = p_Value;

    while (thisEntry.ValueStrings.size() > 0)
    {
        thisEntry.ValueStrings.pop_back();
    }
    thisEntry.ValueStrings.push_back(p_StringValue);

    return true;

}

// Navigation
bool MenuControl::JumpToEntry(int p_IDEntry)
{
    // Wrap around the available menus when selecting one that is out of range
    if (p_IDEntry < 0)
    {
        p_IDEntry = (this->m_MenuEntries.size() - 1) ;
    }
    else if (p_IDEntry >= this->m_MenuEntries.size())
    {
        p_IDEntry = 0 ;
    }
    this->m_SelectedEntry = p_IDEntry;
    return true;
}

bool MenuControl::NextEntry()
{
    // Aller au menu suivant
    do
    {
        this->JumpToEntry(this->GetSelectedEntry() + 1);
        // Si on tombe sur un spacer, on continue de changer
    } while ( (this->m_MenuEntries[this->GetSelectedEntry()].EntryType == MenuControl::TYPE_SPACER) );
    return true;
}

bool MenuControl::PrevEntry()
{
    // Aller au menu précédent
    do
    {
        this->JumpToEntry(this->GetSelectedEntry() - 1);
        // Si on tombe sur un spacer, on continue de changer
    } while ( (this->m_MenuEntries[this->GetSelectedEntry()].EntryType == MenuControl::TYPE_SPACER) );
    return true;
}

void MenuControl::RenderToWindow(SPDisplayWindow& p_window, long p_X, long p_Y, SPTTFFont& p_RenderFont)
{
    // Faire un cadre pouvant contenir le menu en hauteur
    // Nombre d'occurences * taille de police
    unsigned long DrawH = this->m_MenuEntries.size() * p_RenderFont.GetRequestedHeight();
    long DrawX = p_X;
    long DrawY = p_Y;
    p_window.DrawSquareWithOuterBorder(DrawX, DrawY, this->m_MenuWidth, DrawH, 0, 0, 0, 128, 2, 255, 255, 255, 255);

    for (int i = 0; i < this->m_MenuEntries.size(); i++)
    {
        MenuEntry& thisEntry = this->m_MenuEntries[i];

        DrawY = p_Y+(p_RenderFont.GetRequestedHeight()*i);

        // Selection square
        if (i == this->m_SelectedEntry)
        {
            p_window.DrawSquare(DrawX, DrawY, this->m_MenuWidth, p_RenderFont.GetRequestedHeight(), 255, 255, 255, 128);
        }

        // Label
        // If label is empty, no need to render the string
        if (this->GetLabel(i) != "" )
        {
            this->m_StrBufEntries[i].SetColor(C_FontProperties::MENUELTFONTR, C_FontProperties::MENUELTFONTG, C_FontProperties::MENUELTFONTB);
            this->m_StrBufEntries[i].RasterizeString( this->GetLabel(i) , p_RenderFont);
            this->m_StrBufEntries[i].RenderToWindow(p_window, DrawX, DrawY);
        }


        // Value

        switch (thisEntry.EntryType)
        {
            case MenuControl::TYPE_NUMSELECTOR:
            case MenuControl::TYPE_STRINGLIST:
            case MenuControl::TYPE_NUMANDSTRING:

                this->m_StrBufValues[i].SetColor(C_FontProperties::MENUELTFONTR, C_FontProperties::MENUELTFONTG, C_FontProperties::MENUELTFONTB);
                switch (thisEntry.EntryType)
                {
                    case MenuControl::TYPE_NUMSELECTOR:
                        // Si c'est un sélecteur numérique, on dessine la valeur numérique
                        this->m_StrBufValues[i].RasterizeString( "< " + SPConv::NumToStr( this->GetValue(i) ) + " >" , p_RenderFont);
                        break;
                    case MenuControl::TYPE_STRINGLIST:
                        // Si c'est un sélecteur textuel, on dessine la valeur textuelle
                        this->m_StrBufValues[i].RasterizeString( "< " + this->GetString(i) + " >" , p_RenderFont);
                        break;
                    case MenuControl::TYPE_NUMANDSTRING:
                        // Si c'est un sélecteur textuel, on dessine la valeur textuelle
                        this->m_StrBufValues[i].RasterizeString( "- " + this->GetString(i) + " -" , p_RenderFont);
                        break;
                }
                this->m_StrBufValues[i].RenderToWindow(p_window, DrawX+this->m_MenuWidth, DrawY, SPTexture::RENDER_FROMTOPRIGHT);

            default:
                // Dans les autres cas, on ne dessine rien
            break;
        }

    }

}
