#ifndef SPBASESTATE_H
#define SPBASESTATE_H

/* Simple model for a state-based engine. Inherit from this to use */
class SPBaseState
{
    public:
        SPBaseState();
        virtual ~SPBaseState();
        virtual bool Play(long p_Delta = 0) = 0; // Events and game logic
        virtual bool Frame() = 0; // Drawing graphics
    protected:
        virtual bool LoadResources() = 0; // Load resources (graphics, sounds)
        virtual bool UnloadResources() = 0; // Unload everything
    private:
};

#endif // SPBASESTATE_H
