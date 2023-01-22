#include "Image.h"
#include "CallDef.h"
#include "Input.h"

namespace
{
    std::vector<ImageSet*> FileSet;      //Fbxの構造体の動的配列
}

namespace Image
{
    int Load(LPCWSTR filename)
    {
        HRESULT hr;
        ImageSet* File = new ImageSet;
        File->FileName = filename;
        for (auto itr = FileSet.begin(); itr != FileSet.end(); itr++)
        {
            //同じ名前のファイルをすでにロードしていた場合
            if (File->FileName == (*itr)->FileName)
            {
                File->pSprite = (*itr)->pSprite;
                break;
            }
        }
        //見つからなかった場合、新しくロードする
        File->pSprite = new Sprite;
        hr = File->pSprite->Initialize(filename);
        if (FAILED(hr)) //ロードに失敗した場合
        {
            SAFE_DELETE(File->pSprite);
            SAFE_DELETE(File);
        }

        FileSet.push_back(File);
        return (int)FileSet.size() - 1;
    }

    int Load(std::string filename)
    {
        wchar_t file[CHAR_MAX];
        size_t ret;
        mbstowcs_s(&ret, file, filename.c_str(), filename.length());
        return Load(file);
    }

    void SetTransform(int hPicture, Transform transform)
    {
        FileSet[hPicture]->transform = transform;
    }

    void Draw(int hModel)
    {
        FileSet[hModel]->pSprite->Draw(FileSet[hModel]->transform);
    }

    bool IsHitCursor(int hModel)
    {
        UINT wid = (UINT)(FileSet[hModel]->pSprite->GetImgWidth() * FileSet[hModel]->transform.scale_.x / 2);
        UINT hgt = (UINT)(FileSet[hModel]->pSprite->GetImgHeight() * FileSet[hModel]->transform.scale_.y / 2);
        float Left = (FileSet[hModel]->transform.position_.x + 1) * (Direct3D::scrWidth / 2) - wid;
        float Right = (FileSet[hModel]->transform.position_.x + 1) * (Direct3D::scrWidth / 2) + wid;
        float Top = (-FileSet[hModel]->transform.position_.y + 1) * (Direct3D::scrHeight / 2) - hgt;
        float Bottom = (-FileSet[hModel]->transform.position_.y + 1) * (Direct3D::scrHeight / 2) + hgt;
        if (Left <= Input::GetMousePosition().x && Input::GetMousePosition().x <= Right &&
            Top <= Input::GetMousePosition().y && Input::GetMousePosition().y <= Bottom)
        {
            return true;
        }
        return false;
    }

    void Release()
    {
        for (int i = 0; i < FileSet.size(); i++)
        {
            if (!FileSet[i]->FindFbx)
            {
                SAFE_DELETE(FileSet[i]->pSprite);
            }
            SAFE_DELETE(FileSet[i]);
        }
        FileSet.clear();
    }
}