// ===============================
// Проект: Калькулятор на Cardputer
// Автор: @ilalex
// Используем библиотеки:
//   - M5Cardputer (для дисплея, клавиатуры)
//   - splashIcon (иконка для сплешскрина)
// ===============================

#include <M5Cardputer.h>
#include "splashIcon.h"

String inputText = "0";
String resultText = "";
int selectedButton = 0;
bool lastEnterState = false;
bool lastDelState = false;
bool lastLeft = false;
bool lastRight = false;
bool lastArrowLeftPressed = false;
bool lastArrowRightPressed = false;

// Для всех возможных кодов символов ASCII (0–127)
bool lastDigitPressed[128] = { false };

const char* buttons[] = { "+", "-", "*", "/", "=" };
const int buttonCount = sizeof(buttons) / sizeof(buttons[0]);

void drawUI() {
  auto& display = M5Cardputer.Display;
  display.clear();

  // Поле ввода
  display.setCursor(10, 10);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print(inputText);

  // Кнопки
  int btnY = 85;
  int btnRadius = 16;
  int spacing = 15;
  int totalWidth = buttonCount * btnRadius * 2 + (buttonCount - 1) * spacing;
  int startX = (display.width() - totalWidth) / 2;

  for (int i = 0; i < buttonCount; i++) {
    int centerX = startX + i * (btnRadius * 2 + spacing) + btnRadius;
    int centerY = btnY + btnRadius;

    display.fillCircle(centerX, centerY, btnRadius, i == selectedButton ? YELLOW : DARKGREY);

    display.setTextColor(BLACK);
    display.setTextSize(2);
    int textWidth = 12;
    int textHeight = 16;
    display.setCursor(centerX - textWidth / 2, centerY - textHeight / 2);
    display.print(buttons[i]);
  }

  // Результат
  if (resultText.length() > 0) {
    display.setCursor(10, 40);
    display.setTextSize(2);
    display.setTextColor(GREEN);
    display.print("= ");
    display.print(resultText);
  }
}

void drawIconScaled2x(int x, int y, const uint16_t* iconData, int width, int height) {
  auto& display = M5Cardputer.Display;
  for (int j = 0; j < height; j++) {
    for (int i = 0; i < width; i++) {
      uint16_t color = iconData[j * width + i];
      int drawX = x + i * 2;
      int drawY = y + j * 2;
      display.fillRect(drawX, drawY, 2, 2, color);
    }
  }
}

// Парсер выражений с приоритетом операций
float parseExpression(const char* &expr);

float parseNumber(const char* &expr) {
  while (*expr == ' ') expr++;  // Пропуск пробелов

  char* end;
  float number = strtof(expr, &end);
  expr = end;

  return number;
}

float parseFactor(const char* &expr) {
  return parseNumber(expr);
}

float parseTerm(const char* &expr) {
  float value = parseFactor(expr);

  while (true) {
    while (*expr == ' ') expr++;
    if (*expr == '*') {
      expr++;
      value *= parseFactor(expr);
    } else if (*expr == '/') {
      expr++;
      float divisor = parseFactor(expr);
      if (divisor == 0) return NAN;
      value /= divisor;
    } else {
      break;
    }
  }

  return value;
}

float parseExpression(const char* &expr) {
  float value = parseTerm(expr);

  while (true) {
    while (*expr == ' ') expr++;
    if (*expr == '+') {
      expr++;
      value += parseTerm(expr);
    } else if (*expr == '-') {
      expr++;
      value -= parseTerm(expr);
    } else {
      break;
    }
  }

  return value;
}

float evaluateExpression(const String& expr) {
  const char* cstr = expr.c_str();
  return parseExpression(cstr);
}

void setup() {
  auto cfg = M5.config();
  M5Cardputer.begin(cfg, true);
  M5Cardputer.Display.setRotation(1);
  M5Cardputer.Display.setSwapBytes(true);

  // Сплэш-экран
  auto& display = M5Cardputer.Display;
  display.fillScreen(BLACK);

  int iconWidth = 32;
  int iconHeight = 32;
  int iconScale = 2;
  int iconX = (display.width() - iconWidth * iconScale) / 2;
  int iconY = 20;

  drawIconScaled2x(iconX, iconY, splashIconCalculator, iconWidth, iconHeight);

  // Подпись: Calculator
  display.setTextColor(WHITE);
  display.setTextSize(2);
  const char* title = "Calculator";
  int titleWidth = strlen(title) * 12;  // 6 пикселов * 2
  display.setCursor((display.width() - titleWidth) / 2, iconY + iconHeight * iconScale + 10);
  display.print(title);

  // Подпись: by ILALEX
  display.setTextSize(1);
  const char* subtitle = "by ILALEX";
  int subtitleWidth = strlen(subtitle) * 6;
  display.setCursor((display.width() - subtitleWidth) / 2, iconY + iconHeight * iconScale + 32);
  display.print(subtitle);

  delay(1500);

  drawUI();
}

void loop() {
  M5Cardputer.update();
  auto status = M5Cardputer.Keyboard.keysState();

  // Сброс кнопкой G0 (BtnA)
  if (M5.BtnA.wasPressed()) {
    inputText = "0";
    resultText = "";
    drawUI();
    return;
  }

  // Backspace
  if (status.del && !lastDelState) {
    if (resultText.length() > 0) {
      inputText = "0";
      resultText = "";
    } else {
      if (inputText.length() > 1)
        inputText.remove(inputText.length() - 1);
      else
        inputText = "0";
    }
    drawUI();
  }
  lastDelState = status.del;

  // Enter — выбираем кнопку
  if (status.enter && !lastEnterState) {
    String btn = buttons[selectedButton];
    if (btn == "=") {
      float res = evaluateExpression(inputText);
      resultText = isnan(res) ? "Error" : String(res);
    } else {
      if (inputText.endsWith("+") || inputText.endsWith("-") ||
          inputText.endsWith("*") || inputText.endsWith("/")) {
        inputText.remove(inputText.length() - 1);
      }
      inputText += btn;
      resultText = "";
    }
    drawUI();
  }
  lastEnterState = status.enter;

  // Стрелки ← (0x2C) и → (0x2F)
  bool leftPressed = false, rightPressed = false;

  for (char c : status.word) {
    if (c == 0x2C) leftPressed = true;
    if (c == 0x2F) rightPressed = true;
  }

  if (leftPressed && !lastArrowLeftPressed) {
    selectedButton = (selectedButton - 1 + buttonCount) % buttonCount;
    drawUI();
  }
  if (rightPressed && !lastArrowRightPressed) {
    selectedButton = (selectedButton + 1) % buttonCount;
    drawUI();
  }

  lastArrowLeftPressed = leftPressed;
  lastArrowRightPressed = rightPressed;

  // Ввод цифр и точки по одиночному нажатию
  for (char c : status.word) {
    if ((c >= '0' && c <= '9') || c == '.') {
      if (!lastDigitPressed[(int)c]) {
        if (resultText.length() > 0) {
          inputText = "";
          resultText = "";
        }
        if (inputText == "0") inputText = "";
        inputText += c;
        drawUI();
      }
      lastDigitPressed[(int)c] = true;
    }
  }

  // Сбросим флаги тех клавиш, которые больше не нажаты
  for (int i = 32; i < 127; ++i) {
    if (std::find(status.word.begin(), status.word.end(), (char)i) == status.word.end()) {
      lastDigitPressed[i] = false;
    }
  }

  delay(5);  // Мягкий отклик
}

