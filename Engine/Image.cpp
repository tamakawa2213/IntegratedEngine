#include "Image.h"
#include "CallDef.h"
#include "Input.h"
#include "IniOperator.h"
#include "Math.h"

namespace
{
    std::vector<ImageSet*> FileSet;      //Fbxの構造体の動的配列
}

namespace Image
{
    void CallStatus(int hPict); //iniファイルに保存された初期位置を呼び出す
}

namespace Image
{
    int Load(std::string filename)
    {
        HRESULT hr;
        ImageSet* File = new ImageSet;
        File->FileName = filename;

        wchar_t file[CHAR_MAX];
        size_t ret;
        mbstowcs_s(&ret, file, filename.c_str(), filename.length());

        //同じファイルを使っていないか検索
        auto itr = std::find(FileSet.begin(), FileSet.end(), File);
        if (itr != FileSet.end())
        {
            //同じ名前のファイルをすでにロードしていた場合
            File->pSprite = (*itr)->pSprite;
            File->FindFbx = true;
            return (int)std::distance(FileSet.begin(), itr);
        }
        
        //見つからなかった場合、新しくロードする
        File->pSprite = new Sprite;
        hr = File->pSprite->Initialize(file);
        if (FAILED(hr)) //ロードに失敗した場合
        {
            SAFE_DELETE(File->pSprite);
            SAFE_DELETE(File);
            return -1;
        }
        FileSet.push_back(File);
        CallStatus((int)FileSet.size() - 1);
        return (int)FileSet.size() - 1;
    }

    void SetTransform(int hPict, Transform transform)
    {
        FileSet[hPict]->transform = transform;
    }

    void SetPosition(int hPict, XMFLOAT3 pos)
    {
        FileSet[hPict]->transform.position_ = pos;
    }

    XMFLOAT3 GetPosition(int hPict)
    {
        return FileSet[hPict]->transform.position_;
    }

    std::string GetFileName(int hPict)
    {
        return FileSet[hPict]->FileName;
    }

    void Draw(int hPict)
    {
        FileSet[hPict]->pSprite->Draw(FileSet[hPict]->transform, FileSet[hPict]->Alpha);
    }

    bool IsHitCursor(int hPict)
    {
        UINT wid = (UINT)(FileSet[hPict]->pSprite->GetImgWidth() * FileSet[hPict]->transform.scale_.x / 2);
        UINT hgt = (UINT)(FileSet[hPict]->pSprite->GetImgHeight() * FileSet[hPict]->transform.scale_.y / 2);
        float Left = (FileSet[hPict]->transform.position_.x + 1) * (Direct3D::scrWidth / 2) - wid;
        float Right = (FileSet[hPict]->transform.position_.x + 1) * (Direct3D::scrWidth / 2) + wid;
        float Top = (-FileSet[hPict]->transform.position_.y + 1) * (Direct3D::scrHeight / 2) - hgt;
        float Bottom = (-FileSet[hPict]->transform.position_.y + 1) * (Direct3D::scrHeight / 2) + hgt;
        if (Left <= Input::GetMousePosition().x && Input::GetMousePosition().x <= Right &&
            Top <= Input::GetMousePosition().y && Input::GetMousePosition().y <= Bottom)
        {
            return true;
        }
        return false;
    }

    int IsHitCursorAny()
    {
        for (int i = 0; i < FileSet.size(); i++)
        {
            if (IsHitCursor(i))
                return i;
        }
        return -1;
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

    const Texture* GetpTexture(int hPict)
    {
        return FileSet[hPict]->pSprite->GetpTexture();
    }

    void AllAlterAlpha(float alpha)
    {
        for (int i = 0; i < FileSet.size(); i++)
        {
            FileSet[i]->Alpha = alpha;
        }
    }

    void CallStatus(int hPict)
    {
        //"Assets\\"を省いた文字列を取得
        std::string file = FileSet[hPict]->FileName.substr(7);
        int i = IniOperator::AddList("Assets\\ImageStatus.ini", file);
        FileSet[hPict]->transform.position_ = Math::PixelToTransform({
        (float)IniOperator::GetValue(i, "x", 0),
        (float)IniOperator::GetValue(i, "y", 0), 0 });
    }
}