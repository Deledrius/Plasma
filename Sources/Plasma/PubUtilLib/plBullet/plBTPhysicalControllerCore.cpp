/*==COPYING==*

CyanWorlds.com Engine - MMOG client, server and tools
Copyright (C) 2011 Cyan Worlds, Inc.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

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

*==COPYING==*/

#include "plBTPhysicalControllerCore.h"
#include "plSimulationMgr.h"
#include "plBTPhysical.h"
#include "../pnSceneObject/plSimulationInterface.h"
#include "../pnSceneObject/plSceneObject.h"
#include "../pnMessage/plCorrectionMsg.h"
#include "../plAvatar/plArmatureMod.h"
#include "../pnSceneObject/plCoordinateInterface.h"
#include "../plDrawable/plDrawableGenerator.h"
#include "../plPhysical/plPhysicalProxy.h"
#include "../pnMessage/plSetNetGroupIDMsg.h"
#include "../plMessage/plCollideMsg.h"
#include "../plModifier/plDetectorLog.h"

#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>


#define kPhysxSkinWidth 0.1f
#define kPhysZOffset ((fRadius + (fHeight / 2)) + kPhysxSkinWidth)

#define AvatarMass 200.f

static std::vector<plBTPhysicalControllerCore*> gControllers;
static bool gRebuildCache = false;

plPhysicalControllerCore* plPhysicalControllerCore::Create(plKey ownerSO, float height, float width, bool human)
{
    float radius = width / 2.f;
    float realHeight = height - width;
    return new plBTPhysicalControllerCore(ownerSO, realHeight, radius, human);
}

plBTPhysicalControllerCore::plBTPhysicalControllerCore(plKey ownerSO, float height, float radius, bool human)
  : plPhysicalControllerCore(ownerSO, height, radius), fHuman(human)
{
    fLastGlobalLoc.Reset();
    ICreateController();
    Enable(false);
    gControllers.push_back(this);
}

plBTPhysicalControllerCore::~plBTPhysicalControllerCore()
{
}

