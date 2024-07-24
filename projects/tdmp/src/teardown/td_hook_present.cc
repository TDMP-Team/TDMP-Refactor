#include "pch.h"
#include "memory/hooks.h"
#include "teardown/td_hook_present.h"
#include "offsets_generated.h"

using namespace mp;
using namespace td;

// Forward Functions
//------------------------------------------------------------------------
HRESULT __stdcall h_createDXGIFactory1(const IID* const riid, void** ppFactory);
HRESULT __stdcall h_D3D12Present(IDXGISwapChain3* pThis, UINT syncInterval, UINT flags);
LRESULT CALLBACK windowProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Types
//------------------------------------------------------------------------
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

using tCreateSwapChainForHwnd = std::add_pointer_t<HRESULT __fastcall(
	IDXGIFactory                          *pFactory,
	IUnknown                              *pDevice,
	HWND                                  hWnd,
	const DXGI_SWAP_CHAIN_DESC1           *pDesc,
	const DXGI_SWAP_CHAIN_FULLSCREEN_DESC *pFullscreenDesc,
	IDXGIOutput                           *pRestrictToOutput,
	IDXGISwapChain1                       **ppSwapChain
)>;

using tD3D12Present  = std::add_pointer_t<HRESULT(IDXGISwapChain* SwapChain, UINT SyncInterval, UINT Flags)>;
using tResizeBuffers = std::add_pointer_t<HRESULT(IDXGISwapChain* SwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)>;

static tCreateSwapChainForHwnd oCreateSwapChainForHwnd = nullptr;
static tD3D12Present           oPresent = nullptr;

// static bool                    gInitialized = false;
static WNDPROC                 gWindowProc;
static HWND                    gHwnd;

static bool bShowMenu = false;

// Hooks
//------------------------------------------------------------------------
HRESULT __fastcall h_CreateSwapChainForHwnd(
	IDXGIFactory                          *pFactory,
	IUnknown                              *pDevice,
	HWND                                  hWnd,
	const DXGI_SWAP_CHAIN_DESC1           *pDesc,
	const DXGI_SWAP_CHAIN_FULLSCREEN_DESC *pFullscreenDesc,
	IDXGIOutput                           *pRestrictToOutput,
	IDXGISwapChain1                       **ppSwapChain)
{
	gHwnd = hWnd;

	console::writeln("CreateSwapChainForHwnd Called!");
	HRESULT result = oCreateSwapChainForHwnd(pFactory, pDevice, hWnd, pDesc, pFullscreenDesc, pRestrictToOutput, ppSwapChain);

	void** vTable = *reinterpret_cast<void***>(*ppSwapChain);
	auto pD3D11Present = static_cast<tD3D12Present>(vTable[8]);

	mem::hooks::addHook("d3d12::present", pD3D11Present, &h_D3D12Present, &oPresent);

	return result;
};

HRESULT __stdcall h_createDXGIFactory1(const IID* const riid, void** ppFactory) {
	console::writeln("Creating DXGI Factory");
	mp::funcs::renderer::createDXGIFactory1(riid, ppFactory);

	void** factoryVTable = *reinterpret_cast<void***>(*ppFactory);
	mem::hooks::addHook("factory::CreateSwapChainForHwnd", factoryVTable[15], &h_CreateSwapChainForHwnd, &oCreateSwapChainForHwnd);

	return mp::funcs::renderer::createDXGIFactory1(riid, ppFactory);
}

HRESULT __stdcall h_D3D12Present(IDXGISwapChain3* pThis, UINT SyncInterval, UINT Flags) {
    // TODO: Figure where to get the commandlist from teardown so I can use it with ImGui (it's probably a lot more complex..)
    // OpenGL & DX11 are super easy, but DX12 is a bit more manual.

	return oPresent(pThis, SyncInterval, Flags);
}

LRESULT CALLBACK windowProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_KEYDOWN && wParam == VK_F2)
		bShowMenu = !bShowMenu;

	if (bShowMenu) {
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
		ImGuiIO& io = ImGui::GetIO();
		if (io.WantCaptureMouse)
			return true;
	}

	return CallWindowProc(gWindowProc, hWnd, uMsg, wParam, lParam);
}

// Public Functions
//------------------------------------------------------------------------
void renderer::hookPresent(renderer::type type) {
	if (type == renderer::type::d3d12) {
		mem::hooks::addHook("renderer::createDXGIFactory1", mp::offsets::renderer::createDXGIFactory1, &h_createDXGIFactory1, &funcs::renderer::createDXGIFactory1);
	} else {
		console::writeln("Present hooking is not implemented for OpenGL yet");
	}
}