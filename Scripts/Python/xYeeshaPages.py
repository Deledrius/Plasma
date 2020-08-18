# -*- coding: utf-8 -*-
""" *==LICENSE==*

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

 *==LICENSE==* """

"""
Module: xYeeshaPages
Age: global
Date: January 2002
Author: Doug McBride
Manages and records the finding of Yeesha Pages
"""

from Plasma import *
from PlasmaTypes import *
from PlasmaKITypes import *
from PlasmaVaultConstants import *
from PlasmaNetConstants import *


# define the attributes that will be entered in max
actClickableBook = ptAttribActivator(1, "Act: Clickable Yeesha Page")
PageNumber = ptAttribInt(2, "Yeesha Page Number")

# Linking Books GUI tags
kDialogName = "YeeshaPageGUI"
kPageButton = 100

kYeeshaPageFirst = 201
kYeeshaPageCancel = 299
kYeeshaPageCount = 25  # Be sure to update this when adding new Yeesha Pages
YeeshaPageIDList = list(range(kYeeshaPageFirst, kYeeshaPageFirst + kYeeshaPageCount))

kYeeshaPageCleftPillars = 25


class xYeeshaPages(ptModifier):
    "The Yeesha Page python code"

    def __init__(self):
        ptModifier.__init__(self)
        self.id = 5225
        version = 6
        self.version = version
        PtDebugPrint("__init__xYeeshaPages v.", version)

    def OnFirstUpdate(self):
        PtLoadDialog(kDialogName, self.key)

    def __del__(self):
        "destructor - get rid of any dialogs that we might have loaded"
        PtUnloadDialog(kDialogName)

    def OnServerInitComplete(self):
        ageName = PtGetAgeName()
        if ageName == "Cleft":
            ageSDL = PtGetAgeSDL()
            ageSDL.setFlags("clftIsCleftDone", 1, 1)
            ageSDL.sendToClients("clftIsCleftDone")

    def OnNotify(self, state, id, events):
        global LocalAvatar

        if state and id == actClickableBook.id and PtWasLocallyNotified(self.key):
            PtLoadDialog(kDialogName, self.key)
            if PtIsDialogLoaded(kDialogName):
                self.IDrawLinkPanel()
                PtShowDialog(kDialogName)

    def OnGUINotify(self, id, control, event):
        if event == kExitMode:
            PtHideDialog(kDialogName)
            return

        if isinstance(control, ptGUIControlButton):
            btnID = control.getTagID()
        else:
            return

        if event == 2 and btnID in YeeshaPageIDList:
            PtDebugPrint(
                f"xYeeshaPages.OnGUINotify():\tPicked up page number: {PageNumber.value}"
            )
            PtHideDialog(kDialogName)

            vault = ptVault()
            psnlSDL = vault.getPsnlAgeSDL()
            if psnlSDL:
                YeeshaPageVar = psnlSDL.findVar(f"YeeshaPage{PageNumber.value}")

                PtDebugPrint(
                    f"xYeeshaPages.py: The previous value of the SDL variable YeeshaPage{PageNumber.value} is {YeeshaPageVar.getInt()}."
                )

                if YeeshaPageVar.getInt() != 0:
                    PtDebugPrint(
                        f"xYeeshaPages.py: You've already found Yeesha Page #{PageNumber.value}. Move along. Move along."
                    )
                    return

                else:
                    PtDebugPrint(
                        f"xYeeshaPages.py: Yeesha Page #{PageNumber.value} is new to you."
                    )

                    PtDebugPrint(
                        f"xYeeshaPages.py: Trying to update the value of the SDL variable YeeshaPage{PageNumber.value} to 1."
                    )
                    YeeshaPageVar.setInt(4)
                    vault.updatePsnlAgeSDL(psnlSDL)

                    PtSendKIMessageInt(kStartBookAlert, 0)

                    if PageNumber.value == kYeeshaPageCleftPillars:
                        # Cleft is done, set SDL to start link back to Relto
                        actClickableBook.disableActivator()
                        PtSendKIMessage(kDisableKIandBB, 0)
                        ageSDL = PtGetAgeSDL()
                        ageSDL["clftIsCleftDone"] = (1,)
                        vault = ptVault()
                        vault.addChronicleEntry("CleftSolved", 1, "yes")
                        PtDebugPrint(
                            "Chronicle updated with variable 'CleftSolved'.",
                            level=kDebugDumpLevel,
                        )
            else:
                PtDebugPrint(
                    f"xYeeshaPages: Error trying to access the Chronicle psnlSDL. psnlSDL = {psnlSDL}"
                )

        elif event == 2 and btnID == kYeeshaPageCancel:
            PtHideDialog(kDialogName)

    def IDrawLinkPanel(self):
        global kDialogName

        if PageNumber.value not in YeeshaPageIDList:
            PtDebugPrint(
                f"xYeeshaPages.IDrawLinkPanel():\tERROR: couldn't find page named {PageNumber.value}"
            )
            return

        mydialog = PtGetDialogFromString(kDialogName)
        for page in YeeshaPageIDList:
            if page == PageNumber.value:
                ptGUIControlButton(mydialog.getControlFromTag(page)).show()
            else:
                ptGUIControlButton(mydialog.getControlFromTag(page)).hide()
