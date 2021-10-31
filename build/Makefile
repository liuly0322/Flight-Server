CXX = g++
CFLAGS = -std=c++14 -O2 -Wall 

TARGET = server
OBJS = ../code/log/*.cpp ../code/timer/*.cpp \
       ../code/http/*.cpp ../code/server/*.cpp \
       ../code/buffer/*.cpp ../code/main.cpp \
	   ../code/ticket/*.cpp

all: $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o ../bin/$(TARGET)  -pthread

clean:
	rm -rf ../bin/$(OBJS) $(TARGET)




