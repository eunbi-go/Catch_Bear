#pragma once
#include "Component.h"
#include "Frustum.h"

enum class PROJECTION_TYPE
{
	PERSPECTIVE,	// 원근 투영
	ORTHOGRAPHIC,	// 직교 투영
};

class Camera : public Component
{
public:
	Camera();
	virtual ~Camera();

	virtual void FinalUpdate() override;
	
	void SetProjectionType(PROJECTION_TYPE type) { _type = type; }
	PROJECTION_TYPE GetProjectionType() { return _type; }

	void SortGameObject();
	void SortShadowObject();

	void Render_Deferred();
	void Render_Forward();
	void Render_Shadow();

	// 특정 비트 하나만 끄거나 키도록 하고 싶을 때 사용
	void SetCullingMaskLayerOnOff(uint8 layer, bool on)
	{
		if (on)
			_cullingMask |= (1 << layer);
		else
			_cullingMask &= ~(1 << layer);
	}

	void SetCullingMaskAll() { SetCullingMask(UINT32_MAX); }	// 아무것도 찍지 않도록 설정
	void SetCullingMask(uint32 mask) { _cullingMask = mask; }
	bool IsCulled(uint8 layer) { return (_cullingMask & (1 << layer)) != 0; }

	void SetNear(float value) { _near = value; }
	void SetFar(float value) { _far = value; }
	void SetFOV(float value) { _fov = value; }
	void SetScale(float value) { _scale = value; }
	void SetWidth(float value) { _width = value; }
	void SetHeight(float value) { _height = value; }

	Matrix& GetViewMatrix() { return _matView; }
	Matrix& GetProjectionMatrix() { return _matProjection; }
	



private:
	PROJECTION_TYPE _type = PROJECTION_TYPE::PERSPECTIVE;

	float	_near = 1.f;
	float	_far = 1000.f;
	float	_fov = XM_PI / 4.f;
	float	_scale = 1.f;
	float	_width = 0.f;
	float	_height = 0.f;

	Matrix	_matView = {};
	Matrix	_matProjection = {};

	Frustum	_frustum;
	uint32	_cullingMask = 0;	// 각 비트가 컬링해야 하는지 아닌지를 나타냄

private:
	// 내가 찍어야 하는 애들을 구분해서 벡터에 담아둠
	vector<shared_ptr<GameObject>>	_vecDeferred;
	vector<shared_ptr<GameObject>>	_vecForward;
	vector<shared_ptr<GameObject>>	_vecParticle;
	vector<shared_ptr<GameObject>>	_vecShadow;

public:
	// TEMP - PushData()에 넘겨줄 데이터(임시)
	static Matrix	S_MatView;
	static Matrix	S_MatProjection;

};

