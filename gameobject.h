#pragma once
#include	<directxmath.h>

class GameObject {
	uint64_t m_id = 0;						// ¯•Êq
protected:
	DirectX::XMFLOAT3 m_pos{};				// ˆÊ’u
	DirectX::XMFLOAT4X4 m_mtx{};				// p¨
public:
	GameObject() {};
	virtual ~GameObject() {}
	virtual bool Init() = 0;
	virtual void Finalize() = 0;

	// p¨‚ğæ“¾
	DirectX::XMFLOAT4X4 GetMtx() {
		return m_mtx;
	}

	// ˆÊ’u‚ğæ“¾
	DirectX::XMFLOAT3 GetPos() {
		return m_pos;
	}

	// ‚h‚c‚ğæ“¾
	uint64_t GetID() {
		return m_id;
	}
};