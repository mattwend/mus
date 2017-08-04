
#include <atomic>
#include <chrono>
#include <cmath>
#include <csignal>
#include <functional>
#include <iostream>
#include <thread>
#include <jack/jack.h>

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

std::atomic<bool> quit(false);

void signal_handler(int sig)
{
	quit.store(true);
}

#define TABLE_SIZE   (200)
typedef struct
{
	float sine[TABLE_SIZE];
	int left_phase;
	int right_phase;
}
paTestData;

class Engine_Jack
{
private:
	paTestData data;

	jack_client_t* volatile _client;
	jack_port_t* output_port1;
	jack_port_t* output_port2;

	std::string _client_name;
	std::string _server_name;

public:
	Engine_Jack(std::string arg1, std::string arg2);
	~Engine_Jack();

	void close () {	jack_client_close (_client); }

	void connect();

	void init_data ();

	// JACK calls this shutdown_callback if the server ever shuts down or
	// decides to disconnect the client.
	//void jack_shutdown(void *arg)
	//{
		//exit (1);
	//}

	static int _process(jack_nframes_t nframes, void *arg)
	{
		return static_cast<Engine_Jack*>(arg)->process ( nframes, (void*)&( static_cast<Engine_Jack*>(arg)->get_data() ) );
		//return static_cast<Engine_Jack*>(arg)->process(nframes, NULL);
	}

	paTestData& get_data () { return data; }

private:
	int process(jack_nframes_t nframes, void* arg)
	{
		jack_default_audio_sample_t *out1, *out2;
		paTestData* data = (paTestData*)arg;

		out1 = (jack_default_audio_sample_t*)jack_port_get_buffer (output_port1, nframes);
		out2 = (jack_default_audio_sample_t*)jack_port_get_buffer (output_port2, nframes);

		for (int i = 0; i < nframes; i++)
		{
			out1[i] = data->sine[data->left_phase];  // left
			out2[i] = data->sine[data->right_phase];  // right
			data->left_phase += 1;
			if ( data->left_phase >= TABLE_SIZE ) data->left_phase -= TABLE_SIZE;
			data->right_phase += 3; // higher pitch so we can distinguish left and right.
			if ( data->right_phase >= TABLE_SIZE ) data->right_phase -= TABLE_SIZE;
		}

		return 0;
	}
};

void Engine_Jack::init_data ()
{
	for ( int i = 0; i < TABLE_SIZE; ++i )
	{
		data.sine[i] = 0.2 * (float) std::sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
	}
	data.left_phase = data.right_phase = 0;
}

Engine_Jack::Engine_Jack(std::string arg1, std::string arg2) : _client_name (arg1), _server_name (arg2)
{
	jack_status_t status;
	jack_options_t options = JackNullOption;

	init_data();

	// open a client connection to the JACK server

	_client = jack_client_open (_client_name.c_str(), options, &status, _server_name.c_str());
	if (_client == NULL) {
		// TODO throw some error
		std::cerr << "jack_client_open() failed, status = 0x" << status << std::endl;
		if (status & JackServerFailed) {
			std::cerr << "Unable to connect to JACK server\n";
		}
		exit (1);
	}

	if (status & JackServerStarted) {
		std::cerr << "JACK server started\n";
	}
	if (status & JackNameNotUnique) {
		_client_name = jack_get_client_name(_client);
		std::cerr << "unique name " << _client_name << "assigned\n";
	}

	// TODO decouple the callback process from this class, i.e. use a function object or similar c++ constructs
	// tell the JACK server to call `process()' whenever
	// there is work to be done.
	jack_set_process_callback (_client, _process, this);

	// tell the JACK server to call `jack_shutdown()' if
	// it ever shuts down, either entirely, or if it
	// just decides to stop calling us.
	// TODO send a signal to the programm to quit (e.g. std::raise())
	// jack_on_shutdown (client, jack_shutdown, 0);

	// create two ports
	output_port1 = jack_port_register (_client, "output1", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	output_port2 = jack_port_register (_client, "output2", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

	if ( (output_port1 == NULL) || (output_port2 == NULL)) {
		std::cerr << "no more JACK ports available\n";
		exit(1);
	}

	// Tell the JACK server that we are ready to roll.  Our
	// process() callback will start running now.
	if ( jack_activate(_client) ) {
		std::cerr << "cannot activate client";
		exit(1);
	}
}

Engine_Jack::~Engine_Jack()
{
	jack_client_close(_client);
}

void Engine_Jack::connect()
{
	const char **ports;

	ports = jack_get_ports (_client, NULL, NULL,
			JackPortIsPhysical|JackPortIsInput);
	if (ports == NULL) {
		std::cerr << "no physical playback ports\n";
		exit (1);
	}

	if (jack_connect (_client, jack_port_name (output_port1), ports[0])) {
		std::cerr << "cannot connect output ports\n";
	}

	if (jack_connect (_client, jack_port_name (output_port2), ports[1])) {
		std::cerr << "cannot connect output ports\n";
	}

	//if (jack_connect (_client, jack_port_name (output_port2), ports[1])) {
		//std::cerr << "cannot connect output ports\n";
	//}

	jack_free (ports);
}

int main ()
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

	Engine_Jack engine("test_client", "");
	engine.init_data ();
	engine.connect();

	// keep running until the Ctrl+C
	while (true) {
		if(quit.load())
		{
			std::cout << "\nBye!\n";
			break;
		}
		std::cout << "running\n";
		std::flush(std::cout);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	return 0;
}
