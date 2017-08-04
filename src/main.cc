// author: post@matthiaswende.de (Matthias Wende)
// date  : 2017.08.01
  
#include <atomic>
#include <csignal>
#include <iostream>
#include <iterator>
#include <sstream>
#include <thread>
#include "modules.h"

#include <jack/jack.h>

using namespace mus_modules;

std::atomic<bool> quit(false);

void signal_handler(int sig)
{
	quit.store(true);
}

struct Output_Data
{
	unsigned int samplerate;
	unsigned int nsamples;
	//std::string output_module_name;
	//int no_outputs;
};

class SynthApp
{
private:
	std::unique_ptr< Module<void*, SinGen> > sg = std::make_unique< Module<void*, SinGen> >();
public:
	SynthApp ();
	//~SynthApp ();
	void run ();
};

SynthApp::SynthApp ()
{
	//singen
}
void SynthApp::run ()
{
	while (true) {
		if (quit.load())
		{
			std::cout << "\nBye!\n";
			break;
		}
		std::cout << "running\n";
		std::flush(std::cout);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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

	SynthApp mySynth;
	mySynth.run ();
	
	return 0;
}
