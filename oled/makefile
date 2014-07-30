PROGRAM  = image-test
LCD_OFF  = lcd-off
OBJDIR   = obj
BINDIR   = bin
$(shell mkdir -p $(OBJDIR) $(BINDIR))

DEBUG    = 0
CC       = g++
CFLAGS   = -Wall -I../i2c
LINC     = -L../i2c/bin
LIBS     = -li2c
OBJS     = image-test.cpp \
           ssd1306.cpp

ifeq ($(DEBUG), 1)
CFLAGS += -DDEBUG -g
endif

ALLOBJS = $(OBJS:%.cpp=$(OBJDIR)/%.o)

ALL: $(PROGRAM) $(LCD_OFF)

$(PROGRAM): $(ALLOBJS)
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $(ALLOBJS) $(LINC) $(LIBS)

$(LCD_OFF): $(ALLOBJS)
	$(CC) $(CFLAGS) off.cpp -o $(BINDIR)/$(LCD_OFF) $(LINC) $(LIBS)

$(OBJDIR)/%.o : %.cpp
	$(CC) $(CFLAGS) -c $*.cpp -o $(OBJDIR)/$*.o

clean:
	rm -rf $(OBJDIR) $(BINDIR)
