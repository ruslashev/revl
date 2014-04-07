CXX = clang++
EXECNAME = revl

default: $(EXECNAME)
	./$(EXECNAME)

$(EXECNAME): .objs/main.o .objs/lexer.o .objs/utils.o .objs/parser.o .objs/evaluator.o
	$(CXX) -o $@ $^

.objs/%.o: %.cpp
	$(CXX) -c -o $@ $< -Wall -g -std=c++0x
 
clean:
	-rm -f .objs/*.o $(EXECNAME)

