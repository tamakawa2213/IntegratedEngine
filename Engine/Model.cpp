#include "Model.h"
#include "Direct3D.h"
#include <memory>
#include <vector>

namespace
{
    struct Fileset
    {
        std::unique_ptr<Fbx> pFbx;	//Fbx�̃|�C���^
        Transform transform;		//transform�N���X
        std::string FileName;		//�t�@�C���̖��O
        float Alpha;		    	//���f���̓����x

        Fileset() : pFbx(nullptr), transform(), FileName(), Alpha(1.0f) {}
        Fileset(const std::string& file) : pFbx(nullptr), transform(), FileName(file), Alpha(1.0f) {}

        bool operator ==(const Fileset& fl) const { return (FileName == fl.FileName); }
        bool operator !=(const Fileset& fl) const { return !(*this == fl); }
    };

    //Fileset�̓��l��r�̃I�[�o�[���[�h
    //shared_ptr�̔�r���D�悳���ׁA�t���C�E�F�C�g�ɂ͂��ꂪ�K�v
    bool operator ==(const std::shared_ptr<Fileset> fl, const std::shared_ptr<Fileset> fr) { return (fl->FileName == fr->FileName); }
    bool operator !=(const std::shared_ptr<Fileset> fl, const std::shared_ptr<Fileset> fr) { return !(fl == fr); }

    std::vector<std::shared_ptr<Fileset>> FileSet;      //Fbx�̍\���̂̓��I�z��
}
namespace Model
{

    int Load(const std::string& filename)
    {
        std::shared_ptr<Fileset> File = std::make_shared<Fileset>(filename);

        //�������O�̃t�@�C�������łɃ��[�h���Ă����ꍇ
        if (auto itr = std::find(FileSet.begin(), FileSet.end(), File); itr != FileSet.end())
        {
             return (int)std::distance(FileSet.begin(), itr);
        }

        //������Ȃ������ꍇ�A�V�������[�h����
        File->pFbx = std::make_unique<Fbx>();
        if (FAILED(File->pFbx->Load(filename))) //���[�h�Ɏ��s�����ꍇ
        {
            return -1;
        }

        FileSet.push_back(std::move(File));
        return (int)FileSet.size() - 1;
    }

    void SetTransform(unsigned hModel, const Transform& transform)
    {
        FileSet[hModel]->transform = transform;
    }

    void Draw(unsigned hModel, const Light* lightpos)
    {
        FileSet[hModel]->pFbx->Draw(FileSet[hModel]->transform, FileSet[hModel]->Alpha, lightpos);
    }

    void Draw(unsigned hModel, const Transform& transform, const Light* lightpos)
    {
        FileSet[hModel]->transform = transform;
        FileSet[hModel]->pFbx->Draw(FileSet[hModel]->transform, FileSet[hModel]->Alpha, lightpos);
    }

    void Draw(unsigned hModel, const XMFLOAT3& Chroma, float Bright, const Light* lightpos)
    {
        FileSet[hModel]->pFbx->Draw(FileSet[hModel]->transform, Chroma, Bright, FileSet[hModel]->Alpha, lightpos);
    }

    void Release()
    {
        FileSet.clear();
    }

    void RayCast(unsigned hModel, RayCastData& Raydata)
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

    void SetTexture(unsigned hModel, const Texture* tex)
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