#pragma once

//RELEASE�����l�Ƀ}�N���쐬
#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

//HRESULT�����s�����Ƃ���Release�Ȃ��ŌĂяo���}�N��
#define HR_FAILED(hr, text) if(FAILED(hr)) [[unlikely]] {MessageBox(NULL, text, L"Error", MB_OKCANCEL);return hr; }
//HRESULT�����s�����Ƃ���Release����ŌĂяo���}�N��
#define HR_FAILED_RELEASE(hr, text, p) if(FAILED(hr)) [[unlikely]] {MessageBox(NULL, text, L"Error", MB_OKCANCEL); SAFE_RELEASE(p);return hr; }

#define SET_UI(UINAME) RootUI* pRootUI = (RootUI*)FindObject("RootUI"); pRootUI->Link<UINAME>();

//�V�[���؂�ւ����s���}�N��
#define SCENE_CHANGE(SCENE_ID) SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager"); pSceneManager->ChangeScene(SCENE_ID);

//���f�������[�h�������
//���� : �i�[���������f���ԍ� �t�@�C����
#define MLoad(hModel, filename) hModel = Model::Load(filename); assert(hModel >= 0);

//�摜�����[�h�������
//���� : �i�[�������摜�ԍ� �t�@�C����
#define ILoad(hPict, filename) hPict = Image::Load(filename); assert(hPict >= 0);

//XMFLOAT3�̗v�f���܂Ƃ߂Ĕ��]������
//���� : �t�]���XMFLOAT3 ���ƂȂ�XMFLOAT3
#define RE_XMFLOAT3(Rev, Orig) Rev = {-Orig.x, -Orig.y, -Orig.z};