# Pianify
Pianify is an application whose 2 main goals are:
- transformation of an audio file to the MIDI format
- recognition of different instruments that play across a song

# Installation

 1. Clone the repo: `git clone https://github.com/Radu-Lucian/pianify.git`
 2. Building the project requires CMake 3.11+ and MSVC 2019.
 3. Check that you have Qt installed, provide in the base CMakeList.txt file the path to the Qt MSVC binaries, example:
  
        set(Qt5_DIR "C:/Qt/5.13.1/msvc2017_64/lib/cmake/Qt5")

 4. To build the project (only x64 CPU arhitecture is suported):

		 cd pianify
		 mkdir build
		 cd build  
		 cmake -A x64 ../

# Dependencies
Pianify is written entirely in C++ and depends on the following included libraries:

 - Aquila
 - tiny-dnn
 - QCustomPlot 
 - midifile
 - Boost

The only required library is:
 - Qt (tested on version 5.13.1)
