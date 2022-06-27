#include "pch.h"
#include "ScoreUIManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Texture.h"
#include "MeshRenderer.h"
#include "Resources.h"

void ScoreUIManager::SetPlayerScoreTexture(int nPlayer, int nScore)
{
	wstring scoreUIName1 = L"Player" + to_wstring(nPlayer + 1) + L"Score1";
	wstring scoreUIName2 = L"Player" + to_wstring(nPlayer + 1) + L"Score2";
	wstring scoreUIName3 = L"Player" + to_wstring(nPlayer + 1) + L"Score3";
	
	shared_ptr<GameObject> mScore1 = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObject(scoreUIName1);
	shared_ptr<GameObject> mScore2 = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObject(scoreUIName2);
	shared_ptr<GameObject> mScore3 = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObject(scoreUIName3);

	int score = nScore;
	int temp = nScore % 100;

	int score1 = nScore / 100;
	int score2 = temp / 10;
	int score3 = temp % 10;

	shared_ptr<Texture> texScore1, texScore2, texScore3;
	texScore1 = GET_SINGLE(Resources)->Load<Texture>(L"score" + to_wstring(score1), L"..\\Resources\\Texture\\score\\score" + to_wstring(score1) + L".png");
	texScore2 = GET_SINGLE(Resources)->Load<Texture>(L"score" + to_wstring(score2), L"..\\Resources\\Texture\\score\\score" + to_wstring(score2) + L".png");
	texScore3 = GET_SINGLE(Resources)->Load<Texture>(L"score" + to_wstring(score3), L"..\\Resources\\Texture\\score\\score" + to_wstring(score3) + L".png");

	mScore1->GetMeshRenderer()->GetMaterial()->SetTexture(0, texScore1);
	mScore2->GetMeshRenderer()->GetMaterial()->SetTexture(0, texScore2);
	mScore3->GetMeshRenderer()->GetMaterial()->SetTexture(0, texScore3);
}
