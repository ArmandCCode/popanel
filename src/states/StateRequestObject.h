#ifndef StateRequestObject_H
#define StateRequestObject_H

class StateRequestObject
{
    public:
        StateRequestObject();
        virtual ~StateRequestObject();

        // Toujours valides
        static const int STARTUP = -1;
        static const int CLOSEAPP = -2;

        // Garder un ordre séquentiel
        static const int TITLE = 0;
        static const int GAME = 1;
        static const int KEYCONFIG = 2;

        int GetDesiredState() { return m_DesiredState; }
        void SetDesiredState(int val) { m_DesiredState = val; }
    protected:
    private:
        int m_DesiredState;
};

#endif // StateRequestObject_H
