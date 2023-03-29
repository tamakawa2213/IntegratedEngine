#include "Transform.h"
#include "CallDef.h"
#include "GameObject.h"

Transform::Transform():matTranslate_(XMMatrixIdentity()),matRotate_(XMMatrixIdentity()),matScale_(XMMatrixIdentity()),
position_(), rotate_(), scale_(1, 1, 1), pParent_(nullptr)
{
}

Transform::~Transform()
{
}

void Transform::Calclation()
{
    //ˆÚ“®s—ñ
    matTranslate_ = XMMatrixTranslation( position_.x, position_.y, position_.z);

    //‰ñ“]s—ñ
    matRotate_ = XMMatrixRotationX(XMConvertToRadians(rotate_.x))
        * XMMatrixRotationY(XMConvertToRadians(rotate_.y))
        * XMMatrixRotationZ(XMConvertToRadians(rotate_.z));

    //Šg‘ås—ñ
    matScale_ = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
}

XMMATRIX Transform::GetWorldMatrix()
{
    Calclation();
    if (pParent_)
    {
        return matScale_ * matRotate_ * matTranslate_ * pParent_->GetWorldMatrix();
    }
    //Šg‘å * ‰ñ“] * ˆÚ“®
    return (matScale_ * matRotate_ * matTranslate_);
}

XMMATRIX Transform::GetNormalMatrix()
{
    return (XMMatrixInverse(nullptr, matScale_) * matRotate_);
}

void Transform::LookAt(XMFLOAT3 target)
{
    float SightX = target.x - position_.x;
    float SightY = target.y - position_.y;
    float SightZ = target.z - position_.z;

    XMVECTOR axisY = XMVectorSet(SightX, 0, SightZ, 0);
    axisY = XMVector3Normalize(axisY);

    XMVECTOR Sight = XMVectorSet(SightX, SightY, SightZ, 0);
    Sight = XMVector3Normalize(Sight);
    if (SightZ > 0)
    {
        rotate_.y = XMVectorGetX(axisY) * 90;
    }
    else
    {
        rotate_.y = (-XMVectorGetX(axisY) * 90) + 180;
    }
    rotate_.x = -XMVectorGetY(Sight) * 90;
}