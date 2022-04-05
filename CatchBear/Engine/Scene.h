#pragma once

class GameObject;

class Scene
{
public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();

	shared_ptr<class Camera> GetMainCamera();

	void Render();

	void ClearRTV();

	void RenderShadow();
	void RenderDeferred();
	void RenderLights();
	void RenderFinal();

	void RenderForward();

private:
	void PushLightData();

public:
	void AddGameObject(shared_ptr<GameObject> gameObject);
	void AddPlayers(uint64 _playerid, shared_ptr<GameObject> gameObject);
	void RemoveGameObject(shared_ptr<GameObject> gameObject);

	const vector<shared_ptr<GameObject>>& GetGameObjects() { return _gameObjects; }
	const shared_ptr<GameObject>& GetPlayer(uint64 playerid) { return _players[playerid]; }

	vector<shared_ptr<class Light>>	GetLights() { return _lights; }

private:
	// 벡터를 레이어 단계로 구분한다
	// [0]: 벽, [1]: 몬스터
	// -> 벡터 하나를 만들어주는 것이 아니라 벡터의 배열 or 벡터의 벡터로 만들어서 구분해서 관리
	vector<shared_ptr<GameObject>>		_gameObjects;
	vector<shared_ptr<class Camera>>	_cameras;
	vector<shared_ptr<class Light>>		_lights;
	map<uint64, shared_ptr<GameObject>> _players;
};

