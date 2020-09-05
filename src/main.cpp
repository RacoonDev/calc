
//вапо аущшквом щзшуакртмолртцывмоауыволтимжывмолиывмьтывжд цаюэжцуатцушолтцу
#include <math.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Keypad.h>
#include <Arduino.h>
#define R_Button 12
#define L_Button 11
#define LED 13
LiquidCrystal_I2C lcd(0x27, 16, 2);

const byte ROWS = 4;
const byte COLS = 4;
byte rowPins[ROWS] = {A0, A1, A2, A3};
byte colPins[COLS] = {5, 4, 3, 2};
char hexaKeys[ROWS][COLS] = {
    {'.', '0', '=', '/'},
    {'1', '2', '3', '*'},
    {'4', '5', '6', '-'},
    {'7', '8', '9', '+'}};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
bool dsp, bt_flag1, bt_flag2, isVal1, isVal2, isSign, isResult, timer;
String entered_pass;
int mode, HH, MM, TimerH, TimerM, TimerS, timeSecs, bt_count, mode_count, Hour, Minutes, Less;
String sVal1, sVal2;
char cSign, prevKey;
float fResult;
unsigned long period_time = (long)5 * 24 * 60 * 60 * 1000;
unsigned long my_timer;

void BT1()
{
  bool bt1 = digitalRead(R_Button);
  if (bt1 == 1 && bt_flag1 == 0)
  {
    dsp = !dsp;
    bt_flag1 = 1;
    delay(100);
  }
  if (bt1 == 0 && bt_flag1 == 1)
  {
    bt_flag1 = 0;
    lcd.noBacklight();
  }
}
void BT2()
{
  bool bt2 = digitalRead(L_Button);
  if (bt2 == 1 && bt_flag2 == 0)
  {
    bt_flag2 = 1;
    mode++;
    delay(100);
  }
  if (bt2 == 0 && bt_flag2 == 1)
  {
    bt_flag2 = 0;
  }
}
void Off()
{
  lcd.noBacklight();
  digitalWrite(LED, 0);
  isVal1 = 0;
  isVal2 = 0;
  isSign = 0;
  isResult = 0;
  timer = 0;
}
void On()
{
  lcd.backlight();
  digitalWrite(LED, 1);
  BT2();
}
void Power()
{
  if (dsp == 1)
  {
    On();
  }
  else
  {
    Off();
  }
}
void printAll()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  if (isResult)
  {
    lcd.print(String(fResult));
  }
  else
  {
    if (isSign)
    {
      if (sVal2 != 0)
      {
        lcd.print(String(sVal2));
      }
    }
    else
    {
      if (sVal1 != 0)
      {
        lcd.print(String(sVal1));
      }
    }
  }
  lcd.setCursor(0, 1);
  lcd.print(cSign);
}

void E_printAll()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  if (isResult)
  {
    lcd.print(String(fResult));
  }
  else
  {
    if (isSign)
    {
      if (sVal2 != 0)
      {
        lcd.print(String(sVal2));
      }
    }
    else
    {
      if (sVal1 != 0)
      {
        lcd.print(String(sVal1));
      }
    }
  }
  lcd.setCursor(0, 1);
  switch (cSign)
  {
  case '+':
    lcd.print("sqrt");
    break;
  case '-':
    lcd.print("pow^2");
    break;
  }
}

