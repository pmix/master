# Set the environment variable PMIX_INSTALLDIR to point to your local PMIx install directory
PMIX_INSTALLDIR ?= $(HOME)/pmix-install

#INCLUDES = -I$(PMIX_INSTALLDIR)/include -I. -I../../include -I../.. -I../../src/include -I/usr/local/include/event2
INCLUDES = -I$(PMIX_INSTALLDIR)/include -I. -I../../include -I../.. -I../../src/include
#vpath %.h .:$(PMIX_INSTALLDIR)/include:../../include:../..:../../src/include:/usr/local/include/event2
vpath %.h .:$(PMIX_INSTALLDIR)/include:../../include:../..:../../src/include
LDIR = $(PMIX_INSTALLDIR)/lib

CC = gcc
CFLAGS = $(INCLUDES) -fno-omit-frame-pointer -g -Wfatal-errors #-Wall -Wextra -Wpedantic -Wconversion -Wshadow
# Use CFLAGS below instead if you want to run with Sanitizer
#CFLAGS = $(INCLUDES) -fno-omit-frame-pointer -fsanitize=address -fsanitize=leak -fsanitize=undefined -g -Wfatal-errors -Wall
LDFLAGS = -Wl,-rpath,$(LDIR) -L/usr/local/lib -Wl,-rpath,/usr/local/lib #-Wl,--stack,10485760
LIBS =-lpmix -lpthread -levent # -lefence # <-- can link against electric fence for memory checking

# Executables
EXE = pmix_test test_init_fin test_helloworld test_get_basic test_get_peers test_fence_basic test_fence_wildcard
# List of all .c source files.
PCFILES = pmix_test.c test_common.c cli_stages.c test_server.c server_callbacks.c base64_enc_dec.c
TCCOMMONFILES = test_common.c test_clientapp_funcs.c base64_enc_dec.c
TC1FILES = test_init_fin.c $(TCCOMMONFILES)
TC2FILES = test_helloworld.c $(TCCOMMONFILES)
TC3FILES = test_get_basic.c $(TCCOMMONFILES)
TC4FILES = test_get_peers.c $(TCCOMMONFILES)
TC5FILES = test_fence_basic.c $(TCCOMMONFILES)
TC6FILES = test_fence_wildcard.c $(TCCOMMONFILES)
TCFILES = $(TC1FILES) $(TC2FILES) $(TC3FILES) $(TC4FILES) $(TC5FILES) $(TC6FILES)
CFILES = $(PCFILES) $(TCFILES)

OBJ = $(CFILES:%.c=%.o)
POBJ = $(PCFILES:%.c=%.o)
T1OBJ = $(TC1FILES:%.c=%.o)
T2OBJ = $(TC2FILES:%.c=%.o)
T3OBJ = $(TC3FILES:%.c=%.o)
T4OBJ = $(TC4FILES:%.c=%.o)
T5OBJ = $(TC5FILES:%.c=%.o)
T6OBJ = $(TC6FILES:%.c=%.o)

all: $(EXE)
.PHONY: all

clean:
	rm -f $(EXE) $(OBJ) $(DEP)
.PHONY: clean

pmix_test: $(POBJ)
	$(CC) -o $@ $^ $(CFLAGS) -L$(LDIR) $(LDFLAGS) $(LIBS)
test_init_fin: $(T1OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -L$(LDIR) $(LDFLAGS) $(LIBS)
test_helloworld: $(T2OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -L$(LDIR) $(LDFLAGS) $(LIBS)
test_get_basic: $(T3OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -L$(LDIR) $(LDFLAGS) $(LIBS)
test_get_peers: $(T4OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -L$(LDIR) $(LDFLAGS) $(LIBS)
test_fence_basic: $(T5OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -L$(LDIR) $(LDFLAGS) $(LIBS) -lm
test_fence_wildcard: $(T6OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -L$(LDIR) $(LDFLAGS) $(LIBS) -lm

%.o : %.c
	$(CC) -c -o $@ $< $(CFLAGS)

