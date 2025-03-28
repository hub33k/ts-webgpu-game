#pragma once

/*
# Usage

HK_PROFILE_BEGIN_SESSION("Startup", "hub33kProfile-Startup.json");
auto app = Game::CreateApplication({ argc, argv });
HK_PROFILE_END_SESSION();

HK_PROFILE_FUNCTION();

{
  HK_PROFILE_SCOPE("Renderer Prep");
  hub33k::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
  hub33k::RenderCommand::Clear();
}

*/

// TODO (hub33k): implement

// #include "Core/Log.hpp"
// #include <algorithm>
// #include <chrono>
// #include <fstream>
// #include <iomanip>
// #include <mutex>
// #include <sstream>
// #include <string>
// #include <thread>

#define HK_PROFILE_BEGIN_SESSION(name, filepath)
#define HK_PROFILE_END_SESSION()
#define HK_PROFILE_SCOPE(name)
#define HK_PROFILE_FUNCTION()
