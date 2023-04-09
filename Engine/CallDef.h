#pragma once

//RELEASEも同様にマクロ作成
#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

//HRESULTが失敗したときにReleaseなしで呼び出すマクロ
#define HR_FAILED(hr, text) if(FAILED(hr)) [[unlikely]] {MessageBox(NULL, text, L"Error", MB_OKCANCEL);return hr; }
//HRESULTが失敗したときにReleaseありで呼び出すマクロ
#define HR_FAILED_RELEASE(hr, text, p) if(FAILED(hr)) [[unlikely]] {MessageBox(NULL, text, L"Error", MB_OKCANCEL); SAFE_RELEASE(p);return hr; }

#define SET_UI(UINAME) RootUI* pRootUI = (RootUI*)FindObject("RootUI"); pRootUI->Link<UINAME>();

//シーン切り替えを行うマクロ
#define SCENE_CHANGE(SCENE_ID) SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager"); pSceneManager->ChangeScene(SCENE_ID);

//モデルをロードするもの
//引数 : 格納したいモデル番号 ファイル名
#define MLoad(hModel, filename) hModel = Model::Load(filename); assert(hModel >= 0);

//画像をロードするもの
//引数 : 格納したい画像番号 ファイル名
#define ILoad(hPict, filename) hPict = Image::Load(filename); assert(hPict >= 0);

//XMFLOAT3の要素をまとめて反転させる
//引数 : 逆転後のXMFLOAT3 元となるXMFLOAT3
#define RE_XMFLOAT3(Rev, Orig) Rev = {-Orig.x, -Orig.y, -Orig.z};