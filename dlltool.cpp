#include <Windows.h>
#include "json.hpp"
using json = nlohmann::json;

extern "C" {

	// returns heap allocated char* (caller frees)
	__declspec(dllexport)
		const char* pcall(const char* json_in)
	{
		json req = json::parse(json_in);
		json resp;
		resp["city"] = req["city"];
		resp["status"] = "ok";
		resp["temperature"] = "14C";

		std::string out = resp.dump();
		char* mem = (char*)std::malloc(out.size() + 1);
		memcpy(mem, out.c_str(), out.size() + 1);
		return mem;
	}

	// free memory returned by CallJson
	__declspec(dllexport)
		void pdelete(const char* p)
	{
		std::free((void*)p);
	}

}