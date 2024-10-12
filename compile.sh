#Compile
time g++ -o main main.cpp -I./include -L./lib -lraylib -lssl -lcrypto -std=c++11 

#Run As Server Mode
./main ?mode=server

#Run As Client Mode
./main ?mode=client