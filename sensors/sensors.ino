#include <math.h>
#include <OneWire.h>
#include <DS18B20.h>

OneWire oneWire(2);
DS18B20 sensor(&oneWire);

float PTC_KTY81_110_table20to90[12] = {
684,
747,
815,
886,
961,
1040,
1122,
1209,
1299,
1392,
1490,
1591
};

float NTC_640_10K_table20to90[23] = {
  96358,
  72500,
  55046,
  42157,
  32554,
  25339,
  19872,
  15698,
  12488,
  10000,
  8059,
  6535,
  5330,
  4372,
  3605,
  2989,
  2490,
  2084,
  1753,
  1481,
  1256,
  1070,
  915.4
};

float NTC_B57164K0472K000_table20to90[23] = {
43406,
32937,
25217,
19392,
15040,
11743,
9241,
7330,
5855,
4700,
3777,
3071,
2512,
2066,
1709,
1419,
1184,
993,
837,
709,
603,
515,
441
};

double PTC_KTY81_110_findTMP(double R){
  int i = 0;
  while((PTC_KTY81_110_table20to90[i]<R)&&(i < 12)){
    i++;
  }
  //Serial.println(i);
  int tmp = (10*(i-1)) - 20;
  //Serial.println(tmp);
  //Serial.println(R);
  if(i > 0){
    float bot = (PTC_KTY81_110_table20to90[i] - PTC_KTY81_110_table20to90[i-1]);
    //Serial.println(bot);
    float top = R - PTC_KTY81_110_table20to90[i-1];
    //Serial.println(top);
    return tmp + (top/bot)*10;
  }else{
    return -20;
  }
}


double NTC_640_10K_findTMP(double R){
  int i = 0;
  while((NTC_640_10K_table20to90[i]>R)&&(i < 23)){
    i++;
  }
  //Serial.println(i);
  int tmp = (5*i) - 20;
  //Serial.println(tmp);
  //Serial.println(R);
  if(i > 0){
    float bot = (NTC_640_10K_table20to90[i-1] - NTC_640_10K_table20to90[i]);
    //Serial.println(bot);
    float top = R - NTC_640_10K_table20to90[i];
    //Serial.println(top);
    return tmp - (top/bot)*5;
  }else{
    return -20;
  }
}

double NTC_B57164K0472K000_findTMP(double R){
  int i = 0;
  while((NTC_B57164K0472K000_table20to90[i]>R)&&(i < 23)){
    i++;
  }
  //Serial.println(i);
  int tmp = (5*i) - 20;
  //Serial.println(tmp);
  //Serial.println(R);
  if(i > 0){
    float bot = (NTC_B57164K0472K000_table20to90[i-1] - NTC_B57164K0472K000_table20to90[i]);
    //Serial.println(bot);
    float top = R - NTC_B57164K0472K000_table20to90[i];
    //Serial.println(top);
    return tmp - (top/bot)*5;
  }else{
    return -20;
  }
}

float foo()
{
  Serial.println("-------");
  //float a = ((float)analogRead(A0))/675;
  float a = ((float)analogRead(A0))/414;;
  Serial.print("v =");
  Serial.println(analogRead(A0));
  //a = 0.41;
  Serial.print("a =");
  Serial.println(a, 10);
  //float top = ((a/68*4700))+(830*4700/(830+47000));
  float top = (a*47)+81.5596;
  Serial.print("top =");
  Serial.println(top, 10);
  float bot = (a/68)- 0.98264;
  Serial.print("bot =");
  Serial.println(bot, 10);
  return -top/bot;
}

double PTC_KTY81_110(int analogValue, int R)
{
  double a = ((double)analogValue)/1024;
  double Rt = (a*((double)R))/(1-a);
  //float temperature = (-0.00003*Rt*Rt)+(0.1957*Rt)-137.69;
  double temperature = PTC_KTY81_110_findTMP(Rt);
  return temperature;
}



double NTC_640_10K(int analogValue, int R)
{
  double a = ((double)analogValue)/1024;
  //Serial.print("a = ");
  //Serial.println(a,10);
  double Rt = (a*((double)R))/(1-a);
  //Serial.print("Rt = ");
  //Serial.println(Rt,10);
  //double temperature = (-23.65*log(Rt)) + 245.85;
  double temperature = NTC_640_10K_findTMP(Rt);
  return temperature;
}


double NTC_B57164K0472K000(int analogValue, int R)
{
  double a = ((double)analogValue)/1024;
  //Serial.print("a = ");
  //Serial.println(a,10);
  double Rt = (a*((double)R))/(1-a);
  //Serial.print("Rt = ");
  //Serial.println(Rt,10);
  //double temperature = (-23.65*log(Rt)) + 245.85;
  double temperature = NTC_B57164K0472K000_findTMP(Rt);
  return temperature;
}

double DS18b20()
{
  sensor.requestTemperatures();
  while (!sensor.isConversionComplete());
  return sensor.getTempC();
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("-------");
  Serial.println(NTC_640_10K_findTMP(11000));
  Serial.println("-------");
  sensor.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("-------");
  Serial.print("PTC_KTY81_110 : ");
  Serial.println(PTC_KTY81_110(analogRead(A0), 1000));
  Serial.print("NTC_640_10K : ");
  Serial.println(NTC_640_10K(analogRead(A1), 10000));
  Serial.print("NTC_B57164K0472K000 : ");
  Serial.println(NTC_B57164K0472K000(analogRead(A2), 4700));
  Serial.print("DS18b20 : ");
  Serial.println(DS18b20());
  delay(500);
}
