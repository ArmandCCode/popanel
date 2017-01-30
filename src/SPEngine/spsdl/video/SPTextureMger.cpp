#include "SPEngine/spsdl/video/SPTextureMger.h"

SPTextureMger::SPTextureMger()
{

}

SPTextureMger::~SPTextureMger()
{

    this->UnloadEverything();
}

bool SPTextureMger::LoadTextureList(std::map<int, std::string> m_TexPathList)
{
    for (unsigned int i = 0; i < m_TexPathList.size(); i++)
    {
        if ( m_TexPathList.find(i) == m_TexPathList.end() )
        {
            this->UnloadEverything();
            return false;
        }
        else
        {
            this->AttachAndLoadTexture( i, m_TexPathList[i] );
        }
    }

    return true;
}

void SPTextureMger::AttachAndLoadTexture(int p_IDTex, std::string p_sTexPath)
{
    SPTexture tmpTex;
    // Envoyer une texture vide dans le tableau
    this->m_texList[p_IDTex] = tmpTex;

    // Charger la texture
    try
    {
        this->m_texList.at(p_IDTex).LoadSurface(p_sTexPath);
    }
    catch (SPException e)
    {
        std::string sMess("SPTextureMger::AttachAndLoadTexture() : Unable to attach filename \"" + p_sTexPath + "\".\n");
        e.add(sMess);
        this->m_texList.erase(p_IDTex);
        throw e;
    }

}

void SPTextureMger::UnloadTexture(int p_IDTex)
{
    this->m_texList.erase(p_IDTex);
}

SPTexture& SPTextureMger::GetTextureByID(int p_IDTex)
{
    return this->m_texList[p_IDTex];
}

SPTexture& SPTextureMger::operator[](int p_IDTex){
    return this->GetTextureByID(p_IDTex);
}


void SPTextureMger::UnloadEverything()
{
    this->m_texList.clear();
}
