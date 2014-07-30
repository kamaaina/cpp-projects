LIBRARY  = libi2c.a
AR       = ar
ARFLAGS  = rcs
OBJDIR   = obj
BINDIR   = bin
$(shell mkdir -p $(OBJDIR) $(BINDIR))

DEBUG    = 0
CC       = g++
CFLAGS   = -Wall
LINC     =
LIBS     =
OBJS     = i2c.cpp

ifeq ($(DEBUG), 1)
CFLAGS += -DDEBUG -g
endif

ALLOBJS = $(OBJS:%.cpp=$(OBJDIR)/%.o)

ALL: $(LIBRARY)

$(LIBRARY): $(ALLOBJS)
	$(AR) $(ARFLAGS) -o $(BINDIR)/$@ $(ALLOBJS)

$(OBJDIR)/%.o : %.cpp
	$(CC) $(CFLAGS) -c $*.cpp -o $(OBJDIR)/$*.o

clean:
	rm -rf $(OBJDIR) $(BINDIR)
