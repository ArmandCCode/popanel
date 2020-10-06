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
        static const int TYPE_SPACER = 3; // Just a spacer, cannot be selected
        static const int TYPE_NUMANDSTRING = 4; // Will accept a integer-string pair

        bool AddMenuEntry(std::string p_Label, int p_EntryType = TYPE_VALIDATOR, long p_DefaultValue = 0, long p_MinValue = 0, long p_MaxValue = 99, std::vector<std::string> p_ValueStrings = std::vector<std::string>() );

        void RemoveEverything();

        std::string GetLabel(int p_IDEntry);

        long GetValue(int p_IDEntry); // get the integer part of the integer-string pair
        std::string GetString(int p_IDEntry); // get the string part of the integer-string pair

        bool SetValue(int p_IDEntry, int p_Value); // Force a value (as long as it is in range) // just returns false if type is TYPE_NUMANDSTRING
        bool NextValue(int p_IDEntry); // Next value, loops back to minimum value if value above maximum // just returns false if type is TYPE_NUMANDSTRING
        bool PrevValue(int p_IDEntry); // Previous value, loops forward to maximum value if value below minimum // // just returns false if type is TYPE_NUMANDSTRING

        bool SetValueNumAndString(int p_IDEntry, int p_Value, std::string p_StringValue); // This function will do nothing and return false if the type of the entry is not TYPE_NUMANDSTRING

        // Navigation
        long GetSelectedEntry() { return m_SelectedEntry; }
        bool JumpToEntry(int p_IDEntry);
        bool NextEntry();
        bool PrevEntry();

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
