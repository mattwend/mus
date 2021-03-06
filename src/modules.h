// author: post@matthiaswende.de (Matthias Wende)

#ifndef H_MODULES
#define H_MODULES

#include <chrono>
#include <cmath>
#include <memory>
#include <vector>

#include "types.h"

namespace mus_modules {

	// Module : A → B
	// modules might be soundprocessors, control modules (e.g. osc) or graphic widgets
	template <typename A, typename B>
	class Module
	{
		public:
			virtual B operator() (A) = 0;
			virtual ~Module () = default;
	};

	class Mod_SinGen : public Module <mus_audio_buffer_t&, mus_audio_buffer_t&>
	{
		public:
			Mod_SinGen ();
			mus_audio_buffer_t& operator() (mus_audio_buffer_t& buffer) override;
			void init_data ();
		private:
			static const int table_size = 400;
			float sine[table_size];
			int phase = 0;
	};

	class Noop : public Module <mus_audio_buffer_t&, mus_audio_buffer_t&>
	{
		public:
			mus_audio_buffer_t& operator() (mus_audio_buffer_t& buffer) override;
	};

	//class Mod_Output : public Module<void, Mod_Output>
	//{
	//};

	//class SumBuffers : public Module
	//{
		//private:
			//std::vector<std::shared_ptr<std::string>> buffer_list;
		//public:
			//SumBuffers() {set_type(type_);}
			//void process();
			//inline int size_of_buffer_list() {return buffer_list.size();}
			//inline void clear() {reset_buffer(); buffer_list.clear();}
			//inline std::vector<std::shared_ptr<std::string>>& get_buffer_list() {return buffer_list;}
		//private:
			//static const std::string type_;
			//static ModuleCreator<SumBuffers> mod_;
	//};

	//class ForkBuffers : public Module
	//{
		//private:
			//std::vector<std::shared_ptr<std::string>> buffer_list;
		//public:
			//ForkBuffers() {set_type(type_);}
			//void process();
			//inline int size_of_buffer_list() {return buffer_list.size();}
			//inline void clear() {reset_buffer(); buffer_list.clear();}
			//inline std::vector<std::shared_ptr<std::string>>& get_buffer_list() {return buffer_list;}
		//private:
			//static const std::string type_;
			//static ModuleCreator<ForkBuffers> mod_;
	//};

} // namespace core

#endif
