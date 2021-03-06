// Copyright(c) 2017 POLYGONTEK
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http ://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "Math/Math.h"

BE_NAMESPACE_BEGIN

/// Image representation
class Image {
public:
    /// Various image format type
    /// Channels (RGBALX) are described by little-endian format (opposed to D3D style)
    enum Format {
        UnknownFormat,
        // plane bytes format
        L_8,
        A_8,
        LA_8_8,
        LA_16_16,
        R_8,
        R_SNORM_8,
        RG_8_8,
        RG_SNORM_8_8,
        RGB_8_8_8,
        RGB_SNORM_8_8_8,
        BGR_8_8_8, 
        RGBX_8_8_8_8,
        BGRX_8_8_8_8,
        RGBA_8_8_8_8,
        RGBA_SNORM_8_8_8_8,
        BGRA_8_8_8_8, 
        ABGR_8_8_8_8, 
        ARGB_8_8_8_8,
        // packed format
        RGBX_4_4_4_4,
        BGRX_4_4_4_4,
        RGBA_4_4_4_4,
        BGRA_4_4_4_4,
        ABGR_4_4_4_4,
        ARGB_4_4_4_4,
        RGBX_5_5_5_1,
        BGRX_5_5_5_1,
        RGBA_5_5_5_1,
        BGRA_5_5_5_1,
        ABGR_1_5_5_5,
        ARGB_1_5_5_5,
        RGB_5_6_5,
        BGR_5_6_5,
        RGBE_9_9_9_5,
        // float format
        L_16F,
        L_32F,
        A_16F,
        A_32F,
        LA_16F_16F,
        LA_32F_32F,
        R_16F,
        R_32F,
        RG_16F_16F,
        RG_32F_32F,
        RGB_11F_11F_10F,
        RGB_16F_16F_16F,
        RGB_32F_32F_32F,
        RGBA_16F_16F_16F_16F,
        RGBA_32F_32F_32F_32F,
        // DXT (BTC)
        RGBA_DXT1,
        RGBA_DXT3, 	
        RGBA_DXT5,
        XGBR_DXT5,
        DXN1,
        DXN2,
        // PVRTC
        RGB_PVRTC_2BPPV1,
        RGB_PVRTC_4BPPV1,
        RGBA_PVRTC_2BPPV1,
        RGBA_PVRTC_4BPPV1,
        RGBA_PVRTC_2BPPV2,
        RGBA_PVRTC_4BPPV2,
        // ETC
        RGB_8_ETC1,
        RGB_8_ETC2,
        RGBA_8_8_ETC2,
        RGBA_8_1_ETC2,
        R_11_EAC,
        SignedR_11_EAC,
        RG_11_11_EAC,
        SignedRG_11_11_EAC,
        // ATC
        RGB_ATC,
        RGBA_EA_ATC, // Explicit alpha
        RGBA_IA_ATC, // Interpolated alpha
        // depth format
        Depth_16,
        Depth_24,
        Depth_32F,
        DepthStencil_24_8,
        DepthStencil_32F_8,
        NumImageFormats
    };

    enum FormatType {
        Packed          = BIT(0),
        Float           = BIT(1),
        Half            = BIT(2),
        Depth           = BIT(3),
        Stencil         = BIT(4),
        DepthStencil    = Depth | Stencil,
        Compressed      = BIT(5)
    };

    /// Image flags
    enum Flag {
        ClampFlag       = BIT(0),
        NormalMapFlag   = BIT(5),
        SRGBFlag        = BIT(6),
        LinearFlag      = BIT(7)
    };

    /// Image resample filter
    enum ResampleFilter {
        Nearest,
        Bilinear,
        Bicubic
    };

    /// Compression quality
    enum CompressionQuality {
        Fast,
        Normal,
        HighQuality
    };

    Image();
    Image(int width, int height, int depth, int numSlices, int numMipmaps, Image::Format format, byte *data, int flags);
    Image(const Image &other);
    Image(Image &&other);
    ~Image();

    bool                IsEmpty() const { return pic == nullptr; }
    const char *        FormatName() const { return Image::FormatName(format); }
    int                 BytesPerPixel() const { return Image::BytesPerPixel(format); }
    int                 BytesPerBlock() const { return Image::BytesPerBlock(format); }
    int                 NumComponents() const { return Image::NumComponents(format); }
    void                GetBits(int *redBits, int *greenBits, int *blueBits, int *alphaBits) const { Image::GetBits(format, redBits, greenBits, blueBits, alphaBits); }
    bool                HasAlpha() const { return Image::HasAlpha(format); }
    bool                IsSRGB() const { return (flags & SRGBFlag) ? true : false; }
    bool                IsLinear() const { return (flags & LinearFlag) ? true : false; }
    bool                IsPacked() const { return Image::IsPacked(format); }
    bool                IsCompressed() const { return Image::IsCompressed(format); }
    bool                IsFloatFormat() const { return Image::IsFloatFormat(format); }
    bool                IsHalfFormat() const { return Image::IsHalfFormat(format); }
    bool                IsDepthFormat() const { return Image::IsDepthFormat(format); }
    bool                IsDepthStencilFormat() const { return Image::IsDepthStencilFormat(format); }

