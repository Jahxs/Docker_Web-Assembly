# Academic Research & Proofs of Concept: Docker and WebAssembly (WASM)

**Institution:** Central University (Universidad Central)  
**Faculty / Degree:** Systems Engineering  
**Course:** Web Programming  
**Authors:**
- Jhostin Molina
- Sebastian Escuntar  

---

## 1. Project Overview
This repository contains an academic inquiry and two practical Proofs of Concept (POCs) demonstrating modern containerization and client-side binary execution technologies:

1. **Docker (`docker-poc/`):** Operating System-level virtualization demonstrating containerized deployment of an Nginx web server on Alpine Linux, exposed on port `8080`.
2. **WebAssembly (`webassembly-poc/`):** High-performance binary execution demonstrating a C function compiled via Emscripten into `.wasm` bytecode and executed inside the browser and Node.js on port `8000`.

---

## 2. Directory Structure
```text
Docker_Web-Assembly/
├── ACADEMIC_RESEARCH_REPORT.md   # Complete theoretical research report & defense guide
├── README.md                     # Main repository documentation (this file)
├── docker-poc/                   # Docker Proof of Concept
│   ├── Dockerfile                # Nginx Alpine container definition
│   ├── index.html                # "Hello from Docker!" webpage
│   └── README.md                 # Docker project guide
└── webassembly-poc/              # WebAssembly Proof of Concept
    ├── add.c                     # C source code with exported addition function
    ├── add.wasm                  # Compiled WebAssembly binary bytecode (349 bytes)
    ├── add.js                    # Emscripten runtime glue code
    ├── index.html                # Interactive UI with inputs & calculate button
    ├── style.css                 # Application styling
    └── README.md                 # WebAssembly project guide
```

---

## 3. Quick Start & Verification

### 3.1. Docker POC
* **URL:** [http://localhost:8080](http://localhost:8080)
* **Build Command:**
  ```powershell
  docker build -t docker-poc-web:1.0 ./docker-poc
  ```
* **Run Command:**
  ```powershell
  docker run -d --name docker-poc-app -p 8080:80 docker-poc-web:1.0
  ```
* **Check Status:**
  ```powershell
  docker ps --filter "name=docker-poc-app"
  ```

---

### 3.2. WebAssembly POC
* **URL:** [http://localhost:8000](http://localhost:8000)
* **Compile C to Wasm (via Emscripten Container):**
  ```powershell
  docker run --rm -v "c:\Users\User\Docker_Web-Assembly\webassembly-poc:/src" emscripten/emsdk emcc add.c -O2 -s WASM=1 -s EXPORTED_FUNCTIONS="['_add']" -s EXPORTED_RUNTIME_METHODS="['ccall','cwrap']" -o add.js
  ```
* **Start Web Server:**
  ```powershell
  docker run -d --name wasm-poc-server -p 8000:80 -v "c:/Users/User/Docker_Web-Assembly/webassembly-poc:/usr/share/nginx/html:ro" nginx:alpine
  ```
* **CLI Test (Node.js):**
  ```powershell
  node -e "const m = require('./webassembly-poc/add.js'); m.onRuntimeInitialized = () => { console.log('10 + 5 =', m._add(10, 5)); };"
  ```

---

## 4. Academic Documentation
For the full theoretical analysis (covering architecture, historical background, comparison matrix, and professor defense questions), see:
* [`ACADEMIC_RESEARCH_REPORT.md`](./ACADEMIC_RESEARCH_REPORT.md)