void plBTPhysicalControllerCore::Enable(bool enable)
{
    if (fEnabled != enable)
    {
        fEnabled = enable;
        if (fEnabled)
            fEnableChanged = true;
        else
        {
            fBody->setCollisionFlags(fBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
            fBody->setLinearFactor(btVector3(0.f, 0.f, 0.f));
            fBody->setAngularFactor(btVector3(0.f, 0.f, 0.f));
        }
    }
}

void plBTPhysicalControllerCore::SetSubworld(plKey world) 
{   
    if (fWorldKey != world)
    {
         bool wasEnabled = fEnabled;
        //need to inform detectors in the old world that we are leaving
        //IInformDetectors(false);
        //done informing old world
        SimLog("Changing subworlds!");
        IDeleteController();
        SimLog("Deleted old controller");
        fWorldKey = world;
        if (GetSubworldCI())
            fPrevSubworldW2L = GetSubworldCI()->GetWorldToLocal();
        // Update our subworld position and rotation
        const plCoordinateInterface* subworldCI = GetSubworldCI();
        if (subworldCI)
        {
            const hsMatrix44& w2s = fPrevSubworldW2L;
            hsMatrix44 l2s = w2s * fLastGlobalLoc;
            l2s.GetTranslate(&fLocalPosition);
            fLocalRotation.SetFromMatrix44(l2s);
        }
        else
        {
            fLastGlobalLoc.GetTranslate(&fLocalPosition);
            fLocalRotation.SetFromMatrix44(fLastGlobalLoc);
        }
        hsMatrix44 w2l;
        fLastGlobalLoc.GetInverse(&w2l);
//        if (fProxyGen)
//            fProxyGen->SetTransform(fLastGlobalLoc, w2l);
        // Update the physical position
        SimLog("creating new controller");
        hsPoint3 PositionPlusOffset=fLocalPosition;
        PositionPlusOffset.fZ +=kPhysZOffset;
        //placing new controller and kinematic in the appropriate location
        ICreateController(PositionPlusOffset);
//        RebuildCache();
    }
}

void plBTPhysicalControllerCore::GetState(hsPoint3& pos, float& zRot)
{   
    // Temporarily use the position point while we get the z rotation
    fLocalRotation.NormalizeIfNeeded();
    fLocalRotation.GetAngleAxis(&zRot, (hsVector3*)&pos);

    if (pos.fZ < 0)
        zRot = (2 * (float)M_PI) - zRot; // axis is backwards, so reverse the angle too

    pos = fLocalPosition;
}

void plBTPhysicalControllerCore::SetState(const hsPoint3& pos, float zRot)
{
    plSceneObject* so = plSceneObject::ConvertNoRef(fOwner->ObjectIsLoaded());
    if (so)
    {
        hsQuat worldRot;
        hsVector3 zAxis(0.f, 0.f, 1.f);
        worldRot.SetAngleAxis(zRot, zAxis);

        hsMatrix44 l2w, w2l;
        worldRot.MakeMatrix(&l2w);
        l2w.SetTranslate(&pos);

        // Localize new position and rotation to global coords if we're in a subworld
        const plCoordinateInterface* ci = GetSubworldCI();
        if (ci)
        {
            const hsMatrix44& subworldL2W = ci->GetLocalToWorld();
            l2w = subworldL2W * l2w;
        }
        l2w.GetInverse(&w2l);
        so->SetTransform(l2w, w2l);
        so->FlushTransform();
    }
}

void plBTPhysicalControllerCore::SetMovementStrategy(plMovementStrategy* strategy)
{
    if (fKinematicCCT != strategy->IsKinematic())
    {
        IDeleteController();
        fKinematicCCT = !fKinematicCCT;
        ICreateController(fLocalPosition);
    }
    fMovementStrategy = strategy;
}

void plBTPhysicalControllerCore::LeaveAge()
{
    SetPushingPhysical(nullptr);
    if (fWorldKey)
        SetSubworld(nullptr);

    // Disable all collisions
    fBody->setCollisionFlags(fBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    fBody->setLinearFactor(btVector3(0.f, 0.f, 0.f));
    fBody->setAngularFactor(btVector3(0.f, 0.f, 0.f));
}

int plBTPhysicalControllerCore::SweepControllerPath(const hsPoint3& startPos, const hsPoint3& endPos, bool vsDynamics, bool vsStatics, uint32_t& vsSimGroups, std::vector<plControllerSweepRecord>& hits)
{
/*    NxCapsule tempCap;
    tempCap.p0 =plPXConvert::Point( startPos);
    tempCap.p0.z = tempCap.p0.z + fPreferedRadius;
    tempCap.radius = fPreferedRadius ;
    tempCap.p1 = tempCap.p0;
    tempCap.p1.z = tempCap.p1.z + fPreferedHeight;

    NxVec3 vec;
    vec.x = endPos.fX - startPos.fX;
    vec.y = endPos.fY - startPos.fY;
    vec.z = endPos.fZ - startPos.fZ;

    int numberofHits = 0;
    int HitsReturned = 0;
    WhatWasHitOut.clear();
    NxScene *myscene = plSimulationMgr::GetInstance()->GetScene(fWorldKey);
    NxSweepQueryHit whatdidIhit[10];
    unsigned int flags = NX_SF_ALL_HITS;
    if(vsDynamics)
        flags |= NX_SF_DYNAMICS;
    if(vsStatics)
        flags |= NX_SF_STATICS;
    numberofHits = myscene->linearCapsuleSweep(tempCap, vec, flags, nil, 10, whatdidIhit, nil, vsSimGroups);
    if(numberofHits)
    {//we hit a dynamic object lets make sure it is not animatable
        for(int i=0; i<numberofHits; i++)
        {
            plControllerSweepRecord CurrentHit;
            CurrentHit.ObjHit=(plPhysical*)whatdidIhit[i].hitShape->getActor().userData;
            CurrentHit.Norm.fX = whatdidIhit[i].normal.x;
            CurrentHit.Norm.fY = whatdidIhit[i].normal.y;
            CurrentHit.Norm.fZ = whatdidIhit[i].normal.z;
            if(CurrentHit.ObjHit != nil)
            {
                hsPoint3 where;
                where.fX = whatdidIhit[i].point.x;
                where.fY = whatdidIhit[i].point.y;
                where.fZ = whatdidIhit[i].point.z;
                CurrentHit.locHit = where;
                CurrentHit.TimeHit = whatdidIhit[i].t ;
                WhatWasHitOut.insert(CurrentHit);
                HitsReturned++;
            }
        }
    } 

    return HitsReturned; */
    return 0; // BULLET STUB
}

void plBTPhysicalControllerCore::SetGlobalLoc(const hsMatrix44& l2w)
{
    fLastGlobalLoc = l2w;
    // Update our subworld position and rotation
    const plCoordinateInterface* subworldCI = GetSubworldCI();
    if (subworldCI)
    {
        const hsMatrix44& w2s = fPrevSubworldW2L;
        hsMatrix44 l2s = w2s * l2w;

        l2s.GetTranslate(&fLocalPosition);
        fLocalRotation.SetFromMatrix44(l2s);
    }
    else
    {
        l2w.GetTranslate(&fLocalPosition);
        fLocalRotation.SetFromMatrix44(l2w);
    }
    hsMatrix44 w2l;
    l2w.GetInverse(&w2l);
/*    if (fProxyGen)
        fProxyGen->SetTransform(l2w, w2l);
    // Update the physical position
    NxExtendedVec3 nxPos(fLocalPosition.fX, fLocalPosition.fY, fLocalPosition.fZ + kPhysZOffset);
    fController->setPosition(nxPos); */
    fBody->getWorldTransform().setOrigin(btVector3(fLocalPosition.fX, fLocalPosition.fY, fLocalPosition.fZ + kPhysZOffset));
}


void plBTPhysicalControllerCore::GetPositionSim(hsPoint3& pos) const
{
    if (fKinematicCCT)
    {
        //btVector3 btPos = fController->getPosition();
        //pos.Set(btPos.x(), btPos.y(), btPos.z());
    }
    else
    {
        btVector3 btPos = fBody->getWorldTransform().getOrigin();
        pos.Set(btPos.x(), btPos.y(), btPos.z());
    }

    
}

void plBTPhysicalControllerCore::Move(hsVector3 displacement, unsigned int collideWith, unsigned int &collisionResults)
{
    /*fController->move(plBTConvert::Vector(displacement), collideWith, 0.00001f, colFlags);
    collisionResults = 0;
    if (colFlags & NXCC_COLLISION_DOWN)
        collisionResults |= kBottom;
    if (colFlags & NXCC_COLLISION_UP)
        collisionResults |= kTop;
    if (colFlags & NXCC_COLLISION_SIDES)
        collisionResults |= kSides;*/
}

void plBTPhysicalControllerCore::SetLinearVelocitySim(const hsVector3& linearVel)
{
    if (!fKinematicCCT)
    {
        btVector3 btVec = btVector3(linearVel.fX, linearVel.fY, linearVel.fZ);
        fBody->setLinearVelocity(btVec);
    }
}

void plBTPhysicalControllerCore::Apply(float delSecs)
{
    plBTPhysicalControllerCore* controller;
    int numControllers = gControllers.size();
    for (int i = 0; i < numControllers; ++i)
    {

        controller = gControllers[i];
        //if (gRebuildCache && controller->fController)
        //    controller->fController->reportSceneChanged();

#ifndef PLASMA_EXTERNAL_RELEASE
        //controller->fDbgCollisionInfo.SetCount(0);
#endif

        controller->IDispatchQueuedMsgs();
        controller->IApply(delSecs);
        controller->IProcessDynamicHits();
    }
    gRebuildCache = false;
}

void plBTPhysicalControllerCore::Update(int numSubSteps, float alpha)
{
    //gControllerMgr.updateControllers();

    plBTPhysicalControllerCore* controller;
    int numControllers = gControllers.size();
    for (int i = 0; i < numControllers; ++i)
    {
        controller = gControllers[i];

        controller->IUpdate(numSubSteps, alpha);

#ifndef PLASMA_EXTERNAL_RELEASE
        //if (fDebugDisplay)
            //controller->IDrawDebugDisplay();
#endif
    }
}

void plBTPhysicalControllerCore::UpdateNonPhysical(float alpha)
{
    plBTPhysicalControllerCore* controller;
    int numControllers = gControllers.size();
    for (int i = 0; i < numControllers; ++i)
    {
        controller = gControllers[i];
        controller->IUpdateNonPhysical(alpha);
    }
}

void plBTPhysicalControllerCore::RebuildCache() { gRebuildCache = true; }

plBTPhysicalControllerCore* plBTPhysicalControllerCore::GetController(btRigidBody& body)
{
    plBTPhysicalControllerCore* controller;
    int numControllers = gControllers.size();
    for (int i = 0; i < numControllers; ++i)
    {
        controller = gControllers[i];
        if (controller->fBody == &body)
            return controller;
    }
    return nullptr;
}

bool plBTPhysicalControllerCore::AnyControllersInThisWorld(plKey world)
{
    plBTPhysicalControllerCore* controller;
    int numControllers = gControllers.size();
    for (int i = 0; i < numControllers; ++i)
    {
        controller = gControllers[i];
        if (controller->GetSubworld() == world)
            return true;
    }

    return false;
}
int plBTPhysicalControllerCore::GetNumberOfControllersInThisSubWorld(plKey world)
{
    int count = 0;
    plBTPhysicalControllerCore* controller;
    int numControllers = gControllers.size();
    for (int i = 0; i < numControllers; ++i)
    {
        controller = gControllers[i];
        if (controller->GetSubworld() == world)
            ++count;
    }

    return count;
}

int plBTPhysicalControllerCore::GetControllersInThisSubWorld(plKey world, int maxToReturn, plBTPhysicalControllerCore** bufferout)
{
    int count = 0;
    plBTPhysicalControllerCore* controller;
    int numControllers = gControllers.size();
    for (int i = 0; i < numControllers; ++i)
    {
        controller = gControllers[i];
        if (controller->GetSubworld() == world)
        {
            if (count < maxToReturn)
            {
                 bufferout[count] = controller;
                ++count;
            }
        }
    }
    return count;
}

void plBTPhysicalControllerCore::ICreateController()
{
    BtScene *scene = plSimulationMgr::GetInstance()->GetScene(fWorldKey);

    btCapsuleShape *shape = new btCapsuleShape(fRadius, fHeight);
    btVector3 inertia;
    shape->calculateLocalInertia(AvatarMass, inertia);
    btRigidBody::btRigidBodyConstructionInfo cinfo(AvatarMass, new btDefaultMotionState, shape, inertia);
    cinfo.m_friction = 0;
    cinfo.m_restitution = 0;
    fBody = new btRigidBody(cinfo);
    fBody->setUserPointer(this);
    fBody->setCollisionFlags(fBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    fBody->setAngularFactor(btVector3(0.f, 0.f, 1.f));

    short colgroups = 0;
    colgroups |= 1 << plSimDefs::kGroupAvatarBlocker;
    colgroups |= 1 << plSimDefs::kGroupDetector;
    colgroups |= 1 << plSimDefs::kGroupDynamic;
    colgroups |= 1 << plSimDefs::kGroupStatic;
    scene->world->addRigidBody(fBody, 1 << plSimDefs::kGroupAvatar, colgroups);
}

void plBTPhysicalControllerCore::ICreateController(const hsPoint3& origin)
{
    BtScene *scene = plSimulationMgr::GetInstance()->GetScene(fWorldKey);

    btCapsuleShape *shape = new btCapsuleShape(fRadius, fHeight);
    btVector3 inertia;
    shape->calculateLocalInertia(AvatarMass, inertia);
    btRigidBody::btRigidBodyConstructionInfo cinfo(AvatarMass, new btDefaultMotionState, shape, inertia);
    cinfo.m_friction = 0;
    cinfo.m_restitution = 0;
    fBody = new btRigidBody(cinfo);
    fBody->setUserPointer(this);
    fBody->setCollisionFlags(fBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
    fBody->setAngularFactor(btVector3(0.f, 0.f, 1.f));
    fBody->getWorldTransform().setOrigin(btVector3(origin.fX, origin.fY, origin.fZ));

    short colgroups = 0;
    colgroups |= 1 << plSimDefs::kGroupAvatarBlocker;
    colgroups |= 1 << plSimDefs::kGroupDetector;
    colgroups |= 1 << plSimDefs::kGroupDynamic;
    colgroups |= 1 << plSimDefs::kGroupStatic;
    scene->world->addRigidBody(fBody, 1 << plSimDefs::kGroupAvatar, colgroups);
}

void plBTPhysicalControllerCore::IDeleteController()
{
    BtScene* scene = plSimulationMgr::GetInstance()->GetScene(fWorldKey);
    scene->world->removeRigidBody(fBody);
    delete fBody->getMotionState();
    btCollisionShape *shape = fBody->getCollisionShape();
    delete shape;
    delete fBody;
}

void plBTPhysicalControllerCore::IDispatchQueuedMsgs()
{
    int numMsgs = fQueuedCollideMsgs.size();
    if (numMsgs)
    {
        plSimulationMgr* simMgr = plSimulationMgr::GetInstance();
        for (int i = 0; i < numMsgs; ++i)
            ;// simMgr->AddCollisionMsg(fQueuedCollideMsgs[i]);
    }
    //fQueuedCollideMsgs.clear();
}

void plBTPhysicalControllerCore::IProcessDynamicHits()
{
    int numHits = fDynamicHits.size();
    if (numHits)
    {
        plBTPhysical* phys;

        plSimulationMgr* simMgr = plSimulationMgr::GetInstance();
        for (int i = 0; i < numHits; ++i)
        {
            phys = fDynamicHits[i];

            // If this is the local avatar, we need to take ownership of this dynamic if we haven't already
            if (fLOSDB == plSimDefs::kLOSDBLocalAvatar && !phys->IsLocallyOwned() && !phys->GetProperty(plSimulationInterface::kNoOwnershipChange))
            {
                plSynchedObject* obj = plSynchedObject::ConvertNoRef(phys->GetObjectKey()->ObjectIsLoaded());
                obj->SetNetGroupConstant(plNetGroup::kNetGroupLocalPhysicals);

                plSetNetGroupIDMsg* setNetGroupID = new plSetNetGroupIDMsg;
                setNetGroupID->fId = plNetGroup::kNetGroupRemotePhysicals;
                setNetGroupID->SetBCastFlag(plMessage::kNetPropagate | plMessage::kNetForce);
                setNetGroupID->SetBCastFlag(plMessage::kLocalPropagate, false);
                setNetGroupID->Send(obj->GetKey());
            }

            phys->ApplyHitForce();
            //simMgr->ConsiderSynch(phys, nullptr);
        }
        fDynamicHits.clear();
    }
}

