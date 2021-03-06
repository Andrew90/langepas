﻿#pragma once
#include "DlgTemplates\ParamDlg.hpp"

/*
PARAM_TITLE(iСontrolСircuits, L"Цепи управления")
PARAM_TITLE(iCycle          , L"Цикл")
PARAM_TITLE(iReady          , L"Готовность")
PARAM_TITLE(iControl        , L"Контроль")
PARAM_TITLE(iBase           , L"База")
PARAM_TITLE(iReserve        , L"Резерв")       

PARAM_TITLE(oWork      , L"Работа")
PARAM_TITLE(oToShiftThe, L"Перекладка")
PARAM_TITLE(oResult1   , L"Результат1")
PARAM_TITLE(oResult2   , L"Результат2")
PARAM_TITLE(oPowerBM   , L"Питание БМ")
PARAM_TITLE(oReserve   , L"Резерв")

PARAM_TITLE(iCross0, L"Вход поперечного модуля")
PARAM_TITLE(iCross1, L"Выход поперечного модуля")
PARAM_TITLE(iLong0 , L"Вход продольного модуля")
PARAM_TITLE(iLong1 , L"Выход продольного модуля")
PARAM_TITLE(iThick0, L"Вход толщиномера")
PARAM_TITLE(iThick1, L"Выход толщиномера")

PARAM_TITLE(iSolid, L"Труба в модуле группы прочности");
*/

PARAM_TITLE(iSQ1pr   , L"\"SQ1pr\"-Датчик наличия трубы в продольный модуль")
PARAM_TITLE(iSQ2pr   , L"\"SQ2pr\"-Датчик наличия трубы на выходе из продольного модуля")
PARAM_TITLE(iRPpr    , L"\"RPpr\"-Продольный модуль в рабочем положении")
PARAM_TITLE(iOPpr    , L"\"OPpr\"-Продольный модуль в положении обслуживания")
PARAM_TITLE(iSQ1po   , L"\"SQ1po\"-Датчик наличия трубы на выходе в поперечный модуль")
PARAM_TITLE(iSQ2po   , L"\"SQ2po\"-Датчик наличия трубы на выходе из поперечного модуля")
PARAM_TITLE(iRPpo    , L"\"RPpo\"-Поперечный модуль в рабочем положении")
PARAM_TITLE(iOPpo    , L"\"OPpo\"-Поперечный модуль в положении обслуживания")
PARAM_TITLE(iSQ1t    , L"\"SQ1t\"-Датчик наличия трубы на входе в модуль толщины")
PARAM_TITLE(iSQ2t    , L"\"SQ2t\"-Датчик наличия трубы на выходе из модуля толщины")
PARAM_TITLE(iRPt     , L"\"RPt\"-Модуль толщины в рабочем положении")
PARAM_TITLE(iOPt     , L"\"Opt\"-Модуль толщины в положении обслуживания")
PARAM_TITLE(iReadyT  , L"\"ReadyT\"-Шкаф толщины в состоянии готовности")
PARAM_TITLE(iControlT, L"\"ControlT\"-Шкаф толщины выполняет контроль")
PARAM_TITLE(iResultT , L"\"ResultT\"-Шкаф толщины сформировал результат контроля")
PARAM_TITLE(iReserve , L"\"Reserve\"-Резервный сигнал из шкафа толщины")



PARAM_TITLE(iZU           , L"\"ZU\"-Признак включения цепей управления")
PARAM_TITLE(iPCH_B        , L"\"PCH_B\"-Сигнал состояния частотного преобразователя(В)")
PARAM_TITLE(iPCH_RUN      , L"\"PCH_RUN\"-Сигнал состояния частотного преобразователя(Run)")
PARAM_TITLE(iPCH_A        , L"\"PCH_A\"-Сигнал состояния частотного преобразователя(А)")
PARAM_TITLE(iCycle        , L"\"Cycle\"-Сигнал ЦИКЛ из транспортной системы")
PARAM_TITLE(iReadyR1      , L"\"ReadyR1\"-Наличие готовой к контролю трубы на входном рольганге")
PARAM_TITLE(iDone         , L"\"Done\"-Операция выполнена(из транспортной системы)")
PARAM_TITLE(iReserve0     , L"\"Reserve0\"-Резервный сигнал из транспортной системы")
PARAM_TITLE(iEtalon       , L"\"Etalon\"-Сигнал ЭТАЛОН из транспортной системы")
PARAM_TITLE(iWork_pnevmo  , L"\"Work_pnevmo\"-Признак рабочего режима контроллера пневмооборудования")
PARAM_TITLE(iRevers_pnevmo, L"\"Revers_pnevmo\"-Признак режима реверс контроллера пневмооборудования")
PARAM_TITLE(iError_pnevmo , L"\"Error_pnevmo\"-Признак аварии контроллера пневмооборудования")
PARAM_TITLE(iReserve1     , L"\"Reserve1\"-Резервный выход ")
PARAM_TITLE(iReserve2     , L"\"Reserve2\"-Резервный выход")
PARAM_TITLE(iSQ1DM        , L"\"SQ1DM\"-Датчик паличия трубы на входе в размагничивание")
PARAM_TITLE(iSQ2DM        , L"\"SQ2DM\"-Датчик паличия трубы на выходе в размагничивание")



