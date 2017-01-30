#ifndef MENUCONTROL_H
#define MENUCONTROL_H

#include <string>
#include <vector>
#include "SPEngine/sputil/SPConvertTools.h"
#include "SPEngine/spsdl/video/SPDisplayWindow.h"
#include "SPEngine/spsdl/video/SPTTFFont.h"
#include "SPEngine/spsdl/video/SPStringBuffer.h"
#include "gui/consts/PanelGUIConsts.h"

struct MenuEntry
{
    int IDEntry;
    std::string Label;
    int EntryType; // Validateur, Sélecteur numérique (titre + id numérique),Sélecteur texte (titre + id + texte), Séparateur (vide, non surlignable)
    long Value;
    long MinValue;
    long MaxValue;
    std::vector<std::string> ValueStrings;
};

class MenuControl
{
    public:
        MenuControl();
        virtual ~MenuControl();

        static const int TYPE_VALIDATOR = 0;
        static const int TYPE_NUMSELECTOR = 1;
        static const int TYPE_STRINGLIST = 2;
        static const int TYPE_SPACER = 3; // Ne peut pas être mis en surbrillance, sert juste a espacer les menus
        static const int TYPE_NUMANDSTRING = 4; // Will accept a number (int) and string pair

        bool AddMenuEntry(std::string p_Label, int p_EntryType = TYPE_VALIDATOR, long p_DefaultValue = 0, long p_MinValue = 0, long p_MaxValue = 99, std::vector<std::string> p_ValueStrings = std::vector<std::string>() );

        void RemoveEverything();

        std::string GetLabel(int p_IDEntry); // Récupérer le libellé de l'entrée

        long GetValue(int p_IDEntry); // Récupérer la valeur actuelle de l'entrée
        std::string GetString(int p_IDEntry); // Récupérer la chaîne associée à la valeur actuelle de l'entrée

        bool SetValue(int p_IDEntry, int p_Value); // Forcer une valeur (dans les limites du domaine défini par défaut) // ne fait rien et renvoie faux si le type est un TYPE_NUMANDSTRING
        bool NextValue(int p_IDEntry); // Aller à la valeur suivante : boucle sur la minimale si on va au delà du max // ne fait rien et renvoie faux si le type est un TYPE_NUMANDSTRING
        bool PrevValue(int p_IDEntry); // Aller à la valeur précédente : boucle sur la minimale si on va en dessous du min // ne fait rien et renvoie faux si le type est un TYPE_NUMANDSTRING

        bool SetValueNumAndString(int p_IDEntry, int p_Value, std::string p_StringValue); // This function will do nothing and return false if the type of the entry is not TYPE_NUMANDSTRING

        // Navigation
        long GetSelectedEntry() { return m_SelectedEntry; }
        bool SelectEntry(int p_IDEntry); // Sauter à un menu du code indiqué
        bool NextEntry(); // Aller au menu suivant
        bool PrevEntry(); // Aller au menu précédent

        void SetMenuWidth(unsigned long p_width) { m_MenuWidth = p_width; }

        void UpdateLabels(SPTTFFont& p_RenderFont);

        // Dessine le cadre, le sélecteur et les textes
        void RenderToWindow(SPDisplayWindow& p_window, long p_X, long p_Y, SPTTFFont& p_RenderFont);

    protected:
    private:

        std::vector<MenuEntry> m_MenuEntries;

        int m_SelectedEntry;

        unsigned long m_MenuWidth;
        std::vector<SPStringBuffer> m_StrBufEntries;
        // Note : le contenu de ces string buffers changera régulièrement : non optimal dans un contexte GPU, mais OK dans un contexte CPU comme SDL 1.2
        std::vector<SPStringBuffer> m_StrBufValues;

        // Safety check for menu entry IDs, when trying to access their values directly. Throws an exception if ID out of range.
        void isIDValid(int p_IDEntry);

};

#endif // MENUCONTROL_H
