# Websocket - raylib

This is a simple implementation of a real-time chat using c++, raylib and nodepp

# Dependencies
```bash
📌 nodepp: https://github.com/NodeppOficial/nodepp
📌 raylib: https://www.raylib.com/
```

# Preview
[Imagen](https://github.com/user-attachments/assets/609e7346-0be3-4875-a23b-bd9c8ad5ae18)

# Build & Run
```bash
time g++ -o main main.cpp -I./include -L./lib -lraylib -lssl -lcrypto -std=c++11 
./main ?mode=server #Run As Server Mode
./main ?mode=client #Run As Client Mode
```
