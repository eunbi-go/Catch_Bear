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

	shared_ptr<class GameObject> Pick(int32 screenX, int32 screenY);

public:
	shared_ptr<Scene> GetActiveScene() { return _activeScene; }


public:
	void MakePlayer(uint64 _playerID);


private:
	shared_ptr<Scene> LoadTestScene();


private:
	void LoadMapFile(shared_ptr<Scene> scene);
	void LoadMap(shared_ptr<Scene> scene);



private:
	shared_ptr<Scene> _activeScene;

	array<wstring, MAX_LAYER>	_layerNames;		// 레이어 이름, ex) 1번에 해당하는 레이어는 UI다
	map<wstring, uint8>			_layerIndex;		// 레이어의 인덱스 번호를 찾기 위해
	bool						bisPlayerCreate = false;

	// Test용, main에 병합하고 따로 브랜치 생성해서 수정할 것 
	vector<Vec3>	_transform;
	vector<Vec3>	_scale;
	vector<Vec3>	_rotate;
	vector<wstring>	_objectName;
};

