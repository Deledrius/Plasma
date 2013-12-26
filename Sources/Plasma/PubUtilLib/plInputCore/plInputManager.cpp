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
#include "hsWindows.h"

#include "plInputManager.h"
#include "plPipeline.h"
#include "plInputDevice.h"
#include "plInputInterfaceMgr.h"
#include "hsStream.h"
#include "pnKeyedObject/plKey.h"
#include "pnKeyedObject/plFixedKey.h"
#include "hsResMgr.h"
#include "hsTimer.h"
#include "plgDispatch.h"
#include "pnMessage/plTimeMsg.h"
#include "pnMessage/plCmdIfaceModMsg.h"
#include "pnMessage/plPlayerPageMsg.h"

#include "plDInputManager.h"

bool         plInputManager::fUseDInput = false;
uint8_t      plInputManager::bRecenterMouse = 0;
hsWindowHndl plInputManager::fhWnd = nullptr;


plInputManager* plInputManager::fInstance = nil;

plInputManager::plInputManager( hsWindowHndl hWnd ) :
fDInputMgr(nullptr),
fInterfaceMgr(nullptr),
localeC("C")
{
    fhWnd = hWnd;
    fInstance = this;
    fActive = false;
    fFirstActivated = false;
    fMouseScale = 1.f;
}

plInputManager::plInputManager() :
fDInputMgr(nullptr),
fInterfaceMgr(nullptr)
{
    fInstance = this;
    fActive = false;
    fFirstActivated = false;
    fMouseScale = 1.f;
}

plInputManager::~plInputManager() 
{
    fInterfaceMgr->Shutdown();
    fInterfaceMgr = nullptr;

    for (int i = 0; i < fInputDevices.Count(); i++)
    {
        fInputDevices[i]->Shutdown();
        delete(fInputDevices[i]);
    }
#if HS_BUILD_FOR_WIN32
    if (fDInputMgr)
        delete fDInputMgr;
#endif
}

//static
void plInputManager::SetRecenterMouse(bool b)
{ 
    if (b)
        bRecenterMouse++;
    else if (bRecenterMouse > 0)
        bRecenterMouse--;
}


void plInputManager::RecenterCursor()
{
#if HS_BUILD_FOR_WIN32
    RECT rect;
    GetClientRect(fhWnd, &rect);
    POINT pt;
    ClientToScreen(fhWnd, &pt);
    SetCursorPos( pt.x, pt.y );
#endif
}
void plInputManager::CreateInterfaceMod(plPipeline* p)
{
    fInterfaceMgr = new plInputInterfaceMgr();
    fInterfaceMgr->Init();
}

bool plInputManager::MsgReceive(plMessage* msg)
{
    for (int i=0; i<fInputDevices.Count(); i++)
        if (fInputDevices[i]->MsgReceive(msg))
            return true;

#if HS_BUILD_FOR_WIN32
    if (fDInputMgr)
        return fDInputMgr->MsgReceive(msg);
#endif

    return hsKeyedObject::MsgReceive(msg);
}

void plInputManager::Update()
{
#if HS_BUILD_FOR_WIN32
    if (fDInputMgr)
        fDInputMgr->Update();
#endif
}

void plInputManager::SetMouseScale( float s )
{
}

// Sometimes the keyboard driver "helps" us translating a key involved in a key
// combo. For example pressing shif-numpad8 will actually generate a KEY_UP event,
// the same as the up arrow. This function undoes that translation.
plKeyDef plInputManager::UntranslateKey(plKeyDef key, bool extended)
{
    if (!extended)
    {
        if (key == KEY_UP)
            return KEY_NUMPAD8;
        if (key == KEY_DOWN)
            return KEY_NUMPAD2;
        if (key == KEY_LEFT)
            return KEY_NUMPAD4;
        if (key == KEY_RIGHT)
            return KEY_NUMPAD6;
    }

    return key;
}

#if HS_BUILD_FOR_WIN32

void plInputManager::InitDInput(hsWindowInst hInst, hsWindowHndl hWnd)
{
    if (fUseDInput)
    {
        fDInputMgr = new plDInputMgr;
        fDInputMgr->Init(hInst, hWnd);
    }
}

