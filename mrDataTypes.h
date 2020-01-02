#pragma once

typedef char mrInt8;
typedef unsigned char mrUInt8;
typedef short mrInt16;
typedef unsigned short mrUInt16;
typedef long mrInt32;
typedef unsigned long mrUInt32;
typedef int mrInt;
typedef unsigned int mrUInt;

typedef float mrReal32;
typedef double mrReal64;

enum mrBool32
{
	mrFalse = 0,
	mrTrue = 1,

	mrBool32_Force32 = 0xFFFFFFFF
};

enum mrError32
{
	mrNoError = 0,
	mrErrorRegisterClass = 1,
	mrErrorInitDirect3D = 2,
	mrErrorGetAdapterDisplayMode = 3,
	mrErrorCreateDevice = 4,
	mrErrorClear = 5,
	mrErrorBeginScene = 6,
	mrErrorEndScene = 7,
	mrErrorPresent = 8,
	mrErrorDrawPrimitive = 9,
	mrErrorNotBitmapFile = 10,
	mrErrorBitmapNotSupported = 11,
	mrErrorTargaNotSupported = 12,
	mrErrorInvalidRawImage = 13,
	mrErrorCreateImageSurface = 14,
	mrErrorCreateTexture = 15,
	mrErrorDInputCreate = 16,
	mrErrorKeyboardCreateDevice = 17,
	mrErrorKeyboardSetDataFormat = 18,
	mrErrorKeyboardSetCooperativeLevel = 19,
	mrErrorKeyboardAcquire = 20,
	mrErrorKeyboardGetDeviceData = 21,
	mrErrorMouseCreateDevice = 22,
	mrErrorMouseSetDataFormat = 23,
	mrErrorMouseSetCooperativeLevel = 24,
	mrErrorMouseSetProperty = 25,
	mrErrorMouseGetDeviceData = 26,
	mrErrorJoystickEnumDevices = 27,
	mrErrorJoystickSetDataFormat = 28,
	mrErrorJoystickSetCooperativeLevel = 29,
	mrErrorJoystickSetProperty = 30,
	mrErrorJoystickAcquire = 31,	
	mrErrorJoystickGetDeviceState = 32,
	mrErrorCreateSoundDevice = 33,
	mrErrorSetCoperativeLevel = 34,
	mrErrorWaveNotSupported = 35,
	mrErrorCreateSoundBuffer = 36,
	mrErrorSoundBufferLock = 37,
	mrErrorSoundSetVolume = 38,
	mrErrorPlay = 39,
	mrErrorStop = 40,

	mrError32_Force32 = 0xFFFFFFFF
};
