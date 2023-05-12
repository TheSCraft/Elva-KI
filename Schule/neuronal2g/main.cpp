#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include <d3d12.h>
#include <dxgi1_4.h>
#include <tchar.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <time.h>

using namespace std;
vector<string> explode(const string& delimiter, const string& explodeme);
double sigmoid(double x);
double dSigmoid(double x);
double zufallszahl();
void mischen(int* array, size_t n);
void neuro(double lerngenauichkeit,int ein,bool trai ,int anzahllernzyclen, int anzahlversteckterneuroneneins, int anzahlversteckterneuronenzwei, string datein,char c,int out,bool prog);
#ifdef _DEBUG
#define DX12_ENABLE_DEBUG_LAYER
#endif

#ifdef DX12_ENABLE_DEBUG_LAYER
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif

struct FrameContext
{
    ID3D12CommandAllocator* CommandAllocator;
    UINT64                  FenceValue;
};

// Data
static int const                    NUM_FRAMES_IN_FLIGHT = 3;
static FrameContext                 g_frameContext[NUM_FRAMES_IN_FLIGHT] = {};
static UINT                         g_frameIndex = 0;

static int const                    NUM_BACK_BUFFERS = 3;
static ID3D12Device* g_pd3dDevice = NULL;
static ID3D12DescriptorHeap* g_pd3dRtvDescHeap = NULL;
static ID3D12DescriptorHeap* g_pd3dSrvDescHeap = NULL;
static ID3D12CommandQueue* g_pd3dCommandQueue = NULL;
static ID3D12GraphicsCommandList* g_pd3dCommandList = NULL;
static ID3D12Fence* g_fence = NULL;
static HANDLE                       g_fenceEvent = NULL;
static UINT64                       g_fenceLastSignaledValue = 0;
static IDXGISwapChain3* g_pSwapChain = NULL;
static HANDLE                       g_hSwapChainWaitableObject = NULL;
static ID3D12Resource* g_mainRenderTargetResource[NUM_BACK_BUFFERS] = {};
static D3D12_CPU_DESCRIPTOR_HANDLE  g_mainRenderTargetDescriptor[NUM_BACK_BUFFERS] = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
void WaitForLastSubmittedFrame();
FrameContext* WaitForNextFrameResources();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

