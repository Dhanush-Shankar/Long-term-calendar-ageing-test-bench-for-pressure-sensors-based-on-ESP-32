Source Code for ESP32
#include <Arduino.h>
#include <U8g2lib.h>
#include <MUIU8g2.h>
#include <WiFi.h>
#include <SimpleRotary.h>
#include <Adafruit_NeoPixel.h>
#include <PubSubClient.h>
#include "time.h"
#include <EEPROM.h>
#include <SD.h>
#include <Ticker.h>
#include <Wire.h>
#include "DFRobot_INA219.h"
#include "HX711.h"
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
/* macros */
#define mqttPort 1883
#define SD_DETECT_PIN 25
#define PIN 13       // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 30 // Popular NeoPixel ring size
// The next two macros define the scroll speed of the short story
#define SCROLL_DELTA 2
#define SCROLL_DELAY 400
#define SD_CS_PIN 33
#define EEPROM_SIZE 2
SimpleRotary rotary(15,2,5);
uint8_t rotary_event = 0; // 0 = not turning, 1 = CW, 2 = CCW
uint8_t push_event = 0;   // 0 = not pushed, 1 = pushed
bool data_send_flag = false;
bool sd_card_init = false;
uint8_t setf = 0;
uint8_t battery_percent = 0;
MUIU8G2 mui;
DFRobot_INA219_IIC ina219(&Wire, 0x42);
U8G2_ST7567_OS12864_F_4W_HW_SPI u8g2(U8G2_R2, /* cs=*/32, /* dc=*/27, /*
reset=*/26);
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Ticker periodicTicker;
struct tm timeinfo;
// time
const char *ntpServer = "time.nist.gov";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;
const char *interval[] = {"15Sec", "30Sec", "1min", "2min", "5min", "10min",
                          "15min", "30min", "1hr", "3hr", "6hr", "12hr", "24hr"};
char string_list[250] = {0};
uint8_t number_input = 2;
uint16_t int_idx = 0;
// variable where the user can input a number between 0 and 9
uint8_t exit_code = 0; // return value from the menu system
uint8_t is_redraw = 1;
float LoadCell_Val = 0.0;
float Press_Sens_value[16] = {0.0}; // max 16 sensors
uint8_t Tot_Sens_Present = 2;
int connectionDelay = 1;
long interrupttime = 0;
// display
char buf[48];                   // there are at most 8 glyphs per line, max buf size is
8 * 3 = 24 uint8_t total_lines; // the total number of lines in the story
// uint8_t i; // loop variable for the lines
uint8_t line_cnt; // number of lines to draw, usually equal to
lines_per_draw
    uint8_t start_line; // topmost visible line, derived from top_window_pos
uint8_t lines_per_draw; // how many lines to draw on the screen, derived from
font and display height
    uint16_t glyph_height; // height of the glyphs
uint16_t top_window_pos;   // defines the display position in pixel within
the text
    uint16_t total_height; // total height in pixel, derived from font height
and total_lines
    u8g2_uint_t top_offset; // offset between the first visible line and the
display
    /* function definition */
    uint8_t
    mui_hrule(mui_t *ui, uint8_t msg);