void clearAll()
{
  lcd.clear();
  fResult = 0;
  cSign = ' ';
  prevKey = ' ';
  sVal1 = "";
  isVal1 = 1;
  sVal2 = "";
  isVal2 = 0;
  isSign = 0;
  isResult = 0;
}
void Print(int nomer, String f_str, String s_str)
{
  if (bt_count == nomer)
  {
    lcd.setCursor(0, 0);
    lcd.print(f_str);
    lcd.setCursor(0, 1);
    lcd.print(s_str);
    delay(100);
    lcd.clear();
  }
}
void Border(int min, int max)
{
  if (bt_count < min)
  {
    bt_count = 0;
  }
  if (bt_count > max)
  {
    bt_count = 0;
  }
}
void keypad()
{
  if (dsp == 1)
  {
    char Key = customKeypad.getKey();
    switch (Key)
    {
    case '.':
      bt_count--;
      break;
    case '/':
      bt_count++;
      break;
    case '0':
      bt_count -= 5;
      break;
    case '=':
      bt_count += 5;
      break;
    case '8':
      mode_count++;
      break;
    case '5':
      mode_count--;
      break;
    case '+':
      bt_count = 0;
      break;
    }
  }
}
void keypad_ecalc()
{
  if (dsp == 1)
  {
    char Key = customKeypad.getKey();
    switch (Key)
    {
    case '8':
      mode_count++;
      break;
    case '5':
      mode_count--;
      break;
    }
  }
}
void keypadTime()
{
  if (dsp == 1)
  {
    char Key = customKeypad.getKey();
    switch (Key)
    {
    case '-':
      HH += 5;
      break;
    case '*':
      HH -= 5;
      break;
    case '4':
      MM += 10;
      break;
    case '1':
      MM -= 10;
      break;
    case '=':
      HH++;
      break;
    case '0':
      HH--;
      break;
    case '/':
      MM++;
      break;
    case '.':
      MM--;
      break;
    case '5':
      mode_count--;
      break;
    case '8':
      mode_count++;
      break;
    }
  }
}
void keypadTimer()
{
  if (dsp == 1)
  {
    char Key = customKeypad.getKey();
    switch (Key)
    {
    case '+':
      timer = 1;
      break;
    case '7':
      timer = 0;
      break;
    case '/':
      TimerH++;
      break;
    case '=':
      TimerH--;
      break;
    case '.':
      TimerM++;
      break;
    case '0':
      TimerM--;
      break;
    case '8':
      mode_count++;
      break;
    case '5':
      mode_count--;
      break;
    case '9':
      TimerS++;
      break;
    case '6':
      TimerS--;
      break;
    }
  }
}
void bells()
{
  keypad();
  Border(0, 6);
  Print(0, "Raspisanie", "Zvonkov");
  Print(1, "1 ypok", "8:00-8:45");
  Print(2, "2 ypok", "9:05-9:50");
  Print(3, "3 ypok", "10:00-10:45");
  Print(4, "4 ypok", "10:55-11:40");
  Print(5, "5 ypok", "11:50-12:35");
  Print(6, "6 ypok", "12:55-13:40");
}
void calc()
{
  lcd.setCursor(15, 0);
  lcd.print("U");
  char customKey = customKeypad.getKey();
  if (customKey)
  {
    if (customKey == '+' || customKey == '-' || customKey == '*' || customKey == '/')
    {
      cSign = customKey;
      isSign = 1;
      isVal2 = 1;
    }
    else if (customKey == '.' && prevKey == '.')
    {
      clearAll();
    }
    else if (customKey == '=')
    {
      if (isVal1 && isVal2 && isSign)
      {
        if (cSign == '+')
        {
          fResult = sVal1.toFloat() + sVal2.toFloat();
        }
        else if (cSign == '-')
        {
          fResult = sVal1.toFloat() - sVal2.toFloat();
        }
        else if (cSign == '*')
        {
          fResult = sVal1.toFloat() * sVal2.toFloat();
        }
        else if (cSign == '/')
        {
          fResult = sVal1.toFloat() / sVal2.toFloat();
        }
        else
        {
          fResult = 0;
        }
        cSign = '=';
        isResult = 1;
      }
    }
    else
    {
      if (!isResult)
      {
        if (isVal2)
        {
          sVal2 = sVal2 + String(customKey);
        }
        else
        {
          if (isVal1)
          {
            sVal1 = sVal1 + String(customKey);
          }
        }
      }
    }
    printAll();
    prevKey = customKey;
  }
}
void e_calc()
{
  lcd.setCursor(15, 0);
  lcd.print("E");
  char customKey = customKeypad.getKey();
  if (customKey)
  {
    if (customKey == '+' || customKey == '-' || customKey == '*' || customKey == '/')
    {
      cSign = customKey;
      isSign = 1;
      isVal2 = 1;
    }
    else if (customKey == '.' && prevKey == '.')
    {
      clearAll();
    }
    else if (customKey == '=')
    {
      if (isVal1 && isSign)
      {
        if (cSign == '+')
        {
          fResult = sqrt(sVal1.toFloat());
        }

        if (cSign == '-')
        {
          fResult = pow(sVal1.toFloat(), 2);
        }

        else
        {
          fResult = 0;
        }
        cSign = '=';
        isResult = 1;
      }
    }
    else
    {
      if (!isResult)
      {
        if (isVal2)
        {
          sVal2 = sVal2 + String(customKey);
        }
        else
        {
          if (isVal1)
          {
            sVal1 = sVal1 + String(customKey);
          }
        }
      }
    }
    E_printAll();
    prevKey = customKey;
  }
}
void CheckValTimer()
{
  if (TimerM < 0)
  {
    TimerM = 0;
  }
  if (TimerM > 60)
  {
    TimerM = 60;
  }
  if (TimerH < 0)
  {
    TimerH = 0;
  }
  if (TimerH > 24)
  {
    TimerH = 24;
  }
  if (TimerS < 0)
  {
    TimerS = 0;
  }
  if (TimerS > 60)
  {
    TimerS = 60;
  }
}
void Help_Math()
{
  keypad();
  Border(0, 16);
  Print(0, "Math", "");
  Print(1, "(a+b)^2=", "a^2+2ab+b^2");
  Print(2, "(a-b)^2=", "a^2-2ab+b^2");
  Print(3, "a^2-b^2=", "(a+b)*(a-b)");
  Print(4, "a^3+b^3= (a+b)", "(a^2+b^2-ab)");
  Print(5, "a^3-b^3= (a-b)", "(a^2+b^2+ab)");
  Print(6, "(a+b)^3= a^3+", "3ba^3+3ab^3+b^3");
  Print(7, "(a-b)^3= a^3-", "3ba^3+3ab^3-b^3");
  Print(8, "Cila", "AxMeDa");
  Print(9, "b(n)", "b(n)-1*q");
  Print(10, "log(a)b=c", "A^c=b");
  Print(11, "log(a)((b*c))=", "log(a)b+log(a)c");
  Print(12, "log(a)((b:c))=", "log(a)b-log(a)c");
  Print(13, "log(a)b^n=", "n*log(a)");
  Print(14, "log(a^m)b=", "1/m*log(a)b");
  Print(15, "log(a)b", "1/log(b)a");
  Print(16, "log(a)b", "log(c)b/log(c)a");
}
void Help_Phys()
{
  keypad();
  Border(0, 17);
  Print(0, "Physics", "");
  Print(1, "Skorost", "V=S/t");
  Print(2, "Sila tyajesti", "F=m*g");
  Print(3, "Ves tela", "P=m*g");
  Print(4, "Sila arhimeda", "p(j)*g*Vm");
  Print(5, "Moshnost", "N=A/t");
  Print(6, "Davlenie", "p=F/S");
  Print(7, "Ravnom Dvijenie", "N=F*v");
  Print(8, "Mechanich Rabota", "A=F*S");
  Print(9, "Kinetich Enrg", "E(k)=(m*v^2)/2");
  Print(10, "Moment silb|", "M=F*I");
  Print(11, "Zakon OMA", "I=U/R");
  Print(12, "Napryajenie", "U=A/q");
  Print(13, "Plotnost", "p=m/V");
  Print(14, "Potencial Enrg", "E=m*g*h(kx^2/2)");
  Print(15, "Gidravl press", "F1/F2=S1/S2");
  Print(16, "Sila uprugosti", "F(upr)=k*D(I)");
  Print(17, "Enotishe", "sila");
}

