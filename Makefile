CXX = g++
CFLAGS = -L/usr/local/lib -lSDL2 -I/usr/local/include/SDL2 -D_THREAD_SAFE

OUTPUTDIR = dist

.cpp:
	@mkdir -p $(OUTPUTDIR)
	$(CXX) $(CFLAGS) -o $(OUTPUTDIR)/$@ $@.cpp

build:
	cd src/; \
	$(CXX) $(CFLAGS) -o ../$(OUTPUTDIR)/chip8 main.cpp io.cpp cpu.cpp memory.cpp chip8.cpp

clean: 
	$(RM) $(OUTPUTDIR)/*

