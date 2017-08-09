// author: post@matthiaswende.de (Matthias Wende)
  
#include <atomic>
#include <csignal>
#include <iostream>
#include <iterator>
#include <sstream>
#include <thread>

#include <jack/jack.h>

#include "modules.h"
#include "output_jack.h"
#include "types.h"

using namespace mus_modules;
using namespace mus_output;

std::atomic<bool> quit (false);

void signal_handler (int sig)
{
	quit.store(true);
}

class SynthApp
{
private:
	Mod_SinGen mod_sg;
	Noop noop;
public:
	void operator() (mus_audio_buffer_t& buffer);
	void run ();
};

void SynthApp::operator() (mus_audio_buffer_t& buffer)
{
	mod_sg (noop (buffer));
}

void SynthApp::run ()
{
	while (true) {
		if ( quit.load () )
		{
			std::cout << "\nBye!\n";
			break;
		}
		// TODO Process input control data, draw gui
		std::cout << "running\n";
		std::flush (std::cout);
		std::this_thread::sleep_for ( std::chrono::milliseconds (1000) );
	}
}

int main()
{
	// install a signal handler to properly quit the programm
	#ifdef WIN32
	std::signal(SIGINT, signal_handler);
	std::signal(SIGABRT, signal_handler);
	std::signal(SIGTERM, signal_handler);
	#else
	std::signal(SIGQUIT, signal_handler);
	std::signal(SIGTERM, signal_handler);
	std::signal(SIGHUP, signal_handler);
	std::signal(SIGINT, signal_handler);
	#endif

	// TODO synthapp should be a function object of type std::function
	// and the () operator is overloaded by the process method
	SynthApp mySynth;
	Engine_Jack engine ("mus", "", mySynth);

	engine.connect ();
	mySynth.run ();
	
	return 0;
}