void less(String No)
{
  lcd.setCursor(14, 1);
  lcd.print(No);
}
void pl()
{
  switch (Less)
  {
  case 1:
    less("L1");
    break;
  case 2:
    less("L2");
    break;
  case 3:
    less("L3");
    break;
  case 4:
    less("L4");
    break;
  case 5:
    less("L5");
    break;
  case 6:
    less("L6");
    break;
  case 7:
    less("D1");
    break;
  case 8:
    less("D2");
    break;
  case 0:
    less("PM");
    break;
  }
}
void Time()
{
  keypadTime();
  int th, tm;

  th = Hour + HH;
  tm = Minutes + MM;

  if (th >= 23 && tm >= 60)
  {
    th = 0;
    HH = 0;
    tm = 0;
  }
  lcd.setCursor(0, 0);
  lcd.print("Hours>>");
  lcd.print(th);
  lcd.setCursor(0, 1);
  lcd.print("Mins>>");
  lcd.print(tm);
  lcd.setCursor(14, 0);
  lcd.print(timeSecs);
  delay(100);
  lcd.clear();

  if (th == 8 && tm <= 45)
  {
    Less = 1;
  }
  else
  {
    Less = 0;
  }
  if (th == 9 && tm >= 5 && tm <= 50)
  {
    Less = 2;
  }
  if (th == 10 && tm <= 45)
  {
    Less = 3;
  }
  if (th == 10 && tm >= 55 || th == 11 && tm <= 40)
  {
    Less = 4;
  }
  if (th == 11 && tm >= 50 || th == 12 && tm <= 35)
  {
    Less = 5;
  }
  if (th == 12 && tm >= 55 || th == 13 && tm <= 40)
  {
    Less = 6;
  }
  if (th == 14 && tm >= 40 || th == 15 && tm <= 20)
  {
    Less = 7;
  }
  if (th == 15 && tm >= 25 || th == 16 && tm <= 5)
  {
    Less = 8;
  }

  if (Less != 1 && Less != 2 && Less != 3 && Less != 4 && Less != 5 && Less != 6 && Less != 7 && Less != 8 && th < 8 || th > 16 && tm > 5)
  {
    Less = 0;
  }

  if (Minutes < 0 || tm < 0)
  {
    Minutes = 0;
    tm = 0;
    MM = 0;
  }
  if (Minutes >= 60 || tm >= 60)
  {
    Minutes = 0;
    tm = 0;
    MM = 0;
    HH++;
  }
  if (th < 0 || th >= 24 || Hour < 0 || Hour >= 24)
  {
    Hour = 0;
    th = 0;
    HH = 0;
  }
  pl();
}

