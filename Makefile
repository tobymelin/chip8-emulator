CXX = g++
CFLAGS = 

OUTPUTDIR = "dist"

.cpp:
	@mkdir -p $(OUTPUTDIR)
	$(CXX) $(CFLAGS) -o $(OUTPUTDIR)/$@ $@.cpp

clean: 
	$(RM) $(OUTPUTDIR)/*