PARAM_TITLE(oPowerPCH  , L"\"PowerPCH\"-Включение питания частотного преобразователя")
PARAM_TITLE(oRL        , L"\"RL\"-Млатший бит кода частоты вращения")
PARAM_TITLE(oRM        , L"\"RM\"-Средний бит кода частоты вращнния")
PARAM_TITLE(oRH        , L"\"RH\"-Старший бит кода скорости вращения")
PARAM_TITLE(oSTF       , L"\"ST\"-Включение вращения продольного модуля")
PARAM_TITLE(oPowerSU   , L"\"PowerSU\"-Включение питания сканирующего устройства продольного модуля")
PARAM_TITLE(oMagnet    , L"\"Magnet\"-Включение намагничивания поперечного модуля")
PARAM_TITLE(oCooling   , L"\"Cooling\"-Включение обдува соленоидов поперечного модуля")
PARAM_TITLE(oReloc1    , L"\"Reloc1\"-Сигнал перекладка1 в транспортную систему")
PARAM_TITLE(oRP        , L"\"RP\"-Сигнал рабочего положения модулей в транспортную систему")
PARAM_TITLE(oOP        , L"\"OP\"-Сигнал положения обслуживания модулей в транспортную систему")
PARAM_TITLE(oDefect    , L"\"Defect\"-Признак брака в транспортную систему")
PARAM_TITLE(oReloc2    , L"\"Reloc2\"-Сигнал перекладка2 в транспортную систему")
PARAM_TITLE(oWorkPR    , L"\"WorkPR\"-Признак работы продольного модуля в транспортную систему")
PARAM_TITLE(oWorkPO    , L"\"WorkPO\"-Признак работы поперечного модуля в транспортную систему")
PARAM_TITLE(oWorkT 	   , L"\"WorkT\"-Признак работы модуля толщины в транспортную систему")
					 

PARAM_TITLE(oPR_OP     , L"\"PR_OP\"-Перевод продольного модуля в положение обслуживания")
PARAM_TITLE(oPR_RP     , L"\"PR_RP\"-Перевод продольного модуля в рабочее положени")
PARAM_TITLE(oPO_OP     , L"\"PO_OP\"-Перевод поперечного модуля в положение обслуживания")
PARAM_TITLE(oPO_RP     , L"\"PO_RP\"-Перевод поперечного модуля в рабочее положение")
PARAM_TITLE(oT_OP      , L"\"T_OP\"-Перевод модуля толщины в положение обслуживания")
PARAM_TITLE(oT_RP      , L"\"T_RP\"-Перевод модуля толщины в рабочее положение")
PARAM_TITLE(oReserve0  , L"\"Reserve0\"-Резерв")
PARAM_TITLE(oReserve1  , L"\"Reserve1\"-Резерв")
PARAM_TITLE(oT_Work    , L"\"T_Work\"-Сигнал РАБОТА в шкаф толщины")
PARAM_TITLE(oT_Cycle   , L"\"T_Cycle\"-Сигнал ЦИКЛ в шкаф толщины")
PARAM_TITLE(oT_Base    , L"\"T_Base\"-Сигнал БАЗА в шкаф толщины")
PARAM_TITLE(oT_reserv  , L"\"T_reserv\"-Резервный сигнал в шкаф толщины")
PARAM_TITLE(oReserve2  , L"\"Reserve2\"-Резерв")
PARAM_TITLE(oReserve3  , L"\"Reserve3\"-Резерв")
PARAM_TITLE(oReserve4  , L"\"Reserve4\"-Резерв")
PARAM_TITLE(oReserve5  , L"\"Reserve5\"-Резерв")