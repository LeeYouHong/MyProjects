#include "GlobalManager.h"

CGlobalManager::CGlobalManager()
:m_bOnline(false)
{
    //ctor
}

CGlobalManager::~CGlobalManager()
{
    //dtor
}

CGlobalManager *CGlobalManager::Instance(void)
{
    static CGlobalManager instance;
    return &instance;
}
