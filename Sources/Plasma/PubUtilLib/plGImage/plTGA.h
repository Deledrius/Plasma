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
//  plTGA Class Header                                                       //
//  Cyan, Inc.                                                               //
//                                                                           //
//// Version History //////////////////////////////////////////////////////////
//                                                                           //
//  8.15.2001 mcn - Created.                                                 //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#ifndef _plTGA_h
#define _plTGA_h


//// Class Definition /////////////////////////////////////////////////////////

class plTGA
{
private:

    plMipmap* IRead(hsStream* inStream);
    bool IWrite(const plMipmap* source, hsStream* outStream);

public:

    plMipmap* ReadFromStream(hsStream* inStream) { return IRead(inStream); }
    plMipmap* ReadFromFile(const plFileName& fileName);

    bool WriteToStream(hsStream* outStream, plMipmap* sourceData) { return IWrite(sourceData, outStream); }
    bool WriteToFile(const plFileName& fileName, const plMipmap* sourceData);

    static plTGA& Instance(void);

    struct plTargaImage {
        uint8_t idLength;
        uint8_t mapType;
        uint8_t imageType;

        uint16_t colorMapStartOffset;
        uint16_t colorMapNumEntries;
        uint8_t colorMapEntrySize;

        uint16_t xOrigin;
        uint16_t yOrigin;
        uint16_t imageWidth;
        uint16_t imageHeight;
        uint8_t bpp;
        uint8_t alphaChannelDepth;
        uint8_t imageDirection;
    };

    enum ColorMapType {
        kNoColorMap = 0,
        kGeneric    = 1
    };

    enum ImageType {
        kNoImage                =  0,
        kUncompressedIndexed    =  1,
        kUncompressedTrueColor  =  2,
        kUncompressedGreyScale  =  3,
        kRLEIndexed             =  9,
        kRLETrueColor           = 10,
        kRLEGreyScale           = 11
    };

private:

    plMipmap* IReadUTCFromStream(hsStream* inStream, plTargaImage* newImage);
};

#endif // _plTGA_h
