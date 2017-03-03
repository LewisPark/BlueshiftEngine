#pragma once

BE_NAMESPACE_BEGIN

/*
-------------------------------------------------------------------------------

    Render System

-------------------------------------------------------------------------------
*/

class CmdArgs;
struct view_t;

class RenderSystem {
    friend class RenderContext;
    friend class RenderWorld;
    friend class PhysDebugDraw;

public:
    RenderSystem();

    void                    Init(const Renderer::Settings *settings);
    void                    Shutdown();

    bool                    IsInitialized() const { return initialized; }

    bool                    IsFullscreen() const;

    void                    SetGamma(double gamma);
    void                    RestoreGamma();

    RenderContext *         AllocRenderContext(bool isMainContext = false);
    void                    FreeRenderContext(RenderContext *renderContext);

                            // valid only between BeginFrame()/EndFrame()
    RenderContext *         GetCurrentRenderContext() { return currentContext; }

    RenderContext *         GetMainRenderContext() { return mainContext; }

    RenderWorld *           AllocRenderWorld();
    void                    FreeRenderWorld(RenderWorld *renderWorld);

    RenderWorld *           GetRenderWorld() const { return primaryWorld; }

    void                    CheckModifiedCVars();

private:
    void                    RecreateScreenMapRT();
    void                    RecreateHDRMapRT();
    void                    RecreateShadowMapRT();
    void *                  GetCommandBuffer(int bytes);
    void                    IssueCommands();

    void                    CmdDrawView(const view_t *view);
    void                    CmdScreenshot(int x, int y, int width, int height, const char *filename);

    bool                    initialized;
    unsigned short          savedGammaRamp[768];

    RenderWorld *           primaryWorld;
    
    Array<RenderContext *>  renderContexts;
    RenderContext *         currentContext;
    RenderContext *         mainContext;

    static void             Cmd_ScreenShot(const CmdArgs &args);    
};

BE_INLINE RenderSystem::RenderSystem() {
    initialized = false;
}

extern RenderSystem         renderSystem;

BE_NAMESPACE_END