constexpr auto anzahleingangsneuronen = 26;
constexpr auto anzahlausgangsneuronen = 2;
constexpr auto anzahltraningsdaten = 26;
constexpr auto sim = 100;
double outputone = 0;
double outputtwo = 0;
// Main code
int main(int, char**)
{
    
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"ImGui Example", NULL };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Neuronales Netzwerk", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX12_Init(g_pd3dDevice, NUM_FRAMES_IN_FLIGHT,
        DXGI_FORMAT_R8G8B8A8_UNORM, g_pd3dSrvDescHeap,
        g_pd3dSrvDescHeap->GetCPUDescriptorHandleForHeapStart(),
        g_pd3dSrvDescHeap->GetGPUDescriptorHandleForHeapStart());


    
    
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main Guiloop
    bool done = false;
    bool acktivwin = false;
    static int outt = 10;

    while (!done)
    {
       
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        static int zyklen = 0, groe = 1, einsneur = 1, zweineuro = 1;
        
        if (done)
            break;

        // Starten des Dear ImGui frame
        ImGui_ImplDX12_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        
        {
            static float f = 0.0f;
            static bool zur = false,prog=false;
            


            ImGui::Begin("Neuronales Netzwerk");                          

            ImGui::Text("Bitte einstellungen einstellen");               
            
           ImGui::Checkbox("Einstellungen", &show_another_window);
          
           if (!zur)if (ImGui::Button("Tastatur Eingabe"))ImGui::SetKeyboardFocusHere();
           if (!zur)ImGui::SliderFloat("Lerngenauigkeit", &f, 0.0f, 1.0f);
            if (!zur)ImGui::SameLine();
            if(!zur)ImGui::Text("Lerngenauigkeit = %f", f);

            static int art = -1;
            const char* names[] = { "Manuell", "Zufaellig", "Laden" };
            static bool toggles[] = { true, false, false};

            
            if (ImGui::Button("Anfangswerte"))
                ImGui::OpenPopup("my_select_popup");
            ImGui::SameLine();
            ImGui::TextUnformatted(art == -1 ? "<Nichts>" : names[art]);
            if (ImGui::BeginPopup("my_select_popup"))
            {
                ImGui::SeparatorText("Anfangswerte"); 
                for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                    if (ImGui::Selectable(names[i]))
                        art = i;
                ImGui::EndPopup();
            }

            if(!zur)ImGui::DragInt("Trainigszyklen", &zyklen, groe, 0, 9999999);
            ImGui::SliderInt("Neuronen ersterebene", &einsneur, 1, sim);
            ImGui::SliderInt("Neuronen zweiterebene", &zweineuro, 1, sim);
            ImGui::Checkbox("Nur testen", &zur);
            if (zur) {
                art = 2;
            }
            ImGui::Checkbox("Zeige einzelne lern schritte", &prog);
            if (ImGui::IsItemHovered())ImGui::SetTooltip("Verlangsamt extrem das programm");
        

            static char buf[64] = ""; ImGui::InputText("Name", buf, 64);
            string simon=buf;
            if (ImGui::Button("Start")) {
                if (!zur) {
                    
                    neuro(f, art, zur, zyklen, einsneur, zweineuro, simon,NULL,outt,prog);
                }
                if (zur) {
                    acktivwin = true;
                }
            }
            ImGui::SameLine();
            ImGui::End();

            if (acktivwin)
            {
                ImGui::Begin("Training/,Test", &acktivwin);
                ImGui::Text("Training");
                static char in[64] = ""; ImGui::InputText("Input", in, 64);
                char inn = in[0];
                if (ImGui::Button("Test"))
                    
                    neuro(f, art, true, zyklen, einsneur, zweineuro, simon,inn,outt,false);
                ImGui::Text("Vokal: %f" ,outputone);
                ImGui::Text("Konsonant: %f" ,outputtwo);

                if (ImGui::Button("Schliesen"))
                    acktivwin = false;
                ImGui::End();
            }
        }

        if (show_another_window)
        {
            ImGui::Begin("Einstellungen", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Einstellungen");
            ImGui::ColorEdit3("clear color", (float*)&clear_color);
            ImGui::SliderInt("Genauichkeit von Zyklen", &groe, 0, 100);
            ImGui::SliderInt("Ausgabe in prozent", &outt, 0, 100);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            if (ImGui::Button("Schliesen"))
                show_another_window = false;
            ImGui::End();
        }
        

        // Rendering
        ImGui::Render();

        FrameContext* frameCtx = WaitForNextFrameResources();
        UINT backBufferIdx = g_pSwapChain->GetCurrentBackBufferIndex();
        frameCtx->CommandAllocator->Reset();

        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrier.Transition.pResource = g_mainRenderTargetResource[backBufferIdx];
        barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        g_pd3dCommandList->Reset(frameCtx->CommandAllocator, NULL);
        g_pd3dCommandList->ResourceBarrier(1, &barrier);

        // Render Dear ImGui graphics
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dCommandList->ClearRenderTargetView(g_mainRenderTargetDescriptor[backBufferIdx], clear_color_with_alpha, 0, NULL);
        g_pd3dCommandList->OMSetRenderTargets(1, &g_mainRenderTargetDescriptor[backBufferIdx], FALSE, NULL);
        g_pd3dCommandList->SetDescriptorHeaps(1, &g_pd3dSrvDescHeap);
        ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), g_pd3dCommandList);
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
        g_pd3dCommandList->ResourceBarrier(1, &barrier);
        g_pd3dCommandList->Close();

        g_pd3dCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&g_pd3dCommandList);

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault(NULL, (void*)g_pd3dCommandList);
        }

        g_pSwapChain->Present(1, 0); // Present with vsync
        //g_pSwapChain->Present(0, 0); // Present without vsync

        UINT64 fenceValue = g_fenceLastSignaledValue + 1;
        g_pd3dCommandQueue->Signal(g_fence, fenceValue);
        g_fenceLastSignaledValue = fenceValue;
        frameCtx->FenceValue = fenceValue;
    }

    WaitForLastSubmittedFrame();

    // Cleanup
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions
bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC1 sd;
    {
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = NUM_BACK_BUFFERS;
        sd.Width = 0;
        sd.Height = 0;
        sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        sd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        sd.Scaling = DXGI_SCALING_STRETCH;
        sd.Stereo = FALSE;
    }

    // [DEBUG] Enable debug interface
