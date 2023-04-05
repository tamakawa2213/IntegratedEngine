#include "Model.h"
#include "CallDef.h"
#include "Direct3D.h"
#include <memory>
#include <vector>

namespace
{
    struct Fileset
    {
        std::unique_ptr<Fbx> pFbx;	//Fbxのポインタ
        Transform transform;		//transformクラス
        std::string FileName;		//ファイルの名前
        bool FindFbx;	        	//Fbxファイルを事前にロードしているか
        float Alpha;		    	//モデルの透明度

        Fileset() : pFbx(nullptr), transform(), FileName(), FindFbx(false), Alpha(1.0f) {}
    };
    std::vector<std::shared_ptr<Fileset>> FileSet;      //Fbxの構造体の動的配列
}
namespace Model
{

    int Load(const std::string& filename)
    {
        HRESULT hr;
        std::shared_ptr<Fileset> File = std::make_shared<Fileset>();
        File->FileName = filename;

        
        //同じ名前のファイルをすでにロードしていた場合
        if (auto itr = std::find(FileSet.begin(), FileSet.end(), File); itr != FileSet.end())
        {
            (*itr)->FindFbx = true;
            return (int)std::distance(FileSet.begin(), itr);
        }
        //見つからなかった場合、新しくロードする
        if (!File->FindFbx)
        {
            File->pFbx = std::make_unique<Fbx>();
            hr = File->pFbx->Load(filename);
            if (FAILED(hr)) //ロードに失敗した場合
            {
                return -1;
            }
        }

        FileSet.push_back(std::move(File));
        return (int)FileSet.size() - 1;
    }

    void SetTransform(int hModel, const Transform& transform)
    {
        FileSet[hModel]->transform = transform;
    }

    void Draw(int hModel, const Light* lightpos, SHADER_TYPE type)
    {
        FileSet[hModel]->pFbx->Draw(FileSet[hModel]->transform, FileSet[hModel]->Alpha, lightpos, type);
    }

    void Draw(int hModel, const Transform& transform, const Light* lightpos, SHADER_TYPE type)
    {
        FileSet[hModel]->transform = transform;
        FileSet[hModel]->pFbx->Draw(FileSet[hModel]->transform, FileSet[hModel]->Alpha, lightpos, type);
    }

    void Draw(int hModel, const XMFLOAT3& Chroma, float Bright, const Light* lightpos, SHADER_TYPE type)
    {
        FileSet[hModel]->pFbx->Draw(FileSet[hModel]->transform, Chroma, Bright, FileSet[hModel]->Alpha, lightpos, type);
    }

    void Release()
    {
        FileSet.clear();
    }

    void RayCast(int hModel, RayCastData& Raydata)
    {
        XMMATRIX matInv = XMMatrixInverse(nullptr, FileSet[hModel]->transform.GetWorldMatrix());

        XMFLOAT3 point = XMFLOAT3(Raydata.start.x + Raydata.dir.x, Raydata.start.y + Raydata.dir.y, Raydata.start.z + Raydata.dir.z);

        XMVECTOR start = XMLoadFloat3(&Raydata.start);
        start = XMVector3TransformCoord(start, matInv);
        XMStoreFloat3(&Raydata.start, start);

        XMVECTOR pass = XMLoadFloat3(&point);
        pass = XMVector3TransformCoord(pass, matInv);
        XMStoreFloat3(&Raydata.dir, pass - start);

        FileSet[hModel]->pFbx->RayCast(Raydata);
    }

    void SetTexture(int hModel, const Texture* tex)
    {
        FileSet[hModel]->pFbx->SetTexture(tex);
    }

    void AllAlterAlpha(float alpha)
    {
        for (auto&& itr : FileSet)
        {
            itr->Alpha = alpha;
        }
    }
}