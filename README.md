# Websocket - raylib

this is a simple implementation of a real-time chat using c++, raylib and nodepp

# Dependencies
```bash
📌 nodepp: https://github.com/NodeppOficial/nodepp
📌 raylib: https://www.raylib.com/
```

# Preview

# Build & Run
```bash
time g++ -o main main.cpp -I./include -L./lib -lraylib -lssl -lcrypto -std=c++11 
./main ?mode=server #Run As Server Mode
./main ?mode=client #Run As Client Mode
```