#ifdef DX12_ENABLE_DEBUG_LAYER
    ID3D12Debug* pdx12Debug = NULL;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&pdx12Debug))))
        pdx12Debug->EnableDebugLayer();
#endif

    // Create device
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    if (D3D12CreateDevice(NULL, featureLevel, IID_PPV_ARGS(&g_pd3dDevice)) != S_OK)
        return false;

    // [DEBUG] Setup debug interface to break on any warnings/errors
#ifdef DX12_ENABLE_DEBUG_LAYER
    if (pdx12Debug != NULL)
    {
        ID3D12InfoQueue* pInfoQueue = NULL;
        g_pd3dDevice->QueryInterface(IID_PPV_ARGS(&pInfoQueue));
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
        pInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
        pInfoQueue->Release();
        pdx12Debug->Release();
    }
#endif

    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        desc.NumDescriptors = NUM_BACK_BUFFERS;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        desc.NodeMask = 1;
        if (g_pd3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_pd3dRtvDescHeap)) != S_OK)
            return false;

        SIZE_T rtvDescriptorSize = g_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = g_pd3dRtvDescHeap->GetCPUDescriptorHandleForHeapStart();
        for (UINT i = 0; i < NUM_BACK_BUFFERS; i++)
        {
            g_mainRenderTargetDescriptor[i] = rtvHandle;
            rtvHandle.ptr += rtvDescriptorSize;
        }
    }

    {
        D3D12_DESCRIPTOR_HEAP_DESC desc = {};
        desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        desc.NumDescriptors = 1;
        desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        if (g_pd3dDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_pd3dSrvDescHeap)) != S_OK)
            return false;
    }

    {
        D3D12_COMMAND_QUEUE_DESC desc = {};
        desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        desc.NodeMask = 1;
        if (g_pd3dDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&g_pd3dCommandQueue)) != S_OK)
            return false;
    }

    for (UINT i = 0; i < NUM_FRAMES_IN_FLIGHT; i++)
        if (g_pd3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_frameContext[i].CommandAllocator)) != S_OK)
            return false;

    if (g_pd3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_frameContext[0].CommandAllocator, NULL, IID_PPV_ARGS(&g_pd3dCommandList)) != S_OK ||
        g_pd3dCommandList->Close() != S_OK)
        return false;

    if (g_pd3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_fence)) != S_OK)
        return false;

    g_fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (g_fenceEvent == NULL)
        return false;

    {
        IDXGIFactory4* dxgiFactory = NULL;
        IDXGISwapChain1* swapChain1 = NULL;
        if (CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)) != S_OK)
            return false;
        if (dxgiFactory->CreateSwapChainForHwnd(g_pd3dCommandQueue, hWnd, &sd, NULL, NULL, &swapChain1) != S_OK)
            return false;
        if (swapChain1->QueryInterface(IID_PPV_ARGS(&g_pSwapChain)) != S_OK)
            return false;
        swapChain1->Release();
        dxgiFactory->Release();
        g_pSwapChain->SetMaximumFrameLatency(NUM_BACK_BUFFERS);
        g_hSwapChainWaitableObject = g_pSwapChain->GetFrameLatencyWaitableObject();
    }

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->SetFullscreenState(false, NULL); g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_hSwapChainWaitableObject != NULL) { CloseHandle(g_hSwapChainWaitableObject); }
    for (UINT i = 0; i < NUM_FRAMES_IN_FLIGHT; i++)
        if (g_frameContext[i].CommandAllocator) { g_frameContext[i].CommandAllocator->Release(); g_frameContext[i].CommandAllocator = NULL; }
    if (g_pd3dCommandQueue) { g_pd3dCommandQueue->Release(); g_pd3dCommandQueue = NULL; }
    if (g_pd3dCommandList) { g_pd3dCommandList->Release(); g_pd3dCommandList = NULL; }
    if (g_pd3dRtvDescHeap) { g_pd3dRtvDescHeap->Release(); g_pd3dRtvDescHeap = NULL; }
    if (g_pd3dSrvDescHeap) { g_pd3dSrvDescHeap->Release(); g_pd3dSrvDescHeap = NULL; }
    if (g_fence) { g_fence->Release(); g_fence = NULL; }
    if (g_fenceEvent) { CloseHandle(g_fenceEvent); g_fenceEvent = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }

