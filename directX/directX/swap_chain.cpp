#include "swap_chain.h"

#include <cassert>

//---------------------------------------------------------------------------------
/**
 * @brief    �f�X�g���N�^
 */
SwapChain::~SwapChain() {
    if (swapChain_) {
        swapChain_->Release();
        swapChain_ = nullptr;
    }
}

//---------------------------------------------------------------------------------
/**
 * @brief	�X���b�v�`�F�C���̐���
 * @param	dxgi			dxgi �N���X�̃C���X�^���X
 * @param	window			�E�B���h�E�N���X�̃C���X�^���X
 * @param	commandQueue	�R�}���h�L���[�N���X�̃C���X�^���X
 * @return	�����̐���
 */
[[nodiscard]] bool SwapChain::create(const DXGI& dxgi, const Window& window, const CommandQueue& commandQueue) noexcept {
    // �E�B���h�E�T�C�Y���擾
    const auto [w, h] = window.size();

    DXGI_SWAP_CHAIN_DESC1 desc{};
    desc.BufferCount = 2;                                // �o�b�N�o�b�t�@�̐��i�_�u���o�b�t�@�j
    desc.Width = w;                                // �o�b�N�o�b�t�@�̉���
    desc.Height = h;                                // �o�b�N�o�b�t�@�̏c��
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;       // �o�b�N�o�b�t�@�̃t�H�[�}�b�g
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // �����_�[�^�[�Q�b�g�Ƃ��Ďg�p
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;    // ���t���[����ʍX�V����̂ŕ`�悪�I�������o�b�t�@��j��
    desc.SampleDesc.Count = 1;                                // �}���`�T���v�����O�Ȃ�

    // �ꎞ�I�ȃX���b�v�`�F�C���̍쐬
    // �X���b�v�`�F�C���̃A�b�v�O���[�h���K�v�ɂȂ�
    IDXGISwapChain1* tempSwapChain{};
    {
        const auto hr = dxgi.factory()->CreateSwapChainForHwnd(commandQueue.get(), window.handle(),
            &desc, nullptr, nullptr, &tempSwapChain);
        if (FAILED(hr)) {
            assert(false && "�X���b�v�`�F�C���̍쐬�Ɏ��s");
            return false;
        }
    }

    // �X���b�v�`�F�C���̃o�[�W�������A�b�v�O���[�h
    {
        // �ꎞ�I�ȃX���b�v�`�F�C���� IDXGISwapChain3 �ɕϊ�
        const auto hr = tempSwapChain->QueryInterface(IID_PPV_ARGS(&swapChain_));
        // �ꎞ�I�ȃX���b�v�`�F�C���͉��
        tempSwapChain->Release();

        if (FAILED(hr)) {
            assert(false && "�X���b�v�`�F�C���̃A�b�v�O���[�h�Ɏ��s");
            return false;
        }
    }

    return true;
}

//---------------------------------------------------------------------------------
/**
 * @brief	�X���b�v�`�F�C�����擾����
 * @return	�X���b�v�`�F�C���̃|�C���^
 */
[[nodiscard]] IDXGISwapChain* SwapChain::get() const noexcept {
    if (!swapChain_) {
        assert(false && "�X���b�v�`�F�C�������쐬�ł�");
        return nullptr;
    }
    return swapChain_;
}