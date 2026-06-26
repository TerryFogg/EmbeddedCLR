#pragma once
//
// Copyright (c) .NET Foundation and Contributors
// Portions Copyright (c) Microsoft Corporation.  All rights reserved.
// See LICENSE file in the project root for full license information.
//
#include "Graphics.h"

enum TouchPanel_Flags
{
    TouchPanelDown = 0x1,
    TouchPanelUp = 0x2,
    TouchPanelMove = 0x3,
};
//enum TouchPointContactFlags
//{
//    TouchPointContactFlags_Primary = 0x80000000,
//};
enum InternalFlags
{
    Contact_Down = 0x1,
    Contact_WasDown = 0x2,
};
enum SimpleTouchGesture
{
    NoGesture = 0, // Used to indicate we haven't moved far enough
    Right = 1,
    UpRight = 2,
    Up = 3,
    UpLeft = 4,
    Left = 5,
    DownLeft = 6,
    Down = 7,
    DownRight = 8
};
struct TouchPoint
{
    /// Location is a composite of  bits 0-13: x, 14-27: y
    CLR_UINT32 location;
    /// Contact is a composite of  bits 0-13: width, 14-27: height, 28-31: flags
    //CLR_UINT32 contact;
    CLR_INT64 time;
};

class TouchPanel
{
  public:
    static HRESULT Initialize();
    static HRESULT Uninitialize();
    static SimpleTouchGesture GestureDetect(CLR_INT16 x, CLR_INT16 y);
  private:
    static void TouchIsrProc(GPIO_PIN pin, bool pinState, void *pArg);
    static void TouchCompletion(void *arg);
    void PollTouchPoint();
    TouchPoint *AddTouchPoint(CLR_UINT16 x, CLR_UINT16 y, CLR_INT64 time);
    HAL_COMPLETION m_touchCompletion;
    CLR_INT32 m_InternalFlags;
    CLR_INT32 m_head;
    CLR_INT32 m_tail;

};
