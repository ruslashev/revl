OBJ = .objs/main.o
CXX = clang++
EXECNAME = revl
 
$(EXECNAME): $(OBJ)
	$(CXX) -o $@ $^
	./$(EXECNAME)

.objs/%.o: %.cpp
	$(CXX) -c -o $@ $< -Wall -g -std=c++0x

clean:
	-rm -f .objs/*.o $(EXECNAME)

