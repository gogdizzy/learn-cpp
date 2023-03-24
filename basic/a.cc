#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

enum class ObstacleState : int8_t
{
    DontCare = -2,
    Space = -1,
    Unknown = 0,
    Obstacle = 1
};

void rrlog(int a, int b, int c, int d, const char* s1, const char* s2, const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
}
int main() {
    ObstacleState state = ObstacleState::Space;
    rrlog(1, 2, 3, 4, "t1", "t2", "%d %d %d\n", 22, 16, state);
    return 0;
}
