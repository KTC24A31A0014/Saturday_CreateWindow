#pragma once

#include "DXGI.h"
#include "command_queue.h"
#include "../window/window.h"

class SwapChain final {
public:
    
    SwapChain() = default;
    ~SwapChain();

    [[nodiscard]] bool create(const DXGI& dxgi, const Window& window, const CommandQueue& commandQueue) noexcept;

    [[nodiscard]] IDXGISwapChain* get() const noexcept;

private:
    IDXGISwapChain3* swapChain_{};  /// スワップチェイン
};
