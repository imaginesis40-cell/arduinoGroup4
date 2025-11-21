# include <Wire.h>
# include <LiquidCrystal_I2C.h>
# define LOOP 6                      // 6 * 10 = 1분

float setFlameVlaue(void);
float setDepthVlaue(void);

                                     // 센서 //
const int fireSs = 9;                // 화재감지센서, 아날로그 설정 (pin 9)  
const int trigSs = 1;                // 초음파 출력,  디지털   설정 (pin1)   
const int echoSs = 2;                // 초음파 입력 , 디지털   설정 (pin2) 

float setFlame = 0;
float setDepth = 0;

                                     // 출력장치 //
const int piezo = A0;                // 피에조 부저,  아날로그  설정 (pin A0)
                                     //
LiquidCrystal_I2C lcd(0x27, 16, 2);  // 디스플레이, LiquidCrystal_I2C 클래스의 이름을 "lcd"로 함
                                     // 자동으로 A4,A5 핀으로 설정됨 -> pinMode 필요없음
                                     //--------------------------------------------------------

                                     
void setup() 
{
  lcd.init();                        // A4, A5핀에 연결한 디스플레이 시작                
  lcd.backlight();                   // 화면 on
  lcd.setCursor(3,0);                // 디스플레이 커서 왼쪽에서 3번째 칸
  
    pinMode(fireSs, INPUT_PULLUP);   // 입력장치(센서들) 세팅
    pinMode(trigSs, OUTPUT);
    pinMode(echoSs, INPUT);

    pinMode (piezo, OUTPUT);         // 출력장치 세팅


    setFlameVlaue();
    setDepthVlaue();

  lcd.print("smart.trashcan.start"); // 프로그램 시작
  delay(1500);
  lcd.noBacklight();
}

void loop() 
{
  
}

float setFlameVlaue()
{
  float flameMax = 0;
  float flameMin = 1023;

  while(millis() <= 5)
    {
      setFlame = analogRead(fireSs);

      if(flameMax > setFlame)          
        flameMax = setFlame;
      if(flameMin < setFlame)          // 내부 풀업 저항 -> 부등호 반대
        flameMin = setFlame;  
      delay(10);
    }
    setFlame = (flameMax + flameMin)/2;

    return setFlame;
}
float setDepthVlaue()
{

}
