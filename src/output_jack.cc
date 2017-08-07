// author: post@matthiaswende.de (Matthias Wende)

#include "output_jack.h"

Engine_Jack::Engine_Jack(std::string arg1, std::string arg2) : _client_name (arg1), _server_name (arg2)
{
	jack_status_t status;
	jack_options_t options = JackNullOption;

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

	jack_set_process_callback (_client, process_wrapper, this);

	// TODO send a signal (SIGTERM) to the programm to quit (e.g. std::raise())
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

int Engine_Jack::_process (jack_nframes_t nframes, void* arg)
{
	jack_default_audio_sample_t *out1, *out2;

	// TODO is there a way for (realtime) error handling
	out1 = (jack_default_audio_sample_t*)jack_port_get_buffer (output_port1, nframes);
	out2 = (jack_default_audio_sample_t*)jack_port_get_buffer (output_port2, nframes);

	// refresh the audio buffer of the output module in the (yet to build) array struct
	// call the process callback

	return 0;
}