void TimerOff()
{
  timer = 0;
  keypadTimer();
}
void Timing()
{
  if (millis() - my_timer >= 1000)
  {
    my_timer = millis();
    TimerS--;
  }
}
void Timing_End(int Times)
{
  for (int i = 1; i < Times;)
  {
    lcd.noBacklight();
    delay(250);
    lcd.backlight();
    delay(250);
    i++;
  }
}
void TimerDisplay()
{
  lcd.setCursor(0, 0);
  lcd.print("Time left H:M:S");
  lcd.setCursor(5, 1);
  lcd.print(TimerH);
  lcd.print(":");
  lcd.print(TimerM);
  lcd.print(":");
  lcd.print(TimerS);
  delay(100);
  lcd.clear();
}
void Timer()
{
  if (dsp == 1)
  {
    timer = 0;
    keypadTimer();
    TimerDisplay();
    while (timer == 1)
    {
      keypadTimer();
      if (TimerS != 0)
      {
        Timing();
      }
      if (TimerS == 0)
      {
        if (TimerM != 0)
        {
          TimerM--;
          TimerS = 60;
        }
        if (TimerM == 0)
        {
          if (TimerH == 0 && TimerS == 0)
          {
            Timing_End(5);
            TimerOff();
          }
          if (TimerH != 0)
          {
            TimerH--;
            TimerM = 60;
          }
        }
      }
      TimerDisplay();
      CheckValTimer();
    }
    CheckValTimer();
  }
}

void Help()
{
  if (mode_count < -2)
  {
    mode_count = 0;
  }
  switch (mode_count)
  {
  case 0:
    Help_Phys();
    break;
  case 1:
    Help_Math();
    break;
  case 2:
    bells();
    break;
  case -1:
    Time();
    break;
  case -2:
    Timer();
    break;
  }
  if (mode_count > 2)
  {
    mode_count = 0;
  }
}

void TimeChange()
{
  uint32_t sec = millis() / 1000ul;
  Hour = (sec / 3600ul) % 24;
  Minutes = (sec % 3600ul) / 60ul;
  timeSecs = (sec % 3600ul) % 60ul;
}

void setup()
{
  pinMode(R_Button, INPUT_PULLUP);
  pinMode(L_Button, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  lcd.init();
  Serial.begin(9600);
  my_timer = millis();
}
void loop()
{
  BT1();
  Power();
  if (dsp == 1)
  {
    if (mode < 0 || mode > 2)
    {
      mode = 0;
    }
    if (mode == 2)
    {
      e_calc();
    }
    if (mode == 1)
    {
      calc();
    }
    if (mode == 0)
    {
      Help();
      TimeChange();
    }
  }
}
