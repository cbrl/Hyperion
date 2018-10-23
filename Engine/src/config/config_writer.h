#pragma once

#include "io/io.h"

class Engine;

namespace ConfigWriter {

	void write(const Engine& engine, const fs::path& file);
}