uint16_t animal_name_list_get_cnt(void *data);
const char *animal_name_list_get_str(void *data, uint16_t index);
boolean wifi_connected = false, sd_found = false, battery_charging = false;
// the Wifi radio's status
#define WIFI_APT ""
#define WIFI_PASSWORD ""
// Mux control pins
int s0 = 16;
int s1 = 17;
int s2 = 12;
int s3 = 14;
// Mux in "SIG" pin
int SIG_pin = 34;
WiFiClient client;
int status = WL_IDLE_STATUS;
int keyIndex = 0; // your network key Index number (needed only for
WEP)
unsigned long myChannelNumber = 1756353;
const char *server = "mqtt3.thingspeak.com";
unsigned long updateInterval = 15;
uint8_t interval_input = 0;
PubSubClient mqttClient(client);
// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 32;
const int LOADCELL_SCK_PIN = 4;
HX711 scale;
#define CH_CLIENT_ID ""
#define CH_USERNAME ""
#define CH_PASSWORD ""
muif_t muif_list[] = {
    /* normal text style */
    MUIF_U8G2_FONT_STYLE(0, u8g2_font_helvR08_tr),
    MUIF_RO("HR", mui_hrule),
    /* Leave the menu system */
    MUIF_VARIABLE("LV", &exit_code, mui_u8g2_btn_exit_wm_fi),
    /* input for a number between 0 to 9 */
    MUIF_U8G2_U8_MIN_MAX("IN", &number_input, 1, 16,
                         mui_u8g2_u8_min_max_wm_mud_pi),
    MUIF_U8G2_U16_LIST("NA", &int_idx, NULL, animal_name_list_get_str,
                       animal_name_list_get_cnt, mui_u8g2_u16_list_line_wa_mud_pi),
    /* MUI_LABEL is used to place fixed text on the screeen */
    MUIF_LABEL(mui_u8g2_draw_text)};
fds_t fds_data[] MUI_PROGMEM =
    MUI_FORM(1)
        MUI_STYLE(0)
            MUI_XY("HR", 0, 14)
                MUI_LABEL(5, 12, "Change Configuration")
                    MUI_LABEL(5, 30, "Sen Cnt:")
                        MUI_LABEL(5, 43, "Data Int:")
                            MUI_XY("IN", 60, 30)
                                MUI_XYA("NA", 50, 43, 44)
                                    MUI_XYT("LV", 64, 59, " OK ");
