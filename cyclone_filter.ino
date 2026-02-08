// Определяем пин, к которому подключен провод тахометра.
// Для Arduino Uno/Nano/Mega пин 2 соответствует прерыванию 0 (INT0).
const int tachometerPin = 2;
// Переменная для хранения количества импульсов, подсчитанных с момента
volatile unsigned long pulseCount = 0;
// Константа, определяющая количество импульсов за один оборот.
const float pulsesPerRevolution = 2.0;
// Переменная для хранения вычисленной скорости вращения в об/мин.
float rpm = 0.0;
// Время последнего измерения (для расчета частоты).
unsigned long lastMeasureTime = 0;
// Интервал, через который будут производиться расчеты (в миллисекундах).
const unsigned long measureInterval = 1000;
/**
* Функция-обработчик прерывания.
* Вызывается каждый раз, когда тахометр генерирует импульс (нарастающий фронт).
*/
void countPulse() {
   // Увеличиваем счетчик импульсов.
 pulseCount++;
}
void setup() {
 // Инициализация последовательного порта для вывода данных.
 Serial.begin(9600);
 Serial.println("Инициализация измерения RPM...");
 // Настройка пина тахометра как входа.
 pinMode(tachometerPin, INPUT);

 // Активация подтягивающего резистора (pull-up) на входном пине.
 digitalWrite(tachometerPin, HIGH);
 // Подключение обработчика прерывания:
 attachInterrupt(digitalPinToInterrupt(tachometerPin), countPulse, RISING);
 lastMeasureTime = millis();
}
void loop() {
 // Проверяем, прошло ли достаточно времени для нового измерения.
 if (millis() - lastMeasureTime >= measureInterval) {

 // Временно сохраняем счетчик импульсов в безопасной переменной
 noInterrupts();
 unsigned long currentPulseCount = pulseCount;
 pulseCount = 0; // Сбрасываем счетчик для следующего интервала.
 interrupts();
 // Расчет RPM:
 // (Количество_импульсов / Импульсы_за_оборот) * (60_секунд /
 // Так как measureInterval в мс (1000), Время_измерения_в_секундах = measureInterval
 // Формула упрощается:
 // rpm = (currentPulseCount / pulsesPerRevolution) * 60;

 // Более точный расчет с учетом фактического прошедшего времени:
 unsigned long timeElapsed = millis() - lastMeasureTime;

 // Расчет RPM:
 // (currentPulseCount / pulsesPerRevolution) - это количество оборотов.
 // Делим на время в минутах: (timeElapsed / 1000.0 / 60.0)
 rpm = (currentPulseCount / pulsesPerRevolution) * (60000.0 / timeElapsed);

 // Выводим результат.
 Serial.print("RPM: ");
 Serial.println(rpm);

 // Обновляем время последнего измерения.
 lastMeasureTime = millis();
 }
}
