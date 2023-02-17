#pragma once
#include	<directxmath.h>

class GameObject {
	uint64_t m_id = 0;						// ���ʎq
protected:
	DirectX::XMFLOAT3 m_pos{};				// �ʒu
	DirectX::XMFLOAT4X4 m_mtx{};				// �p��
public:
	GameObject() {};
	virtual ~GameObject() {}
	virtual bool Init() = 0;
	virtual void Finalize() = 0;

	// �p�����擾
	DirectX::XMFLOAT4X4 GetMtx() {
		return m_mtx;
	}

	// �ʒu���擾
	DirectX::XMFLOAT3 GetPos() {
		return m_pos;
	}

	// �h�c���擾
	uint64_t GetID() {
		return m_id;
	}
};