// This #include statement was automatically added by the Particle IDE.
#include <MQTT.h>

void callback(char* topic, byte* payload, unsigned int length);

/**
 * if want to use IP address,
 * byte server[] = { XXX,XXX,XXX,XXX };
 * MQTT client(server, 1883, callback);
 * want to use domain name,
 * MQTT client("www.sample.com", 1883, callback);
 **/
 
byte server[] = { 192,168,1,149 };
MQTT client(server, 1883, callback);
// recieve message
void callback(char* topic, byte* payload, unsigned int length) {
    long ts = Time.now();
    char p[length + 1];
    int topic_len = strlen(topic);
    char t[topic_len + 1];
    memcpy(t, topic, topic_len);
    memcpy(p, payload, length);
    p[length] = NULL;
    t[topic_len] = NULL;
    String Topic(t);
    String message(p);
    client.publish("sys/log",Topic+", "+message+", "+ts);
    
    // if(message == "warn"){
    //     RGB.color(255,255,0);
    //     client.publish("sys/log/warn",("%s: %s", ts, message));
    // }
}

const int LIGHT_PIN = A0;
// const int TT2_PIN = A1;
// const int TT3_PIN = A2;

const int SV1_PIN = D2;
const int SV2_PIN = D4;
const int SV3_PIN = D5;
const int MV_PIN = D6;

const int CP1_PIN = D7;
const int CP2_PIN = D1;
const int RP1_PIN = D3;

enum opState { SUNNY, DAY, NIGHT, SHUTDOWN };
enum infoType { INFO, WARN, ERR };
enum colour { BLUE, YELLOW, ORANGE, OFF };
enum control { CP1, CP2, RP1, SV1, SV2, SV3, MV };

int CP1_STATE = 0;
int CP2_STATE = 0;
int RP1_STATE = 0;

int SV1_STATE = 1;
int SV2_STATE = 1;
int SV3_STATE = 1;
int MV_STATE = 1;

int TT2 = 0;
int TT3 = 0;

opState currentState = SHUTDOWN;

unsigned long lastSensorPost = 0;
unsigned long lastLogPost = 0;
const unsigned int SENSOR_POST_RATE = 1;
const unsigned int LOG_POST_RATE = 2;

void setup() {

    pinMode(LIGHT_PIN, INPUT);
    pinMode(SV1_PIN, OUTPUT);
    pinMode(SV2_PIN, OUTPUT);
    pinMode(SV3_PIN, OUTPUT);
    pinMode(MV_PIN, OUTPUT);
    pinMode(CP1_PIN, OUTPUT);
    pinMode(CP2_PIN, OUTPUT);
    pinMode(RP1_PIN, OUTPUT);
    
    digitalWrite(SV1_PIN, LOW);
    digitalWrite(SV2_PIN, LOW);
    digitalWrite(SV3_PIN, LOW);
    digitalWrite(MV_PIN, LOW);
    digitalWrite(CP1_PIN, LOW);
    digitalWrite(CP2_PIN, LOW);
    digitalWrite(RP1_PIN, LOW);
    
    
    RGB.control(true);
    // connect to the server
    client.connect("sparkclient");

    // publish/subscribe
    if (client.isConnected()) {
        client.publish("sys/log/info","Controller started...");
    }
}

int getRandData(){
    int num = rand() % (100-20+1)+20;
    return num;
}

void logInfo(infoType type, String msg){
    if(lastLogPost + LOG_POST_RATE <= Time.now()){
        switch(type){
        case INFO:
            client.publish("sys/log/info", String(Time.now())+"::INFO::"+msg);
            break;
        case WARN:
            client.publish("sys/log/warn", String(Time.now())+"::WARNING::"+msg);
            break;
        case ERR:
            client.publish("sys/log/error", String(Time.now())+"::ERROR::"+msg);
            break;
        }   
    }
}

float getSensor(int sensor){
    return analogRead(sensor);
}

void setLED(colour val){
    switch(val){
        case BLUE:
            RGB.color(0,0,255);
            break;
            
        case YELLOW:
            RGB.color(255,215,0);
            break;
        
        case ORANGE:
            RGB.color(255,70,0);
            break;
            
        case OFF:
            RGB.color(0,0,0);
            break;
    }
}