void detect_events(void)
{
    uint8_t tmp;
    // 0 = not pushed, 1 = pushed
    tmp = rotary.push();
    if (tmp != 0) // only assign the push event, never clear the event
        here
            push_event = tmp;
    // 0 = not turning, 1 = CW, 2 = CCW
    tmp = rotary.rotate();
    if (tmp != 0) // only assign the rotation event, never clear the
        event here
            rotary_event = tmp;
}
void handle_events(void)
{
    // 0 = not pushed, 1 = pushed
    if (push_event == 1)
    {
        mui.sendSelect();
        is_redraw = 1;
        push_event = 0;
    }
    // 0 = not turning, 1 = CW, 2 = CCW
    if (rotary_event == 1)
    {
        mui.nextField();
        is_redraw = 1;
        rotary_event = 0;
    }
    if (rotary_event == 2)
    {
        mui.prevField();
        is_redraw = 1;
        rotary_event = 0;
    }
}
uint16_t animal_name_list_get_cnt(void *data)
{
    return sizeof(interval) / sizeof(*interval); /* number of interval */
}
const char *animal_name_list_get_str(void *data, uint16_t index)
{
    return interval[index];
}
long get_interval_in_secs(uint16_t index)
{
    // "1min", "2min", "5min", "10min", "15min", "30min", "1hr", "3hr", "6hr",
    "12hr", "24hr" switch (index)
    {
    case 0:
        return 15;
    case 1:
        return 30;
    case 2:
        return 60;
    case 3:
        return 120;
    case 4:
        return 300;
    case 5:
        return 600;
    case 6:
        return 900;
    case 7:
        return 1800;
    case 8:
        return 3600;
    case 9:
        return 10800;
    case 10:
        return 21600;
    case 11:
        return 43200;
    case 12:
        return 86400;
    default:
        return 60;
    }
    return 60;
}
uint8_t mui_hrule(mui_t *ui, uint8_t msg)
{
    if (msg == MUIF_MSG_DRAW)
    {
        u8g2.drawHLine(0, mui_get_y(ui), u8g2.getDisplayWidth());
    }
    return 0;
}
/* LCD data */
void setChargingBattery()
{
    u8g2.setFont(u8g2_font_streamline_all_t);
    u8g2.setFontDirection(0);
    u8g2.drawGlyph(87, 21, 0x82);
    // u8g2.sendBuffer();
}
void setNonChargingBattery()
{
    u8g2.setFont(u8g2_font_streamline_all_t);
    u8g2.setFontDirection(1);
    u8g2.drawGlyph(87, 0, 0x14e);
    // u8g2.sendBuffer();
}
void setWifiOn()
{
    u8g2.setFont(u8g2_font_streamline_all_t);
    u8g2.setFontDirection(0);
    u8g2.drawGlyph(45, 21, 0x1fd);
    // u8g2.sendBuffer();
}
void setWifiOff()
{
    u8g2.setFont(u8g2_font_streamline_all_t);
    u8g2.setFontDirection(0);
    u8g2.drawGlyph(45, 21, 0x206);
    // u8g2.sendBuffer();
}
void setSDIcon()
{
    u8g2.setFont(u8g2_font_streamline_all_t);
    u8g2.setFontDirection(0);
    u8g2.drawGlyph(67, 21, 0x93);
    // u8g2.sendBuffer();
}
void clearSDIcon()
{
    u8g2.setFont(u8g2_font_streamline_all_t);
    u8g2.setFontDirection(0);
    u8g2.drawGlyph(67, 21, 0x148);
    // u8g2.sendBuffer();
}
void setbatterypercent()
{
    u8g2.setFont(u8g2_font_micro_mr);
    u8g2.setFontDirection(0);
    char buffer[10] = {0};
    sprintf(buffer, "%d%%", battery_percent);
    u8g2.drawStr(110, 13, buffer);
}
void ctrl_icons()
{
    if (wifi_connected)
    {
        setWifiOn();
    }
    else
    {
        setWifiOff();
    }
    if (sd_found)
    {
        setSDIcon();
    }
    else
    {
        clearSDIcon();
    }
    if (battery_charging)
    {
        setChargingBattery();
    }
    else
    {
        setNonChargingBattery();
    }
    setbatterypercent();
}
void monitorSD()
{
    int buttonState = digitalRead(SD_DETECT_PIN);
    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    if (buttonState == HIGH)
    {
        sd_found = false;
    }
    else
    {
        sd_found = true;
    }
}
void printLocalTime()
{
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to obtain time");
        return;
    }
    int val = 20;
    char buffer[100] = {0};
    u8g2.setFont(u8g2_font_t0_11b_tf);
    u8g2.setFontDirection(0);
    strftime(buffer, 100, "%x", &timeinfo);
    u8g2.drawStr(54, 42, buffer);
    strftime(buffer, 100, "%X", &timeinfo);
    u8g2.drawStr(54, 54, buffer);
}
void showLDcellValue()
{
    int val = 20;
    char buffer[100] = {0};
    LoadCell_Val = scale.get_units(10);
    u8g2.setFont(u8g2_font_t0_11b_tf);
    u8g2.setFontDirection(0);
    sprintf(buffer, "LD_C: %.1f g", LoadCell_Val);
    u8g2.drawStr(54, 30, buffer);
    // u8g2.sendBuffer();
}
// wifi related function
void Wificonnect()
{
    // Connect or reconnect to WiFi
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(WIFI_APT);
        while (WiFi.status() != WL_CONNECTED)
        {
            WiFi.begin(WIFI_APT, WIFI_PASSWORD); // Connect to WPA/WPA2 network.
            Change this line if using open or WEP network
                                                  Serial.print(".");
            delay(5000);
        }
        wifi_connected = true;
        Serial.println("\nConnected.");
    }
}
void monitor_server()
{
    // Reconnect to WiFi if it gets disconnected.
    if (WiFi.status() != WL_CONNECTED)
    {
        wifi_connected = false;
        Wificonnect();
    }
    wifi_connected = true;
    // Connect if MQTT client is not connected and resubscribe to channel
    updates.if (!mqttClient.connected())
    {
        mqttConnect();
        // mqttSubscribe( channelID );
    }
}
// Mqtt related functions
// Publish messages to a ThingSpeak channel.
void mqttPublish(long pubChannelID, String message)
{
    String topicString = "channels/" + String(pubChannelID) + "/publish";
    mqttClient.publish(topicString.c_str(), message.c_str());
}
// Connect to MQTT server.
void mqttConnect()
{
    // Loop until connected.
    if (!mqttClient.connected())
    {
        // Connect to the MQTT broker.
        if (mqttClient.connect(CH_CLIENT_ID, CH_USERNAME, CH_PASSWORD))
        {
            Serial.print("MQTT to ");
            Serial.print(server);
            Serial.print(" at port ");
            Serial.print(mqttPort);
            Serial.println(" successful.");
        }
        else
        {
            Serial.print("MQTT connection failed, rc = ");
            // See https://pubsubclient.knolleary.net/api.html#state for the failure
            code explanation.Serial.print(mqttClient.state());
            Serial.println(" Will try again in a few seconds");
        }
    }
}
float readMux(int channel)
{
    int controlPin[] = {s0, s1, s2, s3};
    int muxChannel[16][4] = {
        {0, 0, 0, 0}, // channel 0
        {1, 0, 0, 0}, // channel 1
        {0, 1, 0, 0}, // channel 2
        {1, 1, 0, 0}, // channel 3
        {0, 0, 1, 0}, // channel 4
        {1, 0, 1, 0}, // channel 5
        {0, 1, 1, 0}, // channel 6
        {1, 1, 1, 0}, // channel 7
        {0, 0, 0, 1}, // channel 8
        {1, 0, 0, 1}, // channel 9
        {0, 1, 0, 1}, // channel 10
        {1, 1, 0, 1}, // channel 11
        {0, 0, 1, 1}, // channel 12
        {1, 0, 1, 1}, // channel 13
        {0, 1, 1, 1}, // channel 14
        {1, 1, 1, 1}  // channel 15
    };
    // loop through the 4 sig
    for (int i = 0; i < 4; i++)
    {
        digitalWrite(controlPin[i], muxChannel[channel][i]);
        delay(50);
    }
    // read the value at the SIG pin
    int val = analogRead(SIG_pin);
    // return the value
    float voltage = (val * 5.0) / 1024.0;
    return voltage;
}
void Read_Sensor()
{
    String stringOne = "";
    for (int j = 0; j < Tot_Sens_Present; j++)
    {
        float volt = readMux(j);
        stringOne = stringOne + "S" + j + ":" + String(volt, 2) + "v\n";
    }
    stringOne.toCharArray(string_list, (stringOne.length() + 1));
}
// Write to the SD card (DON'T MODIFY THIS FUNCTION)
void writeFile(fs::FS &fs, const char *path, const char *message)
{
    Serial.printf("Writing file: %s\n", path);
    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        return;
    }
    if (file.print(message))
    {
        Serial.println("File written");
    }
    else
    {
        Serial.println("Write failed");
    }
    file.close();
}
void appendFile(fs::FS &fs, const char *path, const char *message)
{
    Serial.printf("Appending to file: %s\n", path);
    File file = fs.open(path, FILE_APPEND);
    if (!file)
    {
        Serial.println("Failed to open file for appending");
        return;
    }
    if (file.print(message))
    {
        Serial.println("Message appended");
    }
    else
    {
        Serial.println("Append failed");
    }
    file.close();
}
bool writedata_sd(fs::FS &fs, const char *path, const char *message)
{
    // Serial.printf("Appending to file: %s\n", path);
    bool flag = false;
    File file = fs.open(path, FILE_APPEND);
    if (!file)
    {
        Serial.println("Failed to open file for appending");
        return false;
    }
    if (file.print(message))
    {
        Serial.println("Message appended");
        flag = true;
    }
    else
    {
        Serial.println("Append failed");
        flag = false;
    }
    file.close();
    return flag;
}
void mux_init()
{
    pinMode(s0, OUTPUT);
    pinMode(s1, OUTPUT);
    pinMode(s2, OUTPUT);
    pinMode(s3, OUTPUT);
    digitalWrite(s0, LOW);
    digitalWrite(s1, LOW);
    digitalWrite(s2, LOW);
    digitalWrite(s3, LOW);
}
void scroll_init()
{
    u8g2.setFont(u8g2_font_wqy12_t_gb2312b); // two unknown glyphs
    /* get the height of the glyphs */
    glyph_height = u8g2.getMaxCharHeight();
    /* calculate the length of the text in lines */
    total_lines = u8x8_GetStringLineCnt(string_list);
    /* calculate the height of the text in pixel */
    total_height = (uint16_t)total_lines * (uint16_t)glyph_height;
    /* calculate how many lines must be drawn on the screen */
    lines_per_draw = u8g2.getDisplayHeight() / glyph_height;
    lines_per_draw += 2;
    /* start at the top of the text */
    top_window_pos = 0;
}
void scroll_sensor_value()
{
    u8g2.setFont(u8g2_font_wqy12_t_gb2312b); // two unknown glyphs
    u8g2.setFontDirection(0);
    /* calculate the length of the text in lines */
    total_lines = u8x8_GetStringLineCnt(string_list) - 1;
    if (top_window_pos >= (total_lines * glyph_height))
    {
        top_window_pos = 0;
    }
    start_line = top_window_pos / glyph_height;
    top_offset = top_window_pos % glyph_height;
    line_cnt = total_lines - start_line;
    if (line_cnt > lines_per_draw)
        line_cnt = lines_per_draw;
    u8g2.setFont(u8g2_font_wqy12_t_gb2312b);
    for (int i = 0; i < line_cnt; i++) // draw visible lines
    {
        /* copy a line of the text to the local buffer */
        u8x8_CopyStringLine(buf, i + start_line, string_list);
        /* draw the content of the local buffer */
        u8g2.drawUTF8(0, i * glyph_height - top_offset + glyph_height, buf);
    }
    top_window_pos += SCROLL_DELTA;
}
void datalog_int_check()
{
    // for every 5 sec it trigggers
    interrupttime++;
    if (interrupttime >= (updateInterval / 5))
    {
        data_send_flag = true;
        interrupttime = 0;
    }
}
void updateserver(String s)
{
    // Call the loop to maintain connection to the server.
    mqttClient.loop();
    // // Update ThingSpeak channel periodically. The update results in the
    message to the subscriber.
    // if ( abs((int)(millis() - lastPublishMillis)) > updateInterval*1000) {
    // mqttPublish( myChannelNumber, (String("field1=")+String(LoadCell_Val)
    // +String("&field2=")+String(Sen_1_data)
    // +String("&field3=")+String(Sen_2_data)
    // +String("&field4=")+String(Sen_3_data)));
    // lastPublishMillis = millis();
    // }
}
void datalog()
{
    if (data_send_flag)
    {
        char buffer[100] = {0};
        strftime(buffer, 100, "%x %X", &timeinfo);
        String s = String(buffer) + ',' + String(LoadCell_Val) + ',';
        for (int i = 0; i < Tot_Sens_Present; i++)
        {
            s = s + Press_Sens_value[i] + ',';
        }
        s = s + '\n';
        // SD card data log
        if (sd_card_init)
        {
            if (writedata_sd(SD, "/esp_press.csv", s.c_str()))
            {
                data_send_flag = false;
            }
        }
        else
        {
            sd_init();
        }
        // server data push
    }
}
void sd_init()
{
    if (!sd_card_init)
    {
        if (!SD.begin(SD_CS_PIN))
        {
            Serial.println("Card Mount Failed");
            sd_found = false;
        }
        else
        {
            Serial.println("Card Mount Success");
            sd_card_init = true;
        }
        if (sd_found)
        {
            File file = SD.open("/esp_press.csv");
            if (!file)
            {
writeFile(SD, "/esp_press.csv", "Timestamp, Loadcell, s1, s2, s3,
s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14,s15, s16\n");
            }
            file.close();
        }
    }
}
void monitor_battery()
{
    float curr = ina219.getCurrent_mA();
    float bus_voltage = ina219.getBusVoltage_V();
    int p = (bus_voltage - 6) / 2.4 * 100;
    if (p > 100)
        p = 100;
    if (p < 0)
        p = 0;
    battery_percent = p;
    if (curr < 0)
    {
        battery_charging = false;
    }
    else
    {
        battery_charging = true;
    }
}
void ina219_init()
{
    ina219.setBRNG((DFRobot_INA219::eIna219BusVolRange_t)1);
    ina219.setPGA((DFRobot_INA219::eIna219PGABits_t)3);
    ina219.setBADC((DFRobot_INA219::eIna219AdcBits_t)3, (DFRobot_INA219::eIna219A
                                                             dcSample_t)5);
    ina219.setSADC((DFRobot_INA219::eIna219AdcBits_t)3, (DFRobot_INA219::eIna219A
                                                             dcSample_t)5);
    ina219.setMode((DFRobot_INA219::eInaMode_t)7);
}
void loadcell_init()
{
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    scale.set_scale(-254.21); // this value is obtained by
    calibrating the scale with known weights; see the README for details
scale.tare();
}
void setup(void)
{
    WiFi.mode(WIFI_STA);
    // EEPROM.begin(EEPROM_SIZE);
    Serial.begin(115200); // Initialize serial
    ina219.begin();
    ina219_init();
    Serial.println("Battery monitor Init...");
    loadcell_init();
    Serial.println("Load cell Init...");
    scroll_init();
    pinMode(SD_DETECT_PIN, INPUT_PULLUP);
    mux_init();
    Serial.println("mux Init...");
    pixels.begin();
    pixels.clear();
    SPIClass spi = SPIClass(HSPI);
    spi.begin(18 /* SCK */, 19 /* MISO */, 23 /* MOSI */, SD_CS_PIN /* SS */);
    delay(500);
    sd_init();
    while (sd_card_init != true)
    {
        Serial.println("sd re insert..");
        delay(2000);
    }
    Serial.println("sd Init...");
    updateInterval = get_interval_in_secs(int_idx);
    delay(10);
    Tot_Sens_Present = EEPROM.read(0);
    // init and get the time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    u8g2.begin();
    Serial.println("Display Init...");
    mui.begin(u8g2, fds_data, muif_list, sizeof(muif_list) / sizeof(muif_t));
    periodicTicker.attach_ms(5000, datalog_int_check);
    monitorSD();
    Serial.println("Init Done");
}
void loop(void)
{
    byte i;
    pixels.setPixelColor(0, pixels.Color(255, 0, 0));
    pixels.setPixelColor(2, pixels.Color(0, 255, 0));
    pixels.setPixelColor(1, pixels.Color(92, 182, 255));
    pixels.show(); // Send the updated pixel colors to the hardware.
    delay(10);
    /* check whether the menu is active */
    if (mui.isFormActive())
    {
        /* if so, then draw the menu */
        if (is_redraw)
        {
            u8g2.clearBuffer();
            mui.draw();
            u8g2.sendBuffer();
            is_redraw = 0;
        }
        detect_events();
        handle_events();
        if (mui.isFormActive())
        {
            Tot_Sens_Present = number_input;
            interval_input = int_idx;
        }
        if (!mui.isFormActive())
            EEPROM.write(1, int_idx);
        updateInterval = get_interval_in_secs(int_idx);
        EEPROM.write(0, Tot_Sens_Present);
        EEPROM.commit();
        Serial.println(" Menu closed ");
    }
    else
    {
        // 0 = not pushed, 1 = pushed
        i = rotary.push();
        if (i == 1)
        {
            Serial.println("Pushed");
            if (!setf)
            {
                u8g2.clearBuffer();
                mui.gotoForm(/* form_id= */ 1, /* initial_cursor_position= */ 0);
                setf = 1;
            }
        }
        Wificonnect();
        monitor_server();
        Read_Sensor();
        monitorSD();
        monitor_battery();
        datalog();
        u8g2.clearBuffer();
        ctrl_icons();
        scroll_sensor_value();
        showLDcellValue();
        printLocalTime();
        delay(SCROLL_DELAY);
        u8g2.sendBuffer();
        setf = 0;
    }
}