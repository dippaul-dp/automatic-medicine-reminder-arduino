/*
 * ==============================================================================
 * Project : Automatic Medicine Reminder Box using Arduino
 * Course  : Digital Logic Design Sessional (EEE 228)
 * Dept    : Biomedical Engineering, CUET
 * Authors : Dip Muhuri, Sifat Chowdhury, Nizam Uddin Babu, Dip Paul, Jannatul Mawa Taki
 * ==============================================================================
 */

#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal.h>

// Initialize DS3231 RTC and 16x2 LCD in 4-bit mode (RS, EN, D4, D5, D6, D7)
RTC_DS3231 rtc;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int buzzerPin = 13; // Output pin for active buzzer and alert LED

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  lcd.begin(16, 2);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC module. Check connections!");
    lcd.clear();
    lcd.print("RTC Not Found!");
    while (1);
  }

  // Synchronize RTC once if power is lost (or set manual timestamp)
  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Welcome screen sequence
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Digital Medicine");
  lcd.setCursor(0, 1);
  lcd.print("      BOX       ");
  delay(3000);
  lcd.clear();
}

void loop() {
  DateTime now = rtc.now();

  // Display default Live Date & Time screen
  lcd.setCursor(0, 0);
  lcd.print("Date: ");
  printTwoDigits(now.day());
  lcd.print('/');
  printTwoDigits(now.month());
  lcd.print('/');
  lcd.print(now.year(), DEC);

  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  printTwoDigits(now.hour());
  lcd.print(':');
  printTwoDigits(now.minute());
  lcd.print(':');
  printTwoDigits(now.second());

  // Verify scheduled alarm slots
  checkReminders(now);

  delay(1000); // 1-second refresh cycle
}

void checkReminders(DateTime now) {
  if (now.hour() == 1 && now.minute() == 16 && now.second() == 0) {
    displayReminder("Morning Medicine");
    activateBuzzer();
  } else if (now.hour() == 18 && now.minute() == 0 && now.second() == 0) {
    displayReminder("Evening Medicine");
    activateBuzzer();
  } else if (now.hour() == 22 && now.minute() == 0 && now.second() == 0) {
    displayReminder("Goodnight Med");
    activateBuzzer();
  }
}

void displayReminder(const char *reminder) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Reminder:");
  lcd.setCursor(0, 1);
  lcd.print(reminder);
}

void activateBuzzer() {
  digitalWrite(buzzerPin, HIGH);
  delay(30000); // Trigger alert tone for 30 seconds
  digitalWrite(buzzerPin, LOW);
  lcd.clear();
}

void printTwoDigits(int number) {
  if (number < 10) {
    lcd.print("0");
  }
  lcd.print(number, DEC);
}
