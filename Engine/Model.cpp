#include "Model.h"
#include "CallDef.h"

namespace Model
{
    std::vector<Fileset*> FileSet;      //Fbxの構造体の動的配列

    int Model::Load(std::string filename)
    {
        HRESULT hr;
        Fileset* File = new Fileset;
        File->FileName = filename;

        auto itr = std::find(FileSet.begin(), FileSet.end(), File);
        //同じ名前のファイルをすでにロードしていた場合
        if (itr != FileSet.end())
        {
            File->pFbx = (*itr)->pFbx;
            File->FindFbx = true;
            return (int)std::distance(FileSet.begin(), itr);
        }
        //見つからなかった場合、新しくロードする
        if (!File->FindFbx)
        {
            File->pFbx = new Fbx;
            hr = File->pFbx->Load(filename);
            if (FAILED(hr)) //ロードに失敗した場合
            {
                SAFE_DELETE(File->pFbx);
                SAFE_DELETE(File);
                return -1;
            }
        }

        FileSet.push_back(File);
        return (int)FileSet.size() - 1;
    }

    void Model::SetTransform(int hModel, Transform transform)
    {
        FileSet[hModel]->transform = transform;
    }

    void Model::Draw(int hModel, const Light* lightpos, char SHADER_TYPE)
    {
        FileSet[hModel]->pFbx->Draw(FileSet[hModel]->transform, FileSet[hModel]->Alpha, lightpos, SHADER_TYPE);
    }

    void Draw(int hModel, Transform transform, const Light* lightpos, char SHADER_TYPE)
    {
        FileSet[hModel]->transform = transform;
        FileSet[hModel]->pFbx->Draw(FileSet[hModel]->transform, FileSet[hModel]->Alpha, lightpos, SHADER_TYPE);
    }

    void Draw(int hModel, XMFLOAT3 Chroma, float Bright, const Light* lightpos, char SHADER_TYPE)
    {
        FileSet[hModel]->pFbx->Draw(FileSet[hModel]->transform, Chroma, Bright, FileSet[hModel]->Alpha, lightpos, SHADER_TYPE);
    }

    void Release()
    {
        for (auto i : FileSet)
        {
            if (!i->FindFbx)
            {
                SAFE_DELETE(i->pFbx);
            }
            SAFE_DELETE(i);
        }
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
        for (int i = 0; i < FileSet.size(); i++)
        {
            FileSet[i]->Alpha = alpha;
        }
    }
}