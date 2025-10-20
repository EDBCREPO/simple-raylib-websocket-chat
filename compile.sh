export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH

g++ -o main main.cpp -I./include -L./lib -lssl -lcrypto -lz -lraylib

if [ ! $? -eq 0 ]; then
    echo "exit error"; exit;
fi

./main ?mode=server