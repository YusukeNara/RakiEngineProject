#include "DebugDrawer.h"

DebugDrawer* DebugDrawer::GetInstance()
{
    static DebugDrawer ins;

    return &ins;
}

void DebugDrawer::Init()
{
    //HRESULT result;

    //�V�F�[�_�[�R���p�C��
    ////���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
    //ComPtr<ID3DBlob> errorBlob = nullptr; //�G���[�I�u�W�F�N�g
    //result = D3DCompileFromFile(
    //    L"Resources/Shaders/SpriteVS.hlsl", //�V�F�[�_�[�t�@�C����
    //    nullptr,//�V�F�[�_�[�}�N���I�u�W�F�N�g�i����͎g��Ȃ��j
    //    D3D_COMPILE_STANDARD_FILE_INCLUDE, //�C���N���[�h�I�u�W�F�N�g�i�C���N���[�h�\�ɂ���j
    //    "main", "vs_5_0", //�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
    //    D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�O�p�ݒ�
    //    0,
    //    &line_VS, &errorBlob
    //);
    ////�V�F�[�_�[�̃G���[���e��\��
    //if (FAILED(result))
    //{
    //    //errorBlob����G���[���e��string�^�ɃR�s�[
    //    std::string errstr;
    //    errstr.resize(errorBlob->GetBufferSize());

    //    std::copy_n((char*)errorBlob->GetBufferPointer(),
    //        errorBlob->GetBufferSize(),
    //        errstr.begin());
    //    errstr += "\n";
    //    //�G���[���e���o�̓E�C���h�E�ɕ\��
    //    OutputDebugStringA(errstr.c_str());
    //    exit(1);
    //}

    //�p�C�v���C������










}

void DebugDrawer::Rendering()
{
}

void DebugDrawer::DrawLine(RVector3 pos1, RVector3 pos2)
{
}
