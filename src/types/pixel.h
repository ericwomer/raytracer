#include <cstdint>

enum class pixel_depth : uint8_t
{
    BITS_PER_CHANNEL_8,
    BITS_PER_CHANNEL_16,
    BITS_PER_CHANNEL_32,
    BITS_PER_CHANNEL_48
};

template <class T>
class PixelRGBA {
public:
    inline T    r() const { return red; }
    inline void r(T r) { red = r; }
    inline T    g() const { return green; }
    inline void g(T g) { green = g; }
    inline T    b() const { return blue; }
    inline void b(T b) { blue = b; }
    inline T    a() const { return alpha; }
    inline void a(T a) { blue = a; }

private:
    T red;
    T green;
    T blue;
    T alpha;
};