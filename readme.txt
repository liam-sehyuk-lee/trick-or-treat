1. Visual Studio 환경 설정
1-1. MSVC v141 빌드 도구 설치
 - Visual Studio Installer 실행
 - MSVC v141 - VS 2017 C++ x64/x86 빌드 도구를 선택하여 설치

1-2. 프로젝트 속성 설정
 - 플랫폼 도구 집합: 
	Visual Studio 2017 (v141)로 설정
 - Windows SDK 버전:
	<부모 또는 프로젝트 기본값에 상속>으로 설정


2. FMOD 프로젝트 속성 설정
2-1. C++ 설정
 - C++ → 일반 → 추가 포함 디렉터리
 - Debug / Release 구성:
	$(프로젝트 위치)\FMOD Studio API\api\core\inc

2-2. 링커 설정
 - 링커 → 일반 → 추가 라이브러리 디렉터리
 - Debug / Release 구성:
	$(프로젝트 위치)\FMOD Studio API\api\core\lib

2-3. 링커 입력 설정
 - 링커 → 입력 → 추가 종속성
 - Debug 구성:
	fmodL_vc.lib
 - Release 구성:
	fmod_vc.lib

2-4. 빌드 후 이벤트 설정
 - 구성 속성 → 빌드 이벤트 → 빌드 후 이벤트
 - Debug 구성:
	copy /Y "$(프로젝트 위치)\FMOD Studio API\api\core\lib\fmodL.dll" "$(OutDir)"
 - Release 구성:
	copy /Y "$(프로젝트 위치)\FMOD Studio API\api\core\lib\fmod.dll" "$(OutDir)"

2-5. 문자집합 설정 
 - 일반 → 문자집합
 - Release 구성:
	멀티바이트 문자 집합 사용