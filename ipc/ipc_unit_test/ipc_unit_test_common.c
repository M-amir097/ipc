/*
 * Copyright (c) 2021, Nippon Seiki Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <ipc.h>
#include "ipc_unit_test_common.h"

IPC_UNIT_TEST_DATA_LIST IcServiceList[] = {
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, turnR),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, turnL),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, brake),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, seatbelt),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, frontRightSeatbelt),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, frontCenterSeatbelt),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, frontLeftSeatbelt),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, mid1RightSeatbelt),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, mid1CenterSeatbelt),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, mid1LeftSeatbelt),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, mid2RightSeatbelt),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, mid2CenterSeatbelt),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, mid2LeftSeatbelt),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, rearRightSeatbelt),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, rearCenterSeatbelt),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, rearLeftSeatbelt),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, highbeam),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, door),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, frontRightDoor),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, frontLeftDoor),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, rearRightDoor),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, rearLeftDoor),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, trunkDoor),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, hoodDoor),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, eps),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, srsAirbag),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, abs),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, lowBattery),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, oilPress),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, engine),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, fuel),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, immobi),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, tmFail),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, espAct),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, espOff),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, adaptingLighting),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, autoStop),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, autoStopFail),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, parkingLights),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, frontFog),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, exteriorLightFault),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, accFail),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, ldwOff),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, hillDescent),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, autoHiBeamGreen),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, autoHiBeamAmber),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, sportsMode),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, ldwOperate),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, generalWarn),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, drivingPowerMode),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, hotTemp),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, lowTemp),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, gearAtVal),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, gearMtVal),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, spAnalogVal),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, spAnaDigUnitVal),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, taAnalogVal),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, trcomTripAVal),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, trcomTripBVal),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, trcomOdoVal),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, trcomUnitVal),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, avgSpeedAVal),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, avgSpeedBVal),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, hourAVal),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, hourBVal),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, minuteAVal),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, minuteBVal),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, secondAVal),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, secondBVal),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, oTempVal),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, oTempUnitVal),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, cruRangeVal),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, avgFuelAVal),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, avgFuelBVal),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, insFuelAVal),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, insFuelBVal),
    DEFINE_STRUCT_DATA(IPC_DATA_IC_SERVICE_S, fuelEconomyUnitVal)
};

