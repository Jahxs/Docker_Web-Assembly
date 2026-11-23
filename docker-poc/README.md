# Proof of Concept: Docker Containerized Web Application (`docker-poc`)

**Institution:** Central University (Universidad Central)  
**Faculty / Degree:** Systems Engineering  
**Course:** Web Programming  
**Authors:**
- Jhostin Molina
- Sebastian Escuntar  

---

## 1. Project Overview
This project demonstrates how to package, distribute, and execute a lightweight static web application using **Docker** and the **Nginx Alpine** base image.

The objective of this Proof of Concept (POC) is to verify containerization principles, demonstrate how Docker packages code and its dependencies into an immutable image, and serve the application on port `8080` of the host system.

---

## 2. Project File Structure
```text
docker-poc/
├── Dockerfile      # Build specifications and instructions
├── index.html      # Static HTML5 webpage ("Hello from Docker!")
└── README.md       # Technical documentation and execution guide
```

---

## 3. Source Code Explanation

### 3.1. `index.html`
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Docker POC - Academic Demo</title>
</head>
<body>
    <div class="card">
        <h1>Hello from Docker!</h1>
        <p>Proof of Concept successfully deployed using <strong>Docker</strong> and <strong>Nginx Alpine</strong>.</p>
        <div>
            <span class="badge">Nginx Alpine</span>
            <span class="badge">Port 8080:80</span>
            <span class="badge">Docker Container</span>
        </div>
    </div>
</body>
</html>
```
- **Line-by-line explanation:**
  - `<h1>Hello from Docker!</h1>`: Main heading displayed in the browser, fulfilling academic assignment requirements.
  - `<div class="card">`: Centered CSS container providing a polished, professional presentation.
  - `<span class="badge">`: Metadata badges indicating the container runtime environment.

### 3.2. `Dockerfile`
```dockerfile
# Step 1: Base image from official Alpine Linux with Nginx
FROM nginx:alpine

# Step 2: Copy the custom static HTML website into the Nginx public web directory
COPY index.html /usr/share/nginx/html/index.html

# Step 3: Document that the container listens on port 80
EXPOSE 80
```
- **Line-by-line explanation:**
  - `FROM nginx:alpine`: Specifies the starting base image. Alpine Linux is chosen for its minimal footprint (~23 MB) and fast download speed.
  - `COPY index.html /usr/share/nginx/html/index.html`: Copies the local `index.html` from the host build context into the standard document root directory of Nginx.
  - `EXPOSE 80`: Informs the Docker engine and operators that the container listens on internal TCP port 80.

---

## 4. Execution Commands & Technical Guide

### 4.1. Execution on Windows (PowerShell)

1. **Navigate to the project root:**
   ```powershell
   cd c:\Users\User\Docker_Web-Assembly
   ```

2. **Build the Docker Image:**
   ```powershell
   docker build -t docker-poc-web:1.0 ./docker-poc
   ```
   *Explanation:* Reads the `Dockerfile` in `./docker-poc` and creates a local image tagged as `docker-poc-web:1.0`.

3. **Run the Container in Detached Mode:**
   ```powershell
   docker run -d --name docker-poc-app -p 8080:80 docker-poc-web:1.0
   ```
   *Explanation:* 
   - `-d`: Runs the container in the background (detached mode).
   - `--name docker-poc-app`: Assigns an explicit container name.
   - `-p 8080:80`: Forwards host port `8080` to internal container port `80`.
   - `docker-poc-web:1.0`: The built image tag.

4. **Verify Container Status:**
   ```powershell
   docker ps --filter "name=docker-poc-app"
   ```

5. **Verify in Browser / Terminal:**
   - Web Browser: Open `http://localhost:8080`
   - Terminal Verification:
     ```powershell
     (Invoke-WebRequest -Uri "http://localhost:8080").StatusCode
     ```

6. **Cleanup (When finished):**
   ```powershell
   docker stop docker-poc-app
   docker rm docker-poc-app
   ```

---

### 4.2. Execution on AlmaLinux 10 Minimal (VMware)

1. **Install Docker CE (or Podman):**
   ```bash
   sudo dnf install -y yum-utils
   sudo yum-config-manager --add-repo https://download.docker.com/linux/centos/docker-ce.repo
   sudo dnf install -y docker-ce docker-ce-cli containerd.io
   sudo systemctl enable --now docker
   ```

2. **Build the Docker Image:**
   ```bash
   cd ~/docker-poc
   docker build -t docker-poc-web:1.0 .
   ```

3. **Run the Container:**
   ```bash
   docker run -d --name docker-poc-app -p 8080:80 docker-poc-web:1.0
   ```

4. **Open Firewall Port on AlmaLinux:**
   ```bash
   sudo firewall-cmd --zone=public --add-port=8080/tcp --permanent
   sudo firewall-cmd --reload
   ```

5. **Test via cURL:**
   ```bash
   curl http://localhost:8080
   ```

---

## 5. Actual Test Verification Results
- **Build Status:** Successfully built layer `[2/2] COPY index.html /usr/share/nginx/html/index.html`.
- **Container Status:**
  ```text
  CONTAINER ID   IMAGE                STATUS         PORTS                                     NAMES
  7bba7359d1b2   docker-poc-web:1.0   Up 8 minutes   0.0.0.0:8080->80/tcp, [::]:8080->80/tcp   docker-poc-app
  ```
- **HTTP Verification:** HTTP/1.1 `200 OK`
- **Rendered Output:** `"Hello from Docker!"` successfully delivered to the client browser.