#ifdef DX12_ENABLE_DEBUG_LAYER
    IDXGIDebug1* pDebug = NULL;
    if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&pDebug))))
    {
        pDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_SUMMARY);
        pDebug->Release();
    }
#endif
}

void CreateRenderTarget()
{
    for (UINT i = 0; i < NUM_BACK_BUFFERS; i++)
    {
        ID3D12Resource* pBackBuffer = NULL;
        g_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
        g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, g_mainRenderTargetDescriptor[i]);
        g_mainRenderTargetResource[i] = pBackBuffer;
    }
}

void CleanupRenderTarget()
{
    WaitForLastSubmittedFrame();

    for (UINT i = 0; i < NUM_BACK_BUFFERS; i++)
        if (g_mainRenderTargetResource[i]) { g_mainRenderTargetResource[i]->Release(); g_mainRenderTargetResource[i] = NULL; }
}

void WaitForLastSubmittedFrame()
{
    FrameContext* frameCtx = &g_frameContext[g_frameIndex % NUM_FRAMES_IN_FLIGHT];

    UINT64 fenceValue = frameCtx->FenceValue;
    if (fenceValue == 0)
        return; // No fence was signaled

    frameCtx->FenceValue = 0;
    if (g_fence->GetCompletedValue() >= fenceValue)
        return;

    g_fence->SetEventOnCompletion(fenceValue, g_fenceEvent);
    WaitForSingleObject(g_fenceEvent, INFINITE);
}

FrameContext* WaitForNextFrameResources()
{
    UINT nextFrameIndex = g_frameIndex + 1;
    g_frameIndex = nextFrameIndex;

    HANDLE waitableObjects[] = { g_hSwapChainWaitableObject, NULL };
    DWORD numWaitableObjects = 1;

    FrameContext* frameCtx = &g_frameContext[nextFrameIndex % NUM_FRAMES_IN_FLIGHT];
    UINT64 fenceValue = frameCtx->FenceValue;
    if (fenceValue != 0) // means no fence was signaled
    {
        frameCtx->FenceValue = 0;
        g_fence->SetEventOnCompletion(fenceValue, g_fenceEvent);
        waitableObjects[1] = g_fenceEvent;
        numWaitableObjects = 2;
    }

    WaitForMultipleObjects(numWaitableObjects, waitableObjects, TRUE, INFINITE);

    return frameCtx;
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            WaitForLastSubmittedFrame();
            CleanupRenderTarget();
            HRESULT result = g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT);
            assert(SUCCEEDED(result) && "Failed to resize swapchain.");
            CreateRenderTarget();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}

