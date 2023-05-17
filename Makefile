COURSE = cs240
SEMESTER = spring2023
CP_NUMBER = 5
LASTNAME = Klein
GITUSERID = Sam-Klein1

REPODIR = ../$(COURSE)-$(SEMESTER)-cp$(CP_NUMBER)-group-013
TARFILE = CP$(CP_NUMBER)_Group_013.tar

FLAGS = -O3 -g #-w
CC = g++
BIN = bin
OBJ = obj
SRC = src
IN = input_files

all: $(OBJ)/helper.o

pr1: $(BIN)/pr_check
	bin/pr_check $(IN)/prereqs.txt

pr2: $(BIN)/pr_check
	bin/pr_check $(IN)/bad.txt

sch: $(BIN)/sch_check
	bin/sch_check $(IN)/prereqs.txt $(IN)/SanjeevSchedule.txt

wait1: $(BIN)/wait_list
	bin/wait_list 2023Fall $(IN)/bad.txt $(IN)/schedules.txt $(IN)/enroll.txt

wait2: $(BIN)/wait_list
	bin/wait_list 2023Fall $(IN)/prereqs.txt $(IN)/badSchedules.txt $(IN)/enroll.txt

wait3: $(BIN)/wait_list
	bin/wait_list 2023Fall $(IN)/prereqs.txt $(IN)/schedules.txt $(IN)/enroll.txt
# ------------------------------------------------------------------------
# Builds the exe files

$(BIN)/pr_check: $(OBJ)/pr_check.o $(OBJ)/helper.o
	$(CC) $(FLAGS) $(OBJ)/pr_check.o $(OBJ)/helper.o -o $@

$(BIN)/sch_check: $(OBJ)/sch_check.o $(OBJ)/helper.o
	$(CC) $(FLAGS) $(OBJ)/sch_check.o $(OBJ)/helper.o -o $@

$(BIN)/wait_list: $(OBJ)/wait_list.o $(OBJ)/helper.o
	$(CC) $(FLAGS) $(OBJ)/wait_list.o $(OBJ)/helper.o -o $@

# ------------------------------------------------------------------------
# Builds the .o files

$(OBJ)/pr_check.o: $(SRC)/pr_check.cpp
	$(CC) $(FLAGS) -c $(SRC)/pr_check.cpp -o $@

$(OBJ)/sch_check.o: $(SRC)/sch_check.cpp
	$(CC) $(FLAGS) -c $(SRC)/sch_check.cpp -o $@

$(OBJ)/wait_list.o: $(SRC)/wait_list.cpp 
	$(CC) $(FLAGS) -c $(SRC)/wait_list.cpp -o $@

$(OBJ)/helper.o: $(SRC)/helper.cpp
	$(CC) $(FLAGS) -c $(SRC)/helper.cpp -o $@



tar:	clean
	tar cvvf $(TARFILE) $(REPODIR)
	gzip $(TARFILE)

clean:
	rm -f $(OBJ)/*.o $(BIN)/* *.tar.gz
