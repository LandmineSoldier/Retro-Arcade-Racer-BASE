# Retro-Arcade-Racer-BASE
C언어로 고전 레이싱게임 베이스

<C_Racer.c 가 불필요한 요소를 다 없앤 코드입니다.>


#define Third_Run "여기에 곡 파일의 주소를 입력"

#define Third_Run "Write your song file address"



역슬래쉬는 두 번 입력 해주어야합니다.

backslash must be entered twice.

(NO)   C:\Users\user\Desktop\RaceGameSound\y2mate.com-8-Bit-Fantasy-Racing-Music_AKQPbjzMJvw.wav

(YES)  C:\\Users\\user\\Desktop\\RaceGameSound\\y2mate.com-8-Bit-Fantasy-Racing-Music_AKQPbjzMJvw.wav


하지만 wav 파일만 지원합니다.

But it only supports wav files.

만약 wav 파일이 없다면 main 안에 있는 'playSound();' 를 주석처리 해주세요

If there is no wav file, comment out 'playSound();'.
