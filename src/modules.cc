// author: post@matthiaswende.de (Matthias Wende)

#include "modules.h"

namespace mus_modules {

	Mod_SinGen::Mod_SinGen ()
	{
		init_data ();
	}


	void Mod_SinGen::init_data ()
	{
		// TODO this TABLE should be precalculated
		for ( int i = 0; i < table_size; ++i )
		{
			sine[i] = 0.2 * (float) std::sin( ((double)i/(double)table_size) * M_PI * 2. );
		}
	}

	mus_audio_buffer_t& Mod_SinGen::operator() (mus_audio_buffer_t& buffer)
	{
		for ( int i = 0; i < buffer.nframes; ++i )
		{
			buffer.out1[i] = sine[phase];
			buffer.out2[i] = sine[phase];
			++phase;
			if (phase >= table_size) phase -= table_size;
		}

		return buffer;
	}

	mus_audio_buffer_t& Noop::operator() (mus_audio_buffer_t& buffer)
	{
		return buffer;
	}

	//const std::string SumBuffers::type_("sum");
	//ModuleCreator<SumBuffers> SumBuffers::mod_(type_);
	//void SumBuffers::process()
	//{
		//for(auto l : buffer_list)
		//{
			//get_buffer()->append(*l);
		//}
	//}

	//const std::string ForkBuffers::type_("fork");
	//ModuleCreator<ForkBuffers> ForkBuffers::mod_(type_);
	//void ForkBuffers::process()
	//{
		//for(auto l : buffer_list)
		//{
			//*l = get_buffer_content();
		//}
	//}

} // namespace core
