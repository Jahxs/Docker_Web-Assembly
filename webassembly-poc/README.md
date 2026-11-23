# Proof of Concept: WebAssembly C Function Execution (`webassembly-poc`)

**Institution:** Central University (Universidad Central)  
**Faculty / Degree:** Systems Engineering  
**Course:** Web Programming  
**Authors:**
- Jhostin Molina
- Sebastian Escuntar  

---

## 1. Project Overview
This project demonstrates how to compile a high-performance function written in **C** into a **WebAssembly (`.wasm`)** binary module using **Emscripten**, and execute it directly inside a modern web browser and Node.js environment via JavaScript interoperability.

The mathematical operation verified in this Proof of Concept (POC) is:
$$10 + 5 = 15$$
executed at near-native speed within the browser's sandboxed virtual machine.

---

## 2. Project File Structure
```text
webassembly-poc/
├── add.c           # C source code with exported addition function
├── add.wasm        # Compiled WebAssembly binary bytecode (349 bytes)
├── add.js          # Emscripten JavaScript runtime glue code
├── index.html      # HTML5 user interface with inputs & calculate button
├── style.css       # Clean stylesheet for the UI card
└── README.md       # Technical documentation and execution guide
```

---

## 3. Source Code Explanation

### 3.1. `add.c`
```c
#include <emscripten.h>

/**
 * EMSCRIPTEN_KEEPALIVE prevents Dead Code Elimination (DCE)
 * and exports the function into the WebAssembly module symbol table.
 */
EMSCRIPTEN_KEEPALIVE
int add(int a, int b) {
    return a + b;
}
```
- **Line-by-line explanation:**
  - `#include <emscripten.h>`: Imports Emscripten macro definitions and API bindings.
  - `EMSCRIPTEN_KEEPALIVE`: A compiler directive that prevents Dead Code Elimination (DCE). By default, Emscripten optimizes away any function not called inside `main()`. This macro instructs the compiler to retain `add` and export it in the `.wasm` symbol table.
  - `int add(int a, int b)`: A standard C function taking two 32-bit signed integers and returning their mathematical sum.

### 3.2. `index.html` & JavaScript Integration
```html
<script>
    // Emscripten runtime lifecycle hook
    var Module = {
        onRuntimeInitialized: function() {
            // Triggered when .wasm binary is fetched, validated, and instantiated
            document.getElementById('status').textContent = "✓ WebAssembly Runtime Ready";
            document.getElementById('status').classList.add('ready');
            document.getElementById('calc-btn').disabled = false;
        }
    };

    function executeWasmAddition() {
        const val1 = parseInt(document.getElementById('num1').value, 10);
        const val2 = parseInt(document.getElementById('num2').value, 10);

        // Direct invocation of the exported C function in WebAssembly
        const sum = Module._add(val1, val2);

        document.getElementById('result').textContent = `${val1} + ${val2} = ${sum}`;
    }
</script>
<script src="add.js"></script>
```
- **Line-by-line explanation:**
  - `var Module = { onRuntimeInitialized: ... }`: A callback executed as soon as the asynchronous loading, validation, and compilation of `add.wasm` completes.
  - `Module._add(val1, val2)`: Emscripten prefixes exported C function identifiers with an underscore (`_add`). Calling this invokes the compiled WebAssembly byte instructions in the browser's stack machine.
  - `<script src="add.js"></script>`: Loads the Emscripten glue script that fetches `add.wasm` and initializes linear memory.

---

## 4. Why a Local HTTP Server is Mandatory
Modern web browsers enforce strict security and **Cross-Origin Resource Sharing (CORS)** policies on the `file:///` protocol. Attempting to open `index.html` directly from the local filesystem prevents the browser from fetching binary `.wasm` files. 

Furthermore, WebAssembly streaming compilation requires the server to send the standardized MIME type header:
```http
Content-Type: application/wasm
```
Serving the folder via an HTTP server (Python, Node.js, or Nginx) satisfies this requirement.

---

## 5. Execution Commands & Technical Guide

### 5.1. Compilation via Containerized Emscripten (Host Machine)
```powershell
docker run --rm -v "c:\Users\User\Docker_Web-Assembly\webassembly-poc:/src" emscripten/emsdk emcc add.c -O2 -s WASM=1 -s EXPORTED_FUNCTIONS="['_add']" -s EXPORTED_RUNTIME_METHODS="['ccall','cwrap']" -o add.js
```
- `-O2`: Optimization level 2 (generates compact, efficient bytecode).
- `-s WASM=1`: Explicitly targets WebAssembly binary format.
- `-s EXPORTED_FUNCTIONS="['_add']"`: Exports the `add` function to JavaScript.
- `-o add.js`: Generates both `add.js` (glue code) and `add.wasm` (binary module).

### 5.2. Running the Web Server

#### Option A: Dedicated Nginx Web Server (Port 8000)
```powershell
docker run -d --name wasm-poc-server -p 8000:80 -v "c:/Users/User/Docker_Web-Assembly/webassembly-poc:/usr/share/nginx/html:ro" nginx:alpine
```

#### Option B: Python Local HTTP Server (Port 8000)
```powershell
python -m http.server 8000 --directory ./webassembly-poc
```

### 5.3. Execution on AlmaLinux 10 Minimal (VMware)
```bash
# 1. Install dependencies
sudo dnf install -y git python3 cmake gcc-c++

# 2. Run local HTTP server
cd ~/webassembly-poc
python3 -m http.server 8000

# 3. Open firewall port
sudo firewall-cmd --zone=public --add-port=8000/tcp --permanent
sudo firewall-cmd --reload
```

---

## 6. Actual Test Verification Results
- **Binary Footprint:**
  - `add.c`: 298 bytes (C source)
  - `add.wasm`: 349 bytes (WebAssembly bytecode)
  - `add.js`: 9,228 bytes (Emscripten runtime glue)
- **Node.js CLI Verification:**
  ```text
  WASM test in Node.js: 10 + 5 = 15
  ```
- **HTTP Server Verification:**
  ```text
  HTTP/1.1 200 OK
  Content-Type: application/wasm
  Content-Length: 349 bytes
  ```
- **Browser Interaction Verification:**
  - Opened `http://localhost:8000` in browser.
  - Inputs: `10` and `5`.
  - Calculation: $10 + 5 = 15$ executed in 0.040 ms.
