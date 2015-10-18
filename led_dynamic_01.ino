/*
 * Arduino でダイナミック点灯
 * LED 6 個を 2 個ずつアノードコモンで接続
 * C++11 を使用
 */

#include <MsTimer2.h>

// 配列の要素数を返す
// 参照：http://cpp.aquariuscode.com/array_size_template
template<typename Array, int N>
constexpr int arraySize(Array (&)[N]) {
  return N;
}

// LED の点灯状態を送るピン
constexpr int DATA_PINS[] = {8, 9};
// 共通のアノードを接続するピン
constexpr int ANODE_PINS[] = {11, 12, 13};

constexpr int DATA_SIZE = arraySize(DATA_PINS);
constexpr int NUM_OF_ANODES = arraySize(ANODE_PINS);

// LED の点灯パターン
constexpr int LED_PATTERNS[][NUM_OF_ANODES][DATA_SIZE] = {
  { {0, 1}, {1, 1}, {1, 1} },
  { {1, 1}, {0, 1}, {1, 1} },
  { {1, 1}, {1, 1}, {0, 1} },
  { {1, 1}, {1, 1}, {1, 0} },
  { {1, 1}, {1, 0}, {1, 1} },
  { {1, 0}, {1, 1}, {1, 1} },

  { {0, 1}, {0, 1}, {1, 1} },
  { {1, 1}, {0, 1}, {0, 1} },
  { {1, 1}, {1, 1}, {0, 0} },
  { {1, 1}, {1, 0}, {1, 0} },
  { {1, 0}, {1, 0}, {1, 1} },
  { {0, 0}, {1, 1}, {1, 1} }
};

constexpr int NUM_OF_LED_PATTERNS = arraySize(LED_PATTERNS);

// 現在の LED 点灯パターン
int current_led_pattern = 0;
// 現在のアノード
int current_anode = 0;

void setup() {
  // 必要なピンを出力に設定する
  for (auto pin : DATA_PINS) {
    pinMode(pin, OUTPUT);
  }

  for (auto pin : ANODE_PINS) {
    pinMode(pin, OUTPUT);
  }

  MsTimer2::set(4, dynamicLight);
  MsTimer2::start();
}

void loop() {
  // LED パターンを次へ進める
  ++current_led_pattern;
  if (current_led_pattern >= NUM_OF_LED_PATTERNS) {
    current_led_pattern = 0;
  }

  delay(200);
}

// ダイナミック点灯を行う
void dynamicLight() {
  // アノードのオン・オフを切り替える
  for (int i = 0; i < NUM_OF_ANODES; ++i) {
    digitalWrite(ANODE_PINS[i], i == current_anode);
  }

  // アノードに対応する点灯パターンを出力する
  for (int i = 0; i < DATA_SIZE; ++i) {
    digitalWrite(DATA_PINS[i],
                 LED_PATTERNS[current_led_pattern][current_anode][i]);
  }

  // オンにするアノードを移動する
  ++current_anode;
  if (current_anode >= NUM_OF_ANODES) {
    current_anode = 0;
  }
}

