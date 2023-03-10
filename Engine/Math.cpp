#include "Math.h"
#include "CallDef.h"
#include "Direct3D.h"
#include "Transform.h"
#include <random>

namespace Math
{
    float Det(XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c)
    {
        return a.x * b.y * c.z +
            a.z * b.x * c.y +
            a.y * b.z * c.x -
            a.z * b.y * c.x -
            a.y * b.x * c.z -
            a.x * b.z * c.y;
    }

    bool Intersect(XMFLOAT3 start, XMFLOAT3 dir, XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2)
    {
        XMFLOAT3 a = XMFLOAT3(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
        XMFLOAT3 b = XMFLOAT3(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
        XMFLOAT3 d = XMFLOAT3(start.x - v0.x, start.y - v0.y, start.z - v0.z);
        dir = XMFLOAT3(-dir.x, -dir.y, -dir.z);

        float u, v, l;
        u = Det(d, b, dir) / Det(a, b, dir);
        v = Det(a, d, dir) / Det(a, b, dir);
        l = Det(a, b, d) / Det(a, b, dir);
        if (u + v <= 1 && l >= 0 && u >= 0 && v >= 0)
        {
            return true;
        }
        return false;
    }

    bool Intersect(XMFLOAT3 start, XMFLOAT3 dir, XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2, float* dist)
    {
        XMFLOAT3 a = XMFLOAT3(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
        XMFLOAT3 b = XMFLOAT3(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
        XMFLOAT3 d = XMFLOAT3(start.x - v0.x, start.y - v0.y, start.z - v0.z);
        dir = XMFLOAT3(-dir.x, -dir.y, -dir.z);

        float u, v, l;
        u = Det(d, b, dir) / Det(a, b, dir);
        v = Det(a, d, dir) / Det(a, b, dir);
        l = Det(a, b, d) / Det(a, b, dir);
        if (u + v <= 1 && l >= 0 && u >= 0 && v >= 0)
        {
            if (*dist > l)
            {
                *dist = l;
                return true;
            }

        }
        return false;
    }

    bool SegmentToPlane(XMFLOAT3 segstart, XMFLOAT3 segend, XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2)
    {
        XMFLOAT3 a = XMFLOAT3(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
        XMFLOAT3 b = XMFLOAT3(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);
        XMFLOAT3 d = XMFLOAT3(segstart.x - v0.x, segstart.y - v0.y, segstart.z - v0.z);

        XMFLOAT3 dir = XMFLOAT3(segend.x - segstart.x, segend.y - segstart.y, segend.z - segstart.z);
        dir.x *= dir.x;
        dir.y *= dir.y;
        dir.z *= dir.z;
        float dist;
        dist = sqrtf(dir.x + dir.y + dir.z);

        XMVECTOR vDir = XMLoadFloat3(&dir);
        vDir = XMVector3Normalize(vDir);
        XMStoreFloat3(&dir, vDir);

        dir = XMFLOAT3(-dir.x, -dir.y, -dir.z);

        float u, v, l;
        u = Det(d, b, dir) / Det(a, b, dir);
        v = Det(a, d, dir) / Det(a, b, dir);
        l = Det(a, b, d) / Det(a, b, dir);
        if (u + v <= 1 && l >= 0 && u >= 0 && v >= 0 && l < dist)
        {
            return true;
        }
        return false;
    }
    float Wavelet(float Incli, float Coord)
    {
        //-1????1????????
        CLAMP(Coord, -1, 1);

        //????????y = ax
        float y = Incli * Coord;
        //????????C(t) = 1 - (6(|t|^5) -15(t^4) + 10(|t|^3))
        float C = 1 - (6 * fabsf(powf(Coord, 5))) - 15 * (powf(Coord, 4)) + (10 * fabsf(powf(Coord, 3)));

        //?????????????|?????l??????
        return y * C;
    }
    float Waveret_2Dim(float Incli_X, float Incli_Y, float Coord_X, float Coord_Y)
    {
        return Wavelet(Incli_X, Coord_X) * Wavelet(Incli_Y, Coord_Y);
    }

    int xorshift32(int v) {
        v = v ^ (v << 13);
        v = v ^ (v >> 17);
        v = v ^ (v << 15);
        return v;
    }

    int GetDigits(int value, int m, int n)
    {
        int val = n + 1;
        int result;

        //n??????????????????
        val = value % (int)pow(10, val);

        //m??????????????????
        result = val / (int)pow(10, m);

        return result;
    }

    int GetFraction(float value, int m)
    {
        int val;
        val = (int)(value * (int)pow(10, m));
        val = GetDigits(val, 0, 0);
        return val;
    }

    std::vector<int> MakePrime(int article)
    {

        std::vector<int> ans;   //???I?I?????????????z??
        ans.clear();            //??????

        //0??????????????????????
        if (article <= 0)
        {
            return ans;
        }

        ans.reserve(article);   //?z?????v?f?????m??

        std::vector<int> sieve; //?G???g?X?e?l?X????
        sieve.clear();          //??????
        sieve.push_back(2);     //2?????????J?n????

        ans.push_back(2);

        int PrimeNumber = 3;    //?????????????l : ?????l??3??????

        bool IsPrime = true;   //???????????f??????????????????????

        //?z?????v?f?????w?????????????????????}??
        while ((int)ans.size() < article)
        {
            //ans??sieve?????v?f???????????????????o??
            if (ans.size() > sieve.size())
            {
                //sieve???????????l = ans??sieve.size()???i?[???????????l??????
                //?????????l??????????????????????????????
                if (PrimeNumber >= pow((double)ans.at(sieve.size()), 2))
                {
                    sieve.push_back(ans.at(sieve.size()));  //sieve???????l???i?[
                }
            }

            for (const auto& it : sieve)
            {
                //sieve???v?f???l??????????????
                if (PrimeNumber % it == 0)
                {
                    IsPrime = false;    //IsPrime???|????
                    break;              //?????I??
                }
            }

            if (IsPrime)   //???????????f????????
            {
                ans.push_back(PrimeNumber); //?v?f??????
            }

            IsPrime = true; //IsPrime???????I??????????

            //???????f????2???O????????????????
            PrimeNumber += 2;   //PrimeNumber??2?????Z??????
        }
        return ans;
    }

    float GetDistance(Transform tr1, Transform tr2)
    {
        return GetDistance(tr1.position_, tr2.position_);
    }

    float GetDistance(XMFLOAT3 tr1, XMFLOAT3 tr2)
    {
        float ansX = tr1.x - tr2.x;
        float ansY = tr1.y - tr2.y;
        float ansZ = tr1.z - tr2.z;
        ansX *= ansX;
        ansY *= ansY;
        ansZ *= ansZ;
        return sqrtf(ansX + ansY + ansZ);
    }

    float GetDistanceSphere(XMFLOAT3 tr1, XMFLOAT3 tr2, float radius1, float radius2)
    {
        return max(GetDistance(tr1, tr2) - radius1 - radius2, 0);
    }

    float Func_Cubic(float Coord)
    {
        //-1????1????????
        CLAMP(Coord, -1, 1);

        return 1 - (3 * Coord * Coord) + (2 * fabsf(powf(Coord, 3)));
    }

    void ArbRotationAxis(XMFLOAT3* pos, float rot, XMVECTOR axis, XMVECTOR end)
    {
        //?x???@?????x?@??????
        float rad = XMConvertToRadians(rot);

        ArbRotationAxisR(pos, rad, axis, end);
    }
    void ArbRotationAxisR(XMFLOAT3* pos, float rad, XMVECTOR axis, XMVECTOR end)
    {
        //end?????_??????
        XMVECTOR vPos = XMLoadFloat3(pos);
        vPos -= end;
        axis -= end;
        
        //?N?H?[?^?j?I???A?????N?H?[?^?j?I????????
        XMVECTOR Qua = XMQuaternionRotationAxis(axis, rad);
        XMVECTOR Conj = XMQuaternionConjugate(Qua);

        //ans?????????????u??????????
        XMVECTOR ans = vPos;

        //???????????x?N?g??????????
        ans = XMQuaternionMultiply(Conj, vPos);
        ans = XMQuaternionMultiply(ans, Qua);

        ans += end;

        //pos???l???X?V
        XMStoreFloat3(pos, ans);
    }

    void FixValue(float* axis, int value)
    {
        while (*axis > value)
        {
            *axis -= value;
        }
        while (*axis < 0)
        {
            *axis += value;
        }
    }

    XMFLOAT3 TransformToPixel(XMFLOAT3 transform)
    {
        //position_??0~1?????X
        XMFLOAT3 pos = { (transform.x + 1) / 2, (-transform.y + 1) / 2, transform.z };

        pos = { pos.x * Direct3D::scrWidth, pos.y * Direct3D::scrHeight, pos.z };

        return pos;
    }

    XMFLOAT3 PixelToTransform(XMFLOAT3 pixel)
    {
        XMFLOAT3 pos = { pixel.x / Direct3D::scrWidth, pixel.y / Direct3D::scrHeight, pixel.z };
        pos = { pos.x * 2 - 1, -pos.y * 2 + 1, pos.z };
        return pos;
    }
}