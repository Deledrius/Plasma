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
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  plTGAWriter Class Functions                                              //
//  Simple utility class for writing a plMipmap out as a TGA file            //
//  Cyan, Inc.                                                               //
//                                                                           //
//// Version History //////////////////////////////////////////////////////////
//                                                                           //
//  8.15.2001 mcn - Created.                                                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "HeadSpin.h"
#include "hsColorRGBA.h"
#include "plMipmap.h"
#include "hsStream.h"

#include "plTGAWriter.h"

//// Class Statics ////////////////////////////////////////////////////////////

plTGAWriter plTGAWriter::fInstance;

//// IWrite ///////////////////////////////////////////////////////////////////

bool plTGAWriter::IWrite(const plMipmap* source, hsStream* outStream)
{
    bool result = true;

    try {
        int x, y;
        hsRGBAColor32 pixel;

        /// Write header
        outStream->WriteByte(0);  // Size of ID field
        outStream->WriteByte(0);  // Map type
        outStream->WriteByte(2);  // Type 2 image - Unmapped RGB

        outStream->WriteByte(0);  // Color map spec
        outStream->WriteByte(0);  // Color map spec
        outStream->WriteByte(0);  // Color map spec
        outStream->WriteByte(0);  // Color map spec
        outStream->WriteByte(0);  // Color map spec

        outStream->WriteLE16(0);  // xOrigin
        outStream->WriteLE16(0);  // yOrigin

        outStream->WriteLE16((uint16_t)source->GetWidth());
        outStream->WriteLE16((uint16_t)source->GetHeight());

        outStream->WriteByte(24);
        outStream->WriteByte(0);

        /// Write image data (gotta do inverted, stupid TGAs...)
        for(y = source->GetHeight() - 1; y >= 0; y--)
        {
            for(x = 0; x < source->GetWidth(); x++)
            {
                pixel = *((hsRGBAColor32 *)source->GetAddr32(x, y));
                outStream->WriteByte(pixel.b);
                outStream->WriteByte(pixel.g);
                outStream->WriteByte(pixel.r);
            }
        }
    } catch (...) {
        result = false;
    }

    return result;
}

//// WriteToFile //////////////////////////////////////////////////////////////

bool plTGAWriter::WriteToFile(const plFileName& fileName, const plMipmap* sourceData)
{
    hsUNIXStream out;

    if (!out.Open(fileName, "wb")) {
        return false;
    }

    bool ret = IWrite(sourceData, &out);
    out.Close();
    return ret;
}
