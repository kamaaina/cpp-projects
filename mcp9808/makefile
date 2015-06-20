PROGRAM  = $(BINDIR)/mcp9808-test
OBJDIR   = obj
BINDIR   = bin
$(shell mkdir -p $(OBJDIR) $(BINDIR))

DEBUG    = 1
CC       = g++
CFLAGS   = -Wall -I../i2c
LINC     = -L../i2c/bin
LIBS     = -li2c
OBJS     = mcp9808.cpp \
           test.cpp

ifeq ($(DEBUG), 1)
CFLAGS += -DDEBUG -g
endif

ALLOBJS = $(OBJS:%.cpp=$(OBJDIR)/%.o)

ALL: $(PROGRAM)

$(PROGRAM): $(ALLOBJS)
	$(CC) $(CFLAGS) -o $@ $(ALLOBJS) $(LINC) $(LIBS)

$(OBJDIR)/%.o : %.cpp
	$(CC) $(CFLAGS) -c $*.cpp -o $(OBJDIR)/$*.o

clean:
	rm -rf $(OBJDIR) $(BINDIR)