    int                 GetWidth() const { return width; }
    int                 GetWidth(int mipMapLevel) const;
    int                 GetHeight() const { return height; }
    int                 GetHeight(int mipMapLevel) const;
    int                 GetDepth() const { return depth; }
    int                 GetDepth(int mipMapLevel) const;
    int                 NumMipmaps() const { return numMipmaps; }
    int                 NumSlices() const { return numSlices; }
    int                 GetFlags() const { return flags; }
    Image::Format       GetFormat() const { return format; }
    byte *              GetPixels() const { return pic; }
    byte *              GetPixels(int level) const;
    byte *              GetPixels(int level, int sliceIndex) const;

    Color4              GetColor(int x, int y) const;

                        /// Returns number of pixels with given mipmap levels
    int                 NumPixels(int firstLevel = 0, int numLevels = 1) const;
                        /// Returns number of bytes with given mipmap levels
    int                 GetSize(int firstLevel = 0, int numLevels = 1) const;
                        /// Returns number of bytes of a slice with given mipmap levels
                        /// A slice means single cubemap face or single texture of an array texture 
    int                 GetSliceSize(int firstLevel = 0, int numLevels = 1) const;
                        
                        /// Clears allocated pixel data
    void                Clear();

    Image &             InitFromMemory(int width, int height, int depth, int numSlices, int numMipmaps, Image::Format format, byte *data, int flags);
                        /// Creates an image. If data is nullptr, just allocate the memory.
    Image &             Create(int width, int height, int depth, int numSlices, int numMipmaps, Image::Format format, const byte *data, int flags);
    Image &             Create2D(int width, int height, int numMipmaps, Image::Format format, const byte *data, int flags);
    Image &             Create3D(int width, int height, int depth, int numMipmaps, Image::Format format, const byte *data, int flags);
    Image &             CreateCube(int size, int numMipmaps, Image::Format format, const byte *data, int flags);
    Image &             Create2DArray(int width, int height, int numSlices, int numMipmaps, Image::Format format, const byte *data, int flags);

                        /// Creates an cubic image from the six square images
    Image &             CreateCubeFromMultipleImages(const Image *images);

    Image &             CopyFrom(const Image &srcImage, int firstLevel = 0, int numLevels = 1);

    Image &             operator=(const Image &other);
    Image &             operator=(Image &&other);
    
    Image &             GenerateMipmaps();

    bool                ConvertFormat(Image::Format dstFormat, Image &dstImage, bool regenerateMipmaps = false, CompressionQuality compressionQuality = Normal) const;
    bool                ConvertFormatSelf(Image::Format dstFormat, bool regenerateMipmaps = false, CompressionQuality compressionQuality = Normal);

    bool                Resize(int width, int height, Image::ResampleFilter resampleFilter, Image &dstImage) const;
    bool                ResizeSelf(int width, int height, Image::ResampleFilter resampleFilter);

                        /// Flips vertically
    Image &             FlipX();
                        /// Flips horizontally
    Image &             FlipY();

    Image &             AdjustBrightness(float factor);
    Image &             GammaCorrectRGB888(uint16_t ramp[768]);

                        /// Swaps the component red with alpha
    Image &             SwapRedAlphaRGBA8888();

                        /// Makes height map to normal map
    Image               MakeNormalMapRGBA8888(float bumpiness) const;

                        /// Adds normal map to another normal map
    Image &             AddNormalMapRGBA8888(const Image &normalMap);

    bool                Load(const char *filename);

    bool                Write(const char *filename) const;
    bool                WriteDDS(const char *filename) const;
    bool                WritePVR(const char *filename) const;
    bool                WriteBMP(const char *filename) const;
    bool                WritePCX(const char *filename) const;
    bool                WriteTGA(const char *filename) const;
                        /// @param quality value has the range [0, 100]
    bool                WriteJPG(const char *filename, int quality = 100) const; 
    bool                WritePNG(const char *filename) const;
    
                        // static helper functions for image information
    static const char * FormatName(Image::Format imageFormat);
    static int          BytesPerPixel(Image::Format imageFormat);
    static int          BytesPerBlock(Image::Format imageFormat);
    static int          NumComponents(Image::Format imageFormat);
    static void         GetBits(Image::Format imageFormat, int *redBits, int *greenBits, int *blueBits, int *alphaBits);
    static bool         HasAlpha(Image::Format imageFormat);
    static bool         IsPacked(Image::Format imageFormat);
    static bool         IsCompressed(Image::Format imageFormat);
    static bool         IsFloatFormat(Image::Format imageFormat);
    static bool         IsHalfFormat(Image::Format imageFormat);
    static bool         IsDepthFormat(Image::Format imageFormat);
    static bool         IsDepthStencilFormat(Image::Format imageFormat);
    static int          MemRequired(int width, int height, int depth, int numMipmaps, Image::Format imageFormat);
    static int          MaxMipMapLevels(int width, int height, int depth);

