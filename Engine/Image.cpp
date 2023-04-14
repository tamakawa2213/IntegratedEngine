#include "Image.h"
#include "Input.h"
#include "IniOperator.h"
#include "Math.h"
#include <filesystem>
#include <memory>

namespace
{
    struct ImageSet
    {
        std::unique_ptr<Sprite> pSprite;	//Spriteのポインタ
        Transform transform;		        //transformクラス
        std::string FileName;	            //ファイルの名前
        float Alpha;			            //画像の透明度

        ImageSet() : pSprite(nullptr), transform(), FileName(), Alpha(1.0f) {}
        ImageSet(const std::string& file) : pSprite(nullptr), transform(), FileName(file), Alpha(1.0f) {}

        bool operator ==(const ImageSet& fl) const { return (FileName == fl.FileName); }
        bool operator !=(const ImageSet& fl) const { return !(*this == fl); }
    };

    bool operator ==(const std::shared_ptr<ImageSet> fl, const std::shared_ptr<ImageSet> fr) { return (fl->FileName == fr->FileName); }
    bool operator !=(const std::shared_ptr<ImageSet> fl, const std::shared_ptr<ImageSet> fr) { return !(fl == fr); }

    std::vector<std::shared_ptr<ImageSet>> FileSet;      //Fbxの構造体の動的配列
}

namespace Image
{
    void CallStatus(int hPict); //iniファイルに保存された初期位置を呼び出す
}

namespace Image
{
    int Load(const std::string& filename)
    {
        std::shared_ptr<ImageSet> File = std::make_shared<ImageSet>(filename);

        wchar_t file[CHAR_MAX];
        size_t ret;
        mbstowcs_s(&ret, file, filename.c_str(), filename.length());

        //同じファイルを使っていないか検索
        if (auto itr = std::find(FileSet.begin(), FileSet.end(), File); itr != end(FileSet))
        {
            //同じ名前のファイルをすでにロードしていた場合
            return (int)std::distance(FileSet.begin(), itr);
        }
        
        //見つからなかった場合、新しくロードする
        File->pSprite = std::make_unique<Sprite>();
        if (FAILED(File->pSprite->Initialize(file))) //ロードに失敗した場合
        {
            return -1;
        }
        FileSet.push_back(std::move(File));
        CallStatus((int)FileSet.size() - 1);
        return (int)FileSet.size() - 1;
    }

    void SetTransform(int hPict, const Transform& transform)
    {
        FileSet[hPict]->transform = transform;
    }

    void SetPosition(int hPict, const XMFLOAT3& pos)
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
        float Left = (FileSet[hPict]->transform.position_.x + 1) * (Direct3D::scrWidth / 2.0f) - wid;
        float Right = (FileSet[hPict]->transform.position_.x + 1) * (Direct3D::scrWidth / 2.0f) + wid;
        float Top = (-FileSet[hPict]->transform.position_.y + 1) * (Direct3D::scrHeight / 2.0f) - hgt;
        float Bottom = (-FileSet[hPict]->transform.position_.y + 1) * (Direct3D::scrHeight / 2.0f) + hgt;
        if (Left <= Input::Mouse::GetPosition().x && Input::Mouse::GetPosition().x <= Right &&
            Top <= Input::Mouse::GetPosition().y && Input::Mouse::GetPosition().y <= Bottom)
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
        FileSet.clear();
    }

    const Texture* GetpTexture(int hPict)
    {
        return FileSet[hPict]->pSprite->GetpTexture();
    }

    void AllAlterAlpha(float alpha)
    {
        for (auto&& i : FileSet)
        {
            i->Alpha = alpha;
        }
    }

    void CallStatus(int hPict)
    {
        //"Assets\\"を省いた文字列を取得
        std::filesystem::path file = FileSet[hPict]->FileName;
        int i = IniOperator::AddList("Assets\\ImageStatus.ini", file.filename().string());

        //iniファイルで取得した値をtransform値に変換
        FileSet[hPict]->transform.position_ = Math::PixelToTransform({
        (float)IniOperator::GetValue(i, "x", 0),
        (float)IniOperator::GetValue(i, "y", 0), 0 });
    }
}