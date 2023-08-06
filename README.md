# Catch_Bear
2022년도 졸업작품

[Catch Bear] 3인 개발  
실행 동영상: https://www.youtube.com/watch?v=l32nUlgvy5k&t=1s  

1) 개발 기간: 2022.01.01 ~ 2022.08.01  

2) 개발 환경: VisualStudio2019, C++, STL, DirectX12, IOCP, SourceTree, Trello  

3) 목표: DirectX12와 IOCP를 활용한 3인용 멀티 플레이 게임 개발  

4) 담당: 팀장  
FBX 애니메이션  
유니티와 바이너리 파일을 활용한 맵 오브젝트 로딩  
FSM을 활용한 플레이어 상태 설계  
인게임 UI  


관련 소스코드 위치  

- FBX 파일 기반 Key Frame Animation
  
  Engine / 08.Animation / 폴더 내 클래스들

  
- 바이너리 파일 입출력을 이용한 맵 오브젝트 로딩
  
  Engine / 02.Manager / SceneManager.cpp, SceneManager.h

  
- FSM 을 활용한 플레이어 상태 설계
  
  Engine / 01.Object / 00.Component / 02.Player / State / 폴더 내 클래스들

  
- 인스턴싱
  
  Engine / 02.Manager / InstancingManager.cpp, InstancingManager.h
  
  Engine / 00.Engine / InstancingBuffer.cpp, InstancingBuffer.h
  
  
- UI
  
  인게임 내 타이머
  
  Engine / 01.Object / 02.UI / 00.Timer/ 폴더 내 클래스
  

  인게임 내 아이템, 점수 관련 UI
  
  Engine / 01.Object / 02.UI / 01.Item /  폴더 내 클래스들,
  
  Engine / 02.Manager / ItemSlotManager.cpp, ItemSlotManager.h
  
  Engine / 02.Manager / ScoreUIManager.cpp, ScoreUIManager.h
  
  Engine / Score.cpp, Score.h
  
  Engine / StageScene.cpp, StageScene.h
  

  로그인 신, 로비 신
  
  Engine / Button.cpp, Button.h
  
  Engine / FontDevice.cpp, FontDevice.h
  
  Engine / LobbyScene.cpp, LobbyScene.h
  
  Engine / LoginScene.cpp, LoginScene.h
  
