CXX= g++ -std=c++11
CFLAGS= -Wall -Wextra $(MYCFLAGS)
LDFLAGS= $(MYLDFLAGS)
LIBS= $(MYLIBS)

MYCFLAGS=
MYLDFLAGS=
MYLIBS= -lgtest_main -lgtest -lpthread

AR= ar rcu
RANLIB= ranlib
RM= rm -rf

MAIN_A= libstr_vec.a
BASE_O= str_vec.o str_vec_impl.o

MAIN_T= str_vec_test
MAIN_O= str_vec_test.o

# Targets start here
a: $(MAIN_A)

t: $(MAIN_T)

$(MAIN_A): $(BASE_O)
	$(AR) $@ $(BASE_O)
	$(RANLIB) $@

$(MAIN_T): $(MAIN_O) $(MAIN_A)
	$(CXX) -o $@ $(LDFLAGS) $(MAIN_O) $(MAIN_A) $(LIBS)

echo:
	@echo "CXX= $(CXX)"
	@echo "CFLAGS= $(CFLAGS)"
	@echo "LDFLAGS= $(LDFLAGS)"
	@echo "LIBS= $(LIBS)"
	@echo "AR= $(AR)"
	@echo "RANLIB= $(RANLIB)"
	@echo "RM= $(RM)"

clean:
	$(RM) $(MAIN_A) $(BASE_O) $(MAIN_T) $(MAIN_O)

# Convenience targets

.PHONY: a t echo clean

str_vec.o: str_vec.cc str_vec.h
str_vec_impl.o: str_vec_impl.cc str_vec_impl.h
str_vec_test.o: str_vec_test.cc str_vec.h
