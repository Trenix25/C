# Makefile for userdb
#
# Written by Matthew Campbell.
#
# Do not define CC if you are using C4droid.
#
# Compiler to use:
#
CC = gcc
#
# Compiler flags:
#
#CFLAGS = -c -pedantic -std=c17 -Wall
CFLAGS = -c -O2 -pedantic -std=c17 -Wall
#
# Linker flags:
#
LFLAGS = -lm
#
# Define the header file.
#
INC = userdb.h
#
# Define the source code files.
#
SRC = \
      add_data_base_entry.c \
      enter_to_continue.c \
      list_data_base_items.c \
      load_data_base.c \
      print_menu.c \
      read_stdin.c \
      remove_data_base_entry.c \
      save_data_base.c \
      sort_data_base.c \
      userdb.c
#
# Define the object code files.
#
OBJ = \
      add_data_base_entry.o \
      enter_to_continue.o \
      list_data_base_items.o \
      load_data_base.o \
      print_menu.o \
      read_stdin.o \
      remove_data_base_entry.o \
      save_data_base.o \
      sort_data_base.o \
      userdb.o
#
# Define the default target.
#
all: userdb
#
# Define the userdb target.
#
userdb: objects
	@echo "Linking the object files with library files."
	$(CC) $(LFLAGS) $(OBJ) -o userdb
	rm -f *.o
	@echo
#
# Define the objects target.
#
objects: $(SRC) $(INC)
	@echo
	@echo "Compiling the source code files."
	$(CC) $(CFLAGS) $(SRC)
	@echo
#
# Define the clean target.
#
clean:
	@echo
	@echo "Cleaning up."
	rm -f *.o userdb
	@echo
#
# EOF
