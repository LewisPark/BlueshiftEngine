#pragma once

#include "Core/Vertex.h"
#include "BufferCache.h"
#include "Render/SceneEntity.h"

BE_NAMESPACE_BEGIN

class Material;
class Font;

struct GuiSubMesh {
    int                     numVerts;
    int                     numIndexes;

    BufferCache *           vertexCache;
    BufferCache *           indexCache;
};

struct GuiMeshSurf {
    const Material *        material;
    uint32_t                color;

    int                     numVerts;
    int                     numIndexes;
    
    BufferCache             vertexCache;
    BufferCache             indexCache;
};

class GuiMesh {
    friend class RenderWorld;

public:
    enum CoordFrame {
        CoordFrame2D,
        CoordFrame3D
    };

    GuiMesh();

    CoordFrame              GetCoordFrame() const { return coordFrame; }
    void                    SetCoordFrame(CoordFrame frame) { coordFrame = frame; }

    void                    Clear();

    void                    SetClipRect(const Rect &clipRect);

    void                    SetColor(const Color4 &rgba);

    void                    DrawPic(float x, float y, float w, float h, float s1, float t1, float s2, float t2, const Material *material);
    
    int                     DrawChar(float x, float y, float sx, float sy, Font *font, wchar_t charCode);

    void                    DrawText(Font *font, SceneEntity::TextAnchor anchor, SceneEntity::TextAlignment alignment, float lineSpacing, float textScale, const wchar_t *text);

    AABB                    Compute3DTextAABB(Font *font, SceneEntity::TextAnchor anchor, float lineSpacing, float textScale, const wchar_t *text) const;

private:
    void                    PrepareNextSurf();
    void                    DrawTris(const VertexNoLit *verts, const TriIndex *indexes, int vertCount, int indexCount, const Material *material);
    
    Array<GuiMeshSurf>      surfaces;
    GuiMeshSurf *           currentSurf;

    int                     numVerts;
    int                     numIndexes;

    CoordFrame              coordFrame;
    Rect                    clipRect;
};

BE_NAMESPACE_END
