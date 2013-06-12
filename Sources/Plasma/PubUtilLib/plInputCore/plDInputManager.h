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

#ifndef PL_DINPUT_MANAGER_H
#define PL_DINPUT_MANAGER_H

#if HS_BUILD_FOR_WIN32

#include "HeadSpin.h"
#include "hsWindows.h"

#include "plMessage/plInputEventMsg.h"
#include "plDInputDevice.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#define NUM_ACTIONS     17

// {049DE53E-23A2-4d43-BF68-36AC1B57E357}
static const GUID PL_ACTION_GUID = { 0x49de53e, 0x23a2, 0x4d43, { 0xbf, 0x68, 0x36, 0xac, 0x1b, 0x57, 0xe3, 0x57 } };


struct plDIDevice
{
    plDIDevice() : fDevice(nullptr), fCaps(nullptr) {;}
    plDIDevice(IDirectInputDevice8* _device) : fCaps(nullptr) {fDevice = _device;}
    IDirectInputDevice8*    fDevice;
    DIDEVCAPS*              fCaps;
};

struct plDInput
{
    plDInput() :
    fDInput(nullptr),
    fActionFormat(nullptr)
    {;}
    IDirectInput8*          fDInput;
    hsTArray<plDIDevice*>   fSticks;
    DIACTIONFORMAT*         fActionFormat;
};

class plDInputMgr 
{
public:
    plDInputMgr() : fDI(nullptr) {fDI = new plDInput;};
    ~plDInputMgr();

    void Init(HINSTANCE hInst, HWND hWnd);
    void Update();
    void AddDevice(IDirectInputDevice8* device);
    void ConfigureDevice();
    virtual bool MsgReceive(plMessage* msg);
    
    // dinput callback functions
    static int __stdcall EnumGamepadCallback(const DIDEVICEINSTANCE* device, void* pRef);

protected:
    plDInput*                   fDI;
    hsTArray<plDInputDevice*>   fInputDevice;
    static DIACTION             fActionMap[];
    HWND                        fhWnd;
};
// function pointers to dinput callbacks
typedef int (__stdcall * Pfunc1) (const DIDEVICEINSTANCE* device, void* pRef);


#endif // HS_BUILD_FOR_WIN32


#endif // PL_DINPUT_MANAGER_H
