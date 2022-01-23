#include "pch.h"
#include "Material.h"
#include "Engine.h"

Material::Material() : Object(OBJECT_TYPE::MATERIAL)
{
}

Material::~Material()
{
}

void Material::PushData()
{
	// 연관된 모든 것들을 한번에 처리해줌

	// CBV 업로드
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::MATERIAL)->PushData(&_params, sizeof(_params));

	// SRV 업로드
	for (size_t i = 0; i < _textures.size(); i++)
	{
		if (!_textures[i]) continue;

		SRV_REGISTER reg = SRV_REGISTER(static_cast<int8>(SRV_REGISTER::t0) + i);
		GEngine->GetTableDescHeap()->SetSRV(_textures[i]->GetCpuHandle(), reg);
	}

	// 파이프라인 세팅
	_shader->Update();
}
