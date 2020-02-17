/*
 DS1307 library example
*/

#include <DS1307.h>

DS1307 rtc;

void printTime()
{
    uint8_t year, month, day, hour, minute, second;
    format f;
    rtc.getTime(year, month, day, hour, minute, second, f);

    int y = year + 2000;
    char buf[30];
    sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d ", y, month, day, hour, minute, second);
    Serial.print(buf);
    String frm = f == F24 ? "" : f == FPM ? "PM" : "AM";
    Serial.println(frm);
}

void setup()
{
    Serial.begin(115200);

    rtc.begin();

    rtc.startSqw1hz();

    rtc.setTime(20, 12, 25, 22, 10, 10, F24);
    printTime();

    rtc.setPartTime(HOUR, 10, FPM);

    uint8_t val;
    format f;
    rtc.getPartTime(YEAR, val);
    Serial.print(20);
    Serial.print(val);
    Serial.print('-');
    rtc.getPartTime(MONTH, val);
    Serial.print(val);
    Serial.print('-');
    rtc.getPartTime(DAY, val);
    Serial.print(val);
    Serial.print(' ');
    rtc.getPartTime(HOUR, val, f);
    Serial.print(val);
    Serial.print(':');
    rtc.getPartTime(MINUTE, val);
    Serial.print(val);
    Serial.print(':');
    rtc.getPartTime(SECOND, val);
    Serial.print(val);
    Serial.print(' ');
    Serial.println(f == F24 ? "" : f == FPM ? "PM" : "AM");

    Serial.println();

    uint8_t ram[10];
    for (uint8_t i = 0; i < 10; i++)
        ram[i] = i;
    rtc.writeRam(ram, 0, 10);
    for (uint8_t i = 0; i < 10; i++)
        ram[i] = 0;
    rtc.readRam(ram, 0, 10);
    for (uint8_t i = 0; i < 10; i++)
        Serial.println(ram[i]);

    /*  day of week:
    use Jalali library: https://github.com/tondar20/Jalali

    uint8_t wday = jal.day_of_week(1398, 3, 30, JALALI);
    */

    /*  example increase day:
    void buttonPressed(keys k, events e)
    {
        if (k == keyInc && e == keyDown)
        {
            day++;
            if (day > 28 && !jal.is_date_valid(year + 2000, month, day, GREGORIAN))
                day = 1;
        }
    }
    */

    delay(5000);

    rtc.stopSqw1hz();
}

void loop()
{
}
