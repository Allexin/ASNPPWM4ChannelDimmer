#include "PortHandler.h"
#include "TimerOne.h"
#include "FastTimer.h"
#include <EEPROM.h>
#include <avr/wdt.h>

//#define CHANNEL1
#define CHANNEL2
//#define CHANNEL3
#define CHANNEL4


volatile uint8_t tick;
volatile uint8_t dimmer[4];
bool dimmerIsActive = false;

void timeout()
{   
  if (tick>0){
    #ifdef CHANNEL1
    if (dimmer[0] != 0){
      if (dimmer[0]>tick)
        D9_High;
    }
    #endif
    #ifdef CHANNEL2
    if (dimmer[1] != 0){
      if (dimmer[1]>tick)
        D6_High;
    }
    #endif
    #ifdef CHANNEL3
    if (dimmer[2] != 0){
      if (dimmer[2]>tick)
        D5_High;
    }
    #endif
    #ifdef CHANNEL4
    if (dimmer[3] != 0){
      if (dimmer[3]>tick)
        D3_High;
    }
    #endif
    tick--;    
  }
}

void  detect_up()
{ 
  if (D2_Read==LOW){
   tick=255;
   Timer1.resume();
   attachInterrupt(digitalPinToInterrupt(2), detect_down, HIGH);
  }
}  

void  detect_down()
{   
  if (D2_Read==HIGH){
     Timer1.stop();
     #ifdef CHANNEL1
     if (dimmer[0] == 255)
        D9_High;
      else
        D9_Low;
     #endif
     #ifdef CHANNEL2   
      if (dimmer[1] == 255)
        D6_High;
      else
        D6_Low;
     #endif
     #ifdef CHANNEL3
      if (dimmer[2] == 255)
        D5_High;
      else
        D5_Low;
     #endif
     #ifdef CHANNEL4   
      if (dimmer[3] == 255)
        D3_High;
      else
        D3_Low;
     #endif
     tick=255;
     attachInterrupt(digitalPinToInterrupt(2), detect_up, LOW);
  }
} 


void startWatchDog(){
  wdt_enable(WDTO_1S);
}

void resetWatchDog(){
  wdt_reset();
}

const uint8_t CMD_GROUP = 0x01;

const long DEFAULT_BAUD_RATE = 19200;


uint8_t channels[4];
uint8_t target[4];
uint8_t channelChangeStepDelay;
long stepsCounter = 0;

cPortHandler* port;

uint8_t addrSelf;
uint8_t channelsMin[4];

void saveData(){
  EEPROM.write(0, addrSelf);
  EEPROM.write(1, channelsMin[0]);
  EEPROM.write(2, channelsMin[1]);
  EEPROM.write(3, channelsMin[2]);
  EEPROM.write(4, channelsMin[3]);
  uint8_t sum = addrSelf+channelsMin[0]+channelsMin[1]+channelsMin[2]+channelsMin[3];
  EEPROM.write(5, sum); //checksum
}

void loadData(){  
  addrSelf = EEPROM.read(0); 
  channelsMin[0] = EEPROM.read(1); 
  channelsMin[1] = EEPROM.read(2); 
  channelsMin[2] = EEPROM.read(3); 
  channelsMin[3] = EEPROM.read(4); 
  uint8_t checksum = EEPROM.read(5); 
  
  uint8_t sum = addrSelf+channelsMin[0]+channelsMin[1]+channelsMin[2]+channelsMin[3];

  if (checksum!=sum || addrSelf==0){
    addrSelf = 1;
    channelsMin[0] = 0; 
    channelsMin[1] = 0; 
    channelsMin[2] = 0; 
    channelsMin[3] = 0; 
  }
}

long long lastUpdate;
long long sendRegisterMessageTimout = 0;

void sendRegisterMessageWithTimeout(){
  sendRegisterMessageTimout = 500+addrSelf*20;
}

void setup() {
  loadData();
  
  Serial.begin(DEFAULT_BAUD_RATE); 

  port = new cPortHandler(&Serial);

  for (int i = 0; i<4; ++i){
    channels[i] = 0;
    target[i] = 0;
  }
  channelChangeStepDelay = 5;

  saveData();

  sendRegisterMessageWithTimeout();

  startWatchDog();  

  D3_Out; D5_Out; D6_Out; D9_Out;
  D3_Low; D5_Low; D6_Low; D9_Low;
  D2_In;

  //attachInterrupt(digitalPinToInterrupt(2), detect_up, LOW); - turn on in loop if dimming needed
  Timer1.initialize(32);
  Timer1.attachInterrupt( timeout ); 
  Timer1.stop();

  lastUpdate = millis();
}


void sendRegisterAddr(){
    uint8_t buf[4];
    buf[0] = SYS_MSG;
    buf[1] = 1;//PONG
    buf[2] = 1;
    buf[3] = addrSelf;  
    port->queuePackage(START_CHAR_HIGH,BROADCAST_ADDRESS,buf,4);
}

void sendTraceState(){
  uint8_t buf[5];
  buf[0] = SYS_MSG;
  buf[1] = 0x12;
  buf[2] = addrSelf;
  buf[3] = 1;
  buf[4] = 0;      
  port->queuePackage(START_CHAR_HIGH,BROADCAST_ADDRESS,buf,5);
}

const uint8_t deviceTypeLength = 16;
char deviceInfo[deviceTypeLength] = "DIMMER_PWM_4C_AC";
const uint8_t descriptionLength = 40;
char descriptionInfo[descriptionLength] = "Process commands and set PWM to channels";

