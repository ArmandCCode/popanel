#ifndef SPTEXTUREMGER_H
#define SPTEXTUREMGER_H

#include "SPTexture.h"
#include <map>
#include <vector>

class SPTextureMger
{
    public:
        SPTextureMger();
        virtual ~SPTextureMger();

        // Chargement/D�chargement de masse
        bool LoadTextureList(std::map<int, std::string> m_TexPathList);
        void UnloadEverything();

        // R�cup�ration d'une texture pour exploitation
        SPTexture& GetTextureByID(int p_IDTex);
        SPTexture& operator[](int p_IDTex); // Appelle GetTextureByID();

        // Chargement/D�chargement individuel
        void AttachAndLoadTexture(int p_IDTex, std::string p_sTexPath);
        void UnloadTexture(int p_IDTex);

    protected:
        std::map<int, SPTexture> m_texList;
    private:

};

#endif // SPTEXTUREMGER_H
