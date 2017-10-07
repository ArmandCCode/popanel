#ifndef SPEVENT_H
#define SPEVENT_H


class SPEvent
{
    public:
        SPEvent();
        virtual ~SPEvent();

        int GetActionNumber() { return m_ActionNumber; }
        void SetActionNumber(int val) { m_ActionNumber = val; }
        int GetPlayerNumber() { return m_PlayerNumber; }
        void SetPlayerNumber(int val) { m_PlayerNumber = val; }
        bool IsPressEvent() { return m_PressEvent; }
        void SetPressEvent(bool val) { m_PressEvent = val; }

    protected:

    private:
        int m_ActionNumber;
        int m_PlayerNumber;
        bool m_PressEvent;
};

#endif // SPEVENT_H
