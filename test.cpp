#include ".\\copilot.hpp"
#pragma comment(lib,"wininet.lib")

int main()
{
    COPILOT cop(L"f:\\copilot");
//    COPILOT cop(L"f:\\llama\\run","f:\\llama\\models\\mistral-7b-instruct-v0.2.Q5_K_M.gguf","",9991);

	std::vector<wchar_t> dll_path(1000);
    GetFullPathName(L".\\x64\\Debug\\dlltool.dll", 1000, dll_path.data(), 0);
	auto sl1 = cop.ChangeSlash(dll_path.data());
	auto dll_idx = cop.AddDll(sl1.c_str(),"pcall","pdelete");
    cop.AddTool(dll_idx, "GetWeather", "Get the current weather for a city in a specific date",{
        {"city", "str", "Name of the city to get the weather for"},
        {"date", "int", "Date to get the weather for"}
    });

    cop.BeginInteractive();
    auto ans = cop.PushPrompt(L"Tell me the weather in Athens in 25 January 2026",true);
    std::wstring s;
	for (auto& str : ans->strings)
        s += str;
    MessageBox(0, s.c_str(), 0, 0);
    cop.EndInteractive();
}
