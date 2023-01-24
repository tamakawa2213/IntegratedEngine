#include "Math.h"
#include "CallDef.h"
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
        //-1から1に収める
        CLAMP(Coord, -1, 1);

        //一次関数y = ax
        float y = Incli * Coord;
        //五次関数C(t) = 1 - (6(|t|^5) -15(t^4) + 10(|t|^3))
        float C = 1 - (6 * fabsf(powf(Coord, 5))) - 15 * (powf(Coord, 4)) + (10 * fabsf(powf(Coord, 3)));

        //二つの関数を掛けた値が答え
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

        //n桁目以下の桁を取得
        val = value % (int)pow(10, val);

        //m桁目以上の桁を取得
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
        std::vector<int> ans;   //最終的に答えとなる配列
        ans.clear();            //初期化
        ans.reserve(article);   //配列の要素数を確保

        std::vector<int> sieve; //エラトステネスの篩
        sieve.clear();          //初期化
        sieve.push_back(2);     //2から検索開始する

        ans.push_back(2);       //呼び出しは1以上の値を前提とするので予め2を入れる

        int PrimeNumber = 3;    //検索対象の数値 : 初期値は3にする

        bool IsPrime = true;   //検索対象が素数かどうかを判定させる

        //配列の要素数が指定した数を超えるまで挿入
        while ((int)ans.size() < article)
        {
            //ansがsieveより要素数が多い場合に呼び出す
            if (ans.size() > sieve.size())
            {
                //sieveの最後尾の値 = ansのsieve.size()に格納されている値なので
                //その次の値の二乗が検索対象より小さければ
                if (PrimeNumber >= pow((double)ans.at(sieve.size()), 2))
                {
                    sieve.push_back(ans.at(sieve.size()));  //sieveにその値を格納
                }
            }

            //sieveの要素毎に判定させる
            //for (int i = 0; i < sieve.size(); i++)
            //{
            //    //sieveの要素の値で割り切れたら
            //    if (PrimeNumber % sieve.at(i) == 0)
            //    {
            //        IsPrime = false;    //IsPrimeを倒して
            //        break;              //強制終了
            //    }
            //}

            for (const auto& it : sieve)
            {
                //sieveの要素の値で割り切れたら
                if (PrimeNumber % it == 0)
                {
                    IsPrime = false;    //IsPrimeを倒して
                    break;              //強制終了
                }
            }

            if (IsPrime)   //検索対象が素数ならば
            {
                ans.push_back(PrimeNumber); //要素に追加
            }

            IsPrime = true; //IsPrimeを強制的に立たせる

            //偶数の素数が2以外に存在しないので
            PrimeNumber += 2;   //PrimeNumberに2を加算させる
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
        //-1から1に収める
        CLAMP(Coord, -1, 1);

        return 1 - (3 * Coord * Coord) + (2 * fabsf(powf(Coord, 3)));
    }

    void ArbRotationAxis(XMFLOAT3* pos, float rot, XMVECTOR axis, XMVECTOR end)
    {
        //度数法→弧度法に変換
        float rad = XMConvertToRadians(rot);

        ArbRotationAxisR(pos, rad, axis, end);
    }
    void ArbRotationAxisR(XMFLOAT3* pos, float rad, XMVECTOR axis, XMVECTOR end)
    {
        //endを原点に移動
        XMVECTOR vPos = XMLoadFloat3(pos);
        vPos -= end;
        axis -= end;
        
        //クォータニオン、共役クォータニオンを作成
        XMVECTOR Qua = XMQuaternionRotationAxis(axis, rad);
        XMVECTOR Conj = XMQuaternionConjugate(Qua);

        //ansに移動後の位置情報が入る
        XMVECTOR ans = vPos;

        //それぞれのベクトルをかける
        ans = XMQuaternionMultiply(Conj, vPos);
        ans = XMQuaternionMultiply(ans, Qua);

        ans += end;

        //posの値を更新
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
}