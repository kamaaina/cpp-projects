PROGRAM  = bmp180-test
OBJDIR   = obj
BINDIR   = bin
$(shell mkdir -p $(OBJDIR) $(BINDIR))

DEBUG    = 1
CC       = g++
CFLAGS   = -Wall
LINC     =
LIBS     = 
OBJS     = bmp180.cpp \
           test.cpp

ifeq ($(DEBUG), 1)
CFLAGS += -DDEBUG -g
endif

ALLOBJS = $(OBJS:%.cpp=$(OBJDIR)/%.o)

ALL: $(PROGRAM)

$(PROGRAM): $(ALLOBJS)
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $(ALLOBJS) $(LINC) $(LIBS)

$(OBJDIR)/%.o : %.cpp
	$(CC) $(CFLAGS) -c $*.cpp -o $(OBJDIR)/$*.o

clean:
	rm -rf $(OBJDIR) $(BINDIR)