void plInputManager::HandleWin32ControlEvent(UINT message, WPARAM Wparam, LPARAM Lparam, hsWindowHndl hWnd)
{
    if( !fhWnd )
        fhWnd = hWnd;

    bool bExtended;

    switch (message)
    {
    case SYSKEYDOWN:
    case KEYDOWN:
        {
            bExtended = Lparam >> 24 & 1;
            bool bRepeat = ((Lparam >> 29) & 0xf) != 0;
            for (int i=0; i<fInputDevices.Count(); i++)
                fInputDevices[i]->HandleKeyEvent( KEYDOWN, UntranslateKey((plKeyDef)Wparam, bExtended), true, bRepeat ); 
        }
        break;
    case SYSKEYUP:
    case KEYUP:
        {
            bExtended = Lparam >> 24 & 1;
            for (int i=0; i<fInputDevices.Count(); i++)
                fInputDevices[i]->HandleKeyEvent( KEYUP, UntranslateKey((plKeyDef)Wparam, bExtended), false, false ); 
        }
        break;
    case CHAR_MSG:
        {
            wchar_t ch = (wchar_t)Wparam;

            // These are handled by KEYUP/KEYDOWN and should not be sent
            // We don't like garbage getting in string fields
            if (std::iscntrl(ch, localeC))
                break;

            BYTE scan = (BYTE)(Lparam >> 16);
            UINT vkey = MapVirtualKey(scan, 1); //MAPVK_VSC_TO_VK

            bExtended = Lparam >> 24 & 1;
            bool bRepeat = ((Lparam >> 29) & 0xf) != 0;
            bool down = !(Lparam >> 31);
 
            for (int i=0; i<fInputDevices.Count(); i++)
                fInputDevices[i]->HandleKeyEvent( CHAR_MSG, (plKeyDef)vkey, down, bRepeat, ch );
        }
        break;
    case MOUSEWHEEL:
        {
            plMouseEventMsg* pMsg = new plMouseEventMsg;
            int zDelta = GET_WHEEL_DELTA_WPARAM(Wparam);
            pMsg->SetWheelDelta((float)zDelta);
            if (zDelta < 0)
                pMsg->SetButton(kWheelNeg);
            else
                pMsg->SetButton(kWheelPos);

            RECT rect;
            GetClientRect(hWnd, &rect);
            pMsg->SetXPos(LOWORD(Lparam) / (float)rect.right);
            pMsg->SetYPos(HIWORD(Lparam) / (float)rect.bottom);

            pMsg->Send();
        }
        break;
    case MOUSEMOVE:
    case L_BUTTONDN:
    case L_BUTTONUP:
    case R_BUTTONDN:
    case R_BUTTONUP:
    case L_BUTTONDBLCLK:
    case R_BUTTONDBLCLK:
    case M_BUTTONDN:
    case M_BUTTONUP:
        {
            
            RECT rect;
            GetClientRect(hWnd, &rect);
         
            plIMouseXEventMsg* pXMsg = new plIMouseXEventMsg;
            plIMouseYEventMsg* pYMsg = new plIMouseYEventMsg;
            plIMouseBEventMsg* pBMsg = new plIMouseBEventMsg;

            pXMsg->fWx = LOWORD(Lparam);
            pXMsg->fX = (float)LOWORD(Lparam) / (float)rect.right;
            pYMsg->fWy = HIWORD(Lparam);
            pYMsg->fY = (float)HIWORD(Lparam) / (float)rect.bottom;

            // Apply mouse scale
//          pXMsg->fX *= fMouseScale;
//          pYMsg->fY *= fMouseScale;
            
            if (Wparam & MK_LBUTTON && message != L_BUTTONUP)
            {
                pBMsg->fButton |= kLeftButtonDown;
            }
            else
            {
                pBMsg->fButton |= kLeftButtonUp;
            }
                        
            if (Wparam & MK_RBUTTON && message != R_BUTTONUP)
            {
                pBMsg->fButton |= kRightButtonDown;
            }
            else
            {
                pBMsg->fButton |= kRightButtonUp;
            }
            
            if (Wparam & MK_MBUTTON && message != M_BUTTONUP)
            {
                pBMsg->fButton |= kMiddleButtonDown;
            }
            else
            {
                pBMsg->fButton |= kMiddleButtonUp;
            }
            
            if( message == L_BUTTONDBLCLK )
                pBMsg->fButton |= kLeftButtonDblClk;        // We send the double clicks separately
            if( message == R_BUTTONDBLCLK )
                pBMsg->fButton |= kRightButtonDblClk;

            for (int i=0; i<fInputDevices.Count(); i++)
            {
                fInputDevices[i]->MsgReceive(pXMsg);
                fInputDevices[i]->MsgReceive(pYMsg);
                fInputDevices[i]->MsgReceive(pBMsg);
            }
            POINT pt;
            
            if (RecenterMouse() && (pXMsg->fX <= 0.1 || pXMsg->fX >= 0.9) )
            {       
                pt.x = (rect.right - rect.left) / 2;
                pt.y = HIWORD(Lparam);
                ClientToScreen(hWnd, &pt);
                SetCursorPos( pt.x, pt.y );
            }
            else
            if (RecenterMouse() && (pYMsg->fY <= 0.1 || pYMsg->fY >= 0.9) )
            {       
                pt.y = (rect.bottom - rect.top) / 2;
                pt.x = LOWORD(Lparam);
                ClientToScreen(hWnd, &pt);
                SetCursorPos( pt.x, pt.y );
            }
            if (RecenterMouse() && Lparam == 0)
            {
                pt.y = (rect.bottom - rect.top) / 2;
                pt.x = (rect.right - rect.left) / 2;
                ClientToScreen(hWnd, &pt);
                SetCursorPos( pt.x, pt.y );
            }
            delete(pXMsg);
            delete(pYMsg);
            delete(pBMsg);

            

        }
        break;
    case WM_ACTIVATE:
        {
            bool activated = ( LOWORD( Wparam ) == WA_INACTIVE ) ? false : true;
            Activate( activated );
        }
        break;
    }

}
#endif

//// Activate ////////////////////////////////////////////////////////////////
//  Handles what happens when the app (window) activates/deactivates

void    plInputManager::Activate( bool activating )
{
    int     i;


    for( i = 0; i < fInputDevices.GetCount(); i++ )
        fInputDevices[ i ]->HandleWindowActivate( activating, fhWnd );

    fActive = activating;
    fFirstActivated = true;
}

//// AddInputDevice //////////////////////////////////////////////////////////

void    plInputManager::AddInputDevice( plInputDevice *pDev )
{
    fInputDevices.Append( pDev ); 
    if( fFirstActivated )
        pDev->HandleWindowActivate( fActive, fhWnd );
}

