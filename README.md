# Retro-Arcade-Racer-BASE
C언어로 고전 레이싱게임 베이스

<C_Racer.c 가 불필요한 요소를 다 없앤 코드입니다.>


#define runSound "여기에 곡 파일의 주소를 입력"

#define runSound "Write your song file address"



역슬래쉬는 두 번 입력 해주어야합니다.

backslash must be entered twice.

(NO)   "./Sound\8-Bit-Fantasy-Racing-Music.wav"

(YES)  "./Sound\\8-Bit-Fantasy-Racing-Music.wav"


wav 파일만 지원합니다.

only supports wav files.

만약 wav 파일이 없다면 main 안에 있는 'playSound();' 를 주석처리 해주세요

If there is no wav file, comment out 'playSound();'.
