#include "DXGI.h"
#include <cassert>

#pragma comment(lib,"dxgi.lib")

DXGI::~DXGI()
{
	if (dxgiFactory_)
	{
		// DXGI�t�@�N�g���[�̉��
		dxgiFactory_->Release();
		dxgiFactory_ = nullptr;
	}
	// �A�_�v�^�̉��
	if (dxgiAdapter_)
	{
		dxgiAdapter_->Release();
		dxgiAdapter_ = nullptr;
	}
}

[[nodiscard]] bool DXGI::setDisplayAdapter() noexcept
{
#if _DEBUG
	// �f�o�b�O���C���[���I��
	ID3D12Debug* debug;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug))))
	{
		debug->EnableDebugLayer();
	}

#endif

	// DXGI�t�@�N�g���[�̍쐬
	{
		UINT createFactoryFlags = 0;
#if _DEBUG
		createFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif	
		const auto hr = CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory_));
		if (FAILED(hr))
		{
			assert(false && "DXGI�t�@�N�g���[�̍쐬�Ɏ��s");
			return false;
		}
	}

	// �A�_�v�^�̎擾
	{
		// �A�_�v�^�̗�
		auto select = 0;
		IDXGIAdapter1* dxgiAdapter{};

		// �K�؂ȃA�_�v�^��I��
		while (dxgiFactory_->EnumAdapters1(select, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND)
		{
			DXGI_ADAPTER_DESC1 desc{};
			dxgiAdapter->GetDesc1(&desc);

			select++;

			// �\�t�g�E�F�A�A�_�v�^�͏��O
			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				dxgiAdapter->Release();
				continue;
			}

			// Direct3D12 �������Ȃ��ꍇ�����O
			if (FAILED(D3D12CreateDevice(dxgiAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
			{
				dxgiAdapter->Release();
				continue;
			}

			dxgiAdapter_ = dxgiAdapter;
			break;
		}

		if (!dxgiAdapter)
		{
			assert(false && "�A�_�v�^�̎擾�Ɏ��s");
			return false;
		}
	}

	return true;
}

[[nodiscard]] IDXGIFactory4* DXGI::factory() const noexcept
{
	if (!dxgiFactory_)
	{
		assert(false && "DXGI�t�@�N�g���[�����쐬�ł�");
		return nullptr;
	}

	return dxgiFactory_;
}

[[nodiscard]] IDXGIAdapter1* DXGI::displayAdapter() const noexcept
{
	if (!dxgiAdapter_)
	{
		assert(false && "�f�B�X�v���C�A�_�v�^�[�����쐬�ł��B");
		return nullptr;
	}

	return dxgiAdapter_;
}