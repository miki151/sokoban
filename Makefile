

CFLAGS = -Wall -std=c++11 -Wno-sign-compare -Wno-unused-variable -Wno-unused-function -Wfatal-errors -Wno-shift-count-overflow -Wno-tautological-constant-out-of-range-compare -g

CLANG = true

ifdef CLANG
CC = clang++
LD = clang++
CFLAGS += -Werror -DCLANG -ftemplate-depth=512
else
CC = g++
LD = g++
endif

LDFLAGS += -fuse-ld=gold

ifdef SANITIZE_ADDRESS
CFLAGS += -fsanitize=address
endif

ifdef DBG
CFLAGS += -g
endif

ifdef OPT
CFLAGS += -O3
endif

ifdef PROF
CFLAGS += -pg
endif

OBJDIR = obj

NAME = sokoban

IPATH = -I.

CFLAGS += $(IPATH)

LDFLAGS += -L/usr/local/lib

SRCS = $(shell ls -t src/*.cpp)

LIBS = -L/usr/lib/x86_64-linux-gnu -lpthread


OBJS = $(addprefix $(OBJDIR)/,$(SRCS:.cpp=.o))
DEPS = $(addprefix $(OBJDIR)/,$(SRCS:.cpp=.d))

##############################################################################


all: $(NAME)

$(OBJDIR)/%.o: %.cpp ${PCH}
	$(CC) -MMD $(CFLAGS) $(PCHINC) -c $< -o $@

$(NAME): $(OBJS)
	$(LD) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	$(RM) $(OBJDIR)/*.o
	$(RM) $(OBJDIR)/*.d
	$(RM) $(NAME)

-include $(DEPS)
