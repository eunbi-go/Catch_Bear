#pragma once

// 싱글톤
// 언제 어디서나 활용할 수 있도록
// 유일성 보장

class Scene;

enum
{
	MAX_LAYER = 32
};

class SceneManager
{
	DECLARE_SINGLE(SceneManager);

public:
	void Update();
	void Render();
	void LoadScene(wstring sceneName);

	void SetLayerName(uint8 index, const wstring& name);
	const wstring& IndexToLayerName(uint8 index) { return _layerNames[index]; }
	uint8 LayerNameToIndex(const wstring& name);

public:
	shared_ptr<Scene> GetActiveScene() { return _activeScene; }

private:
	shared_ptr<Scene> LoadTestScene();

private:
	shared_ptr<Scene> _activeScene;

	array<wstring, MAX_LAYER>	_layerNames;		// 레이어 이름, ex) 1번에 해당하는 레이어는 UI다
	map<wstring, uint8>			_layerIndex;		// 레이어의 인덱스 번호를 찾기 위해
};

