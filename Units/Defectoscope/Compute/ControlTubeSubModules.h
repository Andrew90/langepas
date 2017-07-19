#pragma once

///// \brief номер трубы по ком-порту
//struct NumberTube
//{
//	char value[8];
//};

///\breaf  Проверка температуры обмоток соленоида
void TestCoilTemperature();

///\breaf Проверка модуля размагничивания 
void CheckDemagnetizeModule();
///\ breaf Установка рабочего положения модулей контроля
void SettingWorkingPositionControlModules();
/// \breaf передача параметров в толщиномер
void TransferParametersThicknessModule();
/// \brief запрос результатов измерения из толщиномера
void GetDataFromThicknessModule();
/// \brief подготовка частотного преобразователя
void FrequencyInverterPreparation();
/// \brief запуск частотного преобразователя
void FrequencyInverterRun();
///TODO Установка режима работы контроллера пневмооборудования
void SettingOperatingModeAirConditioningController();
///TODO Очистка экрана
void CleaningScreen();
///TODO Запрос номера трубы
void RequestPipeNumber(char (&)[9]);
///TODO включить размагничивание
void EnableDemagnetization();
///TODO включить размагничивание
void DisableDemagnetization();
/// работа с АСУ
void WorkACS(char (&numberTube)[9]);
/// сохранение данных в базе
void StoredData(bool);


