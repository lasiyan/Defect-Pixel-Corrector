# Defect-Pixel-Corrector

Defect Pixel Correction based on median Filter processing. This Application made by Visual Studio 2008(VC9) using C++ and MFC

해당 애플리케이션은 Bitmap 이미지에 발생한 Defect Pixel(불량화소)를 검출하기 위한 애플리케이션이다. 해당 애플리케이션은 MFC 기반 프로젝트로 구성되었으며, 발생한 불량화소를 Median Filter를 통해 처리하고 탐지 결과를 List Dialog를 통해 출력 후 수정된 파일을 Bitmap 이미지로 저장한다.

## 개발 환경

* OS : Windows 10 Pro 64bit
* CPU : Intel(R) Core(TM) i7-6500U CPU @ 2.50GHz 2.59 GHz
* RAM : 8.00GB
* IDE : Microsoft Visual Studio 2008 Version 9.0.30729.1 SP

## 사용 기술

C++, Window API, STL Vector, MFC, Image Processing, Median Filter

## 부가 설명

먼저 프로그램을 실행하면 하고 불량 화소가 발생한 이미지를 불러온다(Bitamp Type). 해당 이미지를 불러오면 이미지에 대한 정보(가로, 세로, 픽셀 비트)를 기반으로 생성될 Output List가 구성된다. 이는 Gray Image의 경우 Gray Image로, True Color 이미지의 경우 Gray 또는 24bit RGB로 생성되며 사용자의 선택에 따라 생성 결과물이 달라진다. 또한 슬라이드바를 통해 Hot Pixel과 Dead Pixel의 임계값을 설정하고 해당 값에 따라 Defetc Pixel을 검출한다. 이는 이미지 전반에 대한 Median Filter 적용에 따른 과도한 리소스 낭비를 방지하기 위함이다.

실행을 위한 설정을 모두 마친 후 Defect Pixel Correction 버튼을 누르면 처리가 실행된다. 먼저 파일 경로와 이미지 가로 및 세로, 픽셀 당 비트수를 기반으로 CMyImage 클래스 객체 pFile을 생성하고 해당 객체와 출력 이미지 타입, Hot Pixel 임계 값, Dead Pixel 임계 값을 CIpActivity 클래스로 전달한다. 해당 클래스는 프로그램의 파일에 대한 처리가 수행되는 클래스로 파일의 읽기와 쓰기를 담당한다. 프로그램의 핵심 로직을 담당하는 Defect Pixel Correction은 CPixelProcessing 클래스에서 처리된다.

해당 클래스는 먼저 pFile의 정보와 출력 이미지 타입을 기반으로 pResult 객체를 생성하고 해당 객체에 Defect Pixel이 수정된 픽셀 데이터 값을 저장한다. 수정은 RGB 타입과 Gray 타입, 그리고 출력 타입에 따라 총 3가지 경우로 실행되고 전반적인 로직은 픽셀의 현재 값을 가져와 해당 값이 일정 임계치(Hot Pixel, Dead Pixel 설정 값) 이하거나 또는 이상일 경우 이를 Defect Pixel로 간주하고 Median Filter를 적용하여 해당 픽셀을 필터의 중간 값으로 설정하고 이를 글로벌 벡터 g_cor에 저장한다.

수행이 종료되면 처리된 픽셀 데이터들을 Bitmap 이미지 형태로 저장하고 Defect Pixel이라 간주된 픽셀의 좌표 값들을 Pop up 형식으로 리스트화 하여 출력하며 로직이 종료된다. 한 가지 주의할 사항은 Hot Pixel과 Dead Pixel의 Threshold 값이 너무 넓은 범위를 가질 경우 기존 이미지가 단일 이미지가 아닌 다양한 색상 값을 가지는 이미지라면 검출에 긴 시간이 소요되며 정상적인 Pixel을 Defect으로 간주할 수 있다.

## 실행 과정

1. 솔루션 생성 및 Source, Header Files Include
2. 솔루션 빌드 및 실행
3. Bmp 파일 선택 후 Hot 및 Dead Pixel의 임계 값 설정
4. 출력 이미지 타입 선택
4. Defect Pixel Correction 버튼을 통한 실행

## 실행 결과

추후 업로드 예정입니다.

(클릭 시 유튜브로 이동합니다)
