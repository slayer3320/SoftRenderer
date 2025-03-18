#pragma once

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx11.h"
#include "gameobject/Camera.hpp"
#include "gameobject/Light.hpp"
#include "Scene.hpp"
#include "pipeline/RenderPipeline.hpp"
#include "pipeline/Buffer.hpp"
#include <d3d11.h>
#include <dwmapi.h>
#include <tchar.h>

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
void ui();