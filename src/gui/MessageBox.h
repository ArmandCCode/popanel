#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <string>
#include <vector>
#include "SPEngine/spsdl/video/SPStringBuffer.h"

class MessageBox
{
    public:
        MessageBox();
        virtual ~MessageBox();

        void SetLinesInMessage(unsigned long p_LineNumber);

        void SetLineString(unsigned long p_LineNumber, std::string p_Text, SPTTFFont& p_RenderFont, unsigned long p_red, unsigned long p_green, unsigned long p_blue);

        void SetTextCenter(bool p_Center) { m_MessageBoxTextCenter = p_Center; }
        void SetMessageBoxWidth(unsigned long p_Width) { m_MessageBoxWidth = p_Width; }

        void RenderToWindow(SPDisplayWindow& p_window, long p_X, long p_Y, bool p_BoxCenter = false);

    protected:

    private:
        // Graphic resources
        std::vector<SPStringBuffer> m_MessageText;

        // Display options
        // Frame sizing
        unsigned long m_MessageBoxWidth;
        bool isAutoWidth();
        // Text formatting
        bool m_MessageBoxTextCenter;
};

#endif // MESSAGEBOX_H
