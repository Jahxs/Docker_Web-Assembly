#include <emscripten.h>

/**
 * Academic Proof of Concept - WebAssembly
 * Adds two integers and returns the result.
 * EMSCRIPTEN_KEEPALIVE ensures the compiler does not optimize away
 * or eliminate this function as dead code.
 */
EMSCRIPTEN_KEEPALIVE
int add(int a, int b) {
    return a + b;
}
