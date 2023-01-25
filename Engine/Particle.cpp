#include "Particle.h"
#include "Camera.h"

Particle::Particle(GameObject* parent)
{
}

Particle::~Particle()
{
}

void Particle::Initialize()
{
}

//�X�V
void Particle::Update()
{
    //�e�G�~�b�^�[�̏���
    EmitterUpdate();

    //�e�p�[�e�B�N���̏���
    ParticleUpdate();
}


//�������̃p�[�e�B�N���̍X�V
void Particle::ParticleUpdate()
{
    for (auto particle = particleList_.begin(); particle != particleList_.end();)
    {
        //�������s�����̂ŏ���
        if ((*particle)->life == 0)
        {
            (*particle)->pEmitter->particleNum--;
            delete (*particle);
            particle = particleList_.erase(particle);
        }

        //�܂������Ă�
        else
        {
            (*particle)->life--;
            (*particle)->now.position.x += (*particle)->delta.position.x;
            (*particle)->now.position.y += (*particle)->delta.position.y;
            (*particle)->now.position.z += (*particle)->delta.position.z;
            (*particle)->delta.position.x *= (*particle)->accel;
            (*particle)->delta.position.y *= (*particle)->accel;
            (*particle)->delta.position.z *= (*particle)->accel;
            (*particle)->delta.position.y -= (*particle)->gravity;


            (*particle)->now.scale.x *= (*particle)->delta.scale.x;
            (*particle)->now.scale.y *= (*particle)->delta.scale.y;
            (*particle)->now.color.x += (*particle)->delta.color.x; //R
            (*particle)->now.color.y += (*particle)->delta.color.y; //G
            (*particle)->now.color.z += (*particle)->delta.color.z; //B
            (*particle)->now.color.w += (*particle)->delta.color.w; //A

            particle++;
        }
    }
}


