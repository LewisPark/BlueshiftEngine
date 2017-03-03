#include "Precompiled.h"
#include "Script/LuaVM.h"
#include "Math/Math.h"

BE_NAMESPACE_BEGIN

void LuaVM::RegisterRotation(LuaCpp::Module &module) {
    LuaCpp::Selector _Rotation = module["Rotation"];

    _Rotation.SetClass<Rotation>();
    _Rotation.AddClassCtor<Rotation, const Vec3 &, const Vec3 &, float>();
    _Rotation.AddClassMembers<Rotation>(
        "set", &Rotation::Set,
        "set_origin", &Rotation::SetOrigin,
        "set_vec", static_cast<void(Rotation::*)(const float, const float, const float)>(&Rotation::SetVec),
        "set_angle", &Rotation::SetAngle,
        "origin", &Rotation::GetOrigin,
        "vec", &Rotation::GetVec,
        "angle", &Rotation::GetAngle,
        "to_angles", &Rotation::ToAngles,
        "to_quat", &Rotation::ToQuat,
        "to_mat3", &Rotation::ToMat3,
        "to_mat4", &Rotation::ToMat4,
        "scale", static_cast<Rotation(Rotation::*)(const float)const>(&Rotation::operator*),
        "scale_self", static_cast<Rotation&(Rotation::*)(const float)>(&Rotation::operator*=),
        "rotate_point", &Rotation::RotatePoint,
        "normalize180", &Rotation::Normalize180,
        "normalize360", &Rotation::Normalize360        
    );
    _Rotation.AddClassMembers<Rotation>(
        "__unm", static_cast<Rotation(Rotation::*)() const>(&Rotation::operator-)
    );
}

BE_NAMESPACE_END