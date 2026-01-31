# Copilot/LLama/Ollama SDK for C++

Github released the [Copilot SDK](https://github.com/github/copilot-sdk) and here 's a C++ wrapper around it to be used in Windows. This also allows to use a local LLama-based model through a local llama-server.

# Copilot Installation
* Create a folder with python installed 
* pip install github-copilot-sdk
* Put [copilot.exe](https://github.com/github/copilot-cli/releases/) in that folder

# LLama Installation
Create a folder where [llama-server](https://github.com/ggml-org/llama.cpp/releases) is located.

# Ollama 
Download and run [Ollama](https://ollama.com/), specify to make it visible to the network.

# Usage
```cpp
#include "copilot.h"
#include <iostream>
COPILOT cop(L"c:\\copilot_folder");
cop.flg = CREATE_NEW_CONSOLE; // to display the python console for debugging
cop.BeginInteractive();
auto ans = cop.PushPrompt(L"Tell me a joke",true, [](std::string tok, LPARAM lp)->HRESULT
        {
            COPILOT* cop = (COPILOT*)lp;
            std::wcout << cop->tou(tok.c_str());
            return S_OK;
        }, (LPARAM)&cop);
std::wstring s;
for (auto& str : ans->strings)
    s += str;
MessageBox(0, s.c_str(), 0, 0);
cop.EndInteractive();
```

* PushPrompt's true/false parameter is whether to wait for the response. If false, then the "ans" structure includes a HANDLE event to be triggered when the response is ready.
* You can provide a callback function to receive tokens as they arrive.


# Tool definition
```cpp

// Example of adding a tool from a dll

// DLL code
#include "json.hpp"
using json = nlohmann::json;
extern "C" {

	__declspec(dllexport)
		const char* pcall(const char* json_in)
	{
		json req = json::parse(json_in);
		json resp;
		resp["status"] = "ok";
		resp["temperature"] = "14C";

		std::string out = resp.dump();
		char* mem = (char*)std::malloc(out.size() + 1);
		memcpy(mem, out.c_str(), out.size() + 1);
		return mem;
	}

	// free memory returned by pcall
	__declspec(dllexport)
		void pdelete(const char* p)
	{
		std::free((void*)p);
	}

}


// Main Code
std::vector<wchar_t> dll_path(1000);
GetFullPathName(L".\\x64\\Debug\\dlltool.dll", 1000, dll_path.data(), 0);
auto sl1 = cop.ChangeSlash(dll_path.data());
auto dll_idx = cop.AddDll(sl1.c_str(),"pcall","pdelete");
cop.AddTool(dll_idx, "GetWeather", "Get the current weather for a city in a specific date",{
        {"city", "str", "Name of the city to get the weather for"},
        {"date", "int", "Date to get the weather for"}
    });
...
cop.BeginInteractive();
auto ans = cop.PushPrompt(L"Tell me the weather in Athens in 25 January 2026",true);
```

This adds a tool to Copilot that calls the pcall function in the dlltool.dll. The pcall function receives a json string with the tool parameters and must return a json string with the tool results.
Currently, it returns hardcoded "temperature": "14C", but you can modify it to call a weather API.

# Running a local LLama-based model
```cpp
COPILOT cop(L"f:\\llama\\run","f:\\llama\\models\\mistral-7b-instruct-v0.2.Q5_K_M.gguf","",9991);
```
If you have a local LLama-based model, you can instantiate using the above format, giving a local LLama server port. f:\\llama\\run should be the [llama-server](https://github.com/ggml-org/llama.cpp/releases) location.
PushPrompt will then send requests to the local LLama server instead of the GitHub Copilot server.

# Connecting to Ollama
```cpp
COPILOT_CUSTOM_PROVIDER cp;
cp.type = "openai";
cp.base_url = "http://localhost:11434";
COPILOT cop(L"f:\\copilot", "deepseek-r1:8b", "", 0, optional_api_key, 0, &cp);
```
This will reuse an existing Ollama server with the specified model. You can also use any other provider that requires an API key, such as OpenAI, Azure etc. 


# License
MIT
