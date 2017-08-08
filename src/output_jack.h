// author: post@matthiaswende.de (Matthias Wende)

#ifndef H_OUTPUT_JACK
#define H_OUTPUT_JACK

#include <functional>
#include <iostream>

#include <jack/jack.h>

#include "types.h"

namespace mus_output 
{

class Engine_Jack
{
private:
	jack_client_t* volatile _client;
	jack_port_t* output_port1;
	jack_port_t* output_port2;

	std::string _client_name;
	std::string _server_name;
	std::function<void(mus_audio_buffer_t&)> _process;

public:
	//Engine_Jack(std::string arg1, std::string arg2);
	Engine_Jack(std::string arg1, std::string arg2, std::function<void(mus_audio_buffer_t&)> p);

	~Engine_Jack();

	void close () {	jack_client_close (_client); }

	void connect();

	// JACK calls this shutdown_callback if the server ever shuts down or
	// decides to disconnect the client.
	//void jack_shutdown(void *arg)
	//{
		//exit (1);
	//}

	static int process_wrapper (jack_nframes_t nframes, void *arg)
	{
		//return static_cast<Engine_Jack*>(arg)->_process ( nframes, (void*)&( static_cast<Engine_Jack*>(arg)->get_data() ) );
		return static_cast<Engine_Jack*>(arg)->_process_stub ( nframes, NULL );
	}

private:
	int _process_stub (jack_nframes_t nframes, void* arg);
};

} // namespace mus_output
#endif // H_OUTPUT_JACK
