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
#include "plFileSystem.h"

#include "plTGA.h"


//// Singleton Instance ///////////////////////////////////////////////////////

plTGA& plTGA::Instance(void)
{
    static plTGA theInstance;
    return theInstance;
}

//// IRead ////////////////////////////////////////////////////////////////////

plMipmap* plTGA::IRead(hsStream* inStream)
{
    plMipmap* newMipmap = nullptr;

    try {
        plTargaImage newImage;

        /// Read header data
        newImage.idLength = inStream->ReadByte();
        newImage.mapType = inStream->ReadByte();
        newImage.imageType = inStream->ReadByte();

        newImage.colorMapStartOffset = inStream->ReadLE16();
        newImage.colorMapNumEntries = inStream->ReadLE16();
        newImage.colorMapEntrySize = inStream->ReadByte();

        newImage.xOrigin = inStream->ReadLE16();
        newImage.yOrigin = inStream->ReadLE16();
        newImage.imageWidth = inStream->ReadLE16();
        newImage.imageHeight = inStream->ReadLE16();
        newImage.bpp = inStream->ReadByte();
        uint8_t descriptor = inStream->ReadByte();
        newImage.alphaChannelDepth = descriptor & 0xf;
        newImage.imageDirection = (descriptor & 0x30) >> 4;

        /// Read image data
        // Read ID block
        inStream->Skip(static_cast<uint32_t>(newImage.idLength));

        // Read color map
        switch (newImage.mapType) {
        case kNoColorMap:
                break;
            default:
                hsAssert(0, "Targa color map present but unsupported.");
                break;
        }

        // Read image data
        switch (newImage.imageType) {
            case kNoImage:
                newMipmap = new plMipmap(newImage.imageWidth, newImage.imageHeight, newImage.bpp, 1, plMipmap::kUncompressed);
                break;
            case kUncompressedTrueColor:
                newMipmap = IReadUTCFromStream(inStream, &newImage);
                break;
            default:
                hsAssert(0, plString::Format("Targa image format %d unsupported.", newImage.imageType).c_str());
                break;
        }

        // TODO: Read optional extra data block

    } catch (...) {
        if (newMipmap) {
            delete newMipmap;
            newMipmap = nullptr;
        }
    }

    return newMipmap;
}

//// ReadFromFile /////////////////////////////////////////////////////////////

plMipmap* plTGA::ReadFromFile(const plFileName& fileName)
{
    hsUNIXStream in;

    if (!in.Open(fileName, "rb"))
        return nullptr;

    plMipmap* ret = IRead(&in);
    in.Close();
    return ret;
}

//// IWrite ///////////////////////////////////////////////////////////////////

bool plTGA::IWrite(const plMipmap* source, hsStream* outStream)
{
    bool result = true;

    try {
        int x, y;
        hsRGBAColor32 pixel;

        /// Write header
        outStream->WriteByte(0);                         // Size of ID field
        outStream->WriteByte(kNoColorMap);               // Map type
        outStream->WriteByte(kUncompressedTrueColor);    // Type 2 image - Unmapped RGB

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

bool plTGA::WriteToFile(const plFileName& fileName, const plMipmap* sourceData)
{
    hsUNIXStream out;

    if (!out.Open(fileName, "wb"))
        return false;

    bool ret = IWrite(sourceData, &out);
    out.Close();
    return ret;
}

//// IReadImageDataFromStream /////////////////////////////////////////////////

plMipmap* plTGA::IReadUTCFromStream(hsStream* inStream, plTGA::plTargaImage* newImage)
{
    plMipmap* newMipmap = new plMipmap(newImage->imageWidth, newImage->imageHeight, newImage->bpp, 1, plMipmap::kUncompressed);
    hsRGBAColor32* pixel;

    for(int y = newImage->imageHeight - 1; y >= 0; y--)
    {
        for(int x = 0; x < newImage->imageWidth; x++)
        {
            pixel = reinterpret_cast<hsRGBAColor32 *>(newMipmap->GetAddr32(x, y));

            pixel->b = inStream->ReadByte();
            pixel->g = inStream->ReadByte();
            pixel->r = inStream->ReadByte();
            if (newImage->alphaChannelDepth)
                pixel->a = inStream->ReadByte();
        }
    }

    return newMipmap;
}