//�e�G�~�b�^�̍X�V
void Particle::EmitterUpdate()
{
    for (auto emitter = emitterList_.begin(); emitter != emitterList_.end();)
    {
        //��������ł�G�~�b�^�[
        if ((*emitter)->isDead)
        {
            //���̃G�~�b�^�[����o���p�[�e�B�N�����S�������Ă���G�~�b�^�[���폜
            if ((*emitter)->particleNum <= 0)
            {
                (*emitter)->pBillBoard->Release();
                delete ((*emitter)->pBillBoard);
                delete (*emitter);
                emitter = emitterList_.erase(emitter);
            }
            else
            {
                emitter++;
            }
        }

        //�܂������Ă�
        else
        {
            //�p�[�e�B�N���𔭐�������^�C�~���O�Ȃ�
            if ((*emitter)->data.delay == 0 || (*emitter)->frameCount % (*emitter)->data.delay == 0)
            {
                //�w�����
                for (int i = 0; i < (*emitter)->data.number; i++)
                {
                    //�p�[�e�B�N������
                    ParticleData* pParticle = new ParticleData;

                    //����
                    {
                        //�ʒu
                        pParticle->now.position = (*emitter)->data.position;
                        float dx = (float)((*emitter)->data.positionErr.x == 0 ? 0 : rand() % (int)((*emitter)->data.positionErr.x * 201) - ((*emitter)->data.positionErr.x * 100)) / 100.0f;
                        float dy = (float)((*emitter)->data.positionErr.y == 0 ? 0 : rand() % (int)((*emitter)->data.positionErr.y * 201) - ((*emitter)->data.positionErr.y * 100)) / 100.0f;
                        float dz = (float)((*emitter)->data.positionErr.z == 0 ? 0 : rand() % (int)((*emitter)->data.positionErr.z * 201) - ((*emitter)->data.positionErr.z * 100)) / 100.0f;
                        pParticle->now.position.x += dx;
                        pParticle->now.position.y += dy;
                        pParticle->now.position.z += dz;

                        //�F
                        pParticle->now.color = (*emitter)->data.color;
                        
                        //�g�嗦
                        dx = (float)((*emitter)->data.sizeErr.x == 0 ? 0 : rand() % (int)((*emitter)->data.sizeErr.x * 201) - ((*emitter)->data.sizeErr.x * 100)) / 100.0f + 1.0f;
                        dy = (float)((*emitter)->data.sizeErr.y == 0 ? 0 : rand() % (int)((*emitter)->data.sizeErr.y * 201) - ((*emitter)->data.sizeErr.y * 100)) / 100.0f + 1.0f;
                        pParticle->now.scale.x = (*emitter)->data.size.x * dx;
                        pParticle->now.scale.y = (*emitter)->data.size.y * dy;
                    }

                    //�ω���
                    {
                        //�ړ�����
                        XMVECTOR vecDir = XMLoadFloat3(&(*emitter)->data.dir);
                        float dx = (float)((*emitter)->data.dirErr.x == 0 ? 0 : rand() % (int)((*emitter)->data.dirErr.x * 201) - ((*emitter)->data.dirErr.x * 100)) / 100.0f;
                        float dy = (float)((*emitter)->data.dirErr.y == 0 ? 0 : rand() % (int)((*emitter)->data.dirErr.y * 201) - ((*emitter)->data.dirErr.y * 100)) / 100.0f;
                        float dz = (float)((*emitter)->data.dirErr.z == 0 ? 0 : rand() % (int)((*emitter)->data.dirErr.z * 201) - ((*emitter)->data.dirErr.z * 100)) / 100.0f;
                        XMMATRIX matRotX = XMMatrixRotationX(XMConvertToRadians(dx));
                        XMMATRIX matRotY = XMMatrixRotationY(XMConvertToRadians(dy));
                        XMMATRIX matRotZ = XMMatrixRotationZ(XMConvertToRadians(dz));
                        vecDir = XMVector3TransformCoord(vecDir, matRotX * matRotY * matRotZ);

                        float s = (float)((*emitter)->data.speedErr == 0 ? 0 : rand() % (int)((*emitter)->data.speedErr * 201) - ((*emitter)->data.speedErr * 100)) / 100.0f + 1.0f;
                        vecDir = XMVector3Normalize(vecDir) * ((*emitter)->data.speed * s);
                        XMStoreFloat3(&pParticle->delta.position, vecDir);

                        //�g�嗦
                        pParticle->delta.scale = (*emitter)->data.scale;

                        //�F
                        pParticle->delta.color = (*emitter)->data.deltaColor;
                    }

                    pParticle->life = (DWORD)(*emitter)->data.lifeTime;    //�c�����
                    pParticle->gravity = (*emitter)->data.gravity;  //�d��
                    pParticle->accel = (*emitter)->data.accel;      //����

                    pParticle->pEmitter = *emitter; //������

                    //����
                    particleList_.push_back(pParticle);

                    pParticle->pEmitter->particleNum++; //�������̃p�[�e�B�N�������J�E���g�A�b�v

                }
            }

            (*emitter)->frameCount++;


            //delay��0�̃G�~�b�^�[�͂P���o������폜
            if ((*emitter)->data.delay == 0)
            {
                (*emitter)->isDead = true;

            }

            emitter++;
        }
    }
}

//�p�[�e�B�N���`��
void Particle::Draw()
{
    Direct3D::SetShader(SHADER_BILLBOARD);
    Direct3D::SetBlendMode(BLEND_ADD);

 
    for (auto particle = particleList_.begin(); particle != particleList_.end(); particle++)
    {
        XMMATRIX matWorld;
        XMMATRIX matTrans = XMMatrixTranslation((*particle)->now.position.x, (*particle)->now.position.y, (*particle)->now.position.z);

        XMMATRIX matScale = XMMatrixScaling((*particle)->now.scale.x, (*particle)->now.scale.y, 1.0f);


        matWorld = matScale * Camera::GetBillboardMatrix() * matTrans;
        (*particle)->pEmitter->pBillBoard->Draw(matWorld,(*particle)->now.color);
    }

    
}

void Particle::Release()
{
}


//�G�~�b�^�̍쐬
int Particle::Start(EmitterData emitterData)
{
    int handle = 0;
    for (auto i = emitterList_.begin(); i != emitterList_.end(); i++)
    {
        handle++;
    }


    Emitter* pEmitter = new Emitter;

    pEmitter->data = emitterData;
    pEmitter->handle = handle;
    pEmitter->frameCount = 0;

    pEmitter->pBillBoard = new BillBoard;
    pEmitter->pBillBoard->Load(emitterData.textureFileName);

    emitterList_.push_back(pEmitter);

    return handle;
}

//�G�~�b�^�̍폜
void Particle::End(int handle)
{
    for (auto i = emitterList_.begin(); i != emitterList_.end(); i++)
    {
        if ((*i)->handle == handle)
        {
            (*i)->isDead = true;
            break;
        }
    }
}
