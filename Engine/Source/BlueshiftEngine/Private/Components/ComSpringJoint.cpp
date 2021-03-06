// Copyright(c) 2017 POLYGONTEK
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http ://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "Precompiled.h"
#include "Render/Render.h"
#include "Components/ComTransform.h"
#include "Components/ComRigidBody.h"
#include "Components/ComSpringJoint.h"
#include "Game/Entity.h"
#include "Game/GameWorld.h"

BE_NAMESPACE_BEGIN

OBJECT_DECLARATION("Spring Joint", ComSpringJoint, ComJoint)
BEGIN_EVENTS(ComSpringJoint)
END_EVENTS
BEGIN_PROPERTIES(ComSpringJoint)
    PROPERTY_VEC3("anchor", "Anchor", "", "0 0 0", PropertySpec::ReadWrite),
    PROPERTY_ANGLES("angles", "Angles", "", "0 0 0", PropertySpec::ReadWrite),
    PROPERTY_FLOAT("lowerLimit", "Lower Limit", "", "0", PropertySpec::ReadWrite),
    PROPERTY_FLOAT("upperLimit", "Upper Limit", "", "0", PropertySpec::ReadWrite),
    PROPERTY_FLOAT("stiffness", "Stiffness", "", "2", PropertySpec::ReadWrite),
    PROPERTY_RANGED_FLOAT("damping", "Damping", "", Rangef(0, 1, 0.01f), "0.2", PropertySpec::ReadWrite),
END_PROPERTIES

void ComSpringJoint::RegisterProperties() {
    //REGISTER_ACCESSOR_PROPERTY("Anchor", Vec3, GetAnchor, SetAnchor, "0 0 0", PropertySpec::ReadWrite);
    //REGISTER_ACCESSOR_PROPERTY("Angles", Angles, GetAngles, SetAngles, "0 0 0", PropertySpec::ReadWrite);
    //REGISTER_ACCESSOR_PROPERTY("Lower Limit", float, GetLowerLimit, SetLowerLimit, "0", PropertySpec::ReadWrite);
    //REGISTER_ACCESSOR_PROPERTY("Upper Limit", float, GetUpperLimit, SetUpperLimit, "0", PropertySpec::ReadWrite);
    //REGISTER_ACCESSOR_PROPERTY("Stiffness", float, GetStiffness, SetStiffness, "2", PropertySpec::ReadWrite);
    //REGISTER_ACCESSOR_PROPERTY("Damping", float, GetDamping, SetDamping, "0.2", PropertySpec::ReadWrite).SetRange(0, 1, 0.01f);
}

ComSpringJoint::ComSpringJoint() {
    Connect(&SIG_PropertyChanged, this, (SignalCallback)&ComSpringJoint::PropertyChanged);
}

ComSpringJoint::~ComSpringJoint() {
}

void ComSpringJoint::Init() {
    ComJoint::Init();

    anchor = props->Get("anchor").As<Vec3>();
    axis = props->Get("angles").As<Angles>().ToMat3();
    axis.FixDegeneracies();

    lowerLimit = props->Get("lowerLimit").As<float>();
    upperLimit = props->Get("upperLimit").As<float>();
    stiffness = props->Get("stiffness").As<float>();
    damping = props->Get("damping").As<float>();
}

void ComSpringJoint::Start() {
    ComJoint::Start();

    const ComTransform *transform = GetEntity()->GetTransform();
    const ComRigidBody *rigidBody = GetEntity()->GetComponent<ComRigidBody>();
    assert(rigidBody);

    PhysConstraintDesc desc;
    desc.type           = PhysConstraint::GenericSpring;
    desc.bodyA          = rigidBody->GetBody();
    desc.axisInA        = axis;
    desc.anchorInA      = transform->GetScale() * anchor;

    if (connectedBody) {
        Vec3 worldAnchor = desc.bodyA->GetOrigin() + desc.bodyA->GetAxis() * desc.anchorInA;

        desc.bodyB      = connectedBody->GetBody();  
        desc.axisInB    = axis;
        desc.anchorInB  = connectedBody->GetBody()->GetAxis().TransposedMulVec(worldAnchor - connectedBody->GetBody()->GetOrigin());
    } else {
        desc.bodyB      = nullptr;
    }

    desc.collision      = collisionEnabled;
    desc.breakImpulse   = breakImpulse;
    constraint = physicsSystem.CreateConstraint(&desc);

    PhysGenericSpringConstraint *genericSpringConstraint = static_cast<PhysGenericSpringConstraint *>(constraint);    
    genericSpringConstraint->SetLinearLowerLimit(Vec3(0, 0, lowerLimit));
    genericSpringConstraint->SetLinearUpperLimit(Vec3(0, 0, upperLimit));
    genericSpringConstraint->SetLinearStiffness(Vec3(0, 0, stiffness));
    genericSpringConstraint->SetLinearDamping(Vec3(0, 0, damping));

    if (IsEnabled()) {
        genericSpringConstraint->AddToWorld(GetGameWorld()->GetPhysicsWorld());
    }
}

