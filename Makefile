CXX:=c++
CXXFLAGS:=-W -Wall -O2
FILES:= main.cpp
HEADERS:=PHSerialization.h
EXECUTABLE:=PHSerialization

all: $(FILES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(FILES) -o $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)
