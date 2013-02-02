/*==LICENSE==*

CyanWorlds.com Engine - MMOG client, server and tools
Copyright (C) 2011  Cyan Worlds, Inc.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

Additional permissions under GNU GPL version 3 section 7

If you modify this Program, or any covered work, by linking or
combining it with any of RAD Game Tools Bink SDK, Autodesk 3ds Max SDK,
NVIDIA PhysX SDK, Microsoft DirectX SDK, OpenSSL library, Independent
JPEG Group JPEG library, Microsoft Windows Media SDK, or Apple QuickTime SDK
(or a modified version of those libraries),
containing parts covered by the terms of the Bink SDK EULA, 3ds Max EULA,
PhysX SDK EULA, DirectX SDK EULA, OpenSSL and SSLeay licenses, IJG
JPEG Library README, Windows Media SDK EULA, or QuickTime SDK EULA, the
licensors of this Program grant you additional
permission to convey the resulting work. Corresponding Source for a
non-source form of such a combination shall include the source code for
the parts of OpenSSL and IJG JPEG Library used as well as that of the covered
work.

You can contact Cyan Worlds, Inc. by email legal@cyan.com
 or by snail mail at:
      Cyan Worlds, Inc.
      14617 N Newport Hwy
      Mead, WA   99021

*==LICENSE==*/

#include "HeadSpin.h"

#include "hsStream.h"
#include "hsResMgr.h"
#include "plString.h"
#include "plFileSystem.h"
#include "plGImage/plPNG.h"
#include "plGImage/plJPEG.h"
#include "plGImage/plTGA.h"
#include "plGImage/plMipmap.h"

#include "plClientResMgr.h"


//// Singleton Instance ///////////////////////////////////////////////////////

plClientResMgr& plClientResMgr::Instance(void)
{
    static plClientResMgr theInstance;
    return theInstance;
}

plClientResMgr::plClientResMgr()
{
    this->ClientResources = new std::map<plString, plMipmap*>;
}

plClientResMgr::~plClientResMgr()
{
    if (this->ClientResources) {
        for (auto it = this->ClientResources->begin(); it != this->ClientResources->end(); ++it) {
            if (it->second)
                it->second->UnRef();
        }

        delete this->ClientResources;
    }
}

void plClientResMgr::LoadResources(const plFileName& resfile)
{
    if (!resfile.IsValid())
        return;

    hsUNIXStream in;

    if (in.Open(resfile, "rb")) {
        uint32_t header = in.ReadLE32();
        uint32_t version = in.ReadLE32();
        uint32_t num_resources = 0;

        switch (version) {
            case kPlasma21Format:
                num_resources = in.ReadLE32();

                for (int i = 0; i < num_resources; i++) {
                    plMipmap* res_data = nullptr;
                    uint32_t res_size = 0;
                    plFileName res_name(in.ReadSafeStringLong_TEMP());

                    // Plasma 2.1+ format doesn't encode filetypes, so we'll try some simple
                    // extension sniffing.
                    if (res_name.GetFileExt() == "png") {
                        // Read resource stream size, but the PNG has that info in the header
                        // so it's not needed.
                        res_size = in.ReadLE32();
                        res_data = plPNG::Instance().ReadFromStream(&in);
                    } else if (res_name.GetFileExt() == "jpg") {
                        // Don't read resource stream size, as plJPEG's reader will need it.
                        res_data = plJPEG::Instance().ReadFromStream(&in);
                    } else {
                        // Original Myst 5 format only is known to support Targa,
                        // so default fallback is Targa.
                        // TODO - Add plTarga::ReadFromStream()
                        // for now, just skip the unknown resource and put NULL into the map.
                        res_size = in.ReadLE32();
                        in.Skip(res_size);
                    }

                    (*this->ClientResources)[res_name.GetFileName()] = res_data;
                }

                break;
            default:
                break;
        }

        in.Close();
    }
}

void plClientResMgr::SaveResources(const plFileName& resfile, const uint32_t version)
{
    if (!resfile.IsValid())
        return;

    hsUNIXStream out;
    hsRAMStream tempStream;

    if (out.Open(resfile, "wb")) {
        switch (version) {
            case kPlasma21Format:
                out.WriteLE32(0xcbbcf00d);
                out.WriteLE32(kPlasma21Format);
                out.WriteLE32(this->ClientResources->size());

                for (auto it = this->ClientResources->begin(); it != this->ClientResources->end(); it++) {
                    tempStream.Reset();
                    plFileName res_name = it->first;

                    // Plasma 2.1+ format doesn't encode filetypes, just filenames.
                    out.WriteSafeStringLong(res_name.GetFileName());

                    if (it->second) {
                        if (res_name.GetFileExt() == "png") {
                            plPNG::Instance().WriteToStream(&tempStream, it->second);
                            out.WriteLE32(tempStream.GetPosition());
                            plPNG::Instance().WriteToStream(&out, it->second);
                        } else if (res_name.GetFileExt() == "jpg") {
                            plJPEG::Instance().WriteToStream(&tempStream, it->second);
                            out.WriteLE32(tempStream.GetPosition());
                            plJPEG::Instance().WriteToStream(&out, it->second);
                        } else {
                            // Original Myst 5 format only is known to support Targa,
                            // so default is assumed to be Targa.
                            plTGA::Instance().WriteToStream(&tempStream, it->second);
                            out.WriteLE32(tempStream.GetPosition());
                            plTGA::Instance().WriteToStream(&out, it->second);
                        }
                    } else {
                        // We've gotten an invalid resource somehow, so to keep
                        // the number of entries correct, we'll write out an
                        // empty entry.
                        out.WriteLE32(0);
                    }
                }
                break;

            default:
                break;
        }

        out.Close();
    }
}

plMipmap* plClientResMgr::getResource(const plString& resname)
{
    // Attempt to find the named resource.
    auto it = this->ClientResources->find(resname);

    // Return the resource if we've got it.
    if (it != this->ClientResources->end())
        return it->second;

    // Return nullptr as the resource could not be found.
    hsAssert(0, "Unknown client resource requested.");
    return nullptr;
}

void plClientResMgr::addResource(const plString& resname, plMipmap* resource)
{
    if (!resname.IsEmpty() && resource) {
        // Test if the resource already exists by that name.
        auto it = this->ClientResources->find(resname);

        if (it != this->ClientResources->end()) {
            // Remove the old resource by this name so we can
            // replace it with the new resource.
            if (it->second)
                it->second->UnRef();
            this->ClientResources->erase(it);
        }

        // Insert the new resource.
        resource->Ref();
        (*this->ClientResources)[resname] = resource;
    }
}

bool plClientResMgr::removeResource(const plString& resname)
{
    if (!resname.IsEmpty()) {
        // Test if a resource exists by that name.
        auto it = this->ClientResources->find(resname);

        if (it != this->ClientResources->end()) {
            // Remove the old resource by this name so we can
            // replace it with the new resource.
            if (it->second)
                it->second->UnRef();
            this->ClientResources->erase(it);
            return true;
        }
    }

    return false;
}