void ComSpringJoint::DrawGizmos(const SceneView::Parms &sceneView, bool selected) {
    RenderWorld *renderWorld = GetGameWorld()->GetRenderWorld();

    const ComTransform *transform = GetEntity()->GetTransform();
    Vec3 worldOrigin = transform->GetWorldMatrix() * anchor;

    renderWorld->SetDebugColor(Color4::red, Color4::zero);
    renderWorld->DebugLine(worldOrigin - Mat3::identity[0] * CentiToUnit(5), worldOrigin + Mat3::identity[0] * CentiToUnit(5), 1);
    renderWorld->DebugLine(worldOrigin - Mat3::identity[1] * CentiToUnit(5), worldOrigin + Mat3::identity[1] * CentiToUnit(5), 1);
    renderWorld->DebugLine(worldOrigin - Mat3::identity[2] * CentiToUnit(5), worldOrigin + Mat3::identity[2] * CentiToUnit(5), 1);
}

void ComSpringJoint::PropertyChanged(const char *classname, const char *propName) {
    if (!IsInitalized()) {
        return;
    }

    if (!Str::Cmp(propName, "anchor")) {
        SetAnchor(props->Get("anchor").As<Vec3>());
        return;
    }
    
    if (!Str::Cmp(propName, "angles")) {
        SetAngles(props->Get("angles").As<Angles>());
        return;
    }

    if (!Str::Cmp(propName, "lowerLimit")) {
        SetLowerLimit(props->Get("lowerLimit").As<float>());
        return;
    }

    if (!Str::Cmp(propName, "upperLimit")) {
        SetUpperLimit(props->Get("upperLimit").As<float>());
        return;
    }

    if (!Str::Cmp(propName, "stiffness")) {
        SetStiffness(props->Get("stiffness").As<float>());
        return;
    }

    if (!Str::Cmp(propName, "damping")) {
        SetDamping(props->Get("damping").As<float>());
        return;
    }

    ComJoint::PropertyChanged(classname, propName);
}

const Vec3 &ComSpringJoint::GetAnchor() const {
    return anchor;
}

void ComSpringJoint::SetAnchor(const Vec3 &anchor) {
    this->anchor = anchor;
    if (constraint) {
        ((PhysGenericSpringConstraint *)constraint)->SetFrameA(anchor, axis);
    }
}

const Angles ComSpringJoint::GetAngles() const {
    return axis.ToAngles();
}

void ComSpringJoint::SetAngles(const Angles &angles) {
    this->axis = angles.ToMat3();
    if (constraint) {
        ((PhysGenericSpringConstraint *)constraint)->SetFrameA(anchor, axis);
    }
}

const float ComSpringJoint::GetLowerLimit() const {
    return lowerLimit;
}

void ComSpringJoint::SetLowerLimit(const float limit) {
    this->lowerLimit = limit;
    if (constraint) {
        ((PhysGenericSpringConstraint *)constraint)->SetLinearLowerLimit(Vec3(0, 0, lowerLimit));
    }
}

const float ComSpringJoint::GetUpperLimit() const {
    return upperLimit;
}

void ComSpringJoint::SetUpperLimit(const float limit) {
    this->upperLimit = limit;
    if (constraint) {
        ((PhysGenericSpringConstraint *)constraint)->SetLinearUpperLimit(Vec3(0, 0, upperLimit));
    }
}

const float ComSpringJoint::GetStiffness() const {
    return stiffness;
}

void ComSpringJoint::SetStiffness(const float stiffness) {
    this->stiffness = stiffness;
    if (constraint) {
        ((PhysGenericSpringConstraint *)constraint)->SetLinearStiffness(Vec3(0, 0, stiffness));
    }
}

const float ComSpringJoint::GetDamping() const {
    return damping;
}

void ComSpringJoint::SetDamping(const float damping) {
    this->damping = damping;
    if (constraint) {
        ((PhysGenericSpringConstraint *)constraint)->SetLinearDamping(Vec3(0, 0, damping));
    }
}

BE_NAMESPACE_END
