const uint8_t LED_RED_PIN = 0;
const uint8_t LED_GRN_PIN = 1;
const uint8_t LED_BLU_PIN = 4;

const uint8_t LED_WRN_PIN = 2;

const uint8_t LED_TST_PIN = A3;

const uint8_t WARNING_THRESHOLD = 10;

const uint8_t LED_MAX = 255;

const uint8_t LED_DELAY = 25;

//const uint8_t PROGMEM gamma8[256] = {
//  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
//  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
//  2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
//  5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
//  10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
//  17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
//  25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
//  37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
//  51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
//  69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
//  90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
//  115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
//  144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
//  177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
//  215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
//};


//             attiny85
//               _____
// RST        - |.    | -  VCC
// TST (3/A3) - |     | -  WRN (2/A1)
// BLU (4/A2) - |     | -  GRN (1)
// GND        - |_____| -  RED (0)


typedef struct RgbColor
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
} RgbColor;

//typedef struct HsvColor
//{
//  unsigned char h;
//  unsigned char s;
//  unsigned char v;
//} HsvColor;

RgbColor rgb = {0, 0, 0};

uint16_t hue = 0;

RgbColor hueToRgb(uint16_t hue) {

  float transition = (1.0 - abs(fmod(hue / 60.0, 2) - 1.0));
  transition = transition > 1.0 ? 1.0 : transition;
  transition = transition < 0.0 ? 0.0 : transition;

  float r, g, b;

  if (hue >= 0 && hue < 60) {
    r = 1.0, g = transition, b = 0.0;
  }
  else if (hue >= 60 && hue < 120) {
    r = transition, g = 1.0, b = 0.0;
  }
  else if (hue >= 120 && hue < 180) {
    r = 0.0, g = 1.0, b = transition;
  }
  else if (hue >= 180 && hue < 240) {
    r = 0.0, g = transition, b = 1.0;
  }
  else if (hue >= 240 && hue < 300) {
    r = transition, g = 0.0, b = 1.0;
  } else {
    r = 1.0, g = 0.0, b = transition;
  }

  uint8_t R = r * 255;
  uint8_t G = (g / 1.5) * 255;
  uint8_t B = b * 255;

  RgbColor rgb = {R, G, B};

  return rgb;
}

void write_led_pins() {
  //  analogWrite(LED_RED_PIN, pgm_read_byte(&gamma8[rgb.r]));
  //  analogWrite(LED_GRN_PIN, pgm_read_byte(&gamma8[rgb.g]));
  //  analogWrite(LED_BLU_PIN, pgm_read_byte(&gamma8[rgb.b]));
  analogWrite(LED_RED_PIN, rgb.r);
  analogWrite(LED_GRN_PIN, rgb.g);
  analogWrite(LED_BLU_PIN, rgb.b);
}

void setup() {
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GRN_PIN, OUTPUT);
  pinMode(LED_BLU_PIN, OUTPUT);

  pinMode(LED_TST_PIN, INPUT);

  pinMode(LED_WRN_PIN, OUTPUT);

  digitalWrite(LED_WRN_PIN, 0);

  write_led_pins();
}

void warning_mode() {
  rgb = {0, 0, 0};
  write_led_pins();

  while (true) {
    digitalWrite(LED_WRN_PIN, 1);
    delay(1000);
    digitalWrite(LED_WRN_PIN, 0);
    delay(1000);
  }
}

void check_leds() {
  uint8_t tst = analogRead(LED_TST_PIN);

  if (tst < WARNING_THRESHOLD) {
    warning_mode();
  }
}

bool unique_max() {
  bool r_max = rgb.r > 100;
  bool r_min = rgb.r == 0;

  bool g_max = rgb.g > 100;
  bool g_min = rgb.g == 0;

  bool b_max = rgb.b > 100;
  bool b_min = rgb.b == 0;

  return (r_max && g_min && b_min) || (r_min && g_max && b_min) || (r_min && g_min && b_max);
}

void loop() {
  hue += 1;

  if (hue >= 360) {
    hue = 0;
  }

  rgb = hueToRgb(hue);
  write_led_pins();
  delay(LED_DELAY);

  if (unique_max()) {
    digitalWrite(LED_WRN_PIN, 1);
    check_leds();
  } else {
    digitalWrite(LED_WRN_PIN, 0);
  }
}