void switchState(control Ctl, int state){
    switch(Ctl){
        case CP1:
        {
            if(CP1_STATE != state && state == 1){
                CP1_STATE = state;
                digitalWrite(CP1_PIN, HIGH);
                logInfo(INFO, "CP1 turned ON.");
                delay(500);
                
            }else if(CP1_STATE != state && state == 0){
                CP1_STATE = state;
                digitalWrite(CP1_PIN, LOW);
                logInfo(INFO, "CP1 turned OFF.");
                delay(500);
                
            }else{
                logInfo(ERR, "CP1 in same state "+String(state));
            }   
        }
        break;
        
        case CP2:
        {
            if(CP2_STATE != state && state == 1){
                CP2_STATE = state;
                digitalWrite(CP2_PIN, HIGH);
                logInfo(INFO, "CP2 turned ON.");
                delay(500);
                
            } else if(CP2_STATE != state && state == 0){
                CP2_STATE = state;
                digitalWrite(CP2_PIN, LOW);
                logInfo(INFO, "CP2 turned OFF.");
                delay(500);
                
            } else{
                logInfo(ERR, "CP2 in same state "+String(state));
            }    
        }
        break;
            
        case RP1:
            {
                if(RP1_STATE != state && state == 1){
                    RP1_STATE = state;
                    digitalWrite(RP1_PIN, HIGH);
                    logInfo(INFO, "RP1 turned ON.");
                    delay(500);
                    
                } else if(RP1_STATE != state && state == 0){
                    RP1_STATE = state;
                    digitalWrite(RP1_PIN, LOW);
                    logInfo(INFO, "RP1 turned OFF.");
                    delay(500);
                    
                } else{
                    logInfo(ERR, "RP1 is in same state "+String(state));
                }
            }
            break;
        
        case SV1:
            {
                if(SV1_STATE != state && state == 2){
                    SV1_STATE = state;
                    digitalWrite(SV1_PIN, HIGH);
                    logInfo(INFO, "SV1 switched to loop "+String(state));
                    
                } else if(SV1_STATE != state && state == 1){
                    SV1_STATE = state;
                    digitalWrite(SV1_PIN, LOW);
                    logInfo(INFO, "SV1 switched to loop "+String(state));
                    
                } else{
                    logInfo(ERR, "SV1 didnt switch loop.");
                }
            }
            break;
        
        case SV2:
        {
            if(SV2_STATE != state && state == 2){
                SV2_STATE = state;
                digitalWrite(SV2_PIN, HIGH);
                logInfo(INFO, "SV2 switched to loop "+String(state));
                
            } else if(SV2_STATE != state && state == 1){
                SV2_STATE = state;
                digitalWrite(SV2_PIN, LOW);
                logInfo(INFO, "SV2 switched to loop "+String(state));
                
            } else{
                logInfo(ERR, "SV2 didnt switch loop.");
            }   
        }
        break;
        
        case SV3:
        {
            if(SV3_STATE != state && state == 2){
                SV3_STATE = state;
                digitalWrite(SV3_PIN, HIGH);
                logInfo(INFO, "SV3 switched to loop "+String(state));
                
            } else if(SV3_STATE != state && state == 1){
                SV3_STATE = state;
                digitalWrite(SV3_PIN, LOW);
                logInfo(INFO, "SV3 switched to loop "+String(state));
                
            } else{
                logInfo(ERR, "SV3 didnt switch loop.");
            }    
        }
        break;
        
        case MV:
        {
            if(MV_STATE != state && state == 2){
                MV_STATE = state;
                digitalWrite(MV_PIN, HIGH);
                logInfo(INFO, "MV switched to loop "+String(state));
                
            } else if(MV_STATE != state && state == 1){
                MV_STATE = state;
                digitalWrite(MV_PIN, LOW);
                logInfo(INFO, "MV switched to loop "+String(state));
            } else{
                logInfo(ERR, "MV didnt switch loop.");
            }    
        }
        break;
    }
}

void setValveState(opState state){
    switch(state){
        case SUNNY:
            if(currentState != state){
                currentState = state;
                switchState(SV1, 1);
                switchState(SV2, 2);
                switchState(SV3, 2);
                switchState(MV, 1);
                logInfo(INFO, "Valve state changed to SUNNY.");
                delay(500);
            } else{
                logInfo(ERR, "Valve state didnt change from SUNNY.");
            }
            break;
        
        case DAY:
            if(currentState != state){
                currentState = state;
                switchState(SV1, 2);
                switchState(SV2, 1);
                switchState(SV3, 1);
                switchState(MV, 2);
                logInfo(INFO, "Valve state changed to DAY.");
                delay(500);
            }else{
                logInfo(ERR, "Valve state didnt change from DAY.");
            }
            break;
            
        case NIGHT:
            if(currentState != state){
                currentState = state;
                switchState(SV1, 2);
                switchState(SV2, 2);
                switchState(SV3, 1);
                switchState(MV, 1);
                logInfo(INFO, "Valve state changed to NIGHT.");
                delay(500);
            } else{
                logInfo(ERR, "Valve state didnt change from NIGHT.");
            }
            break;
        
        case SHUTDOWN:
            if(currentState != state){
                currentState = state;
                switchState(SV1, 1);
                switchState(SV2, 1);
                switchState(SV3, 1);
                switchState(MV, 1);
                switchState(CP1, 0);
                switchState(CP2, 0);
                switchState(RP1, 0);
                logInfo(INFO, "Valve State changed to SHUTDOWN.");
                delay(500);
            } else{
                logInfo(ERR, "Valve state didnt change from SHUTDOWN.");
                // System.sleep(SLEEP_MODE_DEEP, 30);
            }
            break;
    }
}

void loop() {
    if (client.isConnected()){
        client.loop();
        postSensorData();
    }
    
    // TT2 = 30;
    TT2 = getRandData();
    // TT3 = 30;
    TT3 = getRandData();
    
    // if(getSensor(LIGHT_PIN) < 500){
    if(TT2 <= 30){
        // Night cycle
        if(TT3 >= 50){
            setLED(BLUE);
            setValveState(NIGHT);
            switchState(CP2, 1);
            switchState(RP1, 1);
        }else {
            // TS empty
            setLED(OFF);
            switchState(RP1, 0);
            switchState(CP2, 0);
            setValveState(SHUTDOWN);
            logInfo(WARN, "Thermal Storage below minimum temperature.");
        }
    }else {
        // Day cycle
        if(TT2 >= 50){
            if(TT3 <= 50){
                setLED(ORANGE);
                setValveState(SUNNY);
                switchState(CP1, 1);
                switchState(CP2, 1);
                switchState(RP1, 1);
            }else{
                setLED(YELLOW);
                setValveState(DAY);
                switchState(CP1, 1);
                switchState(RP1, 1);
            }
        }else{
            setLED(YELLOW);
            setValveState(DAY);
            switchState(CP1, 1);
            switchState(RP1, 1);
        }
    }
}

void postSensorData(){
    if(lastSensorPost + SENSOR_POST_RATE <= Time.now()){
            int light = analogRead(LIGHT_PIN);
            client.publish("sys/sensors", String(Time.now()) +","+ String(light) +","+ String(TT2) +","+ String(TT3));
            lastSensorPost = Time.now();
    }
}
