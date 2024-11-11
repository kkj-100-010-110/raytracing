#ifndef _RTW_STB_IMAGE_H_
#define _RTW_STB_IMAGE_H_

// Disable strict warnings for this header from the Microsoft Visual C++ compiler.
#ifdef _MSC_VER
    #pragma warning (push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <stb/stb_image.h>

#include <cstdlib>
#include <iostream>

class RtwImage
{
public:
    RtwImage() {}

    RtwImage(const char* imageFilename)
    {
        // Loads image data from the specified file. If the RTW_IMAGES environment variable is
        // defined, looks only in that directory for the image file. If the image was not found,
        // searches for the specified image file first from the current directory, then in the
        // images/ subdirectory, then the _parent's_ images/ subdirectory, and then _that_
        // parent, on so on, for six levels up. If the image was not loaded successfully,
        // width() and height() will return 0.

        auto filename = std::string(imageFilename);
        auto imageDir = getenv("RTW_IMAGES");

        // Hunt for the image file in some likely locations.
        if (imageDir && load(std::string(imageDir) + "/" + imageFilename)) return;
        if (load(filename)) return;
        if (load("images/" + filename)) return;
        if (load("../images/" + filename)) return;
        if (load("../../images/" + filename)) return;
        if (load("../../../images/" + filename)) return;
        if (load("../../../../images/" + filename)) return;
        if (load("../../../../../images/" + filename)) return;
        if (load("../../../../../../images/" + filename)) return;

        std::cerr << "ERROR: Could not load image file '" << imageFilename << "'.\n";
    }

    ~RtwImage()
    {
        delete[] bdata;
        STBI_FREE(fdata);
    }

    bool load(const std::string &filename)
    {
        // Loads the linear (gamma=1) image data from the given file name. Returns true if the
        // load succeeded. The resulting data buffer contains the three [0.0, 1.0]
        // floating-point values for the first pixel (red, then green, then blue). Pixels are
        // contiguous, going left to right for the width of the image, followed by the next row
        // below, for the full height of the image.

        auto n = bytesPerPixel; // Dummy out parameter: original components per pixel
        fdata = stbi_loadf(filename.c_str(), &imageWidth, &imageHeight, &n, bytesPerPixel);
        if (fdata == nullptr)
            return false;

        bytesPerScanline = imageWidth * bytesPerPixel;
        convertToBytes();
        return true;
    }

    int width() const { return (fdata == nullptr) ? 0 : imageWidth; }
    int height() const { return (fdata == nullptr) ? 0 : imageHeight; }

    const unsigned char *pixel_data(int x, int y) const
    {
        // Return the address of the three RGB bytes of the pixel at x,y. If there is no image
        // data, returns magenta.
        static unsigned char magenta[] = {255, 0, 255};
        if (bdata == nullptr)
            return magenta;

        x = clamp(x, 0, imageWidth);
        y = clamp(y, 0, imageHeight);

        return bdata + y * bytesPerScanline + x * bytesPerPixel;
    }

private:
    const int bytesPerPixel = 3;
    float *fdata = nullptr;         // Linear floating point pixel data
    unsigned char *bdata = nullptr; // Linear 8-bit pixel data
    int imageWidth = 0;            // Loaded image width
    int imageHeight = 0;           // Loaded image height
    int bytesPerScanline = 0;

    static int clamp(int x, int low, int high)
    {
        // Return the value clamped to the range [low, high).
        if (x < low) return low;
        if (x < high) return x;
        return high - 1;
    }

    static unsigned char floatToByte(float value)
    {
        if (value <= 0.0) return 0;
        if (1.0 <= value)
            return 255;
        return static_cast<unsigned char>(256.0 * value);
    }

    void convertToBytes()
    {
        // Convert the linear floating point pixel data to bytes, storing the resulting byte
        // data in the `bdata` member.

        int total_bytes = imageWidth * imageHeight * bytesPerPixel;
        bdata = new unsigned char[total_bytes];

        // Iterate through all pixel components, converting from [0.0, 1.0] float values to
        // unsigned [0, 255] byte values.

        auto *bptr = bdata;
        auto *fptr = fdata;
        for (auto i = 0; i < total_bytes; i++, fptr++, bptr++)
            *bptr = floatToByte(*fptr);
    }
};

// Restore MSVC compiler warnings
#ifdef _MSC_VER
    #pragma warning(pop)
#endif

#endif//_RTW_STB_IMAGE_H_