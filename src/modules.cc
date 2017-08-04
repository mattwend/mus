// author: post@matthiaswende.de (Matthias Wende)

#include "modules.h"
// #include <algorithm>

namespace mus_modules {

	SinGen::SinGen ()
	{
		init_data ();
	}


	void SinGen::init_data ()
	{
		// TODO this TABLE should be precalculated
		for ( int i = 0; i < TABLE_SIZE; ++i )
		{
			sine[i] = 0.2 * (float) std::sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
		}
	}

	void SinGen::process ()
	{
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
