// author: post@matthiaswende.de (Matthias Wende)

#ifndef H_MODULES
#define H_MODULES

#include <chrono>
#include <cmath>
#include <memory>
#include <vector>

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

#define TABLE_SIZE (200)

namespace mus_modules {

	template<typename T, class M>
	class Module
	{
		private:
			std::vector<std::unique_ptr<T>> buffer_list;
		public:
			//~Module() = default;
			void process () {
				static_cast<M>(this)->process ();
			}
	};

	// using ptr_mod = std::shared_ptr<Module<void*>>;

	class SinGen : public Module<void*, SinGen>
	{
		public:
			SinGen ();
			void process ();
			void init_data ();
		private:
			float sine[TABLE_SIZE];
	};

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