void mischen(int* array, size_t n) {
    if (n > 1)
    {
        size_t i;
        for (i = 0; i < n - 1; i++)
        {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}
double dSigmoid(double x) { return x * (1 - x); }
double sigmoid(double x) { return 1 / (1 + exp(-x)); }
double zufallszahl() { return (((double)rand()) / ((double)RAND_MAX)); }

vector<string> explode(const string& delimiter, const string& str)
{//splittet einen string
    vector<string> arr;

    int strleng = str.length();
    int delleng = delimiter.length();
    if (delleng == 0)
        return arr;

    int i = 0;
    int k = 0;
    while (i < strleng)
    {
        int j = 0;
        while (i + j < strleng && j < delleng && str[i + j] == delimiter[j])
            j++;
        if (j == delleng)
        {
            arr.push_back(str.substr(k, i - k));
            i += delleng;
            k = i;
        }
        else
        {
            i++;
        }
    }
    arr.push_back(str.substr(k, i - k));
    return arr;
}


void neuro(double lerngenauichkeit,int ein,bool trai,int anzahllernzyclen, int anzahlversteckterneuroneneins, int anzahlversteckterneuronenzwei, string datein,char c,int out,bool prog) {
    //initaliesierung verschiedener varriabeln
    double versteckteebeneeins[sim]{};
    double versteckteebenezwei[sim]{};
    double ausgangsebene[2]{};
    double versteckteebeneeinsBias[sim]{};
    double versteckteebenezweiBias[sim]{};
    double ausgangsebeneBias[2]{};
    double versteckteWeightseins[26][sim]{};
    double versteckteWeightszwei[sim][sim]{};
    double ausgangsWeights[sim][2]{};
    double time1 = 0.0, tstart=0;

    double training_inputs[anzahltraningsdaten][26] = { {1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
                                                                            {0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
                                                                            {0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
                                                                            {0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
                                                                            {0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
                                                                            {0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
                                                                            {0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
                                                                            {0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
                                                                            {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
                                                                            {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
                                                                            {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
                                                                            {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
                                                                            {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
                                                                            {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
                                                                            {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
                                                                            {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
                                                                            {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
                                                                            {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
                                                                            {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
                                                                            {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0},
                                                                            {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0},
                                                                            {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0,0.0},
                                                                            {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0,0.0},
                                                                            {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0,0.0},
                                                                            {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,0.0},
                                                                            {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0},
    };
    double training_outputs[anzahltraningsdaten][2] = { {1.0,0.0 },
                                                                             {0.0,1.0 },
                                                                             {0.0,1.0 },
                                                                             {0.0,1.0 },
                                                                             {1.0,0.0 },
                                                                             {0.0,1.0 },
                                                                             {0.0,1.0 },
                                                                             {0.0,1.0 },
                                                                             {1.0,0.0 },
                                                                             {0.0,1.0 },
                                                                             {0.0,1.0 },
                                                                             {0.0,1.0 },
                                                                             {0.0,1.0 },
                                                                             {0.0,1.0 },
                                                                             {1.0,0.0 },
                                                                             {0.0,1.0 },
                                                                             {0.0,1.0 },
                                                                             {0.0,1.0 },
                                                                             {0.0,1.0 },
                                                                             {0.0,1.0 },
                                                                             {1.0,0.0 },
                                                                             {0.0,1.0 },
                                                                             {0.0,1.0 },
                                                                             {0.0,1.0 },
                                                                             {0.0,1.0 },
                                                                             {0.0,1.0 },
    };
    
    
    

    if (ein == 0) {//besetzen mit eingebe per hand (nur zum testen weil zu vielaufwand
        cout << "Versteckte Weights eins" << endl;
        for (int i = 0; i < anzahleingangsneuronen; i++) {
            for (int j = 0; j < anzahlversteckterneuroneneins; j++) {
                cin >> versteckteWeightseins[j][i];
            }
        }
        cout << "versteckte Bias eins" << endl;

        for (int i = 0; i < anzahlversteckterneuroneneins; i++) {
            cin >> versteckteebeneeinsBias[i];

        }

        cout << "Versteckte Weights zwei" << endl;
        for (int i = 0; i < anzahlversteckterneuroneneins; i++) {
            for (int j = 0; j < anzahlversteckterneuronenzwei; j++) {
                cin >> versteckteWeightszwei[j][i];
            }
        }
        cout << "versteckte Bias zwei" << endl;

        for (int i = 0; i < anzahlversteckterneuronenzwei; i++) {
            cin >> versteckteebenezweiBias[i];

        }
        cout << "ausgangs Weights" << endl;
        for (int i = 0; i < anzahlversteckterneuronenzwei; i++) {
            for (int j = 0; j < anzahlausgangsneuronen; j++) {


                cin >> ausgangsWeights[j][i];
            }
        }
        cout << "ausgangs Bias" << endl;

        for (int i = 0; i < anzahlausgangsneuronen; i++) {
            cin >> ausgangsebeneBias[i];
        }
    }

    if (ein == 1) {//besetzen mit zufaulligen zahlen
        for (int i = 0; i < anzahleingangsneuronen; i++) {
            for (int j = 0; j < sim; j++) {
                versteckteWeightseins[i][j] = zufallszahl();
            }
        }
        for (int i = 0; i < sim; i++) {
            versteckteebeneeinsBias[i] = zufallszahl();

            for (int j = 0; j < sim; j++) {
                versteckteWeightszwei[i][j] = zufallszahl();
            }
        }

        for (int i = 0; i < sim; i++) {
            versteckteebenezweiBias[i] = zufallszahl();
            for (int j = 0; j < anzahlausgangsneuronen; j++) {
                ausgangsWeights[i][j] = zufallszahl();
            }
        }
        for (int i = 0; i < anzahlausgangsneuronen; i++) {
            ausgangsebeneBias[i] = zufallszahl();
        }
    }
    string t;
    if (ein == 2) {//einlesen aus binären datein
        t = "saves\\";
        t += datein;
        t += "we.bin";
        std::ifstream inputFile(t, std::ios::in | std::ios::binary);
        inputFile.read(reinterpret_cast<char*>(versteckteWeightseins), anzahleingangsneuronen * sim * sizeof(double));
        inputFile.close();
        t = "saves\\";
        t += datein;
        t += "be.bin";
        std::ifstream inputFilee(t, std::ios::in | std::ios::binary);
        inputFilee.read(reinterpret_cast<char*>(versteckteebeneeinsBias), sim * sizeof(double));
        inputFilee.close();
        t = "saves\\";
        t += datein;
        t += "wz.bin";
        std::ifstream inputFileee(t, std::ios::in | std::ios::binary);
        inputFileee.read(reinterpret_cast<char*>(versteckteWeightszwei), sim * sim * sizeof(double));
        inputFileee.close();
        t = "saves\\";
        t += datein;
        t += "bz.bin";
        std::ifstream inputFileeee(t, std::ios::in | std::ios::binary);
        inputFileeee.read(reinterpret_cast<char*>(versteckteebenezweiBias), sim * sizeof(double));
        inputFileeee.close();
        t = "saves\\";
        t += datein;
        t += "wa.bin";
        std::ifstream inputFileeeee(t, std::ios::in | std::ios::binary);
        inputFileeeee.read(reinterpret_cast<char*>(ausgangsWeights), anzahlausgangsneuronen * anzahlversteckterneuronenzwei * sizeof(double));
        inputFileeeee.close(); 
        t = "saves\\";
        t += datein;
        t += "ba.bin";
        std::ifstream inputFileeeeee(t, std::ios::in | std::ios::binary);
        inputFileeeeee.read(reinterpret_cast<char*>(ausgangsebeneBias), anzahlausgangsneuronen * sizeof(double));
        inputFileeeeee.close();
    }

       int reinfolgetrainingsdaten[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25 };
   
    int temp=0;
   
    if (trai) {//nut ueben
            int w = 0;

            w = (c - 97);
            cout << w << endl;
            cout << "Input: ";
            for (int n = 0; n < anzahleingangsneuronen; n++)
            {
                cout << training_inputs[w][n] << " ";
            }

            for (int j = 0; j < anzahlversteckterneuroneneins; j++) {
                double activation = versteckteebeneeinsBias[j];
                for (int k = 0; k < anzahleingangsneuronen; k++) {
                    activation += training_inputs[w][k] * versteckteWeightseins[k][j];
                }
                versteckteebeneeins[j] = sigmoid(activation);
            }
            for (int j = 0; j < anzahlversteckterneuronenzwei; j++) {
                double activation = versteckteebenezweiBias[j];
                for (int k = 0; k < anzahlversteckterneuroneneins; k++) {
                    activation += versteckteebeneeins[k] * versteckteWeightszwei[k][j];
                }
                versteckteebenezwei[j] = sigmoid(activation);
            }

            // berechnen der ausgangs ebene
            for (int j = 0; j < anzahlausgangsneuronen; j++) {
                double activation = ausgangsebeneBias[j];
                for (int k = 0; k < anzahlversteckterneuronenzwei; k++) {
                    activation += versteckteebenezwei[k] * ausgangsWeights[k][j];
                }
                ausgangsebene[j] = sigmoid(activation);
            }

            cout << " Output: ";
            for (int q = 0; q < anzahlausgangsneuronen; q++)
            {
                cout << ausgangsebene[q] << " ";
            }
            outputone = ausgangsebene[0];
            outputtwo = ausgangsebene[1];

            return;

    }
    int tr = (anzahllernzyclen*out)/100,trr=0;//prozentualer vortschritsbalken
    
    //training
    for (int zyclus = 0; zyclus < anzahllernzyclen; zyclus++) {
        //berechnen der Zeit die benötigt wird
        if (zyclus == 0) {
            tstart = clock();
        }
        if (zyclus == 1) {
            time1 += clock() - tstart;
            
            cout << "time = " << time1 << " milisec."<<endl;
            time1*= anzahllernzyclen;
            time1 /= 1000;
            cout << "Geschaetzte zeit in sec = "<<time1<<endl;
            time1 /= 60;
            cout << "Geschaetzte zeit in min = " << time1 << endl;

        }
        if (zyclus == 100&&time1==0) {
            time1 += clock() - tstart;

            cout << "time*100 = " << time1 << " milisec." << endl;
            time1 *= anzahllernzyclen/100;
            time1 /= 1000;
            cout << "Geschaetzte zeit in sec = " << time1 << endl;
            time1 /= 60;
            cout << "Geschaetzte zeit in min = " << time1 << endl;

        }
        // Vortschritsbalken
        trr ++;
        if (trr == tr) {
            trr = 0;
            cout << "-";
        }
        // das training zufällig machen
        mischen(reinfolgetrainingsdaten, anzahltraningsdaten);

        // damit alle tranings gemacht werden
        for (int x = 0; x < anzahltraningsdaten; x++) {

            int i = reinfolgetrainingsdaten[x];

            // Forward pass

            // berechnen der versteckten ebene
            for (int j = 0; j < anzahlversteckterneuroneneins; j++) {
                double activation = versteckteebeneeinsBias[j];
                for (int k = 0; k < anzahleingangsneuronen; k++) {
                    activation += training_inputs[i][k] * versteckteWeightseins[k][j];
                }
                versteckteebeneeins[j] = sigmoid(activation);
            }
            for (int j = 0; j < anzahlversteckterneuronenzwei; j++) {
                double activation = versteckteebenezweiBias[j];
                for (int k = 0; k < anzahlversteckterneuroneneins; k++) {
                    activation += versteckteebeneeins[k] * versteckteWeightszwei[k][j];
                }
                versteckteebenezwei[j] = sigmoid(activation);
            }

            // berechnen der ausgangs ebene
            for (int j = 0; j < anzahlausgangsneuronen; j++) {
                double activation = ausgangsebeneBias[j];
                for (int k = 0; k < anzahlversteckterneuronenzwei; k++) {
                    activation += versteckteebenezwei[k] * ausgangsWeights[k][j];
                }
                ausgangsebene[j] = sigmoid(activation);
            }

            if (prog == true) { // Ausgabe wenn aktiv
            cout << "Input: ";
            for (int w = 0; w < anzahleingangsneuronen; w++)
            {
                cout << training_inputs[i][w] << " ";
            }
            cout << " Output: ";
            for (int q = 0; q < anzahlausgangsneuronen; q++)
            {
                cout << ausgangsebene[q] << " ";
            }
            cout << " Richtiger Output: ";
            for (int w = 0; w < anzahlausgangsneuronen; w++)
            {
                cout << training_outputs[i][w] << " ";
            }
            cout << " |" << (zyclus * 100) / anzahllernzyclen << "%" << endl;
        }

            // Backprop

            // berechnen der änderungen
            double deltaOutput[2]{};
            for (int j = 0; j < 2; j++) {
                double errorOutput = (training_outputs[i][j] - ausgangsebene[j]);
                deltaOutput[j] = errorOutput * dSigmoid(ausgangsebene[j]);
            }
            double deltaOutputz[sim]{};
            for (int j = 0; j < anzahlversteckterneuronenzwei; j++) {
                double errorOutputz = (training_outputs[i][anzahlausgangsneuronen - 1] - versteckteebenezwei[j]);
                deltaOutputz[j] = errorOutputz * dSigmoid(versteckteebenezwei[j]);
            }

            double deltaHiddenzwei[sim]{};
            for (int j = 0; j < anzahlversteckterneuronenzwei; j++) {
                double errorHidden = 0.0;
                for (int k = 0; k < anzahlausgangsneuronen; k++) {
                    errorHidden += deltaOutput[k] * ausgangsWeights[j][k];
                }
                deltaHiddenzwei[j] = errorHidden * dSigmoid(versteckteebenezwei[j]);
            }

            double deltaHiddeneins[sim]{};
            for (int j = 0; j < anzahlversteckterneuroneneins; j++) {
                double errorHidden = 0.0;
                for (int k = 0; k < anzahlversteckterneuronenzwei; k++) {
                    errorHidden += deltaOutputz[k] * versteckteWeightszwei[j][k];
                }
                deltaHiddeneins[j] = errorHidden * dSigmoid(versteckteebeneeins[j]);
            }

            // aendern der werte
            for (int j = 0; j < anzahlausgangsneuronen; j++) {
                ausgangsebeneBias[j] += deltaOutput[j] * lerngenauichkeit;
                for (int k = 0; k < anzahlversteckterneuronenzwei; k++) {
                    ausgangsWeights[k][j] += versteckteebenezwei[k] * deltaOutput[j] * lerngenauichkeit;
                }
            }
            for (int j = 0; j < anzahlversteckterneuronenzwei; j++) {
                versteckteebenezweiBias[j] += deltaHiddenzwei[j] * lerngenauichkeit;
                for (int k = 0; k < anzahlversteckterneuroneneins; k++) {
                    versteckteWeightszwei[k][j] += deltaHiddeneins[j] * deltaHiddenzwei[j] * lerngenauichkeit;
                }
            }
            for (int j = 0; j < anzahlversteckterneuroneneins; j++) {
                versteckteebeneeinsBias[j] += deltaHiddeneins[j] * lerngenauichkeit;
                for (int k = 0; k < anzahleingangsneuronen; k++) {
                    versteckteWeightseins[k][j] += training_inputs[i][k] * deltaHiddeneins[j] * lerngenauichkeit;
                }
            }
        }
    }
    //Ausgabe der zu speichernden werte
    cout << "Finale Vesteckte Weights erste ebene\n[";
    for (int j = 0; j < anzahlversteckterneuroneneins; j++) {

        for (int k = 0; k < anzahleingangsneuronen; k++) {
            cout << "[" << versteckteWeightseins[k][j] << "]";
        }
    }
    cout << "]\nFinal Versteckte Biases erste ebene\n[";
    for (int j = 0; j < anzahlversteckterneuroneneins; j++) {
        cout << "[" << versteckteebeneeinsBias[j] << "]";
    }

    cout << "]\nFinale Vesteckte Weights zweite ebene\n[";
    for (int j = 0; j < anzahlversteckterneuronenzwei; j++) {

        for (int k = 0; k < anzahlversteckterneuroneneins; k++) {
            cout << "[" << versteckteWeightszwei[k][j] << "]";
        }
    }
    cout << "]\nFinal Versteckte Biases zweite ebene\n[";
    for (int j = 0; j < anzahlversteckterneuronenzwei; j++) {
        cout << "[" << versteckteebenezweiBias[j] << "]";
    }

    cout << "]\nFinal Output Weights\n";
    for (int j = 0; j < anzahlausgangsneuronen; j++) {
        cout << "[";
        for (int k = 0; k < anzahlversteckterneuroneneins; k++) {
            cout << "[" << ausgangsWeights[k][j] << "]";

        }
        cout << "]\n";
    }

    cout << "Final Output Biases\n[";
    for (int j = 0; j < anzahlausgangsneuronen; j++) {
        cout << "[" << ausgangsebeneBias[j] << "]";
    }

    cout << "]\n";
    cout << "Speichern? 1ja 2nein" << endl;
    cin >> temp;
    if (temp == 1) {//schreiben in datein
        
        t = "saves\\";
        t += datein;
        t += "we.bin";
        std::ofstream outputFile(t, std::ios::out | std::ios::binary);
        outputFile.write(reinterpret_cast<char*>(versteckteWeightseins), sim* anzahleingangsneuronen * sizeof(double));
        outputFile.close(); 
        t = "saves\\";
        t += datein;
        t += "be.bin";
        std::ofstream outputFilee(t, std::ios::out | std::ios::binary);
        outputFilee.write(reinterpret_cast<char*>(versteckteebeneeinsBias), sim * sizeof(double));
        outputFilee.close();
        t = "saves\\";
        t += datein;
        t += "wz.bin";
        std::ofstream outputFileee(t, std::ios::out | std::ios::binary);
        outputFileee.write(reinterpret_cast<char*>(versteckteWeightszwei), sim* sim * sizeof(double));
        outputFileee.close();
        t = "saves\\";
        t += datein;
        t += "bz.bin";
        std::ofstream outputFileeee(t, std::ios::out | std::ios::binary);
        outputFileeee.write(reinterpret_cast<char*>(versteckteebenezweiBias), sim * sizeof(double));
        outputFileeee.close();
        t = "saves\\";
        t += datein;
        t += "wa.bin";
        std::ofstream outputFileeeee(t, std::ios::out | std::ios::binary);
        outputFileeeee.write(reinterpret_cast<char*>(ausgangsWeights), anzahlausgangsneuronen*  sim * sizeof(double));
        outputFileeeee.close(); 
        t = "saves\\";
        t += datein;
        t += "ba.bin";
        std::ofstream outputFileeeeee(t, std::ios::out | std::ios::binary);
        outputFileeeeee.write(reinterpret_cast<char*>(ausgangsebeneBias), anzahlausgangsneuronen * sizeof(double));
        outputFileeeeee.close();
    }
        
}

