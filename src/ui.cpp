#include "ui.hpp"

#include <ctime>
#include <chrono>

static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
static ID3D11Texture2D* g_pTexture = nullptr;
static ID3D11ShaderResourceView* g_pTextureView = nullptr;
static UINT g_ResizeWidth = 0, g_ResizeHeight = 0;


void ui()
{

    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);

    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"imgui",WS_POPUP, 0, 0, 2560, 1440, nullptr, nullptr, wc.hInstance, nullptr);//隐藏标题

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
    }

    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    ImGui_ImplWin32_EnableAlphaCompositing(hwnd);

    LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    exStyle |= WS_EX_LAYERED;
    SetWindowLong(hwnd, GWL_EXSTYLE, exStyle);

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;


    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    io.Fonts->AddFontDefault();

    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);


    bool done = false;

    unsigned int width = Buffer::GetInstance()->GetWidth();
    unsigned int height = Buffer::GetInstance()->GetHeight();
    unsigned char* color_data = new unsigned char[width * height * 4];


    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = Buffer::GetInstance()->GetWidth();
    desc.Height = Buffer::GetInstance()->GetHeight();
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem = color_data;
    subResource.SysMemPitch = desc.Width * 4;
    subResource.SysMemSlicePitch = 0;

    while (!done)
    {
        using std::cout,std::endl;
		using std::chrono::duration_cast,std::chrono::system_clock;
		using std::chrono::milliseconds,std::chrono::seconds;
		auto millisec_since_epoch =
		duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        std::cout<<"Prepare: "<<duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()-millisec_since_epoch<<" ms "<<std::endl;
		millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

        Scene::GetInstance()->Update(); //16ms

        std::cout<<"Update: "<<duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()-millisec_since_epoch<<" ms "<<std::endl;
		millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

        for(int i = 0; i < height; i++)
        {
            for(int j = 0; j < width; j++)
            {
                Color color = Buffer::GetInstance()->GetColor(j, i);
                color_data[(i * width + j) * 4] = (unsigned char)(color.r * 255);
                color_data[(i * width + j) * 4 + 1] = (unsigned char)(color.g * 255);
                color_data[(i * width + j) * 4 + 2] = (unsigned char)(color.b * 255);
                color_data[(i * width + j) * 4 + 3] = (unsigned char)(color.a * 255);
            }
        }
        
	    

        g_pd3dDevice->CreateTexture2D(&desc, &subResource, &g_pTexture);
        g_pd3dDevice->CreateShaderResourceView(g_pTexture, NULL, &g_pTextureView);

        std::cout<<"CreateTexture2D: "<<duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()-millisec_since_epoch<<" ms "<<std::endl;
		millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

        ImGui::Begin("Render");
        ImGui::Text("width: %d, height: %d", width, height);
        ImGui::Text("vertex_count: %d", RenderPipeline::GetInstance()->vertex_count);
        ImGui::Text("face_count: %d", RenderPipeline::GetInstance()->face_count);
        ImGui::Text("fragment_count: %d", RenderPipeline::GetInstance()->fragment_count);
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Button("Quit");
        if (ImGui::IsItemClicked())
        {
            done = true;
        }
        ImGui::Image((void*)g_pTextureView, ImVec2((float)width / 2, (float)height / 2));
        
        ImGui::End();

        ImGui::Begin("Control");

        ImGui::Text("GameObject");
        Vector3f gameObjectPosition = Scene::GetInstance()->GetGameObject(0)->GetComponent<Transform>()->position;
        ImGui::SliderFloat("GameObject.X", &gameObjectPosition.x, -10.0f, 10.0f);
        ImGui::SliderFloat("GameObject.Y", &gameObjectPosition.y, -10.0f, 10.0f);
        ImGui::SliderFloat("GameObject.Z", &gameObjectPosition.z, -10.0f, 10.0f);
        Scene::GetInstance()->GetGameObject(0)->GetComponent<Transform>()->position = gameObjectPosition;

        // Vector3f gameObjectRotation = Scene::GetInstance()->GetGameObject(0)->GetComponent<Transform>()->rotation;
        // ImGui::SliderFloat("RX", &gameObjectRotation.x, -10.0f, 10.0f);
        // ImGui::SliderFloat("RY", &gameObjectRotation.y, -10.0f, 10.0f);
        // ImGui::SliderFloat("RZ", &gameObjectRotation.z, -10.0f, 10.0f);
        // Scene::GetInstance()->GetGameObject(0)->GetComponent<Transform>()->rotation = gameObjectRotation;

        Vector3f gameObjectScale = Scene::GetInstance()->GetGameObject(0)->GetComponent<Transform>()->scale;
        ImGui::SliderFloat("SX", &gameObjectScale.x, 0.0f, 1.0f);
        ImGui::SliderFloat("SY", &gameObjectScale.y, 0.0f, 1.0f);
        ImGui::SliderFloat("SZ", &gameObjectScale.z, 0.0f, 1.0f);
        Scene::GetInstance()->GetGameObject(0)->GetComponent<Transform>()->scale = gameObjectScale;

        ImGui::Text("Camera");

        Vector3f cameraPosition = Scene::GetInstance()->GetCamera()->GetComponent<Transform>()->position;
        ImGui::SliderFloat("X", &cameraPosition.x, -10.0f, 10.0f);
        ImGui::SliderFloat("Y", &cameraPosition.y, -10.0f, 10.0f);
        ImGui::SliderFloat("Z", &cameraPosition.z, -10.0f, 10.0f);
        Scene::GetInstance()->GetCamera()->GetComponent<Transform>()->position = cameraPosition;

        ImGui::Text("CameraPosition : (%f, %f, %f)", cameraPosition.x, cameraPosition.y, cameraPosition.z);


        Vector3f lightPosition = Scene::GetInstance()->GetLight()->GetComponent<Transform>()->position;
        ImGui::SliderFloat("LightX", &lightPosition.x, -10.0f, 10.0f);
        ImGui::SliderFloat("LightY", &lightPosition.y, -10.0f, 10.0f);
        ImGui::SliderFloat("LightZ", &lightPosition.z, -10.0f, 10.0f);
        Scene::GetInstance()->GetLight()->GetComponent<Transform>()->position = lightPosition;

        ImGui::Text("LightPosition : (%f, %f, %f)", lightPosition.x, lightPosition.y, lightPosition.z);

        ImGui::End();

        ImGui::Render();

        const float clear_color_with_alpha[4] = { 0, 0, 0, 0 };
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);

        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0); 
    }

    if (g_pTexture) g_pTexture->Release();
    if (g_pTextureView) g_pTextureView->Release();
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

}

bool CreateDeviceD3D(HWND hWnd)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) 
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); 
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) 
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

