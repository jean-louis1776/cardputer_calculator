# 🧮 Calculator for 🖥️ M5Cardputer

**Author:** [@ilalex](https://github.com/jean-louis1776)

This project turns your **M5Cardputer** into a stylish **on-device calculator** with a clean UI, smooth key input, and real expression evaluation with operator precedence.

---

## 🚀 Features

- 🔢 Full expression parsing with +, −, ×, ÷ and operator priority
- 🖥️ Clean UI with live input and result display
- 🎨 Circular on-screen operator buttons with selection
- ⌨️ Keyboard input (digits, dot, arrows, enter, backspace)
- 📟 Splash screen with icon and credits
- 🔁 Reset via G0 (BtnA)

---

## 🧰 Hardware Requirements

- M5Stack Cardputer [Buy](https://shop.m5stack.com/products/m5cardputer)

---

## ⌨️ Controls

| Key | Action |
|-----|--------|
| `0–9`, `.` | Enter digits and decimal point |
| `← / →` | Switch selected operator button |
| `Enter` | Use selected operator or evaluate (`=`) |
| `Backspace` | Delete last character |
| `G0 (BtnA)` | Reset calculator |

---

## 🛠️ Libraries Used

Install via the Arduino Library Manager:

- [M5Cardputer](https://github.com/m5stack/M5Cardputer)

---

## 🖥️ UI Overview

- ✅ Large input and result text
- 🔘 On-screen operator buttons (`+`, `-`, `*`, `/`, `=`)
- 🎨 Highlighted selected button
- 👋 Splash screen with centered icon and title

---

## ⚙️ Expression Logic

Expressions are evaluated with **operator precedence**:

```text
2 + 3 * 4 → 14 (not 20)
```

Errors like division by zero show `"Error"`.

---

## ⚠️ Notes

- Result is cleared on new digit input
- Multiple operators in a row are replaced (e.g., `3++` becomes `3+`)
- Calculator starts at `"0"` and auto-clears on first input

---

## 📷 Demo

> _Coming soon_ – GIF or video demo of the calculator on M5Cardputer

---

## 🙌 Credits

- Expression parser based on recursive descent
- UI inspired by classic calculator layouts

---

## 📜 License

MIT – use it, fork it, customize it!

---

## 🧠 Future Ideas

- Memory buttons (M+, M-, MR)
- Theme switch (light/dark)
- History of recent calculations

---

Enjoy your **pocket calculator on the go**!  
Made with ❤️ by @ilalex
