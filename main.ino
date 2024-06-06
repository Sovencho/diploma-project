const int photoresistorPin = A0; // Пин для фоторезистора
const int relayPin = 4;          // Пин для релейного модуля
const int threshold = 500;       // Пороговое значение освещенности
const unsigned long debounceDelay = 40000; // Задержка таймера в миллисекундах (40 секунд)

bool relayState = LOW;           // Текущее состояние реле
unsigned long timerStart = 0;    // Время начала отсчета таймера
bool timerActive = false;        // Флаг активности таймера

void setup() {
  pinMode(photoresistorPin, INPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, relayState);  // Устанавливаем начальное состояние реле
  Serial.begin(9600);                  // Инициализация последовательного порта для отладки
}

void loop() {
  int lightLevel = analogRead(photoresistorPin); // Чтение значения с фоторезистора
  Serial.print("Light Level: ");
  Serial.println(lightLevel);  // Вывод значения освещенности в сериал монитор

  // Определение текущего состояния освещенности
  bool currentLightState = (lightLevel > threshold) ? LOW : HIGH;

  // Определение желаемого состояния реле на основе текущего уровня освещенности
  bool desiredRelayState = (lightLevel > threshold) ? LOW : HIGH;

  if (desiredRelayState != relayState && !timerActive) {
    // Если состояние освещенности изменилось и таймер не активен
    timerStart = millis();     // Запуск таймера
    timerActive = true;
  }

  if (timerActive) {
    if (millis() - timerStart >= debounceDelay) {
      // Если прошло 40 секунд и состояние освещенности не изменилось обратно
      relayState = desiredRelayState;    // Меняем состояние реле
      digitalWrite(relayPin, relayState);
      timerActive = false;  // Сброс таймера
    } else if (desiredRelayState == relayState) {
      // Если освещенность вернулась в изначальное состояние
      timerActive = false;  // Сброс таймера
    }
  }
  
  delay(100); // Небольшая задержка для предотвращения дребезга
}
