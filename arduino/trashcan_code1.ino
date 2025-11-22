# include <Wire.h>
# include <LiquidCrystal_I2C.h>
# define LOOP 12                     // 6 * 10 = 1분

float setFlameValue(void);
float setDepthValue(void);
void EMERGENCY (void);


                                     // 센서 //
const int fireSs = 9;                // 화재감지센서, 아날로그 설정 (pin 9)  
const int trigSs = 3;                // 초음파 출력,  디지털   설정 (pin1)   
const int echoSs = 2;                // 초음파 입력 , 디지털   설정 (pin2) 

                                     // 출력장치 //
const int piezo = A0;                // 피에조 부저,  아날로그  설정 (pin A0)
                                     //
LiquidCrystal_I2C lcd(0x27, 16, 2);  // 디스플레이, LiquidCrystal_I2C 클래스의 이름을 "lcd"로 함
                                     // 자동으로 A4,A5 핀으로 설정됨 -> pinMode 필요없음
                                     //--------------------------------------------------------


float setFlame = 0;                  // 전역변수들 //
float setDepth = 0;                  // 코드21,22는 정상상태 기준치
int   loopCount= 1;                  // loop함수의 실행횟수
int flameValue = 0;
int depthValue = 0;


void setup() 
{
  lcd.init();                        // A4, A5핀에 연결한 디스플레이 시작                
  lcd.backlight();                   // 화면 on
  lcd.setCursor(3,0);                // 디스플레이 커서 왼쪽에서 3번째 칸
  
    pinMode(fireSs, INPUT);          // 입력장치(센서들) 세팅
    pinMode(trigSs, OUTPUT);
    pinMode(echoSs, INPUT);

    pinMode (piezo, OUTPUT);         // 출력장치 세팅

    setFlame = setFlameValue() * 100.0;
    setDepth = setDepthValue();

  lcd.print("ProgramStart"); // 프로그램 시작
  delay(1000);
  lcd.clear();
  lcd.noBacklight();
}

void loop() 
{
      flameValue = analogRead(fireSs);
      flameValue = constrain(flameValue, 0, 1000);

      if(flameValue > setFlame)
        {
        EMERGENCY();
        }
  
  if(loopCount == LOOP)
    {
    depthValue = setDepthValue();

      if(depthValue < 7)
        {
          lcd.home();
          lcd.print("TRASHCAN IS FULL");
          lcd.backlight();
        }
        else if(depthValue >= 7)
          {
           lcd.clear();
           lcd.noBacklight();
          }
     loopCount = 1;
    }

  loopCount++;
  delay(5000);
}

float setFlameValue()
{
 int sectorFlame[10] = {0};
 int getFlame = 0;
 int flameMax = 0;
 int mode = 0;
 int count = 0;
while(count < 50)
 {
    getFlame = analogRead(fireSs)/100;

    if(getFlame >= 10) getFlame = 9;

    sectorFlame[getFlame]++;

    if(sectorFlame[getFlame] > flameMax)
      {
      flameMax = sectorFlame[getFlame];
      mode = getFlame;
      }
  count++;
  }
return mode;
}
float setDepthValue()
{
  float duration = 0;
  float cmSum = 0;
  float cm = 0;
  float mean = 0;
  int count = 0;

  while(count < 10)
    {
    digitalWrite(trigSs, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigSs, LOW);

    duration = pulseIn(echoSs, HIGH);
    cm = duration / 58.0;
    cmSum += cm;

    count++;
    delay(10);
    }
  mean = cmSum/count;
return mean;
}

void EMERGENCY ()
{
 while(flameValue > setFlame)
  {
  tone(piezo, 1700, 100);
  flameValue = analogRead(fireSs);
  delay(300);
  }
}
