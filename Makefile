
# g++ -D__GXX_EXPERIMENTAL_CXX0X__ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++0x -MMD -MP -MF"src/cpp_openal_opengl_dna.d" -MT"src/cpp_openal_opengl_dna.d" -o "src/cpp_openal_opengl_dna.o" "../src/cpp_openal_opengl_dna.cpp"




# g++  -o "bin/cpp_play_01"  ./src/Audio_Controller.o ./src/Audio_Model.o ./src/Audio_Utility.o ./src/Audio_View.o ./src/Binary_File_IO.o ./src/Circular_Buffer.o ./src/Common_Utilities.o ./src/Consumer.o ./src/Producer.o ./src/Stream_Render_Audio.o ./src/Synthesize_Sound.o ./src/cpp_openal_opengl_dna.o  ./common/controls.o ./common/objloader.o ./common/quaternion_utils.o ./common/shader.o ./common/tangentspace.o ./common/text2D.o ./common/texture.o ./common/vboindexer.o   -lGL -lGLU -lopenal -lGLEW -lm -lglfw


#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

# define the C compiler to use
CC = g++

# define any compile-time flags
CFLAGS =  -Wall -g -std=c++11 -O3

# define any directories containing header files other than /usr/include
#
# INCLUDES = -I/home/stens/include  -I../include

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
# LFLAGS = -L/home/stens/lib  -L../lib

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
LIBS = -lGL -lGLU -lopenal -lGLEW -lm -lglfw -lpthread

# define the C source files
SRCS = src/*.cpp common/*.cpp

# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
OBJS = $(SRCS:.c=.o)

# define the executable file 
MAIN = render_audio_openal

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: depend clean

all:    $(MAIN)
	@echo  simple app to parse WAV file which is rendered using OpenAL

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it