    static Image *      NewImageFromFile(const char *filename);

private:
    bool                LoadDDSFromMemory(const char *name, const byte *data, size_t size);
    bool                LoadPVRFromMemory(const char *name, const byte *data, size_t size);
    bool                LoadPVR2FromMemory(const char *name, const byte *data, size_t size);
    bool                LoadPVR3FromMemory(const char *name, const byte *data, size_t size);
    bool                LoadBMPFromMemory(const char *name, const byte *data, size_t size);
    bool                LoadPCXFromMemory(const char *name, const byte *data, size_t size);
    bool                LoadTGAFromMemory(const char *name, const byte *data, size_t size);
    bool                LoadJPGFromMemory(const char *name, const byte *data, size_t size);
    bool                LoadPNGFromMemory(const char *name, const byte *data, size_t size);
    bool                LoadHDRFromMemory(const char *name, const byte *data, size_t size);
    
    int                 width;          ///< Width
    int                 height;         ///< Height
    int                 depth;          ///< Depth
    int                 numSlices;      ///< Number of array images or 6 for cubic image
    int                 numMipmaps;     ///< Number of mipmaps
    Image::Format       format;         ///< Image format
    int                 flags;
    bool                allocated;      ///< Is memory allocated ?
    byte *              pic;            ///< Actual pixel data
};

BE_INLINE Image::Image() {
    width = 0;
    height = 0;
    depth = 0;
    numSlices = 0;
    numMipmaps = 0;
    format = Image::UnknownFormat;
    flags = 0;
    allocated = false;
    pic = nullptr;
}

BE_INLINE Image::Image(int width, int height, int depth, int numSlices, int numMipmaps, Image::Format format, byte *data, int flags) {
    allocated = false;
    InitFromMemory(width, height, depth, numSlices, numMipmaps, format, data, flags);
    //Create(width, height, depth, numSlices, numMipmaps, format, data, flags);
}

BE_INLINE Image::Image(const Image &rhs) {
    allocated = false;
    Create(rhs.width, rhs.height, rhs.depth, rhs.numSlices, rhs.numMipmaps, rhs.format, rhs.pic, rhs.flags);
}

BE_INLINE Image::Image(Image &&rhs) : Image() {
    BE1::Swap(width, rhs.width);
    BE1::Swap(height, rhs.height);
    BE1::Swap(depth, rhs.depth);
    BE1::Swap(numSlices, rhs.numSlices);
    BE1::Swap(numMipmaps, rhs.numMipmaps);
    BE1::Swap(format, rhs.format);
    BE1::Swap(flags, rhs.flags);
    BE1::Swap(allocated, rhs.allocated);
    BE1::Swap(pic, rhs.pic);
}

BE_INLINE Image::~Image() {
    Clear();
}
    
BE_INLINE int Image::GetWidth(int mipMapLevel) const {
    int a = width >> mipMapLevel;
    return (a == 0) ? 1 : a;
}

BE_INLINE int Image::GetHeight(int mipMapLevel) const {
    int a = height >> mipMapLevel;
    return (a == 0) ? 1 : a;
}

BE_INLINE int Image::GetDepth(int mipMapLevel) const {
    int a = depth >> mipMapLevel;
    return (a == 0) ? 1 : a;
}

BE_INLINE byte *Image::GetPixels(int level) const {
    return (level < numMipmaps) ? pic + GetSize(0, level) : nullptr;
}

BE_INLINE byte *Image::GetPixels(int level, int sliceIndex) const {
    if (level >= numMipmaps || sliceIndex >= numSlices) return nullptr;
    return pic + GetSliceSize(0, numMipmaps) * sliceIndex + GetSliceSize(0, level);
}

BE_INLINE Image &Image::Create2D(int width, int height, int numMipmaps, Image::Format format, const byte *data, int flags) {
    return Create(width, height, 1, 1, numMipmaps, format, data, flags);
}

BE_INLINE Image &Image::Create3D(int width, int height, int depth, int numMipmaps, Image::Format format, const byte *data, int flags) {
    return Create(width, height, depth, 1, numMipmaps, format, data, flags);
}

BE_INLINE Image &Image::CreateCube(int size, int numMipmaps, Image::Format format, const byte *data, int flags) {
    return Create(size, size, 1, 6, numMipmaps, format, data, flags);
}

BE_INLINE Image &Image::Create2DArray(int width, int height, int numSlices, int numMipmaps, Image::Format format, const byte *data, int flags) {
    return Create(width, height, 1, numSlices, numMipmaps, format, data, flags);
}

BE_NAMESPACE_END