void sendInfo(uint8_t hostAddr){
  const uint8_t packageLength = 3+1+deviceTypeLength+1+descriptionLength;
  uint8_t buf[packageLength];
  buf[0] = SYS_MSG;
  buf[1] = 0x21;
  buf[2] = addrSelf;
  
  buf[3] = deviceTypeLength;
  memcpy(&buf[3+1],deviceInfo,deviceTypeLength);
  
  buf[4+deviceTypeLength] = descriptionLength; 
  memcpy(&buf[4+deviceTypeLength+1],descriptionInfo,descriptionLength);
  
  port->queuePackage(START_CHAR_HIGH,BROADCAST_ADDRESS,buf,packageLength);
}

uint8_t sign(long value){
  if (value>0)
    return 1;
  if (value<0)
    return -1;
  return 0;
}

void sendSettings(uint8_t hostAddr){
  uint8_t buf[7];
  buf[0] = SYS_MSG;
  buf[1] = 0x32;
  buf[2] = addrSelf;
  buf[3] = channelsMin[0];
  buf[4] = channelsMin[1];
  buf[5] = channelsMin[2];
  buf[6] = channelsMin[3];    
  port->queuePackage(START_CHAR_HIGH,hostAddr,buf,7);
}

void loop() {
  resetWatchDog();
  
  long long currentTime = millis();
  long dt = currentTime - lastUpdate;
  if (currentTime<lastUpdate)
    dt = currentTime;
  lastUpdate = currentTime;

  if (sendRegisterMessageTimout>0){
      sendRegisterMessageTimout-=dt;
      if (sendRegisterMessageTimout<=0){
        sendRegisterMessageTimout = 0;
        sendRegisterAddr();
        
      }
  }
  

  if (port->processData(dt)){    
    uint8_t* receivedPackage = port->getReceivedPackage();
    
    uint8_t addr = receivedPackage[ADDRESS_POS];
    uint8_t dataLength = receivedPackage[LENGTH_POS];
    if (dataLength>=2 && (addr==0 || addr==addrSelf)){
      uint8_t cmdGroup = receivedPackage[DATA_START_POS+0];
      uint8_t cmd = receivedPackage[DATA_START_POS+1];
      
      if (cmdGroup==SYS_MSG){
        switch(cmd){
          case 0x00:{//PING
            if (dataLength==2){//PING
              sendRegisterMessageWithTimeout();
            }
            else{//PONG
              //wtf??
            }
          };break;
          case 0x11:{//TRACE_STEP
            sendTraceState();
          };break;
          case 0x20:{//REQUEST_INFO
            if (dataLength==3){
              sendInfo(receivedPackage[DATA_START_POS+2]);
            }
          };break;
          case 0x30:{//SET_ADDRESS
            if (dataLength==3){
              uint8_t newAddr = receivedPackage[DATA_START_POS+2];
              if (newAddr!=0 && newAddr!=addrSelf){
                addrSelf = newAddr;
                saveData();
                sendRegisterAddr();
              }
            }
          };break;
          case 0x31:{//REQUEST_SETTINGS
            if (dataLength==3 && addr==addrSelf){
              sendSettings(receivedPackage[DATA_START_POS+2]);
            }
          }; break;
          case 0x33:{//SET_SETTINGS
            if (dataLength==7 && addr==addrSelf){
              channelsMin[0] = receivedPackage[DATA_START_POS+3];  
              channelsMin[1] = receivedPackage[DATA_START_POS+4];  
              channelsMin[2] = receivedPackage[DATA_START_POS+5];  
              channelsMin[3] = receivedPackage[DATA_START_POS+6];  
              saveData();
              sendSettings(receivedPackage[DATA_START_POS+2]);
            }            
          }; break;
        }
      }
      else
      if (cmdGroup==CMD_GROUP){        
        if (addr==addrSelf){
          switch(cmd){
            case 0x03:{//SET_DIMMER_STATE
              if (dataLength==7){
                target[0] = receivedPackage[DATA_START_POS+2];
                target[1] = receivedPackage[DATA_START_POS+3];
                target[2] = receivedPackage[DATA_START_POS+4];
                target[3] = receivedPackage[DATA_START_POS+5];
                channelChangeStepDelay = receivedPackage[DATA_START_POS+6];
              }
            }; break;
          }
        }
      }
      
    }
  }

  if (channelChangeStepDelay==0){
    for (int i = 0; i<4; ++i)
      channels[i] = target[i];
  }
  else{
    stepsCounter+=dt;
    long stepsCount = stepsCounter / (channelChangeStepDelay*20);
    stepsCounter = stepsCounter %( channelChangeStepDelay*20);

    for (int i = 0; i<4; ++i){
      long diff = target[i] - channels[i];
      if (diff!=0){
        if (abs(diff)<=stepsCount){
          channels[i] = target[i];
        }
        else{
            channels[i] += stepsCount*sign(diff);  
        }
      }
    }
  }

  bool needDimming = false;
  for (int i = 0; i<4; ++i){
    if (channels[i]==0){
      dimmer[i] = 0;
    }
    else{
      needDimming = true;
      if (channels[i]<channelsMin[i]){
        dimmer[i] = channelsMin[i];
      }
      else{
        dimmer[i] = channels[i];
      }
    }
  }

  if (dimmerIsActive!=needDimming){
    dimmerIsActive=needDimming;
    if (dimmerIsActive){
      attachInterrupt(digitalPinToInterrupt(2), detect_up, LOW);
    }
    else{
      detachInterrupt(digitalPinToInterrupt(2));
    }
  }
  
  port->startSending();

  if (dimmerIsActive){
  }
  else{
    delay(10);
  }
}
