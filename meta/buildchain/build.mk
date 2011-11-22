.cc.o:
	$(CXX) $(CXXFLAGS) -o $@ $<
link:
	$(CXX) $(LDFLAGS) $(TARGETS) -o $(OUTPUT)
