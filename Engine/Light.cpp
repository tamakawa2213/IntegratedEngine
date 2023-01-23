#include "Light.h"

Light::Light() : pos()
{
}

Light::~Light()
{
}

void Light::SetLight(XMVECTOR update)
{
    pos = update;
}

XMFLOAT4 Light::GetLight()
{
    XMFLOAT4 fpos;
    XMStoreFloat4(&fpos, pos);
    return fpos;
}
