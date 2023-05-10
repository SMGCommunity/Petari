#include "nw4r/lyt/lyt_texMap.h"
#include "nw4r/lyt/lyt_common.h"

namespace nw4r {
    namespace lyt {
        void TexMap::Get(GXTexObj *pObj) const {
            if (detail::IsCITexelFormat(GetTexelFormat())) {
                const u32 tlutName = GXGetTexObjTlut(pObj);
                GXInitTexObjCI(
                    pObj,
                    GetImage(),
                    GetWidth(),
                    GetHeight(),
                    GXCITexFmt(GetTexelFormat()),
                    GetWrapModeS(),
                    GetWrapModeT(),
                    IsMipMap(),
                    tlutName
                );
            }
            else {
                GXInitTexObj(
                    pObj,
                    GetImage(),
                    GetWidth(),
                    GetHeight(),
                    GetTexelFormat(),
                    GetWrapModeS(),
                    GetWrapModeT(),
                    IsMipMap()
                );
            }

            GXInitTexObjLOD(
                pObj,
                GetMinFilter(),
                GetMagFilter(), 
                GetMinLOD(),
                GetMaxLOD(),
                GetLODBias(),
                IsBiasClampEnable(),
                IsEdgeLODEnable(),
                GetAnisotropy()
            );
        }

        void TexMap::Get(GXTlutObj *pObj) const {
            GXInitTlutObj(pObj, GetPalette(), GetPaletteFormat(), GetPaletteEntryNum());
        }

        void TexMap::Set(const GXTexObj &rTexObj) {
            {
                void* pImage;
                u16 width, height;
                GXTexFmt format;
                GXTexWrapMode wrapS, wrapT;
                GXBool mipmap;

                GXGetTexObjAll(&rTexObj, &pImage, &width, &height, &format, &wrapS, &wrapT, &mipmap);

                SetImage(pImage);
                SetSize(width, height);
                SetTexelFormat(format);
                SetWrapMode(wrapS, wrapT);
                SetMipMap(mipmap);
            }
            
            {
                GXTexFilter minFilter;
                GXTexFilter magFilter;
                f32 minLOD, maxLOD, lodBias;
                GXBool biasClamp, edgeLODEnable;
                GXAnisotropy aniso;

                GXGetTexObjLODAll(&rTexObj, &minFilter, &magFilter, &minLOD, &maxLOD, &lodBias, &biasClamp, &edgeLODEnable, &aniso);

                SetFilter(minFilter, magFilter);
                SetLOD(minLOD, maxLOD);
                SetLODBias(lodBias);
                SetBiasClampEnable(biasClamp);
                SetEdgeLODEnable(edgeLODEnable);
                SetAnisotropy(aniso);
            }
        }

        void TexMap::ReplaceImage(TPLPalettePtr ptr, u32 id) {
            if (reinterpret_cast<u32>(ptr->descriptorArray) < 0x80000000) {
                TPLBind(ptr);
            }

            ReplaceImage(TPLGet(ptr, id));
        }

        void TexMap::ReplaceImage(const TPLDescriptor *pDesc) {
            const TPLHeader& tpl = *pDesc->textureHeader;
            SetImage(tpl.data);
            SetSize(tpl.width, tpl.height);
            SetTexelFormat(GXTexFmt(tpl.format));

            if (const TPLClutHeader* const clutHdr = pDesc->CLUTHeader) {
                SetPalette(clutHdr->data);
                SetPaletteFormat(clutHdr->format);
                SetPaletteEntryNum(clutHdr->numEntries);
            }
            else {
                SetPalette(0);
                SetPaletteFormat(GXTlutFmt(0));
                SetPaletteEntryNum(0);
            }
        }
    };
};