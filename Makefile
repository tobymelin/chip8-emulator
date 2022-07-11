CXX = g++
CFLAGS = $(shell sdl2-config --libs --cflags)

OUTPUTDIR = dist

.cpp:
	@mkdir -p $(OUTPUTDIR)
	$(CXX) $(CFLAGS) -o $(OUTPUTDIR)/$@ $@.cpp

build:
	cd src/; \
	$(CXX) $(CFLAGS) -o ../$(OUTPUTDIR)/chip8 main.cpp io.cpp cpu.cpp memory.cpp chip8.cpp

clean: 
	$(RM) $(OUTPUTDIR)/*

