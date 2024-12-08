1. Visual Studio 환경 설정

1-1. MSVC v141 빌드 도구 설치
Visual Studio Installer 실행
MSVC v141 - VS 2017 C++ x64/x86 빌드 도구를 선택하여 설치

1-2. 프로젝트 속성 설정
플랫폼 도구 집합: Visual Studio 2017 (v141)으로 설정
Windows SDK 버전: <부모 또는 프로젝트 기본값에 상속>으로 설정


2. FMOD Engine 설치
2-1. FMOD Engine 2.02.25 다운로드
설치 위치: C:\Program Files (x86)\FMOD SoundSystem\FMOD Studio API Windows
Components: Visual Studio Runtime 체크


3. 프로젝트 속성 설정
FMOD 라이브러리 및 헤더 파일을 프로젝트에 추가하려면 아래와 같은 설정을 해야 합니다.

3-1. 링커 설정
링커 → 일반 → 추가 라이브러리 디렉터리
경로 추가: C:\Program Files (x86)\FMOD SoundSystem\FMOD Studio API Windows\api\core\lib\x64

3-2. 링커 입력 설정
링커 → 입력 → 추가 종속성
추가할 라이브러리:
fmod_vc.lib    // Release 빌드 시
fmodL_vc.lib   // Debug 빌드 시

3-3. C++ 설정
C++ → 일반 → 추가 포함 디렉터리
경로 추가: C:\Program Files (x86)\FMOD SoundSystem\FMOD Studio API Windows\api\core\inc


4. Release 프로젝트 속성 설정
일반 → 문자집합
멀티바이트 문자 집합 사용