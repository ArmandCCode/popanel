#include "SPEngine/spsdl/SPBaseState.h"

SPBaseState::SPBaseState()
{

    this->LoadResources();
}

SPBaseState::~SPBaseState()
{

    this->UnloadResources();
}


bool SPBaseState::LoadResources() // Load resources (graphics, sounds)
{
    return false;
}

bool SPBaseState::UnloadResources() // Unload everything
{
    return false;
}
